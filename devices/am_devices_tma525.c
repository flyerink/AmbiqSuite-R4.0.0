//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
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
// This is part of revision 2.1.0 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#include "am_mcu_apollo.h"
#include "am_devices_tma525.h"
#include "am_bsp.h"
#include "string.h"
#include "am_util_delay.h"

//*****************************************************************************
//
// Global variables.
//
//*****************************************************************************
static void *g_tma525_IOMHandle;

static void am_hal_iom_i2c_read(uint32_t ui32BusAddress, uint32_t *pBuf, uint32_t size);
static void am_hal_iom_i2c_write(uint32_t ui32BusAddress, uint32_t *pBuf, uint32_t size);
rt_size_t rt_i2c_master_xfer(struct rt_i2c_msg          *msgs, unsigned int num);


rt_err_t ztw_tp_read(uint16_t addr, uint8_t *buffer, unsigned long length)
{
    int ret;
    int retries = 0;

    if (buffer == NULL)
    {
        return -1;
    }

    struct rt_i2c_msg msgs[] =
    {
        {
            .addr   = ZTW523_SLAVE_ADDR,
            .flags  = RT_I2C_WR,
            .len    = sizeof(addr),
            .buf    = (uint8_t *)&addr,
        },
        {
            .addr   = ZTW523_SLAVE_ADDR,
            .flags  = RT_I2C_RD,
            .len    = length,
            .buf    = buffer,
        },
    };

    while (retries < IIC_RETRY_NUM)
    {
        ret = rt_i2c_master_xfer(msgs, 2);

        if ( ret == (msgs[0].len - 1 + msgs[1].len) )
        {
            return RT_EOK;
        }

        retries++;
    }

    // TOUCH_LOG("%s i2c read error: %d", __func__, ret);
    return RT_ERROR;
}

rt_err_t ztw_tp_write(rt_uint16_t addr, rt_uint8_t *buffer, rt_size_t length)
{
    if (buffer == NULL)
    {
        return RT_ERROR;
    }

    uint8_t send_buffer[sizeof(rt_uint16_t) + 32 + 1];
    send_buffer[0] = (addr >> 0) & 0xff;  // reg addr
    send_buffer[1] = (addr >> 8) & 0xff;
    memcpy(send_buffer + sizeof(rt_uint16_t), buffer, length);

    struct rt_i2c_msg msgs[] =
    {
        {
            .addr   = ZTW523_SLAVE_ADDR,
            .flags  = RT_I2C_WR,
            .len    = length + sizeof(addr),
            .buf    = send_buffer,
        }
    };

    if (rt_i2c_master_xfer(msgs, 1) == msgs->len - 1)
    {
        return RT_EOK;
    }
    else
    {
        // TOUCH_LOG("ztw_tp_write rt_i2c_transfer error!\r\n");
        return RT_ERROR;
    }
}

rt_err_t ztw_tp_write_reg(rt_uint16_t addr, rt_uint16_t data)
{
    if (ztw_tp_write(addr, (uint8_t *)&data, sizeof(addr)) == RT_EOK)
    {
        return RT_EOK;
    }
    else
    {
        return RT_ERROR;
    }
}

rt_err_t ztw_tp_read_reg(rt_uint16_t addr, rt_uint16_t *data)
{
    if (ztw_tp_read(addr, (uint8_t *)data, 2) == RT_EOK)
    {
        return RT_EOK;
    }
    else
    {
        return RT_ERROR;
    }
}

rt_err_t ztw_tp_write_cmd(rt_uint16_t cmd)
{
    uint16_t data = 1;
    return ztw_tp_write_reg(cmd, data);
}

rt_err_t ztw_power_sequence(void)
{
    uint16_t chip_code = 0;

    if (ztw_tp_write_reg( ZINITIX_VENDOR_REG, 0x0001) != RT_EOK)
    {
        return RT_ERROR;
    }

    am_util_delay_us(10);

    if (ztw_tp_read(CHIP_ID_REG, (uint8_t *)&chip_code, 2) != RT_EOK)
    {
        return RT_ERROR;
    }

    am_util_delay_us(10);

    if (ztw_tp_write_cmd(ZINITIX_INTN_CLEAR_CMD) != RT_EOK)
    {
        return RT_ERROR;
    }

    am_util_delay_us(10);

    if (ztw_tp_write_reg(ZINITIX_NVM_REG, 0x0001) != RT_EOK)
    {
        return RT_ERROR;
    }

    am_util_delay_us(50);

    if (ztw_tp_write_reg(ZINITIX_PROGRAM_START_CMD, 0x0001) != RT_EOK)
    {
        return RT_ERROR;
    }

    am_util_delay_us(500);
    return RT_EOK;
}

static uint16_t get_unaligned_le16(const void *p)
{
    const uint8_t *_p = p;
    return _p[0] | _p[1] << 8;
}

