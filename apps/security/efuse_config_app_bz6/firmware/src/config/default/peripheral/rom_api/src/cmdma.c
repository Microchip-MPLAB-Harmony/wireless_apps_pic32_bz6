/* Copyright (c) 2019-2020 Silex Insight. All Rights reserved. */

#include "../include/sxsymcrypt/statuscodes.h"
#include "../include/sxsymcrypt/internal.h"
#include "crypmasterregs.h"
#include "hw.h"
#include "cmdma.h"


void sx_cmdma_newcmd(struct sx_dmactl *dma, struct sxdesc *d, uint32_t cmd, uint32_t tag)
{

    dma->d = d;
    dma->dmamem.cfg = cmd;

    dma->mapped = sx_map_internal(dma->regs, (char*)&dma->dmamem);
    ADD_INDESC_PRIV(*dma, offsetof(struct sx_dmaslot, cfg),
        sizeof(dma->dmamem.cfg), tag);
}


static void sx_map_descslist(struct sx_regs *regs, struct sxdesc *p,
        struct sxdesc *mappeddesc)
{
    struct sxdesc *next;
    struct sxdesc *s = p;

    (void)regs;
    mappeddesc++;
    while (s != DMA_LAST_DESCRIPTOR) {
        next = s->next;
        if (s->next != DMA_LAST_DESCRIPTOR)
            s->next = mappeddesc++;
        s = next;
    }
}


static void sx_map_descs(struct sx_dmactl *dma, struct sxdesc *indesc)
{
    struct sxdesc *m;

    m = (struct sxdesc *)(dma->mapped + sizeof(struct sx_dmaslot));
    sx_map_descslist(dma->regs, indesc, m);
    m = (struct sxdesc *)(dma->mapped + offsetof(struct sx_dmaslot, outdescs));
    sx_map_descslist(dma->regs, dma->dmamem.outdescs, m);
}


void sx_cmdma_start(struct sx_dmactl *dma, size_t privsz, struct sxdesc *indesc)
{
    struct sxdesc *m;

    sx_map_descs(dma, indesc);
    sx_flush_tohw(dma->regs, (char*)&dma->dmamem, sizeof(dma->dmamem) + privsz);
    m = (struct sxdesc *)(dma->mapped + sizeof(struct sx_dmaslot));
    sx_wrreg_addr(dma->regs, REG_FETCH_ADDR, m);
    m = (struct sxdesc *)(dma->mapped + offsetof(struct sx_dmaslot, outdescs));
    sx_wrreg_addr(dma->regs, REG_PUSH_ADDR, m);
    sx_wrreg(dma->regs, REG_CONFIG, REG_CONFIG_SG);
    sx_wrreg(dma->regs, REG_START, REG_START_ALL);
}


void sx_cmdma_finalize_descs(struct sxdesc *start, struct sxdesc *end)
{
    struct sxdesc *d;

    for (d = start; d < end; d++) {
        d->next = d + 1;
    }
    end->next = DMA_LAST_DESCRIPTOR;
    end->dmatag |= DMATAG_LAST;
    end->sz |= DMA_REALIGN;
}


int sx_cmdma_check(struct sx_dmactl *dma)
{
    uint32_t r = 0xFF;
    uint32_t busy;

    r = sx_rdreg(dma->regs, REG_INT_STATRAW);
    busy = sx_rdreg(dma->regs, REG_STATUS) & REG_STATUS_BUSY_MASK;

    if (r & (DMA_BUS_FETCHER_ERROR_MASK | DMA_BUS_PUSHER_ERROR_MASK)) {
        sx_cmdma_reset(dma->regs);
        return SX_ERR_DMA_FAILED;
    }
    if (busy)
        return SX_ERR_HW_PROCESSING;

    sx_wrreg(dma->regs, REG_INT_STATCLR, ~0);

    return SX_OK;
}


unsigned int sx_cmdma_list_compatible(unsigned int mask)
{
    unsigned int i;
    unsigned int found = 0;

    for (i = 0; i < sizeof(found) * 8; i++) {
        struct sx_regs *regs;
        uint32_t r;

        regs = sx_hw_find_regs(i);
        if (!regs)
            break;
        r = sx_rdreg(regs, REG_HW_PRESENCE);
        if (r & mask)
            found |= 1u << i;
    }

    return found;
}


unsigned int sx_cmdma_filter_compatible(unsigned int compatible,
    uint32_t capsaddr, uint32_t capsmask)
{
    unsigned int i;
    unsigned int usable = 0;
    uint32_t hwcaps;
    struct sx_regs *regs = NULL;

    for (i = 0; compatible; i++, compatible >>= 1) {
        if (compatible & 1) {
            regs = sx_hw_find_regs(i);
            hwcaps = sx_rdreg(regs, capsaddr);
            if ((hwcaps & capsmask) == capsmask)
                usable |= 1u << i;
        }
    }

    return usable;
}


void sx_cmdma_reset(struct sx_regs *regs)
{
    uint32_t intrmask = sx_rdreg(regs, REG_INT_EN);
    sx_wrreg(regs, REG_CONFIG, REG_SOFT_RESET_ENABLE);
    sx_wrreg(regs, REG_CONFIG, 0); //clear SW reset

    /* Wait for soft-reset to end */
    while (sx_rdreg(regs, REG_STATUS) & REG_SOFT_RESET_BUSY);

    if (intrmask)
        sx_wrreg(regs, REG_INT_EN, intrmask);
}
