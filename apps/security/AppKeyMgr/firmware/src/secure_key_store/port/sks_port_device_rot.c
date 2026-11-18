/*******************************************************************************
  Secure Key Store - Device Backend: Security Fuses (read-only, PIC32CX-BZ6)

  File Name:
    sks_port_device_rot.c

  Description:
    Implements the device-security interface of sks_port.h with the
    security configuration holding register of the ROT peripheral
    (ROT_SECCFG), using the DFP register definitions (device.h).

    Only read operations are implemented.  eFuse programming is
    irreversible and is not part of this module.

    DFP mask names used (PIC32CX2051BZ62132 DFP component/rot.h):
      ROT_SECCFG_UUID_LCK_Msk, ROT_SECCFG_DEBUG_LCK_Msk,
      ROT_SECCFG_ROOT_KEY_LCK_Msk, ROT_SECCFG_BOOT_KEY_LCK_Msk,
      ROT_EFUSE_CON_EN_LD_ALL_Msk

    rootKeyLocked == true  -> the SRK is fused and locked; AES-256-GCM uses
                              the device-unique hardware key.
    rootKeyLocked == false -> the SRK is NOT provisioned; the hardware uses
                              a non-unique default key.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#define SKS_MODULE_SOURCE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../sks.h"
#include "sks_port.h"
#include "device.h"   /* ROT_REGS and DFP field definitions */

// *****************************************************************************
// Section: Local Constants
// *****************************************************************************

/** Poll limit for the EN_LD_ALL self-clear.  The reload takes microseconds;
 *  the bound only keeps the application from hanging if the bit never
 *  clears. */
#define SKS_ROT_EFUSE_LOAD_MAX_POLLS    1000000UL

// *****************************************************************************
// Section: Local Helpers
// *****************************************************************************

/* Refresh the holding registers from the eFuses.
 * Returns true when the hardware cleared EN_LD_ALL (load complete). */
static bool sks_rot_efuseLoad(void)
{
    uint32_t polls = 0U;

    ROT_REGS->ROT_EFUSE_RWDATA = 0U;

    /* Set EN_LD_ALL - hardware clears it when the load is complete */
    ROT_REGS->ROT_EFUSE_CON = ROT_EFUSE_CON_EN_LD_ALL_Msk;

    while ((ROT_REGS->ROT_EFUSE_CON & ROT_EFUSE_CON_EN_LD_ALL_Msk) != 0U)
    {
        polls++;
        if (polls >= SKS_ROT_EFUSE_LOAD_MAX_POLLS)
        {
            return false;
        }
    }

    return true;
}

// *****************************************************************************
// Section: Porting Interface Implementation
// *****************************************************************************

/*******************************************************************************
  Function:
    int sks_port_device_read(sks_port_device_info_t *info)
*******************************************************************************/
int sks_port_device_read(sks_port_device_info_t *info)
{
    uint32_t regVal;

    if (info == NULL)
    {
        return SKS_ERR_INVALID_PARAM;
    }

    if (!sks_rot_efuseLoad())
    {
        return SKS_ERR_INVALID_STATE;
    }

    regVal = ROT_REGS->ROT_SECCFG;

    /* Each lock field: 0 = unlocked, non-zero = locked */
    info->debugLocked   = ((regVal & ROT_SECCFG_DEBUG_LCK_Msk)    != 0U);
    info->uuidLocked    = ((regVal & ROT_SECCFG_UUID_LCK_Msk)     != 0U);
    info->rootKeyLocked = ((regVal & ROT_SECCFG_ROOT_KEY_LCK_Msk) != 0U);
    info->bootKeyLocked = ((regVal & ROT_SECCFG_BOOT_KEY_LCK_Msk) != 0U);

    return SKS_OK;
}

/*******************************************************************************
 End of File
*******************************************************************************/