rt_err_t ztw_get_point(uint16_t *x, uint16_t *y, bool *touch_released)
{


    uint32_t size;
    uint8_t buf[2];
    uint8_t buf_data[256];
    uint16_t touch_x, touch_y;
    uint8_t touch_event;

    ztw_tp_read(0x22, buf, 2);

    size = get_unaligned_le16(&buf[0]);

    if (size != 0)
    {
        ztw_tp_read(0x22, buf_data, size);

        touch_x = buf_data[10];
        touch_x <<= 8;
        touch_x += buf_data[9];
        *x = 453 - touch_x;

        touch_y = buf_data[12];
        touch_y <<= 8;
        touch_y += buf_data[11];
        *y = 453 - touch_y;

        touch_event = buf_data[8];
        touch_event >>= 5;
        touch_event &= 0x03;
        switch (touch_event)
        {
            case 0:
            break;

            case 1:
                *touch_released = false;
            break;

            case 2:
            break;

            case 3:
                *touch_released = true;
            break;

            default:break;
        }
    }
    return RT_EOK;
}


static rt_err_t ztw_init(void)
{
    uint8_t i;
    uint16_t eeprom_info;

    for (i = 0; i < 10; i++)
    {
        if ( ztw_tp_write_cmd( ZINITIX_SWRESET_CMD) == 0 )
        {
            // return 0 mean write success then break
            break;
        }

        am_util_delay_us(10);
    }

    uint16_t reg_val = 0;
    zinitix_bit_set(reg_val, BIT_PT_CNT_CHANGE);
    zinitix_bit_set(reg_val, BIT_DOWN);
    zinitix_bit_set(reg_val, BIT_MOVE);
    zinitix_bit_set(reg_val, BIT_UP);
    zinitix_bit_set(reg_val, BIT_PALM);
    zinitix_bit_set(reg_val, BIT_PT_EXIST);

    if (ztw_tp_read_reg(ZINITIX_EEPROM_INFO, &eeprom_info) != RT_EOK)
    {
        // TOUCH_LOG("fail to read eeprom info");
    }

    ztw_tp_write_reg(ZINITIX_SUPPORTED_FINGER_NUM, MAX_SUPPORTED_FINGER_NUM);
    ztw_tp_write_reg(ZINITIX_X_RESOLUTION, 453);
    ztw_tp_write_reg(ZINITIX_Y_RESOLUTION, 453);

    ztw_tp_write_reg(ZINITIX_INITIAL_TOUCH_MODE, TOUCH_POINT_MODE);
    ztw_tp_write_reg(ZINITIX_TOUCH_MODE, TOUCH_POINT_MODE);

    ztw_tp_write_reg(ZINITIX_INT_ENABLE_FLAG, reg_val);

    for (i = 0; i < 10; i++)
    {
        ztw_tp_write_cmd( ZINITIX_CLEAR_INT_STATUS_CMD);
        am_util_delay_us(10);
    }

    return RT_EOK;
}

uint32_t
am_devices_tma525_init(uint32_t ui32Module, am_hal_iom_config_t *psIOMSettings, void **ppIomHandle)
{
    if ( ui32Module > AM_REG_IOM_NUM_MODULES )
    {
        return AM_DEVICES_TPV1P1_STATUS_ERROR;
    }

    //
    // Configure the IOM pins.
    //
    am_bsp_iom_pins_enable(ui32Module, AM_HAL_IOM_I2C_MODE);
#if 0
    //
    // Enable fault detection.
    //
#if AM_APOLLO3_MCUCTRL
    am_hal_mcuctrl_control(AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_ENABLE, 0);
#else // AM_APOLLO3_MCUCTRL
    am_hal_mcuctrl_fault_capture_enable();
#endif // AM_APOLLO3_MCUCTRL
#endif

    //
    // Initialize the IOM instance.
    // Enable power to the IOM instance.
    // Configure the IOM for Serial operation during initialization.
    // Enable the IOM.
    // HAL Success return is 0
    //
    if (am_hal_iom_initialize(ui32Module, &g_tma525_IOMHandle) ||
        am_hal_iom_power_ctrl(g_tma525_IOMHandle, AM_HAL_SYSCTRL_WAKE, false) ||
        am_hal_iom_configure(g_tma525_IOMHandle, psIOMSettings) ||
        am_hal_iom_enable(g_tma525_IOMHandle))
    {
        return AM_DEVICES_TPV1P1_STATUS_ERROR;
    }
    else
    {
        *ppIomHandle = g_tma525_IOMHandle;
        //
        // Return the status.
        //
    }

    ztw_power_sequence();

    uint16_t eeprom_info;

    if (ztw_tp_read_reg(ZINITIX_EEPROM_INFO, &eeprom_info) != RT_EOK)
    {
        // TOUCH_LOG("fail to read eeprom info");
    }
    else
    {
        // TOUCH_LOG("eeprom_info = 0x%08x\n", eeprom_info);
    }

    for ( int i = 0; i < 10; i++ )
    {
        if ( ztw_tp_write_cmd(ZINITIX_SWRESET_CMD) == 0 )
        {
            // return 0 mean write success then break
            break;
        }

        am_util_delay_us(100);
    }

    ztw_tp_write_reg(ZINITIX_INT_ENABLE_FLAG, 0);

    ztw_init();

    return AM_DEVICES_TPV1P1_STATUS_SUCCESS;
}

