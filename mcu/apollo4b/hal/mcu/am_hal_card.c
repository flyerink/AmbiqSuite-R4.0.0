//*****************************************************************************
//
//  am_hal_card.c
//! @file
//!
//! @brief Functions for interfacing with the ambiq card host.
//!
//! @addtogroup ambiq card interface
//! @ingroup apollo4hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2021, Ambiq Micro, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision release_sdk_4_0_0-60f150b187 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "am_mcu_apollo.h"
#include "am_util_stdio.h"
#include "am_util_debug.h"
#include "am_util_delay.h"

//
// Internal macros
//
#define SDIO_TIMING_SCAN_MIN_ACCEPTANCE_LENGTH  2 // Acceptable length should be determined based on system level test.
#define AM_HAL_CARD_DEBUG(fmt, ...) am_util_debug_printf("[CARD] line %04d - "fmt, __LINE__, ##__VA_ARGS__)


//
// Private internal functions
//

//
// CMD0 - go idle
//
static inline uint32_t am_hal_sdmmc_cmd0_go_idle(am_hal_card_t *pCard)
{
    am_hal_card_cmd_t cmd;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx   = MMC_CMD_GO_IDLE_STATE;
    cmd.ui32Arg  = 0x0;
    cmd.ui32RespType = MMC_RSP_NONE;

    pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL);

    return cmd.eError;
}

//
// CMD1 - send operation condition
//
#define SECTOR_MODE       0x4
#define CMD1_CARD_RDY_BIT ((uint32_t)0x1 << 31)

static inline uint32_t am_hal_sdmmc_cmd1_send_op_cond(am_hal_card_t *pCard)
{
    uint32_t ui32CardOCR = 0;
    uint8_t ui8Tries = 100;

    am_hal_card_cmd_t cmd;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_SEND_OP_COND;
    cmd.ui32Arg = SECTOR_MODE << 28;
    cmd.ui32RespType = MMC_RSP_R3;

    do
    {
        pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL);

        if ( cmd.eError == AM_HAL_CMD_ERR_NONE )
        {
            ui32CardOCR = cmd.ui32Resp[0];
            cmd.ui32Arg |= ui32CardOCR;
        }

        if ( ui32CardOCR & CMD1_CARD_RDY_BIT )
        {
            pCard->ui32OCR = ui32CardOCR;
            break;
        }

        am_util_delay_ms(10);
    } while (--ui8Tries != 0);

    return ui8Tries == 0x0 ? AM_HAL_CMD_ERR_TIMEOUT : cmd.eError;
}

//
// CMD2 - send card identification
//
static inline uint32_t am_hal_sdmmc_cmd2_send_cid(am_hal_card_t *pCard)
{
    am_hal_card_cmd_t cmd;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx   = MMC_CMD_ALL_SEND_CID;
    cmd.ui32Arg  = 0x0;
    cmd.ui32RespType = MMC_RSP_R2;

    pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL);

    if ( cmd.eError == AM_HAL_CMD_ERR_NONE )
    {
        memcpy((void *)pCard->ui32CID, (void *)&cmd.ui32Resp[0], 16);
        pCard->bCidValid = 1;
    }

    return cmd.eError;
}

//
// CMD3 - set the relative card address
//
static inline uint32_t am_hal_sdmmc_cmd3_set_rca(am_hal_card_t *pCard, uint16_t ui16RCA)
{
    am_hal_card_cmd_t cmd;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx   = MMC_CMD_SET_RELATIVE_ADDR;
    cmd.ui32Arg  = ui16RCA << 16;
    cmd.ui32RespType = MMC_RSP_R1;

    pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL);

    if ( cmd.eError == AM_HAL_CMD_ERR_NONE )
    {
        pCard->ui8RCA = ui16RCA;
    }

    return cmd.eError;
}

//
// CMD7 - select the card
//
static uint32_t am_hal_sdmmc_cmd7_card_select(am_hal_card_t *pCard)
{
    am_hal_card_cmd_t cmd;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx   = MMC_CMD_SELECT_CARD;
    cmd.ui32Arg  = pCard->ui8RCA << 16;
    cmd.ui32RespType = MMC_RSP_R1b;

    pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL);

    if ( cmd.eError == AM_HAL_CMD_ERR_NONE )
    {
        pCard->eState = AM_HAL_CARD_STATE_TRANS;
    }

    return cmd.eError;
}

//
// CMD8 - get the 512 bytes ext csd
//
static uint32_t am_hal_sdmmc_cmd8_send_ext_csd(am_hal_card_t *pCard)
{
    am_hal_card_cmd_t cmd;
    am_hal_card_cmd_data_t cmd_data;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0x0, sizeof(cmd));
    memset(&cmd_data, 0x0, sizeof(cmd_data));

    cmd.ui8Idx   = MMC_CMD_SEND_EXT_CSD;
    cmd.ui32Arg  = pCard->ui8RCA << 16;
    cmd.ui32RespType = MMC_RSP_R1;

    cmd_data.pui8Buf = (uint8_t *)pCard->ui32ExtCSD;
    cmd_data.ui32BlkCnt = 1;
    cmd_data.ui32BlkSize = 512;
    cmd_data.dir = AM_HAL_DATA_DIR_READ;

    pHost->ops->execute_cmd(pHost->pHandle, &cmd, &cmd_data);

    if ( cmd.eError == AM_HAL_CMD_ERR_NONE )
    {
        pCard->bExtCsdValid = 1;
    }

    return cmd.eError;
}

#if 0 // Unused static function causes compiler warning
static uint32_t am_hal_sdmmc_cmd7_card_deselect(am_hal_card_t *pCard)
{
    pCard->ui8RCA = 0x0;
    return am_hal_sdmmc_cmd7_card_select(pCard);
}
#endif

//
// CMD9 - get the CSD
//
static inline uint32_t am_hal_sdmmc_cmd9_send_csd(am_hal_card_t *pCard)
{
    am_hal_card_cmd_t cmd;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx   = MMC_CMD_SEND_CSD;
    cmd.ui32Arg  = pCard->ui8RCA << 16;
    cmd.ui32RespType = MMC_RSP_R2;

    pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL);

    if ( cmd.eError == AM_HAL_CMD_ERR_NONE )
    {
        memcpy((void *)pCard->ui32CSD, (void *)&cmd.ui32Resp[0], 16);
        pCard->bCsdValid = 1;
    }

    return cmd.eError;
}


