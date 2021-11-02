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

#ifndef NEMA_DC_MIPI_H__
#define NEMA_DC_MIPI_H__

#include "nema_sys_defs.h"
//--------------------------------------------------------------------------
typedef enum {
    MIPI_enter_idle_mode       = 0x39,
    MIPI_enter_invert_mode     = 0x21,
    MIPI_enter_normal_mode     = 0x13,
    MIPI_enter_partial_mode    = 0x12,
    MIPI_enter_sleep_mode      = 0x10,
    MIPI_exit_idle_mode        = 0x38,
    MIPI_exit_invert_mode      = 0x20,
    MIPI_exit_sleep_mode       = 0x11,
    MIPI_get_3D_control        = 0x3f,
    MIPI_get_address_mode      = 0x0b,
    MIPI_get_blue_channel      = 0x08,
    MIPI_get_diagnostic_result = 0x0f,
    MIPI_get_display_mode      = 0x0d,
    MIPI_get_green_channel     = 0x07,
    MIPI_get_pixel_format      = 0x0c,
    MIPI_get_power_mode        = 0x0a,
    MIPI_get_red_channel       = 0x06,
    MIPI_get_scanline          = 0x45,
    MIPI_get_signal_mode       = 0x0e,
    MIPI_nop                   = 0x00,
    MIPI_read_DDB_continue     = 0xa8,
    MIPI_read_DDB_start        = 0xa1,
    MIPI_read_memory_continue  = 0x3e,
    MIPI_read_memory_start     = 0x2e,
    MIPI_set_3D_control        = 0x3d,
    MIPI_set_address_mode      = 0x36,
    MIPI_set_column_address    = 0x2a,
    MIPI_set_display_off       = 0x28,
    MIPI_set_display_on        = 0x29,
    MIPI_set_gamma_curve       = 0x26,
    MIPI_set_page_address      = 0x2b,
    MIPI_set_partial_columns   = 0x31,
    MIPI_set_partial_rows      = 0x30,
    MIPI_set_pixel_format      = 0x3a,
    MIPI_set_scroll_area       = 0x33,
    MIPI_set_scroll_start      = 0x37,
    MIPI_set_tear_off          = 0x34,
    MIPI_set_tear_on           = 0x35,
    MIPI_set_tear_scanline     = 0x44,
    MIPI_set_vsync_timing      = 0x40,
    MIPI_soft_reset            = 0x01,
    MIPI_write_LUT             = 0x2d,
    MIPI_write_memory_continue = 0x3c,
    MIPI_write_memory_start    = 0x2c,
    MIPI_snapshot              = 0xff,

    MIPI_DBIB_STORE_BASE_ADDR  = (1 <<31), // (int)(1<<31)
    MIPI_DBIB_CMD              = (1U<<30),
    MIPI_CMD16                 = (1U<<28),
    MIPI_CMD24                 = (1U<<29),
    MIPI_MASK_QSPI             = (1U<<27),

} nemadc_mipi_cmd_t;

