// -----------------------------------------------------------------------------
// Copyright (c) 2019 Think Silicon S.A.
// Think Silicon S.A. Confidential Proprietary
// -----------------------------------------------------------------------------
//     All Rights reserved - Unpublished -rights reserved under
//         the Copyright laws of the European Union
//
//  This file includes the Confidential information of Think Silicon S.A.
//  The receiver of this Confidential Information shall not disclose
//  it to any third party and shall protect its confidentiality by
//  using the same degree of care, but not less than a reasonable
//  degree of care, as the receiver uses to protect receiver's own
//  Confidential Information. The entire notice must be reproduced on all
//  authorised copies and copies may only be made to the extent permitted
//  by a licensing agreement from Think Silicon S.A..
//
//  The software is provided 'as is', without warranty of any kind, express or
//  implied, including but not limited to the warranties of merchantability,
//  fitness for a particular purpose and noninfringement. In no event shall
//  Think Silicon S.A. be liable for any claim, damages or other liability, whether
//  in an action of contract, tort or otherwise, arising from, out of or in
//  connection with the software or the use or other dealings in the software.
//
//
//                    Think Silicon S.A.
//                    http://www.think-silicon.com
//                    Patras Science Park
//                    Rion Achaias 26504
//                    Greece
// -----------------------------------------------------------------------------

#ifndef NEMA_DC_INTERN_H__
#define NEMA_DC_INTERN_H__


#define NEMADC_MAGIC     0x87452365
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//Color Mode Check
//--------------------------------------------------------------------------
#define NEMADC_DBIB_STALL       (1U <<  31)
typedef enum {
    NEMADC_DBIB_STALL_CHK     = 1  <<  31,
    NEMADC_OPENLDI_CHK        = 1U <<  30,
    NEMADC_JDIMIP_CHK         = 1U <<  29,
    NEMADC_ARGB4444_CHK       = 1U <<  22,
    NEMADC_RGBA4444_CHK       = 1U <<  21,
    NEMADC_GPI_CHK            = 1U <<  20,
    NEMADC_EXTRCTRL_CHK       = 1U <<  19,
    NEMADC_TSC6_CHK           = 1U <<  18,
    NEMADC_TSC_CHK            = 1U <<  17,
    NEMADC_LUT8_CHK           = 1U <<  16,
    NEMADC_RGBA5551_CHK       = 1U <<  15,
    NEMADC_ABGR8888_CHK       = 1U <<  14,
    NEMADC_RGB332_CHK         = 1U <<  13,
    NEMADC_RGB565_CHK         = 1U <<  12,
    NEMADC_BGRA8888_CHK       = 1U <<  11,
    NEMADC_L8_CHK             = 1U <<  10,
    NEMADC_L1_CHK             = 1U <<  9,
    NEMADC_L4_CHK             = 1U <<  8,
    NEMADC_YUVV_CHK           = 1U <<  7,
    NEMADC_RGB24_CHK          = 1U <<  6,
    NEMADC_YUY2_CHK           = 1U <<  5,
    NEMADC_RGBA8888_CHK       = 1U <<  4,
    NEMADC_ARGB8888_CHK       = 1U <<  3,
    NEMADC_V_YUV420_CHK       = 1U <<  2,
    NEMADC_TLYUV420_CHK       = 1U <<  1,
    NEMADC_BLOCK4X4_CHK       = 1U <<  0
} nemadc_colormode_check_t;


//--------------------------------------------------------------------------
//AXI control
//--------------------------------------------------------------------------
typedef enum {
    NEMADC_AXI_16BEAT   = 0x0,
    NEMADC_AXI_2BEAT    = 0x1,
    NEMADC_AXI_4BEAT    = 0x2,
    NEMADC_AXI_8BEAT    = 0x3,
    NEMADC_AXI_32BEAT   = 0x5,
    NEMADC_AXI_64BEAT   = 0x6,
    NEMADC_AXI_128BEAT  = 0x7,

    NEMADC_AXI_FT_HF    = 0x0U << 3,
    NEMADC_AXI_FT_2B    = 0x1U << 3,
    NEMADC_AXI_FT_4B    = 0x2U << 3,
    NEMADC_AXI_FT_8B    = 0x3U << 3
} nemadc_AXI_cfg_t;

typedef enum {
    NemaDC_snapshot              = 0xff,
    //------------------------------------
    NemaDC_store_base_addr       = (1 <<31),
    NemaDC_DBI_cmd               = (1U<<30),
    NemaDC_wcmd16                = (1U<<28),
    NemaDC_wcmd24                = (1U<<29),
    NemaDC_wcmd32                = (1U<<29)|(1U<<28),
    NemaDC_rcmd16                = (1U<<28),
    NemaDC_rcmd24                = (1U<<29),
    NemaDC_rcmd32                = (1U<<29)|(1U<<28),
    NemaDC_mask_qspi             = (1U<<27),
    NemaDC_DBI_ge                = (1U<<27),
    NemaDC_DBI_read              = (1U<<26),
    NemaDC_ext_ctrl              = (1U<<25),
    NemaDC_sline_cmd             = (1U<<24),
    //--------------------------------------
    NemaDC_read_byte             = (0U<<30),
    NemaDC_read_2byte            = (1U<<30),
    NemaDC_read_3byte            = (2 <<30),
    NemaDC_read_4byte            = (3 <<30)
} dc_mipi_cmd_t;

#define SPI_WRITE                                  (2U)
#define SPI_READ                                   (3U)
//--
#define SPICMD                                 (1U<<5U)
#define QSPICMD                                (0U<<5U)
#define QSPIDATA                               (1U<<4U)

#define CMD_OFFSET                                 (8U)
//--
#define CMD1_DATA1         (                 SPI_WRITE)
#define CMD1_DATA4         ( SPICMD|QSPIDATA|SPI_WRITE)
#define CMD4_DATA4         (QSPICMD|QSPIDATA|SPI_WRITE)
#define CMD1_RDAT1         (                 SPI_READ )

#endif
