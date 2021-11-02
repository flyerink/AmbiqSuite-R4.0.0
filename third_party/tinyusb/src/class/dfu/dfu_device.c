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

#include "tusb_option.h"

#if (TUSB_OPT_DEVICE_ENABLED && CFG_TUD_DFU)

#include "dfu_device.h"
#include "device/usbd_pvt.h"

//--------------------------------------------------------------------+
// INTERNAL OBJECT & FUNCTION DECLARATION
//--------------------------------------------------------------------+

static tusb_desc_device_t const dfu_desc_device =
{
	.bLength            = sizeof(tusb_desc_device_t),
	.bDescriptorType    = TUSB_DESC_DEVICE,
	.bcdUSB             = 0x0200,
	.bDeviceClass       = 0x0,
	.bDeviceSubClass    = 0x0,
	.bDeviceProtocol    = 0x0,
	.bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
	.idVendor           = 0xCafe,
	.idProduct          = CFG_TUD_DFU_PRODUCT,
	.bcdDevice          = 0x0100,
	.iManufacturer      = 0x01,
	.iProduct           = 0x02,
	.iSerialNumber      = 0x03,
	.bNumConfigurations = 0x01
};

// DFU mode descriptor, only one DFU interface for downloading&uploading
#define ITF_NUM_DFU_MODE 0
#define CONFIG_TOTAL_LEN TUD_CONFIG_DESC_LEN + TUD_DFU_DESC_LEN

static const uint8_t dfu_mode_desc_configuration[] = {
  // Interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),	
  // Interface number, string index, attributes, detach timeout, transfer size */
  TUD_DFU_MODE_DESCRIPTOR(ITF_NUM_DFU_MODE, 0, CFG_TUD_DFU_ATTR, CFG_TUD_DFU_POLLTIMEOUT, CFG_TUD_DFU_TRANSFER_SIZE)
};

typedef struct
{
  uint32_t total_bytes_sent;       /* Number of bytes sent during upload */
  uint32_t total_bytes_received;   /* Number of bytes received during download */
  //uint32_t alt_setting;          /* DFU alternate setting */
  uint16_t block_nr;               /* DFU block number */
  dfu_state_t state;               /* State of the DFU device */
  dfu_status_t status;             /* Status of the DFU device */
  uint16_t bwPollTimeout;
  dfu_status_type_t get_status_resp;
  uint8_t databuf[CFG_TUD_DFU_BUFSIZE]; /* buffer used for downloading and uploading */
} dfu_interface_t;

static dfu_interface_t _dfu_intf;

static bool dfu_check_app_state(void)
{
  if (_dfu_intf.state == appIDLE ||
    _dfu_intf.state == appDETACH) {
    _dfu_intf.state = appIDLE;
    return true;
  }

  return false;
}

static void dfu_reset_counters(void)
{
  _dfu_intf.total_bytes_sent     = 0;
  _dfu_intf.total_bytes_received = 0;
  _dfu_intf.block_nr             = 0;
}

//--------------------------------------------------------------------+
// DFU Application API
//--------------------------------------------------------------------+



//--------------------------------------------------------------------+
// USBD Driver API
//--------------------------------------------------------------------+
void dfu_init(void)
{
  memset(&_dfu_intf, 0x0, sizeof(_dfu_intf));
  //initial DUF state is dfuIDLE ( = 0) and status = statusOK ( = 0 £©
  _dfu_intf.bwPollTimeout = CFG_TUD_DFU_POLLTIMEOUT;
}

void dfu_reset(uint8_t rhport)
{
  (void) rhport;
  
  if (_dfu_intf.state == appDETACH)
  {
    tud_descriptor_configuration_set(dfu_mode_desc_configuration);
    tud_descriptor_device_set((uint8_t *)&dfu_desc_device);
    _dfu_intf.state = dfuIDLE;
  }
}

bool dfu_open(uint8_t rhport, tusb_desc_interface_t const * itf_desc, uint16_t *p_length)
{
  (void) rhport;

  // Ensure this is DFU Runtime
  TU_ASSERT(itf_desc->bInterfaceSubClass == TUD_DFU_APP_SUBCLASS);
  TU_ASSERT((itf_desc->bInterfaceProtocol == DFU_PROTOCOL_RT) 
          ||(itf_desc->bInterfaceProtocol == DFU_PROTOCOL_DFU));

  //that means it is in the DFU downloading and uploading mode
  //notify the application to do some preparation like open flash
  if (itf_desc->bInterfaceProtocol == DFU_PROTOCOL_DFU)
    tud_dfu_mode_cb(&_dfu_intf.state, &_dfu_intf.status);

  uint8_t const * p_desc = tu_desc_next( itf_desc );
  (*p_length) = sizeof(tusb_desc_interface_t);

  if ( TUSB_DESC_FUNCTIONAL == tu_desc_type(p_desc) )
  {
    (*p_length) += p_desc[DESC_OFFSET_LEN];
    p_desc = tu_desc_next(p_desc);
  }

  return true;
}

