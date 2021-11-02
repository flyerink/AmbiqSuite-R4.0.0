/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Quan Zhang (ambiqmicro.com)
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

/** \ingroup group_class
 *  \defgroup ClassDriver_DFU USB Device Firmware Upgrade class (DFU)
 *  @{ */

#ifndef _TUSB_DFU_H__
#define _TUSB_DFU_H__

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF
//--------------------------------------------------------------------+

// DFU Class Subclass
#define DFU_SUBCLASS			0x01

// DFU Protocol
typedef enum
{
  DFU_PROTOCOL_RT  = 1,
  DFU_PROTOCOL_DFU = 2,
} dfu_protocol_type_t;


//DFU Class Specific Requests
typedef enum
{
  DFU_REQ_DETACH    = 0x00,
  DFU_REQ_DNLOAD    = 0x01,
  DFU_REQ_UPLOAD    = 0x02,
  DFU_REQ_GETSTATUS = 0x03,
  DFU_REQ_CLRSTATUS = 0x04,
  DFU_REQ_GETSTATE  = 0x05,
  DFU_REQ_ABORT     = 0x06
} dfu_request_type_t;

// DFU Descriptor Type
typedef enum
{
  DFU_DESC_FUNCTIONAL = 0x21,
} dfu_descriptor_type_t;

// DFU attributes DFU Functional Descriptor
typedef enum
{
  DFU_ATTR_WILL_DETACH = TU_BIT(3),
  DFU_ATTR_MANIFESTATION_TOLERANT = TU_BIT(2),
  DFU_ATTR_CAN_UPLOAD = TU_BIT(1),
  DFU_ATTR_CAN_DNLOAD = TU_BIT(0)
} dfu_feature_attr_bm_t;

// DFU Specification release
#define DFU_VERSION			0x0110

// Run-Time Functional Descriptor
typedef TU_ATTR_PREPACKED struct TU_ATTR_PACKED 
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bmAttributes;
	uint16_t wDetachTimeOut;
	uint16_t wTransferSize;
	uint16_t bcdDFUVersion;
} dfu_runtime_descriptor_t;

// DFU mode function descriptor is exactly same as DFU runtime descriptor
typedef dfu_runtime_descriptor_t dfu_mode_descriptor_t;

typedef TU_ATTR_PREPACKED struct TU_ATTR_PACKED 
{
	uint8_t bStatus;
	uint8_t bwPollTimeout[3];
	uint8_t bState;
	uint8_t iString;
} dfu_status_type_t;

// bStatus values for the DFU_GETSTATUS response
typedef enum
{
	statusOK,
	errTARGET,
	errFILE,
	errWRITE,
	errERASE,
	errCHECK_ERASED,
	errPROG,
	errVERIFY,
	errADDRESS,
	errNOTDONE,
	errFIRMWARE,
	errVENDOR,
	errUSB,
	errPOR,
	errUNKNOWN,
	errSTALLEDPKT
} dfu_status_t;

// bState values for the DFU_GETSTATUS response
typedef enum
{
	appIDLE,
	appDETACH,
	dfuIDLE,
	dfuDNLOAD_SYNC,
	dfuDNBUSY,
	dfuDNLOAD_IDLE,
	dfuMANIFEST_SYNC,
	dfuMANIFEST,
	dfuMANIFEST_WAIT_RST,
	dfuUPLOAD_IDLE,
	dfuERROR,
} dfu_state_t;

#ifdef __cplusplus
 }
#endif
#endif /* _TUSB_DFU_H__ */