void am_devices_tma525_data_Read(uint8_t *pui8RxBuffer, uint32_t RxNumBytes)
{
    am_hal_iom_transfer_t Transaction;

    Transaction.ui8Priority     = 1;        // High priority for now.
    Transaction.uPeerInfo.ui32I2CDevAddr = 0x38;
    Transaction.ui8RepeatCount  = 0;
    Transaction.ui32PauseCondition = 0;
    Transaction.eDirection      = AM_HAL_IOM_RX;
    Transaction.ui32InstrLen    = 1;
#if defined(AM_PART_APOLLO4B)
    Transaction.ui64Instr       = 0;
#else
    Transaction.ui32Instr       = 0;
#endif
    Transaction.ui32NumBytes    = RxNumBytes;   //8;
    // Transaction.pui32TxBuffer   = (uint32_t *)pui8TxBuffer;
    Transaction.pui32RxBuffer    = (uint32_t *)pui8RxBuffer;
    // Transaction.pui32RxBuffer    = (uint32_t *)RxBuffer;
    //Transaction.bContinue       = true;
    Transaction.ui32PauseCondition = 0;
    // Now set the post processing condition
    Transaction.ui32StatusSetClr = 0;

    if (am_hal_iom_blocking_transfer(g_tma525_IOMHandle, &Transaction))
    {
        // return 0;
        // return AM_DEVICES_TPV1P1_STATUS_ERROR;
    }
}

static void am_hal_iom_i2c_read(uint32_t ui32BusAddress, uint32_t *pBuf, uint32_t size)
{
    am_hal_iom_transfer_t       Transaction;

    Transaction.ui8Priority     = 1;
    Transaction.ui32InstrLen    = 0;
#if defined(AM_PART_APOLLO4B)
    Transaction.ui64Instr       = 0;
#else
    Transaction.ui32Instr       = 0;
#endif
    Transaction.eDirection      = AM_HAL_IOM_RX;
    Transaction.ui32NumBytes    = size;
    Transaction.pui32RxBuffer   = pBuf;
    Transaction.bContinue       = false;
    Transaction.ui8RepeatCount  = 0;
    Transaction.ui32PauseCondition = 0;
    Transaction.ui32StatusSetClr = 0;

    Transaction.uPeerInfo.ui32I2CDevAddr = ui32BusAddress;
    if (am_hal_iom_blocking_transfer(g_tma525_IOMHandle, &Transaction))
    {
        // return 0;
        // return AM_DEVICES_TPV1P1_STATUS_ERROR;
    }
}

static void am_hal_iom_i2c_write(uint32_t ui32BusAddress, uint32_t *pBuf, uint32_t size)
{
    am_hal_iom_transfer_t       Transaction;

    Transaction.ui8Priority     = 1;
    Transaction.ui32InstrLen    = 0;
#if defined(AM_PART_APOLLO4B)
    Transaction.ui64Instr       = 0;
#else
    Transaction.ui32Instr       = 0;    //IOSOFFSET_WRITE_CMD;
#endif
    Transaction.eDirection      = AM_HAL_IOM_TX;
    Transaction.ui32NumBytes    = size;
    Transaction.pui32TxBuffer   = pBuf;
    Transaction.bContinue       = false;
    Transaction.ui8RepeatCount  = 0;
    Transaction.ui32PauseCondition = 0;
    Transaction.ui32StatusSetClr = 0;

    Transaction.uPeerInfo.ui32I2CDevAddr = ui32BusAddress;
    if (am_hal_iom_blocking_transfer(g_tma525_IOMHandle, &Transaction))
    {
        // return 0;
        // return AM_DEVICES_TPV1P1_STATUS_ERROR;
    }
}

rt_size_t rt_i2c_master_xfer(struct rt_i2c_msg          *msgs, rt_uint32_t num)
{
    struct rt_i2c_msg *msg;
    int i;
    rt_uint32_t msg_len = 0;

    for (i = 0; i < num; i++)
    {
        msg = &msgs[i];

        if (msg->flags == RT_I2C_RD)
        {
            am_hal_iom_i2c_read(msg->addr, (uint32_t *)msg->buf, msg->len);
            msg_len += msg->len;
        }
        else if ( msg->flags == RT_I2C_WR )
        {
            am_hal_iom_i2c_write(msg->addr, (uint32_t *)msg->buf, msg->len);
            msg_len += (msg->len - 1);
        }
    }

    return msg_len;
}