//--------------------------------------------------------------------------
#define MIPICFG_DBI_EN           (1U<<31U)             /**< Enables MIPI DBI/SPI interface */
#define MIPICFG_FRC_CSX_0        (1U<<30U)             /**< Enables CSX force value */
#define MIPICFG_FRC_CSX_1        ((1U<<30U)|(1U<<29U)) /**< Force CSX to 1 */
#define MIPICFG_SPI_CSX_V        (1U<<29U)             /**< CSX active high/low */
#define MIPICFG_DIS_TE           (1U<<28U)             /**< Disables Input Tearing Signal */
#define MIPICFG_SPIDC_DQSPI      (1U<<27U)             /**< Enables the usage of SPI_DC wire as SPI_SD1 */
#define MIPICFG_RSTN_DBI_SPI     (1U<<26U)             /**< DBI/SPI interfaces clear */
#define MIPICFG_RESX             (1U<<25U)             /**< Controls MIPI DBI Type-B RESX output signal */
#define MIPICFG_DMA              (1U<<24U)             /**< (unused) Enables pixel data from DMA */
#define MIPICFG_SPI3             (1U<<23U)             /**< Enables SPI 3-wire interface */
#define MIPICFG_SPI4             (1U<<22U)             /**< Enables SPI 4-wire interface */
#define MIPICFG_GPI             ((1U<<23U)|(1U<<22U))  /**< Enables Generic Packet Interface */
#define MIPICFG_EN_STALL         (1U<<21U)             /**< Enables back-pressure from dbi_stall_i signal */
#define MIPICFG_SPI_CPHA         (1U<<20U)             /**< Sets SPI Clock Phase */
#define MIPICFG_SPI_CPOL         (1U<<19U)             /**< Sets SPI Clock Polarity */
#define MIPICFG_SPI_JDI          (1U<<18U)             /**< -- reserved -- */
#define MIPICFG_EN_DVALID        (1U<<18U)             /**< Enables read using external data valid signal */
#define MIPICFG_SPI_HOLD         (1U<<17U)             /**< Binds scanline address with pixel data */
#define MIPICFG_INV_ADDR         (1U<<16U)             /**< Inverts scanline address */
#define MIPICFG_SCAN_ADDR        (1U<<15U)             /**< Scan address used as header of each line */
#define MIPICFG_PIXCLK_OUT_EN    (1U<<14U)             /**< Redirects pixel generation clock to the output */
#define MIPICFG_EXT_CTRL         (1U<<13U)             /**< Enables external control signals */
#define MIPICFG_BLANKING_EN      (1U<<12U)             /**< Enables horizontal blanking */
#define MIPICFG_DSPI_SPIX        (1U<<11U)             /**< Enables DSPI sub-pixel transaction */
#define MIPICFG_QSPI             (1U<<10U)             /**< Enables QSPI */
#define MIPICFG_QSPI_DDR         (1U<<10U)|(1U<<9U)    /**< Enables QSPI DDR */
#define MIPICFG_DSPI             (1U<< 9U)             /**< Enables DSPI */
#define MIPICFG_NULL               (0x00U)
//----------------------------------------
#define MIPI_DCS_RGB111            (1U)
#define MIPI_DCS_RGB332            (2U)
#define MIPI_DCS_RGB444            (3U)
#define MIPI_DCS_RGB565            (5U)
#define MIPI_DCS_RGB666            (6U)
#define MIPI_DCS_RGB888            (7U)
//----------------------------------------
#define MIPICFG_PF_SPI            (3U<<6U)
#define MIPICFG_PF_DSPI           (4U<<6U)
#define MIPICFG_PF_QSPI           (5U<<6U)
#define MIPICFG_PF_DBI8           (0U<<6U)
#define MIPICFG_PF_DBI9           (1U<<6U)
#define MIPICFG_PF_DBI16          (2U<<6U)
#define MIPICFG_PF_GPI            (6U<<6U)
//----------------------------------------
#define MIPICFG_PF_OPT0           (0U<<3U)
#define MIPICFG_PF_OPT1           (1U<<3U)
#define MIPICFG_PF_OPT2           (2U<<3U)
#define MIPICFG_PF_OPT3           (3U<<3U)
#define MIPICFG_PF_OPT4           (4U<<3U)
//---------------------------------------------------------------------------------------------
#define MIPICFG_1RGB111_OPT0     (MIPICFG_PF_SPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB111)   /*0xc1*/
#define MIPICFG_1RGB111_OPT1     (MIPICFG_PF_SPI |MIPICFG_PF_OPT1|MIPI_DCS_RGB111)   /*0xc9*/
#define MIPICFG_1RGB111_OPT2     (MIPICFG_PF_SPI |MIPICFG_PF_OPT2|MIPI_DCS_RGB111)   /*0xd1*/
#define MIPICFG_1RGB111_OPT3     (MIPICFG_PF_SPI |MIPICFG_PF_OPT3|MIPI_DCS_RGB111)   /*0xd9*/
#define MIPICFG_1RGB111_OPT4     (MIPICFG_PF_SPI |MIPICFG_PF_OPT4|MIPI_DCS_RGB111)   /*0xe1*/
#define MIPICFG_1RGB332_OPT0     (MIPICFG_PF_SPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB332)   /*0xc2*/
#define MIPICFG_1RGB444_OPT0     (MIPICFG_PF_SPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB444)   /*0xc3*/
#define MIPICFG_1RGB565_OPT0     (MIPICFG_PF_SPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB565)   /*0xc5*/
#define MIPICFG_1RGB666_OPT0     (MIPICFG_PF_SPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB666)   /*0xc6*/
#define MIPICFG_1RGB888_OPT0     (MIPICFG_PF_SPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB888)   /*0xc7*/
#define MIPICFG_2RGB444_OPT0     (MIPICFG_PF_DSPI|MIPICFG_PF_OPT0|MIPI_DCS_RGB444)   /*0x103*/
#define MIPICFG_2RGB444_OPT1     (MIPICFG_PF_DSPI|MIPICFG_PF_OPT1|MIPI_DCS_RGB444)   /*0x10b*/
#define MIPICFG_2RGB565_OPT0     (MIPICFG_PF_DSPI|MIPICFG_PF_OPT0|MIPI_DCS_RGB565)   /*0x105*/
#define MIPICFG_2RGB666_OPT0     (MIPICFG_PF_DSPI|MIPICFG_PF_OPT0|MIPI_DCS_RGB666)   /*0x106*/
#define MIPICFG_2RGB666_OPT1     (MIPICFG_PF_DSPI|MIPICFG_PF_OPT1|MIPI_DCS_RGB666)   /*0x10e*/
#define MIPICFG_2RGB888_OPT0     (MIPICFG_PF_DSPI|MIPICFG_PF_OPT0|MIPI_DCS_RGB888)   /*0x107*/
#define MIPICFG_2RGB888_OPT1     (MIPICFG_PF_DSPI|MIPICFG_PF_OPT1|MIPI_DCS_RGB888)   /*0x10f*/
//-
#define MIPICFG_4RGB111_OPT0     (MIPICFG_PF_QSPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB111)   /*0x141*/
#define MIPICFG_4RGB332_OPT0     (MIPICFG_PF_QSPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB332)   /*0x142*/
#define MIPICFG_4RGB444_OPT0     (MIPICFG_PF_QSPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB444)   /*0x143*/
#define MIPICFG_4RGB565_OPT0     (MIPICFG_PF_QSPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB565)   /*0x145*/
#define MIPICFG_4RGB666_OPT0     (MIPICFG_PF_QSPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB666)   /*0x146*/
#define MIPICFG_4RGB888_OPT0     (MIPICFG_PF_QSPI |MIPICFG_PF_OPT0|MIPI_DCS_RGB888)   /*0x147*/
#define MIPICFG_8RGB332_OPT0     (MIPICFG_PF_DBI8 |MIPICFG_PF_OPT0|MIPI_DCS_RGB332)   /*0x2*/
#define MIPICFG_8RGB444_OPT0     (MIPICFG_PF_DBI8 |MIPICFG_PF_OPT0|MIPI_DCS_RGB444)   /*0x3*/
#define MIPICFG_8RGB565_OPT0     (MIPICFG_PF_DBI8 |MIPICFG_PF_OPT0|MIPI_DCS_RGB565)   /*0x5*/
#define MIPICFG_8RGB666_OPT0     (MIPICFG_PF_DBI8 |MIPICFG_PF_OPT0|MIPI_DCS_RGB666)   /*0x6*/
#define MIPICFG_8RGB888_OPT0     (MIPICFG_PF_DBI8 |MIPICFG_PF_OPT0|MIPI_DCS_RGB888)   /*0x7*/
#define MIPICFG_16RGB332_OPT0    (MIPICFG_PF_DBI16|MIPICFG_PF_OPT0|MIPI_DCS_RGB332)   /*0x82*/
#define MIPICFG_16RGB444_OPT0    (MIPICFG_PF_DBI16|MIPICFG_PF_OPT0|MIPI_DCS_RGB444)   /*0x83*/
#define MIPICFG_16RGB565_OPT0    (MIPICFG_PF_DBI16|MIPICFG_PF_OPT0|MIPI_DCS_RGB565)   /*0x85*/
#define MIPICFG_16RGB666_OPT0    (MIPICFG_PF_DBI16|MIPICFG_PF_OPT0|MIPI_DCS_RGB666)   /*0x86*/
#define MIPICFG_16RGB666_OPT1    (MIPICFG_PF_DBI16|MIPICFG_PF_OPT1|MIPI_DCS_RGB666)   /*0x8e*/
#define MIPICFG_16RGB888_OPT0    (MIPICFG_PF_DBI16|MIPICFG_PF_OPT0|MIPI_DCS_RGB888)   /*0x87*/
#define MIPICFG_16RGB888_OPT1    (MIPICFG_PF_DBI16|MIPICFG_PF_OPT1|MIPI_DCS_RGB888)   /*0x8f*/
#define MIPICFG_9RGB666_OPT0     (MIPICFG_PF_DBI9 |MIPICFG_PF_OPT0|MIPI_DCS_RGB666)   /*0x46*/
//-----------------------------------------------------------------------------------------------
#define MIPICFG_32RGB332_OPT0    (MIPICFG_PF_GPI  |MIPICFG_PF_OPT0|MIPI_DCS_RGB332)   /*0x182*/
#define MIPICFG_32RGB444_OPT0    (MIPICFG_PF_GPI  |MIPICFG_PF_OPT0|MIPI_DCS_RGB444)   /*0x183*/
#define MIPICFG_32RGB565_OPT0    (MIPICFG_PF_GPI  |MIPICFG_PF_OPT0|MIPI_DCS_RGB565)   /*0x185*/
#define MIPICFG_32RGB666_OPT0    (MIPICFG_PF_GPI  |MIPICFG_PF_OPT0|MIPI_DCS_RGB666)   /*0x186*/
#define MIPICFG_32RGB666_OPT1    (MIPICFG_PF_GPI  |MIPICFG_PF_OPT1|MIPI_DCS_RGB666)   /*0x18e*/
#define MIPICFG_32RGB888_OPT0    (MIPICFG_PF_GPI  |MIPICFG_PF_OPT0|MIPI_DCS_RGB888)   /*0x187*/
//-----------------------------------------------------------------------------------------------
/** \brief Send command or data to MIPI Interface
 *
 * \param cmd    command or data to be sent
 *
 */
