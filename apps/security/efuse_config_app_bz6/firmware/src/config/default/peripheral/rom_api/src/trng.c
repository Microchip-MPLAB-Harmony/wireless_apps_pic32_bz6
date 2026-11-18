/* Copyright (c) 2020 Silex Insight. All Rights reserved. */

#include "../include/sxsymcrypt/trng.h"
#include "../include/sxsymcrypt/statuscodes.h"
#include "crypmasterregs.h"
#include "hw.h"
#include "regdefs.h"
#include "cmdma.h"


#define RNG_CLKDIV            (32)
#define RNG_OFF_TIMER_VAL     (0)
#define RNG_INIT_WAIT_VAL     (1024)
#define RNG_NB_128BIT_BLOCKS  (8)

#define RNG_KEY0              (0x52696368UL)
#define RNG_KEY1              (0x61726420UL)
#define RNG_KEY2              (0x47656973UL)
#define RNG_KEY3              (0x736C6572UL)


typedef enum ba431_state {
    BA431_STATE_RESET = 0x00,
    BA431_STATE_STARTUP = 0x01,
    BA431_STATE_IDLE_RINGS_ON = 0x02,
    BA431_STATE_IDLE_RINGS_OFF = 0x03,
    BA431_STATE_FILL_FIFO = 0x04,
    BA431_STATE_ERROR = 0x05,
} ba431_state_t;


static inline ba431_state_t ba431_get_state(struct sx_trng *ctx)
{
    uint32_t status = sx_rdreg(ctx->regs, BA431_REG_Status_OFST);

    return (ba431_state_t)((status & BA431_FLD_Status_State_MASK) >> BA431_FLD_Status_State_LSB);
}


static void ba431_softreset(struct sx_trng *ctx)
{
    uint32_t ctrl_reg = sx_rdreg(ctx->regs, BA431_REG_Control_OFST);
    sx_wrreg(ctx->regs, BA431_REG_Control_OFST,
             ctrl_reg | BA431_FLD_Control_SoftRst_MASK);
    sx_wrreg(ctx->regs, BA431_REG_Control_OFST,
             ctrl_reg & ~BA431_FLD_Control_SoftRst_MASK);
}


int sx_trng_init(struct sx_trng *ctx, const struct sx_trng_config *config)
{
    unsigned int compatible;
    int i;

    *ctx = (struct sx_trng){0};
    compatible = sx_cmdma_list_compatible(REG_HW_PRESENT_BA431);
    if (!compatible)
        return SX_ERR_INCOMPATIBLE_HW;
    for (i = 0; !(compatible & 1); i++, compatible >>= 1) {
    }
    ctx->regs = sx_hw_find_trng_regs(i);
    if (!ctx->regs)
        return SX_ERR_INCOMPATIBLE_HW;

    //Clear the control reg first
    sx_wrreg(ctx->regs, BA431_REG_Control_OFST, 0x00);

    ba431_softreset(ctx);

    uint32_t fifo_wakeup_level;
    uint32_t rng_off_timer_val = RNG_OFF_TIMER_VAL;
    uint32_t rng_clkdiv = RNG_CLKDIV;
    uint32_t rng_init_wait_val = RNG_INIT_WAIT_VAL;

    fifo_wakeup_level = sx_rdreg(ctx->regs, BA431_REG_FIFODepth_OFST) / 4 - 1;
    if (config) {
        if (config->wakeup_level) {
            if (config->wakeup_level > fifo_wakeup_level)
                return SX_ERR_TOO_BIG;
            fifo_wakeup_level = config->wakeup_level;
        }
        if (config->off_time_delay)
            rng_off_timer_val = config->off_time_delay;
        if (config->init_wait)
            rng_init_wait_val = config->init_wait;
        if (config->sample_clock_div)
            rng_clkdiv = config->sample_clock_div;
    }

    //Program powerdown and clock settings
    sx_wrreg(ctx->regs, BA431_REG_FIFOThreshold_OFST, fifo_wakeup_level);
    sx_wrreg(ctx->regs, BA431_REG_SwOffTmrVal_OFST, rng_off_timer_val);
    sx_wrreg(ctx->regs, BA431_REG_ClkDiv_OFST, rng_clkdiv);
    sx_wrreg(ctx->regs, BA431_REG_InitWaitVal_OFST, rng_init_wait_val);

    //Configure conditioning
    uint32_t control = sx_rdreg(ctx->regs, BA431_REG_Control_OFST);
    control = (control & ~BA431_FLD_Control_Nb128BitBlocks_MASK) |
            (RNG_NB_128BIT_BLOCKS << BA431_FLD_Control_Nb128BitBlocks_LSB) |
            BA431_FLD_Control_HealthTestSel_MASK;
    //sx_wrreg(ctx->regs, BA431_REG_Control_OFST, control);

    //Set key registers with fixed values
    sx_wrreg(ctx->regs, BA431_REG_Key0_OFST, RNG_KEY0);
    sx_wrreg(ctx->regs, BA431_REG_Key1_OFST, RNG_KEY1);
    sx_wrreg(ctx->regs, BA431_REG_Key2_OFST, RNG_KEY2);
    sx_wrreg(ctx->regs, BA431_REG_Key3_OFST, RNG_KEY3);
    ctx->conditioning_key_set = 1;

    //Write control register twice SoftReset = 1 Enble = 0; SoftReset = 0 Enable = 1
    sx_wrreg(ctx->regs, BA431_REG_Control_OFST, control | BA431_FLD_Control_SoftRst_MASK);
    sx_wrreg(ctx->regs, BA431_REG_Control_OFST, control | BA431_FLD_Control_Enable_MASK);

    //Enable NDRNG
    /* sx_wrreg(ctx->regs, BA431_REG_Control_OFST,
             (control & ~BA431_FLD_Control_Enable_MASK) | 0x01);
 */
    return SX_OK;
}