#ifndef REMOVE_EXT_CSD_READ_ISSUE_WORKAROUND
//
// CMD19 and CMD14 read EXT_CSD workaround
//
static inline uint32_t am_hal_cmd19_cmd14_read_extcsd_workaround(am_hal_card_t *pCard)
{
    uint32_t ui32Status;
    uint8_t ui8Testbuf[512];

    am_hal_card_cmd_t cmd;
    am_hal_card_cmd_data_t cmd_data;
    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_SEND_TUNING_BLOCK;
    cmd.ui32Arg = 0x0;
    cmd.ui32RespType = MMC_RSP_R1;

    memset((void *)&cmd_data, 0x0, sizeof(cmd_data));
    cmd_data.pui8Buf = ui8Testbuf;
    cmd_data.ui32BlkCnt = 1;
    cmd_data.ui32BlkSize = 512;
    cmd_data.dir = AM_HAL_DATA_DIR_WRITE;
    cmd_data.eXferMode = AM_HAL_HOST_XFER_PIO;

    ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, &cmd_data);

    if ((ui32Status & 0xFFFF) != AM_HAL_STATUS_SUCCESS)
    {
        return ui32Status;
    }

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_READ_TUNING_BLOCK;
    cmd.ui32Arg = 0x0;
    cmd.ui32RespType = MMC_RSP_R1;

    memset((void *)&cmd_data, 0x0, sizeof(cmd_data));
    cmd_data.pui8Buf = ui8Testbuf;
    cmd_data.ui32BlkCnt = 1;
    cmd_data.ui32BlkSize = 512;
    cmd_data.dir = AM_HAL_DATA_DIR_READ;
    cmd_data.eXferMode = AM_HAL_HOST_XFER_PIO;

    ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, &cmd_data);

    if ((ui32Status & 0xFFFF) != AM_HAL_STATUS_SUCCESS)
    {
        return ui32Status;
    }

    return AM_HAL_STATUS_SUCCESS;
}

#endif

static uint32_t am_hal_card_cache_on(am_hal_card_t *pCard)
{
    uint32_t ui32Mode;
#ifndef AM_HAL_DISABLE_API_VALIDATION
        if ( !pCard || !pCard->pHost )
        {
            AM_HAL_CARD_DEBUG("This card is not connected to a host\n");
            return AM_HAL_STATUS_INVALID_ARG;
        }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if (pCard->ui32CacheSize > 0)
    {
        ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_CACHE_CTRL << 16 | 1 << 8;
        return am_hal_card_mode_switch(pCard, ui32Mode);
    }

    return AM_HAL_STATUS_INVALID_OPERATION;
}

static uint32_t am_hal_card_cache_off(am_hal_card_t *pCard)
{
    uint32_t ui32Mode;
#ifndef AM_HAL_DISABLE_API_VALIDATION
        if ( !pCard || !pCard->pHost )
        {
            AM_HAL_CARD_DEBUG("This card is not connected to a host\n");
            return AM_HAL_STATUS_INVALID_ARG;
        }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if (pCard->ui32CacheSize > 0)
    {
        ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_CACHE_CTRL << 16;
        return am_hal_card_mode_switch(pCard, ui32Mode);
    }

    return AM_HAL_STATUS_INVALID_OPERATION;
}