bool dfu_control_complete(uint8_t rhport, tusb_control_request_t const * request)
{
  (void) rhport;

  //------------- Class Specific Request -------------//
  TU_VERIFY(request->bmRequestType_bit.type == TUSB_REQ_TYPE_CLASS);
  TU_VERIFY(request->bmRequestType_bit.recipient == TUSB_REQ_RCPT_INTERFACE);

  switch (_dfu_intf.state)
  {
    case dfuDNBUSY:
      _dfu_intf.block_nr++;
      _dfu_intf.total_bytes_received += request->wLength;
	  if (tud_dfu_process_download_data_cb)
		tud_dfu_process_download_data_cb(_dfu_intf.databuf, request->wLength, 
			&_dfu_intf.state, &_dfu_intf.status);
	break;
    case dfuUPLOAD_IDLE:
      _dfu_intf.block_nr++;
	  _dfu_intf.total_bytes_sent += request->wLength;
    break;
    default: break;
  }
  return true;
}

bool dfu_control_request(uint8_t rhport, tusb_control_request_t const * request)
{
  (void) rhport;
  
  uint32_t len = 0;

  //------------- Class Specific Request -------------//
  TU_ASSERT(request->bmRequestType_bit.type == TUSB_REQ_TYPE_CLASS);
  TU_ASSERT(request->bmRequestType_bit.recipient == TUSB_REQ_RCPT_INTERFACE);

  switch ( request->bRequest )
  {
    case DFU_REQ_DETACH:
      if (_dfu_intf.state != appIDLE)
      {
        _dfu_intf.state = appIDLE;
        return false; //stall the control pipe
	  }
      _dfu_intf.state = appDETACH;
      tud_control_status(rhport, request);
    break;
	
    case DFU_REQ_GETSTATUS:
      if (_dfu_intf.state == dfuMANIFEST_SYNC)
        _dfu_intf.state = dfuIDLE;
      _dfu_intf.get_status_resp.bStatus = _dfu_intf.status;
      _dfu_intf.get_status_resp.bState  = _dfu_intf.state;
      memcpy(_dfu_intf.get_status_resp.bwPollTimeout, &_dfu_intf.bwPollTimeout, 2);
      _dfu_intf.get_status_resp.iString = 0;
      tud_control_xfer(rhport, request, &_dfu_intf.get_status_resp, sizeof(dfu_status_type_t));
    break;
	
    case DFU_REQ_GETSTATE:
      tud_control_xfer(rhport, request, &_dfu_intf.state, sizeof(_dfu_intf.state));
    break;

    case DFU_REQ_ABORT:
      if (dfu_check_app_state())
        return false;
      dfu_reset_counters();
      _dfu_intf.state = dfuIDLE;
      _dfu_intf.status = statusOK;
    break;
	
    case DFU_REQ_CLRSTATUS:
      if (dfu_check_app_state())
        return false;
      _dfu_intf.state = dfuIDLE;
      _dfu_intf.status = statusOK;
	  tud_control_status(rhport, request);
    break;
	
    case DFU_REQ_DNLOAD:
      if (dfu_check_app_state())
			return false;

      /* Not using dfuDLOAD-SYNC??? */
      switch (_dfu_intf.state) 
      {
        case dfuIDLE:
          dfu_reset_counters();
          _dfu_intf.state = dfuDNBUSY;
          tud_control_xfer(rhport, request, _dfu_intf.databuf, request->wLength);
        break;
	  
        case dfuDNLOAD_IDLE:
          if (request->wLength == 0x0)
            _dfu_intf.state = dfuMANIFEST_SYNC;
          else
            _dfu_intf.state = dfuDNBUSY; 
          tud_control_xfer(rhport, request, _dfu_intf.databuf, request->wLength);
        break;

		default:
			_dfu_intf.state = dfuERROR;
			_dfu_intf.status = errUNKNOWN;
			dfu_reset_counters();
			return false;
		}
		break;

	case DFU_REQ_UPLOAD:
		if (dfu_check_app_state())
			return false;

        switch (_dfu_intf.state) 
        {
          case dfuIDLE:
            dfu_reset_counters();
            _dfu_intf.state = dfuUPLOAD_IDLE;
          case dfuUPLOAD_IDLE:
            if (!request->wLength || _dfu_intf.block_nr != request->wValue) 
            {
              _dfu_intf.state = dfuERROR;
              _dfu_intf.status = errUNKNOWN;
              break;
            }
            //always make sure user application define this callback
            //function for uploading
            if (tud_dfu_fill_upload_data_cb)
              len = tud_dfu_fill_upload_data_cb(_dfu_intf.databuf, request->wLength, 
                    &_dfu_intf.state, &_dfu_intf.status);
            tud_control_xfer(rhport, request, _dfu_intf.databuf, len);
          break;

          default:
            _dfu_intf.state = dfuERROR;
            _dfu_intf.status = errUNKNOWN;
            dfu_reset_counters();
            return false;
		}
		break;

    default: 
      return false; // stall unsupported request
  }

  return true;
}

bool dfu_xfer_cb(uint8_t rhport, uint8_t ep_addr, xfer_result_t result, uint32_t xferred_bytes)
{
  (void) rhport;
  (void) ep_addr;
  (void) result;
  (void) xferred_bytes;
  
  return true;
}

#endif
