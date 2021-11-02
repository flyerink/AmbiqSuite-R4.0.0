/*******************************************************************************
 * Copyright (c) 2019 Think Silicon S.A.
 *
   Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this header file and/or associated documentation files to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Materials, and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * MODIFICATIONS TO THIS FILE MAY MEAN IT NO LONGER ACCURATELY REFLECTS
 * NEMAGFX API. THE UNMODIFIED, NORMATIVE VERSIONS OF THINK-SILICON NEMAGFX
 * SPECIFICATIONS AND HEADER INFORMATION ARE LOCATED AT:
 *   https://think-silicon.com/products/software/nemagfx-api
 *
 *  The software is provided 'as is', without warranty of any kind, express or
 *  implied, including but not limited to the warranties of merchantability,
 *  fitness for a particular purpose and noninfringement. In no event shall
 *  Think Silicon S.A. be liable for any claim, damages or other liability, whether
 *  in an action of contract, tort or otherwise, arising from, out of or in
 *  connection with the software or the use or other dealings in the software.
 ******************************************************************************/


#ifndef NEMA_FONT_H__
#define NEMA_FONT_H__

#include "nema_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NEMA_ALIGNX_LEFT    (0x00U) /**< Align horizontally to the left */
#define NEMA_ALIGNX_RIGHT   (0x01U) /**< Align horizontally to the right */
#define NEMA_ALIGNX_CENTER  (0x02U) /**< Align horizontally centered */
#define NEMA_ALIGNX_JUSTIFY (0x03U) /**< Justify horizontally */
#define NEMA_ALIGNX_MASK    (0x03U) /**< Horizontal alignment mask */
#define NEMA_ALIGNY_TOP     (0x00U) /**< Align vertically to the top */
#define NEMA_ALIGNY_BOTTOM  (0x04U) /**< Align vertically to the bottom */
#define NEMA_ALIGNY_CENTER  (0x08U) /**< Align vertically centered */
#define NEMA_ALIGNY_JUSTIFY (0x0cU) /**< Justify vertically */
#define NEMA_ALIGNY_MASK    (0x0cU) /**< Vertical alignment mask */
#define NEMA_TEXT_WRAP      (0x10U) /**< Use text wrapping */

typedef struct {
    uint32_t   left;     /**< Neighbor character to the left of the current one (Unicode value) */
    int8_t     x_offset; /**< Kerning offset (horizontally) */
} nema_kern_pair_t;

typedef struct {
    uint32_t bitmapOffset;
    uint8_t  width;
    uint8_t  xAdvance;
    int8_t   xOffset, yOffset;
    uint32_t kern_offset;
    uint8_t  kern_length;
} nema_glyph_t;

typedef struct {
    uint32_t  first, last;
    const nema_glyph_t *glyphs;
} nema_font_range_t;

typedef struct {
    nema_buffer_t   bo;
    const nema_font_range_t *ranges;
    const int                bitmap_size;
    const uint8_t           *bitmap;
    uint32_t                 flags;
    uint8_t                  xAdvance; //default xAdvance
    uint8_t                  yAdvance;
    uint8_t                  max_ascender;
    uint8_t                  bpp;
    const nema_kern_pair_t  *kern_pairs;
} nema_font_t;

/** \brief Bind the font to use in future nema_print() calls
 *
 * \param font Pointer to font
 *
 */
void nema_bind_font(nema_font_t *font);

/** \brief Get the bounding box's width and height of a string.
 *
 * \param str Pointer to string
 * \param w Pointer to variable where width should be written
 * \param h Pointer to variable where height should be written
 * \param max_w Max allowed width
 * \return Number of carriage returns
 *
 */
int  nema_string_get_bbox(const char *str, int *w, int *h, int max_w, uint32_t wrap);

/** \brief Print pre-formatted text
 *
 * \param str Pointer to string
 * \param x X coordinate of text-area's top-left corner
 * \param y Y coordinate of text-area's top-left corner
 * \param w Width of the text area
 * \param h Height of the text area
 * \param fg_col Foreground color of text
 * \param align Alignment and wrapping mode
 *
 */
void nema_print(const char *str, int x, int y, int w, int h, uint32_t fg_col, uint32_t align);

/** \brief Print pre-formatted text
 *
 * \param *str Pointer to string
 * \param *cursor_x X position of next character to be drawn. Usually initialized to 0 by the user and then updated internally by the library
 * \param *cursor_y Y position of next character to be drawn. Usually initialized to 0 by the user and then updated internally by the library
 * \param x X coordinate of text-area's top-left corner
 * \param y Y coordinate of text-area's top-left corner
 * \param w Width of the text area
 * \param h Height of the text area
 * \param fg_col Foreground color of text
 * \param align Alignment and wrapping mode
 *
 */
void nema_print_to_position(const char *str, int *pos_x, int *pos_y, int x, int y, int w, int h, uint32_t fg_col, uint32_t align);

#ifdef __cplusplus
}
#endif

#endif // NEMA_FONT_H__