static uint32_t am_hal_card_mmc_init(am_hal_card_t *pCard)
{
    //
    // Reset the card
    //
    if ( am_hal_sdmmc_cmd0_go_idle(pCard) != AM_HAL_CMD_ERR_NONE )
    {
        AM_HAL_CARD_DEBUG("CMD0 Failed\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Set the operation condition of the card
    //
    if ( am_hal_sdmmc_cmd1_send_op_cond(pCard) != AM_HAL_CMD_ERR_NONE )
    {
        AM_HAL_CARD_DEBUG("CMD1 Failed\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Get the CID information of the card
    //
    if ( am_hal_sdmmc_cmd2_send_cid(pCard) != AM_HAL_CMD_ERR_NONE )
    {
        AM_HAL_CARD_DEBUG("CMD2 Failed\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Set relative card address to 0x1
    //
    if ( am_hal_sdmmc_cmd3_set_rca(pCard, 0x1) != AM_HAL_CMD_ERR_NONE )
    {
        AM_HAL_CARD_DEBUG("CMD3 Failed\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Get the card CSD
    //
    if ( am_hal_sdmmc_cmd9_send_csd(pCard) != AM_HAL_CMD_ERR_NONE )
    {
        AM_HAL_CARD_DEBUG("CMD9 Failed\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Select the card
    //
    if ( am_hal_sdmmc_cmd7_card_select(pCard) != AM_HAL_CMD_ERR_NONE )
    {
        AM_HAL_CARD_DEBUG("CMD7 Failed\n");
        return AM_HAL_STATUS_FAIL;
    }

#ifndef REMOVE_EXT_CSD_READ_ISSUE_WORKAROUND
    if (am_hal_cmd19_cmd14_read_extcsd_workaround(pCard) != AM_HAL_STATUS_SUCCESS)
    {
        AM_HAL_CARD_DEBUG("Failed to workaround the read EXT CSD issue\n");
        return AM_HAL_STATUS_FAIL;
    }
#endif

    //
    // Get the ext csd
    //
    if ( am_hal_sdmmc_cmd8_send_ext_csd(pCard) != AM_HAL_CMD_ERR_NONE )
    {
       AM_HAL_CARD_DEBUG("CMD8 Failed\n");
       return AM_HAL_STATUS_FAIL;
    }

    //
    // Parse the CSD and EXT CSD to know the feature of card
    //
    uint32_t ui32CSize = am_hal_unstuff_bits(pCard->ui32CSD, 62, 12);
    pCard->bHighCapcity = (ui32CSize == 0xFFF) ? true : false;

    //
    // High capacity card
    //
    if ( pCard->bHighCapcity )
    {
        // get blksize and capacity information from the ext csd
        pCard->ui16CmdClass = am_hal_unstuff_bits(pCard->ui32CSD, 84, 12);
        pCard->ui8SpecVer = am_hal_unstuff_bits(pCard->ui32CSD, 122, 4);
        pCard->ui32MaxBlks = am_hal_unstuff_bytes(pCard->ui32ExtCSD, 212, 4);
        pCard->ui32BlkSize = am_hal_unstuff_bytes(pCard->ui32ExtCSD, 61, 1) ? 4096 : 512;
        pCard->bUseBlkEmulation = am_hal_unstuff_bytes(pCard->ui32ExtCSD, 62, 1) ? false : true;
        pCard->ui32NativeBlkSize = am_hal_unstuff_bytes(pCard->ui32ExtCSD, 63, 1) ? 4096 : 512;
        pCard->ui32Capacity = pCard->ui32MaxBlks*pCard->ui32BlkSize;
        pCard->ui8DeviceType = am_hal_unstuff_bytes(pCard->ui32ExtCSD, 196, 1);
        pCard->ui32CacheSize = am_hal_unstuff_bytes(pCard->ui32ExtCSD, 249, 4);
    }
    else
    {
      // non high capacity card gets blksize and capacity information from the CSD

    }

    am_hal_card_cache_on(pCard);

    return AM_HAL_STATUS_SUCCESS;
}

static uint32_t am_hal_card_sd_init(am_hal_card_t *pCard)
{
    AM_HAL_CARD_DEBUG("card type %d is not supported yet\n", pCard->eType);
    return AM_HAL_STATUS_FAIL;
}

static uint32_t am_hal_card_type_detect(am_hal_card_t *pCard)
{
    AM_HAL_CARD_DEBUG("card type detect is not supported yet\n");
    return AM_HAL_STATUS_FAIL;
}

static uint32_t am_hal_card_set_bus_width(am_hal_card_t *pCard, am_hal_host_bus_width_e eBusWidth)
{
    uint32_t ui32Mode;
    uint32_t ui32Status;

    am_hal_card_host_t *pHost;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    pHost = pCard->pHost;
    ui32Mode = 0x0;
    if ( eBusWidth != pHost->eBusWidth || pHost->eUHSMode != pCard->cfg.eUHSMode)
    {
        switch ( eBusWidth )
        {
            case AM_HAL_HOST_BUS_WIDTH_1:
                // 3 << 24 | 183 << 16 | 0 << 8
                // 0x03b70000
                ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_BUS_WIDTH | MMC_EXT_SET_BUS_WIDTH1;
                break;
            case AM_HAL_HOST_BUS_WIDTH_4:
                // 3 << 24 | 183 << 16 | 1 << 8
                // 0x03b70100
                if (pCard->cfg.eUHSMode == AM_HAL_HOST_UHS_DDR50)
                {
                    ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_BUS_WIDTH | MMC_EXT_SET_BUS_WIDTH4_DDR;
                }
                else
                {
                    ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_BUS_WIDTH | MMC_EXT_SET_BUS_WIDTH4;
                }
                break;
            case AM_HAL_HOST_BUS_WIDTH_8:
                // 3 << 24 | 183 << 16 | 2 << 8
                // 0x03b70200
                if (pCard->cfg.eUHSMode == AM_HAL_HOST_UHS_DDR50)
                {
                    ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_BUS_WIDTH | MMC_EXT_SET_BUS_WIDTH8_DDR;
                }
                else
                {
                    ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_BUS_WIDTH | MMC_EXT_SET_BUS_WIDTH8;
                }
                break;
        }

        if ( (ui32Status = am_hal_card_mode_switch(pCard, ui32Mode)) != AM_HAL_STATUS_SUCCESS )
        {
            return ui32Status;
        }


        if ( pHost->ops->set_bus_width(pHost->pHandle, eBusWidth) != AM_HAL_STATUS_SUCCESS )
        {
            return AM_HAL_STATUS_FAIL;
        }
    }

    return AM_HAL_STATUS_SUCCESS;
}


static uint32_t am_hal_card_set_uhs_mode(am_hal_card_t *pCard, am_hal_host_uhs_mode_e eUHSMode)
{
    am_hal_card_host_t *pHost;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    pHost = pCard->pHost;

    //
    // Already in this UHS mode
    //
    if ( pHost->eUHSMode == eUHSMode )
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    if ( eUHSMode == AM_HAL_HOST_UHS_DDR50 )
    {
        if (pCard->eType == AM_HAL_CARD_TYPE_EMMC || pCard->eType == AM_HAL_CARD_TYPE_MMC)
        {
            //
            // Card does not support DDR52 mode
            //
            if ((pCard->ui8DeviceType & HS_DDR_52MHZ_18_3V) == 0x0)
            {
                return AM_HAL_STATUS_FAIL;
            }

            if (pHost->ops->set_uhs_mode)
            {
                return pHost->ops->set_uhs_mode(pHost->pHandle, eUHSMode);
            }
        }
        else
        {
            if (pHost->ops->set_uhs_mode)
            {
                return pHost->ops->set_uhs_mode(pHost->pHandle, eUHSMode);
            }
        }
    }
    else
    {
        if (pCard->eType == AM_HAL_CARD_TYPE_EMMC || pCard->eType == AM_HAL_CARD_TYPE_MMC)
        {
            if (pHost->ops->set_uhs_mode)
            {
                return pHost->ops->set_uhs_mode(pHost->pHandle, AM_HAL_HOST_UHS_NONE);
            }
        }
        else
        {
            if (pHost->ops->set_uhs_mode)
            {
                return pHost->ops->set_uhs_mode(pHost->pHandle, eUHSMode);
            }
        }
    }

    return AM_HAL_STATUS_SUCCESS;
}


static uint32_t am_hal_card_set_voltage(am_hal_card_t *pCard, am_hal_host_bus_voltage_e eBusVoltage)
{
    am_hal_card_host_t *pHost;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    pHost = pCard->pHost;

    if ( eBusVoltage != pHost->eBusVoltage )
    {
        if ( pHost->ops->set_bus_voltage(pHost->pHandle, eBusVoltage) != AM_HAL_STATUS_SUCCESS )
        {
            return AM_HAL_STATUS_FAIL;
        }
    }

    return AM_HAL_STATUS_SUCCESS;
}

static uint32_t am_hal_card_set_speed(am_hal_card_t *pCard, uint32_t ui32Clock)
{
    uint32_t ui32Status;
    uint32_t ui32Mode;
    am_hal_card_host_t *pHost;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    pHost = pCard->pHost;

    if ( ui32Clock > pHost->ui32MaxClock || ui32Clock < pHost->ui32MinClock ||
        ((ui32Clock >= 26000000) && (pCard->ui8SpecVer < 4)) )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if ( (pCard->ui8SpecVer >= 4) && (ui32Clock >= 26000000) )
    {
        //
        // Already in the high speed mode
        //
        if ( am_hal_unstuff_bytes(pCard->ui32ExtCSD, 185, 1) != 0x1 )
        {
            // 0x03B90100
            ui32Mode = MMC_EXT_MODE_WRITE_BYTE | MMC_EXT_REGS_HIGH_SPEED | MMC_EXT_SET_HIGH_SPEED;
            if ( (ui32Status = am_hal_card_mode_switch(pCard, ui32Mode)) != AM_HAL_STATUS_SUCCESS )
            {
                return ui32Status;
            }

            //
            // read back the ext_csd and check 'HS_TIMING' is set or not
            //
            if ( am_hal_sdmmc_cmd8_send_ext_csd(pCard) != AM_HAL_CMD_ERR_NONE )
            {
                return AM_HAL_STATUS_FAIL;
            }

            if ( am_hal_unstuff_bytes(pCard->ui32ExtCSD, 185, 1) != 0x1 )
            {
                AM_HAL_CARD_DEBUG("%d - Failed to switch to high speed mode\n", __FUNCTION__);
                return AM_HAL_STATUS_FAIL;
            }
        }
    }

    if ( pHost->ops->set_bus_clock(pHost->pHandle, ui32Clock) != AM_HAL_STATUS_SUCCESS )
    {
        return AM_HAL_STATUS_FAIL;
    }

    return AM_HAL_STATUS_SUCCESS;
}

//
// Static helper function:
//  Count the longest consecutive 1s in a 32bit word
//
static uint32_t
count_consecutive_ones(uint32_t* pVal)
{
    uint32_t count = 0;
    uint32_t data = *pVal;

    while ( data )
    {
        data = (data & (data << 1));
        count++;
    }
    return count;
}

//
// Static helper function:
//  Find and return the mid point of the longest continuous 1s in a 32bit word
//
static uint32_t
find_mid_point(uint32_t* pVal)
{
    uint32_t pattern_len = 0;
    uint32_t max_len = 0;
    uint32_t pick_point = 0;
    bool pattern_start = false;
    uint32_t val = *pVal;

    for ( uint32_t i = 0; i < 32; i++ )
    {
        if ( val & (0x01 << i) )
        {
            pattern_start = true;
            pattern_len++;
        }
        else
        {
            if ( pattern_start == true )
            {
                pattern_start = false;
                if ( pattern_len > max_len )
                {
                    max_len = pattern_len;
                    pick_point = i - 1 - pattern_len / 2;
                }
                pattern_len = 0;
            }
        }
    }

    return pick_point;
}

uint32_t am_hal_card_emmc_calibrate(am_hal_host_uhs_mode_e eUHSMode,
                                    uint32_t ui32Clock,
                                    am_hal_host_bus_width_e eBusWidth,
                                    uint8_t *ui8CalibBuf,
                                    uint32_t ui32StartBlk,
                                    uint32_t ui32BlkCnt,
                                    uint8_t ui8TxRxDelays[2])
{
    am_hal_card_t eMMCard;
    am_hal_card_host_t *pSdhcCardHost;

    uint32_t i;
    uint32_t len = ui32BlkCnt*512;
    uint8_t  ui8TxDelay = 0;
    uint8_t  ui8RxDelay = 0;
    uint32_t ui32TxResult = 0;
    uint32_t ui32RxResultArray[16] = {0};

#ifdef AM_DEBUG_PRINTF
    if (eUHSMode == AM_HAL_HOST_UHS_DDR50)
    {
        AM_HAL_CARD_DEBUG("eMMC Timing Scan for DDR %d Hz %d bit mode...\n", ui32Clock, eBusWidth);
    }
    else
    {
        AM_HAL_CARD_DEBUG("eMMC Timing Scan for SDR %d Hz %d bit mode...\n", ui32Clock, eBusWidth);
    }
#endif

    for (int j = 0; j < 16; j++)
    {
        for (int k = 0; k < 32; k++)
        {
            //
            // Get the uderlying SDHC card host instance
            //
            pSdhcCardHost = am_hal_get_card_host(AM_HAL_SDHC_CARD_HOST, true);

            if (pSdhcCardHost == NULL)
            {
                AM_HAL_CARD_DEBUG("No such card host and stop\n");
                return AM_HAL_STATUS_FAIL;
            }

            ui8TxRxDelays[0] = j;
            ui8TxRxDelays[1] = k;
            AM_HAL_CARD_DEBUG("Test TX delay setting SDIOOTAPDLYSEL = %d, RX delay setting SDIOITAPDLYSEL = %d\n", j, k);
            am_hal_card_host_set_txrx_delay(pSdhcCardHost, ui8TxRxDelays);

            //
            // check if card is present
            //
            if (am_hal_card_host_find_card(pSdhcCardHost, &eMMCard) != AM_HAL_STATUS_SUCCESS)
            {
                AM_HAL_CARD_DEBUG("No card is present now\n");
                return AM_HAL_STATUS_FAIL;
            }

            if (am_hal_card_init(&eMMCard, NULL, AM_HAL_CARD_PWR_CTRL_NONE) != AM_HAL_STATUS_SUCCESS)
            {
                AM_HAL_CARD_DEBUG("card initialization failed\n");
                return AM_HAL_STATUS_FAIL;
            }

            if (am_hal_card_cfg_set(&eMMCard, AM_HAL_CARD_TYPE_EMMC,
                eBusWidth, ui32Clock, AM_HAL_HOST_BUS_VOLTAGE_1_8,
                eUHSMode) != AM_HAL_STATUS_SUCCESS)
            {
                AM_HAL_CARD_DEBUG("card config failed\n");
                continue;
            }

            for (i = 0; i < len; i++)
            {
                ui8CalibBuf[i] = i % 256;
            }

            am_hal_card_block_write_sync(&eMMCard, ui32StartBlk, ui32BlkCnt, (uint8_t *)ui8CalibBuf);

            memset((void *)ui8CalibBuf, 0x0, len);
            am_hal_card_block_read_sync(&eMMCard, ui32StartBlk, ui32BlkCnt, (uint8_t *)ui8CalibBuf);

            for (i = 0; i < len; i++)
            {
                if (ui8CalibBuf[i] != i % 256)
                {
                    break;
                }
            }

            if (i == len)
            {
                ui32RxResultArray[j] |= 0x01 << k;
                AM_HAL_CARD_DEBUG("TX Delay SDIOOTAPDLYSEL = %d, RX Delay SDIOITAPDLYSEL = %d works\n", j, k);
            }
            else
            {
                AM_HAL_CARD_DEBUG("TX Delay SDIOOTAPDLYSEL = %d, RX Delay SDIOITAPDLYSEL = %d doesn't work\n", j, k);
            }
        }
    }

#ifdef AM_DEBUG_PRINTF
    AM_HAL_CARD_DEBUG("\nSDIO TX RX Delay Scan Result:\n");
    for (i = 0; i < 16; i++ )
    {
        AM_HAL_CARD_DEBUG("TX_Delay = %2d, RX_Delay Window = 0x%08X\n", i, ui32RxResultArray[i]);
    }
#endif

    uint32_t ui32Result = 0;
    for (i = 0; i < 16; i++ )
    {
        ui32Result = count_consecutive_ones(&ui32RxResultArray[i]);
        if ( ui32Result >= SDIO_TIMING_SCAN_MIN_ACCEPTANCE_LENGTH )
        {
            ui32TxResult |= 0x01 << i;
        }
    }

    //
    // Check consecutive passing settings
    //
    if ( ui32TxResult == 0 )
    {
        // No acceptable window
        return AM_HAL_STATUS_FAIL;
    }
    else
    {
        ui32Result = count_consecutive_ones(&ui32TxResult);
        if ( ui32Result < SDIO_TIMING_SCAN_MIN_ACCEPTANCE_LENGTH )
        {
            // No acceptable window
            return AM_HAL_STATUS_FAIL;
        }
    }

    //
    // Find TX Delay Value
    //
    ui8TxDelay = find_mid_point(&ui32TxResult);

    AM_HAL_CARD_DEBUG("\nSDIO Timing Scan found a RX delay window %X with TX delay Setting = %d.\n", ui32RxResultArray[ui8TxDelay], ui8TxDelay);

    //
    // Find RX Delay Value
    //
    ui8RxDelay = find_mid_point(&ui32RxResultArray[ui8TxDelay]);
    ui8TxRxDelays[0] = ui8TxDelay;
    ui8TxRxDelays[1] = ui8RxDelay;

    AM_HAL_CARD_DEBUG("Timing Scan set the SDIO TX delay to %d and RX delay to %d.\n", ui8TxDelay, ui8RxDelay);

    //
    // Force the card host to reset with the calibration settings
    //
    pSdhcCardHost = am_hal_get_card_host(AM_HAL_SDHC_CARD_HOST, true);

    if (pSdhcCardHost == NULL)
    {
        AM_HAL_CARD_DEBUG("No such card host and stop\n");
        return AM_HAL_STATUS_FAIL;
    }
    am_hal_card_host_set_txrx_delay(pSdhcCardHost, ui8TxRxDelays);

    return AM_HAL_STATUS_SUCCESS;
}


//
// Public functions
//

uint32_t am_hal_card_pwrctrl_sleep(am_hal_card_t *pCard)
{
    am_hal_card_host_t *pHost;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    pHost = pCard->pHost;

    switch (pCard->eCardPwrCtrlPolicy)
    {
        case AM_HAL_CARD_PWR_CTRL_SDHC_AND_CARD_OFF:
            if (pCard->pCardPwrCtrlFunc)
            {
                //
                // Flush the cache before power off the eMMC card
                //
                am_hal_card_cache_off(pCard);

                //
                // Turn off the power supply of eMMC card
                //
                pCard->pCardPwrCtrlFunc(AM_HAL_CARD_PWR_OFF);
                pCard->eState = AM_HAL_CARD_STATE_PWROFF;
            }

            return pHost->ops->deinit(pHost->pHandle);
        case AM_HAL_CARD_PWR_CTRL_SDHC_OFF:
            //
            // Power off the SDHC controller
            //
            return pHost->ops->pwr_ctrl(pHost->pHandle, false);
        default:
            return AM_HAL_STATUS_SUCCESS;
    }
}

uint32_t am_hal_card_pwrctrl_wakeup(am_hal_card_t *pCard)
{
    uint32_t ui32Status;
    am_hal_card_host_t *pHost;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    pHost = pCard->pHost;

    switch (pCard->eCardPwrCtrlPolicy)
    {
        case AM_HAL_CARD_PWR_CTRL_SDHC_AND_CARD_OFF:
            //
            // ReInit the SDHC controller
            //
            if ((ui32Status = pHost->ops->init(pHost)) != AM_HAL_STATUS_SUCCESS)
            {
                AM_HAL_CARD_DEBUG("wakeup reinit sdhc controller failed\n");
                return ui32Status;
            }

            //
            // Turn on the eMMC card power supply firstly
            //
            if (pCard->eState == AM_HAL_CARD_STATE_PWROFF && pCard->pCardPwrCtrlFunc != NULL)
            {
                AM_HAL_CARD_DEBUG("turn on the eMMC power supply\n");
                pCard->pCardPwrCtrlFunc(AM_HAL_CARD_PWR_ON);
            }

            //
            // ReInit the card again
            //
            if ((ui32Status = am_hal_card_init(pCard,
                pCard->pCardPwrCtrlFunc, pCard->eCardPwrCtrlPolicy)) != AM_HAL_STATUS_SUCCESS)
            {
                AM_HAL_CARD_DEBUG("wakeup reinit sd card failed\n");
                return AM_HAL_STATUS_FAIL;
            }

            if (am_hal_card_cfg_set(pCard, pCard->eType, pCard->cfg.eBusWidth,
                pCard->cfg.ui32Clock, pCard->cfg.eIoVoltage, pCard->cfg.eUHSMode) != AM_HAL_STATUS_SUCCESS)
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CARD_PWR_CTRL_SDHC_OFF:
            //
            // Powrer on the SDHC controller
            //
            if ((ui32Status = pHost->ops->pwr_ctrl(pHost->pHandle, true)) != AM_HAL_STATUS_SUCCESS)
            {
                AM_HAL_CARD_DEBUG("power on the sdhc controller failed\n");
                return ui32Status;
            }

#if 0
            am_hal_cmd19_cmd14_read_extcsd_workaround(pCard);
#endif
            break;
        default:
            break;

    }

    return AM_HAL_STATUS_SUCCESS;
}

uint32_t am_hal_card_host_find_card(am_hal_card_host_t *pHost, am_hal_card_t *pCard)
{

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pHost || !pCard )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION


    if ( pHost->bCardInSlot ||
        pHost->ops->get_cd(pHost->pHandle) )
    {
        pCard->eState = AM_HAL_CARD_STATE_PRESENT;
        pCard->pHost  = pHost;
        //
        // Fill the default setting with the host's current value
        //
        pCard->cfg.eBusWidth = pHost->eBusWidth;
        pCard->cfg.eIoVoltage = pHost->eBusVoltage;
        pCard->cfg.ui32Clock = pHost->ui32MinClock;
        pCard->cfg.eUHSMode = pHost->eUHSMode;
        pCard->eState = AM_HAL_CARD_STATE_PWROFF;
        return AM_HAL_STATUS_SUCCESS;
    }
    else
    {
        pCard->eState = AM_HAL_CARD_STATE_NOT_PRESENT;
        return AM_HAL_STATUS_FAIL;
    }
}


uint32_t am_hal_card_cfg_set(am_hal_card_t *pCard, am_hal_card_type_e eType,
                             am_hal_host_bus_width_e eBusWidth,
                             uint32_t ui32Clock,
                             am_hal_host_bus_voltage_e eIoVoltage,
                             am_hal_host_uhs_mode_e eUHSMode)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        AM_HAL_CARD_DEBUG("This card is not connected to a host\n");
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( pCard->eState != AM_HAL_CARD_STATE_TRANS )
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    pCard->eType = eType;
    pCard->cfg.eBusWidth = eBusWidth;
    pCard->cfg.ui32Clock = ui32Clock;
    pCard->cfg.eIoVoltage = eIoVoltage;
    pCard->cfg.eUHSMode = eUHSMode;

    if (am_hal_card_set_voltage(pCard, pCard->cfg.eIoVoltage) != AM_HAL_STATUS_SUCCESS )
    {
        AM_HAL_CARD_DEBUG("Failed to change bus voltage\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Configure the card bus speed
    //
    if ( am_hal_card_set_speed(pCard, pCard->cfg.ui32Clock) != AM_HAL_STATUS_SUCCESS )
    {
        AM_HAL_CARD_DEBUG("Failed to change bus speed\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Configure the card bus width
    //
    if ( am_hal_card_set_bus_width(pCard, pCard->cfg.eBusWidth) != AM_HAL_STATUS_SUCCESS )
    {
        AM_HAL_CARD_DEBUG("Failed to change bus width\n");
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Select the UHS mode
    //
    if (am_hal_card_set_uhs_mode(pCard, pCard->cfg.eUHSMode) != AM_HAL_STATUS_SUCCESS)
    {
        AM_HAL_CARD_DEBUG("Failed to change UHS mode\n");
        return AM_HAL_STATUS_FAIL;
    }

    return AM_HAL_STATUS_SUCCESS;
}

uint32_t
am_hal_card_init(am_hal_card_t *pCard,
                 am_hal_card_pwr_ctrl_func pCardPwrCtrlFunc,
                 am_hal_card_pwr_ctrl_policy_e eCardPwrCtrlPolicy)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        AM_HAL_CARD_DEBUG("This card is not connected to a host\n");
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Turn on the eMMC card power supply firstly
    //
    if (pCard->eState == AM_HAL_CARD_STATE_PWROFF && pCardPwrCtrlFunc != NULL)
    {
        AM_HAL_CARD_DEBUG("turn on the eMMC power supply\n");
        pCardPwrCtrlFunc(AM_HAL_CARD_PWR_ON);
    }

    pCard->pCardPwrCtrlFunc = pCardPwrCtrlFunc;
    pCard->eState = AM_HAL_CARD_STATE_PWRON;
    pCard->eCardPwrCtrlPolicy = eCardPwrCtrlPolicy;

    AM_HAL_CARD_DEBUG("card type is %d\n", pCard->eType);

    // Note: remove below code unless 'am_hal_card_type_detect' is finished
    pCard->eType = AM_HAL_CARD_TYPE_EMMC;

    switch (pCard->eType)
    {
        case AM_HAL_CARD_TYPE_UNKNOWN:
            if ( am_hal_card_type_detect(pCard) != AM_HAL_STATUS_SUCCESS )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CARD_TYPE_MMC:
        case AM_HAL_CARD_TYPE_EMMC:
            if ( am_hal_card_mmc_init(pCard) != AM_HAL_STATUS_SUCCESS )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
        case AM_HAL_CARD_TYPE_SDSC:
        case AM_HAL_CARD_TYPE_SDHC:
        case AM_HAL_CARD_TYPE_SDIO:
        case AM_HAL_CARD_TYPE_COMBO:
            if ( am_hal_card_sd_init(pCard) != AM_HAL_STATUS_SUCCESS )
            {
                return AM_HAL_STATUS_FAIL;
            }
            break;
    }

    return AM_HAL_STATUS_SUCCESS;
}

uint32_t am_hal_card_deinit(am_hal_card_t *pCard)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        AM_HAL_CARD_DEBUG("This card is not connected to a host\n");
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    memset((void *)pCard, 0x0, sizeof(am_hal_card_t));

    return AM_HAL_STATUS_SUCCESS;
}

#ifdef  DSP_RAM1_WORKAROUND

#define DSP_RAM1_START_ADDR 0x101C4000
#define DSP_RAM1_SIZE       96*1024  // 96KB

#define DSP_RAM1_TMP_ADDR   DSP_RAM1_START_ADDR

#endif

#define ENABLE_SDHC_AUTO_CMD23_FEATURE

static uint32_t am_hal_card_block_rw(am_hal_card_t *pCard, uint32_t ui32Blk, uint32_t ui32BlkCnt, uint8_t *pui8Buf, bool bRead, bool bASync)
{
    uint32_t ui32Status;

    am_hal_card_host_t *pHost;
    am_hal_card_cmd_t cmd;
    am_hal_card_cmd_data_t cmd_data;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( pCard->eState != AM_HAL_CARD_STATE_TRANS )
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    pHost = pCard->pHost;

    //
    // Check if the start block number and block count is valid or not
    //
    if ( ui32Blk >= pCard->ui32MaxBlks || (ui32BlkCnt > 0xFFFF) || (ui32Blk + ui32BlkCnt) > pCard->ui32MaxBlks )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if ( pHost->eXferMode == AM_HAL_HOST_XFER_ADMA && ui32BlkCnt > pHost->ui32MaxADMA2BlkNums )
    {
        ui32BlkCnt = pHost->ui32MaxADMA2BlkNums;
    }

#ifdef DSP_RAM1_WORKAROUND
    if (bRead == false)
    {
        memcpy((void *)DSP_RAM1_TMP_ADDR, pui8Buf, ui32BlkCnt*512);
        pui8Buf = (uint8_t *)DSP_RAM1_TMP_ADDR;
    }
#endif

#ifndef ENABLE_SDHC_AUTO_CMD23_FEATURE
    //
    // Send CMD23 firstly for mulitple blocks transfer
    //
    memset((void *)&cmd, 0x0, sizeof(cmd));
    if ( ui32BlkCnt > 1 )
    {
        cmd.ui8Idx = MMC_CMD_SET_BLOCK_COUNT;
        cmd.ui32Arg = ui32BlkCnt;
        cmd.ui32RespType = MMC_RSP_R1;

        if ( (ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL)) != AM_HAL_STATUS_SUCCESS )
        {
            return ui32Status;
        }
    }
#endif

    memset((void *)&cmd, 0x0, sizeof(cmd));
    memset((void *)&cmd_data, 0x0, sizeof(cmd_data));
    if ( bRead )
    {
        cmd.ui8Idx = ui32BlkCnt > 1 ? MMC_CMD_READ_MULTIPLE_BLOCK : MMC_CMD_READ_SINGLE_BLOCK;
    }
    else
    {
        cmd.ui8Idx = ui32BlkCnt > 1 ? MMC_CMD_WRITE_MULTIPLE_BLOCK : MMC_CMD_WRITE_SINGLE_BLOCK;
    }

    cmd.ui32Arg = pCard->bHighCapcity ? ui32Blk : ui32Blk * pCard->ui32BlkSize;
    cmd.ui32RespType = MMC_RSP_R1;
    cmd.bASync = bASync;
#ifdef ENABLE_SDHC_AUTO_CMD23_FEATURE
    cmd.bAutoCMD23 = ui32BlkCnt > 1 ? true : false;
#endif

    cmd_data.pui8Buf = pui8Buf;
    cmd_data.ui32BlkCnt = ui32BlkCnt;
    cmd_data.ui32BlkSize = pCard->ui32BlkSize;
    cmd_data.dir = bRead ? AM_HAL_DATA_DIR_READ : AM_HAL_DATA_DIR_WRITE;

    if ( cmd.bASync )
    {
        pHost->AsyncCmd = cmd;
        pHost->AsyncCmdData = cmd_data;
    }

    ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, &cmd_data);

    return ui32Status;
}

uint32_t am_hal_card_block_read_sync(am_hal_card_t *pCard, uint32_t ui32Blk, uint32_t ui32BlkCnt, uint8_t *pui8Buf)
{
    return am_hal_card_block_rw(pCard, ui32Blk, ui32BlkCnt, pui8Buf, true, false);
}

uint32_t am_hal_card_block_write_sync(am_hal_card_t *pCard, uint32_t ui32Blk, uint32_t ui32BlkCnt, uint8_t *pui8Buf)
{
    return am_hal_card_block_rw(pCard, ui32Blk, ui32BlkCnt, pui8Buf, false, false);
}


uint32_t am_hal_card_block_read_async(am_hal_card_t *pCard, uint32_t ui32Blk, uint32_t ui32BlkCnt, uint8_t *pui8Buf)
{
    return am_hal_card_block_rw(pCard, ui32Blk, ui32BlkCnt, pui8Buf, true, true);
}

uint32_t am_hal_card_block_write_async(am_hal_card_t *pCard, uint32_t ui32Blk, uint32_t ui32BlkCnt, uint8_t *pui8Buf)
{
    return am_hal_card_block_rw(pCard, ui32Blk, ui32BlkCnt, pui8Buf, false, true);
}


uint32_t am_hal_card_block_erase(am_hal_card_t *pCard, uint32_t ui32Blk, uint32_t ui32BlkCnt)
{
    uint32_t ui32Status;
    am_hal_card_host_t *pHost;
    am_hal_card_cmd_t cmd;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( pCard->eState != AM_HAL_CARD_STATE_TRANS )
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Check if the start block number and block count is valid or not
    //
    if ( ui32Blk >= pCard->ui32MaxBlks || ui32Blk + ui32BlkCnt >= pCard->ui32MaxBlks )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    pHost = pCard->pHost;

    //
    // Define the erase start address
    //
    memset((void *)&cmd, 0x0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_ERASE_GROUP_START;
    cmd.ui32Arg = pCard->bHighCapcity ? ui32Blk : ui32Blk * pCard->ui32BlkSize;
    cmd.ui32RespType = MMC_RSP_R1;

    if ( (ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL)) != AM_HAL_STATUS_SUCCESS )
    {
        return ui32Status;
    }

    //
    // Define the erase end address
    //
    memset((void *)&cmd, 0x0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_ERASE_GROUP_END;
    cmd.ui32Arg = pCard->bHighCapcity ? (ui32Blk + ui32BlkCnt - 1) : (ui32Blk + ui32BlkCnt - 1) * pCard->ui32BlkSize;
    cmd.ui32RespType = MMC_RSP_R1;

    if ( (ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL)) != AM_HAL_STATUS_SUCCESS )
    {
        return ui32Status;
    }

    //
    // Do the erase job
    //
    memset((void *)&cmd, 0x0, sizeof(cmd));
    cmd.bCheckBusyCmd = true;
    cmd.ui8Idx = MMC_CMD_ERASE;
    cmd.ui32Arg = (uint32_t)AM_HAL_TRIM;
    cmd.ui32RespType = MMC_RSP_R1b;

    if ( (ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL)) != AM_HAL_STATUS_SUCCESS )
    {
        return ui32Status;
    }

    return pHost->ops->card_busy(pHost->pHandle, 100);
}


uint32_t am_hal_card_status(am_hal_card_t *pCard, uint32_t *pui32Status)
{
    uint32_t ui32Status;
    am_hal_card_cmd_t cmd;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( pCard->eState < AM_HAL_CARD_STATE_STDY )
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_SEND_STATUS;
    cmd.ui32Arg = pCard->ui8RCA << 16;
    cmd.ui32RespType = MMC_RSP_R1;

    if ( (ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL)) != AM_HAL_STATUS_SUCCESS )
    {
        return ui32Status;
    }

    if ( cmd.eError == AM_HAL_CMD_ERR_NONE )
    {
        *pui32Status = cmd.ui32Resp[0];
    }

    return ui32Status;
}

am_hal_card_state_e am_hal_card_state(am_hal_card_t *pCard)
{
    uint32_t ui32Status = 0;
    uint32_t ui32State;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_CARD_STATE_ERROR;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( am_hal_card_status(pCard, &ui32Status) != AM_HAL_STATUS_SUCCESS )
    {
        return AM_HAL_CARD_STATE_ERROR;
    }
    else
    {
        ui32State = ui32Status >> 9 & 0xF;
        if ( ui32State <= 10 )
        {
            pCard->eState = (am_hal_card_state_e)(ui32State + 2);
        }
        else
        {
            return AM_HAL_CARD_STATE_ERROR;
        }
    }

    return pCard->eState;
}

//
// CMD6 - Mode switch
//

uint32_t am_hal_card_mode_switch(am_hal_card_t *pCard, uint32_t ui32Mode)
{
    uint32_t ui32Status;
    uint32_t ui32CardStatus;
    uint32_t ui32Timeout;
    am_hal_card_cmd_t cmd;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( pCard->eState != AM_HAL_CARD_STATE_TRANS )
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    am_hal_card_host_t *pHost = pCard->pHost;

    memset(&cmd, 0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_SWITCH;
    cmd.ui32Arg = ui32Mode;
    pCard->bExtCsdValid = 0;

    if ( pCard->eType == AM_HAL_CARD_TYPE_MMC || pCard->eType == AM_HAL_CARD_TYPE_EMMC )
    {
        cmd.bCheckBusyCmd = true;
        cmd.ui32RespType = MMC_RSP_R1b;
    }
    else
    {
        cmd.ui32RespType = MMC_RSP_R1;
    }

    if ( (ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, NULL)) != AM_HAL_STATUS_SUCCESS )
    {
        return ui32Status;
    }

    if ( pCard->eType == AM_HAL_CARD_TYPE_MMC || pCard->eType == AM_HAL_CARD_TYPE_EMMC )
    {
        if ((ui32Status = pHost->ops->card_busy(pHost->pHandle, 10)) != AM_HAL_STATUS_SUCCESS)
        {
            return ui32Status;
        }
    }

    ui32CardStatus = 0x0;
    ui32Timeout = DEFAULT_CMD6_TIMEOUT_MS;
    do
    {
        ui32Status = am_hal_card_status(pCard, &ui32CardStatus);
        if (ui32Status == AM_HAL_STATUS_SUCCESS && (ui32CardStatus & MMC_STATUS_SWITCH_ERROR))
        {
            AM_HAL_CARD_DEBUG("switch failed - mode is 0x%x !\n", ui32Mode);
            return AM_HAL_STATUS_FAIL;
        }
        if (ui32Status == AM_HAL_STATUS_SUCCESS && (ui32CardStatus & MMC_STATUS_RDY_FOR_DATA))
        {
            AM_HAL_CARD_DEBUG("switch succeeded - mode is 0x%x !\n", ui32Mode);
            return AM_HAL_STATUS_SUCCESS;
        }
        am_util_delay_ms(1);
    } while (ui32Timeout--);

    return ui32Timeout ? AM_HAL_STATUS_SUCCESS : AM_HAL_STATUS_FAIL;
}

static uint32_t am_hal_card_cmd56_read(am_hal_card_t *pCard, uint32_t ui32Arg, uint8_t *pui8Buf, bool bASync)
{
    uint32_t ui32Status;

    am_hal_card_host_t *pHost;
    am_hal_card_cmd_t cmd;
    am_hal_card_cmd_data_t cmd_data;

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( pCard->eState != AM_HAL_CARD_STATE_TRANS )
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    pHost = pCard->pHost;

    memset((void *)&cmd, 0x0, sizeof(cmd));
    cmd.ui8Idx = MMC_CMD_CMD56;
    cmd.ui32Arg = ui32Arg;
    cmd.ui32RespType = MMC_RSP_R1;
    cmd.bASync = bASync;

    memset((void *)&cmd_data, 0x0, sizeof(cmd_data));
    cmd_data.pui8Buf = pui8Buf;
    cmd_data.ui32BlkCnt = 1;
    cmd_data.ui32BlkSize = 512;
    cmd_data.dir = AM_HAL_DATA_DIR_READ;

    if ( cmd.bASync )
    {
        pHost->AsyncCmd = cmd;
        pHost->AsyncCmdData = cmd_data;
    }

    ui32Status = pHost->ops->execute_cmd(pHost->pHandle, &cmd, &cmd_data);

    return ui32Status;
}

uint32_t am_hal_card_cmd56_read_async(am_hal_card_t *pCard, uint32_t ui32Arg, uint8_t *pui8Buf)
{
    return am_hal_card_cmd56_read(pCard, ui32Arg, pui8Buf, true);
}

uint32_t am_hal_card_cmd56_read_sync(am_hal_card_t *pCard, uint32_t ui32Arg, uint8_t *pui8Buf)
{
    return am_hal_card_cmd56_read(pCard, ui32Arg, pui8Buf, false);
}


uint32_t am_hal_card_register_evt_callback(am_hal_card_t *pCard, am_hal_host_event_cb_t pfunCallback)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        AM_HAL_CARD_DEBUG("both Card and Host are needed to be initialized firstly\n");
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    pCard->pHost->pfunEvtCallback = pfunCallback;

    return AM_HAL_STATUS_SUCCESS;
}

uint32_t am_hal_card_get_cid_field(am_hal_card_t *pCard, uint16_t ui16Offset, uint8_t ui8Size)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->bCidValid )
    {
        return 0xFFFFFFFF;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    return am_hal_unstuff_bits(pCard->ui32CID, ui16Offset, ui8Size);


}

uint32_t am_hal_card_get_csd_field(am_hal_card_t *pCard, uint16_t ui16Offset, uint8_t ui8Size)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->bCsdValid )
    {
        return 0xFFFFFFFF;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    return am_hal_unstuff_bits(pCard->ui32CSD, ui16Offset, ui8Size);

}

uint32_t am_hal_card_get_ext_csd_field(am_hal_card_t *pCard, uint16_t ui16Offset, uint8_t ui8Size)
{

#ifndef AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard || !pCard->pHost)
    {
        return 0xFFFFFFFF;
    }

#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( !pCard->bExtCsdValid )
    {
        if (am_hal_sdmmc_cmd8_send_ext_csd(pCard) != AM_HAL_CMD_ERR_NONE)
        {
            return 0xFFFFFFFF;
        }
        pCard->bExtCsdValid = 1;
    }

    return am_hal_unstuff_bytes(pCard->ui32ExtCSD, ui16Offset, ui8Size);

}

uint32_t am_hal_card_get_info(am_hal_card_t *pCard, am_hal_card_info_t *pCardInfo)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !pCard || !pCard->pHost )
    {
        AM_HAL_CARD_DEBUG("both Card and Host are needed to be initialized firstly\n");
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    pCardInfo->eType        = pCard->eType;
    pCardInfo->ui16CmdClass = pCard->ui16CmdClass;
    pCardInfo->ui8RCA       = pCard->ui8RCA;
    pCardInfo->ui32MaxBlks  = pCard->ui32MaxBlks;
    pCardInfo->ui32BlkSize  = pCard->ui32BlkSize;
    if ( pCard->bHighCapcity )
    {
        pCardInfo->ui32LogiMaxBlks = pCard->ui32MaxBlks;
        pCardInfo->ui32LogiBlkSize = pCard->ui32BlkSize;
    }
    else
    {
        pCardInfo->ui32LogiMaxBlks = (pCard->ui32MaxBlks) * ((pCard->ui32BlkSize) / 512);
        pCardInfo->ui32LogiBlkSize = 512;
    }

    return AM_HAL_STATUS_SUCCESS;
}