void nemadc_MIPI_out(int cmd);

/** \brief Configure NemaDC's serial interace
 *
 * \param cfg    configuration mode
 *
 */
void nemadc_MIPI_CFG_out(int cfg);

/** \brief Read data from MIPI interface
 *
 */
int  nemadc_MIPI_in(void);

/** \brief Read MIPI DBI Type-B parameters
 *
 * \param  cmd      MIPI DCS command
 * \param  n_params Number of parameters to read (max: 3)
 */
unsigned nemadc_MIPI_read(int cmd, int n_params);

/** \brief Send DCS command to display over the physical interface
 *
 * \param  cmd      MIPI DCS command
 */
void nemadc_MIPI_cmd(int cmd);

/** \brief Similar to nemadc_MIPI_cmd, with command parameters
 *
 * \param  cmd       MIPI DCS command
 * \param  n_params  Number of cmd parameters
 */
void nemadc_MIPI_cmd_params(int cmd, int n_params,...);



/** \brief Does Partial Update in MIPI
 *
 * \param  start_x    start x coordinate
 * \param  start_y    start y coordinate
 * \param  end_x      end x coordinate
 * \param  end_y      end y coordinate
 */
int  nemadc_MIPI_updateregion(int start_x, int start_y,
                             int end_x,   int end_y);

 /** \brief Convenience function. Sends exit_sleep and display_on commands
 *
 */
