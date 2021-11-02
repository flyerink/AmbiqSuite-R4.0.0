/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Quan Zhang <ambiqmicro.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#ifndef _TUSB_DFU_RT_DEVICE_H_
#define _TUSB_DFU_RT_DEVICE_H_

#include "common/tusb_common.h"
#include "device/usbd.h"
#include "dfu.h"

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+
#ifndef CFG_TUD_DFU_BUFSIZE
#define CFG_TUD_DFU_BUFSIZE 512
#endif

#define CFG_TUD_DFU_TRANSFER_SIZE CFG_TUD_DFU_BUFSIZE

#ifndef CFG_TUD_DFU_PRODUCT
#define CFG_TUD_DFU_PRODUCT 0xdf
#endif

#ifndef CFG_TUD_DFU_ATTR
#define CFG_TUD_DFU_ATTR (DFU_ATTR_CAN_DNLOAD | DFU_ATTR_CAN_UPLOAD | DFU_ATTR_WILL_DETACH)
#endif

#ifndef CFG_TUD_DFU_POLLTIMEOUT
#define CFG_TUD_DFU_POLLTIMEOUT 1000
#endif

//--------------------------------------------------------------------+
// Common Definitions
//--------------------------------------------------------------------+
void tud_descriptor_configuration_set(const uint8_t *desc);
void tud_descriptor_device_set(const uint8_t *desc);

//--------------------------------------------------------------------+
// Application API
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// Application Callback API (weak is optional)
//--------------------------------------------------------------------+
TU_ATTR_WEAK void tud_dfu_mode_cb(dfu_state_t *state, dfu_status_t *status);

TU_ATTR_WEAK uint32_t tud_dfu_fill_upload_data_cb(uint8_t *data, 
  uint32_t len, dfu_state_t *state, dfu_status_t *status);
  
TU_ATTR_WEAK void tud_dfu_process_download_data_cb(uint8_t *data, 
  uint32_t len, dfu_state_t *state, dfu_status_t *status);

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+
void dfu_init(void);
void dfu_reset(uint8_t rhport);
bool dfu_open(uint8_t rhport, tusb_desc_interface_t const * itf_desc, uint16_t *p_length);
bool dfu_control_request(uint8_t rhport, tusb_control_request_t const * request);
bool dfu_control_complete(uint8_t rhport, tusb_control_request_t const * request);
bool dfu_xfer_cb(uint8_t rhport, uint8_t ep_addr, xfer_result_t event, uint32_t xferred_bytes);

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_DFU_RT_DEVICE_H_ */