/* static int ba431_setup_conditioning_key(struct sx_trng *ctx)
{
    uint32_t key[4];

    uint32_t level = sx_rdreg(ctx->regs, BA431_REG_FIFOLevel_OFST);
    //FIFO level in 4-byte words
    if (level < 4)
        return SX_ERR_HW_PROCESSING;

    for (size_t i = 0; i < sizeof(key)/sizeof(key[0]); i++)
        key[i] = sx_rdreg(ctx->regs, BA431_REG_FIFODATA_OFST);

    sx_wrreg(ctx->regs, BA431_REG_Key0_OFST, key[0]);
    sx_wrreg(ctx->regs, BA431_REG_Key1_OFST, key[1]);
    sx_wrreg(ctx->regs, BA431_REG_Key2_OFST, key[2]);
    sx_wrreg(ctx->regs, BA431_REG_Key3_OFST, key[3]);

    ctx->conditioning_key_set = 1;

    return SX_OK;
} */


int sx_trng_get(struct sx_trng *ctx, char *dst, size_t size)
{
    int status = SX_OK;

    //Check startup tests
    switch (ba431_get_state(ctx)) {
        case BA431_STATE_ERROR:
            return SX_ERR_RESET_NEEDED;
            break;
        case BA431_STATE_RESET:
            return SX_ERR_HW_PROCESSING;
            break;
        case BA431_STATE_STARTUP:
        default:
            break;
    }

    /* Block sizes above the FIFO wakeup level to guarantee that the
     * hardware will be able at some time to provide the requested bytes. */
    uint32_t wakeup_level = sx_rdreg(ctx->regs, BA431_REG_FIFOThreshold_OFST);
    if (size > (wakeup_level + 1) * 16)
        return SX_ERR_TOO_BIG;
    //Check level
    uint32_t level = sx_rdreg(ctx->regs, BA431_REG_FIFOLevel_OFST);
    //FIFO level in 4-byte words
    if (size > level * 4)
        return SX_ERR_HW_PROCESSING;

    while (size) {
        uint32_t data;
        data = sx_rdreg(ctx->regs, BA431_REG_FIFODATA_OFST);
        for (int i = 0; i < 4 && size; i++, size--) {
            *dst = (char)(data & 0xFF);
            dst++;
            data >>= 8;
        }
    }

    return status;
}