void nemadc_MIPI_enable(void);

 /** \brief Convenience function. Sends display_off and enter_sleep_mode commands
 *
 */
void nemadc_MIPI_disable(void);

// for backward compatibility
#define nemadc_MIPI_set_mode nemadc_MIPI_set_pixel_format

 /** \brief Set the display pixel format. Sends set_pixel_format command to the display
 *
 * \param  pixel_format    pixel format
 */
void nemadc_MIPI_set_pixel_format(int pixel_format);

/** \brief Set the frame position. Sends set_column_address and set_page_address commands.
 *
 * \param  minx    frames' minimum x
 * \param  miny    frame's minimum y
 * \param  maxx    frame's maximum x
 * \param  maxy    frame's maximum y
 */
void nemadc_MIPI_set_position(int minx, int miny, int maxx, int maxy);

/** \brief Set the display partial area and enter Partial Display Mode
 *
 * \param  minx    partial areas' minimum x
 * \param  miny    partial areas' minimum y
 * \param  maxx    partial areas' maximum x
 * \param  maxy    partial areas' maximum y
 */
void nemadc_MIPI_set_partial_mode(int minx, int miny, int maxx, int maxy);

 /** \brief Convenience function. Send a write_memory_start command in order to start transfering the frame to the display
 *
 */
void nemadc_MIPI_start_frame_transfer(void);
#endif // NEMA_DC_MIPI_H__
