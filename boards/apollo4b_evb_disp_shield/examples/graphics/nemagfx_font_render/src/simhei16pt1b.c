//*****************************************************************************
//
//! @file simhei16pt1b.c
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

#ifndef SIMHEI16PT1B_C
#define SIMHEI16PT1B_C

#include "simhei16pt1b.h"

#ifndef NEMA_GPU_MEM
#define NEMA_GPU_MEM
#endif // NEMA_GPU_MEM
#if defined(__CC_ARM)
// Suppress warning message: extended constant initialiser used
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#endif
// This will be read by the GPU only
const uint8_t NEMA_GPU_MEM g_sSimhei16pt1bBitmaps[] = {
  // 0x4f8b - 0x4f8b
  0x03, 0x00, 0x00, 0x70, 0x03, 0x80, 0x00, 0x70, 0x07, 0x7F, 0xE0, 0x70,
  0x07, 0x7F, 0xE0, 0x70, 0x06, 0x0C, 0x00, 0x70, 0x0E, 0x0C, 0x0E, 0x70,
  0x0E, 0x0C, 0x0E, 0x70, 0x1E, 0x0C, 0x0E, 0x70, 0x1E, 0x1C, 0x0E, 0x70,
  0x3E, 0x1F, 0xEE, 0x70, 0x7E, 0x1F, 0xEE, 0x70, 0x7E, 0x38, 0xCE, 0x70,
  0xEE, 0x38, 0xCE, 0x70, 0x2E, 0x70, 0xCE, 0x70, 0x0E, 0x70, 0xCE, 0x70,
  0x0E, 0xED, 0xCE, 0x70, 0x0E, 0x4F, 0xCE, 0x70, 0x0E, 0x07, 0x8E, 0x70,
  0x0E, 0x03, 0x8E, 0x70, 0x0E, 0x03, 0x8E, 0x70, 0x0E, 0x03, 0x0E, 0x70,
  0x0E, 0x07, 0x00, 0x70, 0x0E, 0x0E, 0x00, 0x70, 0x0E, 0x1E, 0x00, 0x70,
  0x0E, 0x3C, 0x00, 0x70, 0x0E, 0x78, 0x03, 0xE0, 0x0E, 0x70, 0x03, 0xE0,
  0x0E, 0x20, 0x03, 0x80,
  // 0x662f - 0x662f
  0x07, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 0xFC, 0x00, 0x06, 0x00, 0x0C, 0x00,
  0x06, 0x00, 0x0C, 0x00, 0x07, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xFC, 0x00,
  0x06, 0x00, 0x0C, 0x00, 0x06, 0x00, 0x0C, 0x00, 0x06, 0x00, 0x0C, 0x00,
  0x07, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF, 0xE0,
  0x00, 0x06, 0x00, 0x00, 0x02, 0x06, 0x00, 0x00, 0x07, 0x06, 0x00, 0x00,
  0x07, 0x07, 0xFF, 0x00, 0x06, 0x07, 0xFF, 0x00, 0x0F, 0x06, 0x00, 0x00,
  0x0F, 0xC6, 0x00, 0x00, 0x1D, 0xE6, 0x00, 0x00, 0x38, 0x7E, 0x00, 0x30,
  0x78, 0x3F, 0xFF, 0xE0, 0xF0, 0x07, 0xFF, 0xE0, 0x60, 0x00, 0x00, 0x00,
  // 0x6c49 - 0x6c49
  0x20, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x3C, 0x7F, 0xFF, 0x80,
  0x1E, 0x7F, 0xFF, 0x80, 0x0E, 0x18, 0x03, 0x80, 0x00, 0x18, 0x03, 0x00,
  0x00, 0x1C, 0x03, 0x00, 0x00, 0x0C, 0x07, 0x00, 0xE0, 0x0C, 0x07, 0x00,
  0xF0, 0x0C, 0x06, 0x00, 0x7C, 0x0E, 0x06, 0x00, 0x1E, 0x06, 0x0E, 0x00,
  0x0C, 0x06, 0x0C, 0x00, 0x00, 0x07, 0x1C, 0x00, 0x00, 0x03, 0x18, 0x00,
  0x00, 0x03, 0xB8, 0x00, 0x04, 0x01, 0xB0, 0x00, 0x06, 0x01, 0xF0, 0x00,
  0x0E, 0x00, 0xE0, 0x00, 0x0C, 0x00, 0xE0, 0x00, 0x1C, 0x01, 0xF0, 0x00,
  0x18, 0x03, 0xB8, 0x00, 0x38, 0x0F, 0x1E, 0x00, 0x70, 0x1E, 0x0F, 0x00,
  0x70, 0x78, 0x07, 0xC0, 0xE1, 0xF0, 0x03, 0xE0, 0x23, 0xC0, 0x00, 0xE0,
  0x01, 0x80, 0x00, 0x40,
  // 0x5b57 - 0x5b57
  0x00, 0x18, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00,
  0x7F, 0xFF, 0xFF, 0xC0, 0x7F, 0xFF, 0xFF, 0xC0, 0x70, 0x00, 0x00, 0xC0,
  0x70, 0x00, 0x00, 0xC0, 0x70, 0x00, 0x00, 0xC0, 0x70, 0x00, 0x00, 0xC0,
  0x07, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x7C, 0x00,
  0x00, 0x00, 0xF0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x07, 0x80, 0x00,
  0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE0,
  0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
  0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
  0x00, 0x06, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00,
  0x00, 0x7C, 0x00, 0x00,
  // 0x5e93 - 0x5e93
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00,
  0x00, 0x03, 0x80, 0x00, 0x1F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xF0,
  0x1C, 0x00, 0x00, 0x00, 0x1C, 0x03, 0x80, 0x00, 0x1C, 0x07, 0x00, 0x00,
  0x1C, 0x06, 0x00, 0x00, 0x1D, 0xFF, 0xFF, 0xE0, 0x19, 0xFF, 0xFF, 0xE0,
  0x18, 0x1C, 0x00, 0x00, 0x18, 0x18, 0xE0, 0x00, 0x18, 0x38, 0xE0, 0x00,
  0x18, 0x70, 0xE0, 0x00, 0x18, 0xFF, 0xFF, 0xC0, 0x18, 0xFF, 0xFF, 0xC0,
  0x18, 0x60, 0xE0, 0x00, 0x18, 0x00, 0xE0, 0x00, 0x18, 0x00, 0xE0, 0x00,
  0x38, 0x00, 0xE0, 0x00, 0x3B, 0xFF, 0xFF, 0xF0, 0x33, 0xFF, 0xFF, 0xF0,
  0x30, 0x00, 0xE0, 0x00, 0x70, 0x00, 0xE0, 0x00, 0x70, 0x00, 0xE0, 0x00,
  0xE0, 0x00, 0xE0, 0x00, 0x20, 0x00, 0xE0, 0x00,
  // 0x56fe - 0x56fe
  0xFF, 0xFF, 0xFF, 0x80, 0xFF, 0xFF, 0xFF, 0x80, 0xC0, 0x00, 0x01, 0x80,
  0xC0, 0x78, 0x01, 0x80, 0xC0, 0x70, 0x01, 0x80, 0xC0, 0xFF, 0xE1, 0x80,
  0xC1, 0xFF, 0xE1, 0x80, 0xC1, 0xC1, 0xC1, 0x80, 0xC3, 0xE1, 0x81, 0x80,
  0xC7, 0x73, 0x01, 0x80, 0xC2, 0x3E, 0x01, 0x80, 0xC0, 0x1E, 0x01, 0x80,
  0xC0, 0x7F, 0x81, 0x80, 0xC3, 0xE3, 0xF9, 0x80, 0xCF, 0x90, 0xF9, 0x80,
  0xC6, 0x3E, 0x11, 0x80, 0xC0, 0x1F, 0x81, 0x80, 0xC0, 0x03, 0x01, 0x80,
  0xC0, 0x60, 0x01, 0x80, 0xC0, 0xFC, 0x01, 0x80, 0xC0, 0x3F, 0x81, 0x80,
  0xC0, 0x07, 0x01, 0x80, 0xC0, 0x00, 0x01, 0x80, 0xFF, 0xFF, 0xFF, 0x80,
  0xFF, 0xFF, 0xFF, 0x80, 0xC0, 0x00, 0x01, 0x80, 0xC0, 0x00, 0x01, 0x80,
  // 0x5f62 - 0x5f62
  0x00, 0x00, 0x00, 0x00, 0x3C, 0x0F, 0x81, 0xC0, 0x3F, 0xFF, 0x83, 0xC0,
  0x26, 0x19, 0x83, 0x80, 0x06, 0x18, 0x07, 0x00, 0x06, 0x18, 0x0E, 0x00,
  0x06, 0x18, 0x1E, 0x00, 0x06, 0x18, 0x3C, 0x00, 0x06, 0x18, 0x78, 0x00,
  0x06, 0x18, 0x70, 0x40, 0x06, 0x18, 0x00, 0xE0, 0x06, 0x18, 0x01, 0xC0,
  0x7F, 0xFF, 0x83, 0x80, 0x7F, 0xFF, 0x87, 0x00, 0x06, 0x18, 0x0F, 0x00,
  0x06, 0x18, 0x1E, 0x00, 0x06, 0x18, 0x78, 0x00, 0x0E, 0x18, 0xF0, 0x20,
  0x0E, 0x18, 0x60, 0x70, 0x0C, 0x18, 0x00, 0xF0, 0x0C, 0x18, 0x01, 0xE0,
  0x1C, 0x18, 0x03, 0x80, 0x1C, 0x18, 0x0F, 0x00, 0x38, 0x18, 0x1E, 0x00,
  0x38, 0x18, 0x7C, 0x00, 0x70, 0x19, 0xF0, 0x00, 0x70, 0x18, 0x60, 0x00,
  0x00, 0x00, 0x00, 0x00,
  // 0x7684 - 0x7684
  0x0C, 0x00, 0x80, 0x00, 0x0E, 0x01, 0xC0, 0x00, 0x0E, 0x01, 0xC0, 0x00,
  0x0C, 0x01, 0xC0, 0x00, 0x0C, 0x01, 0x80, 0x00, 0x1C, 0x03, 0xFF, 0x80,
  0xFF, 0xE3, 0xFF, 0x80, 0xFF, 0xE3, 0xFF, 0x80, 0xC0, 0xE7, 0x01, 0x80,
  0xC0, 0xE6, 0x01, 0x80, 0xC0, 0xEE, 0x01, 0x80, 0xC0, 0xFC, 0x01, 0x80,
  0xC0, 0xE4, 0x81, 0x80, 0xC0, 0xE1, 0x81, 0x80, 0xFF, 0xE1, 0xC1, 0x80,
  0xFF, 0xE0, 0xC1, 0x80, 0xC0, 0xE0, 0xE1, 0x80, 0xC0, 0xE0, 0xE1, 0x80,
  0xC0, 0xE0, 0x71, 0x80, 0xC0, 0xE0, 0x41, 0x80, 0xC0, 0xE0, 0x01, 0x80,
  0xC0, 0xE0, 0x01, 0x80, 0xFF, 0xE0, 0x03, 0x80, 0xFF, 0xE0, 0x03, 0x80,
  0xFF, 0xE0, 0x03, 0x80, 0xC0, 0xE0, 0x7F, 0x00, 0xC0, 0xE0, 0x7E, 0x00,
  0x00, 0x00, 0x38, 0x00,
  // 0x4f8b - 0x4f8b
  0x03, 0x00, 0x00, 0x70, 0x03, 0x80, 0x00, 0x70, 0x07, 0x7F, 0xE0, 0x70,
  0x07, 0x7F, 0xE0, 0x70, 0x06, 0x0C, 0x00, 0x70, 0x0E, 0x0C, 0x0E, 0x70,
  0x0E, 0x0C, 0x0E, 0x70, 0x1E, 0x0C, 0x0E, 0x70, 0x1E, 0x1C, 0x0E, 0x70,
  0x3E, 0x1F, 0xEE, 0x70, 0x7E, 0x1F, 0xEE, 0x70, 0x7E, 0x38, 0xCE, 0x70,
  0xEE, 0x38, 0xCE, 0x70, 0x2E, 0x70, 0xCE, 0x70, 0x0E, 0x70, 0xCE, 0x70,
  0x0E, 0xED, 0xCE, 0x70, 0x0E, 0x4F, 0xCE, 0x70, 0x0E, 0x07, 0x8E, 0x70,
  0x0E, 0x03, 0x8E, 0x70, 0x0E, 0x03, 0x8E, 0x70, 0x0E, 0x03, 0x0E, 0x70,
  0x0E, 0x07, 0x00, 0x70, 0x0E, 0x0E, 0x00, 0x70, 0x0E, 0x1E, 0x00, 0x70,
  0x0E, 0x3C, 0x00, 0x70, 0x0E, 0x78, 0x03, 0xE0, 0x0E, 0x70, 0x03, 0xE0,
  0x0E, 0x20, 0x03, 0x80,
  // 0x7a0b - 0x7a0b
  0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x3F, 0xE3, 0xFF, 0xE0,
  0x7F, 0x03, 0xFF, 0xE0, 0x23, 0x03, 0x00, 0x60, 0x03, 0x03, 0x00, 0x60,
  0x03, 0x03, 0x00, 0x60, 0x03, 0x03, 0x00, 0x60, 0x03, 0x03, 0x00, 0x60,
  0x7F, 0xF3, 0xFF, 0xE0, 0x7F, 0xF3, 0xFF, 0xE0, 0x73, 0x70, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x07, 0xFF, 0xF0,
  0x0F, 0x67, 0xFF, 0xF0, 0x1F, 0x60, 0x18, 0x00, 0x1B, 0x70, 0x18, 0x00,
  0x3B, 0x38, 0x18, 0x00, 0x73, 0x20, 0x18, 0x00, 0xF3, 0x03, 0xFF, 0xE0,
  0x63, 0x03, 0xFF, 0xE0, 0x43, 0x00, 0x18, 0x00, 0x03, 0x00, 0x18, 0x00,
  0x03, 0x00, 0x18, 0x00, 0x03, 0x00, 0x18, 0x00, 0x03, 0x0F, 0xFF, 0xF8,
  0x03, 0x0F, 0xFF, 0xF8, 0x03, 0x00, 0x00, 0x00
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs0[] = {
  {     0,  28,  31,    1,  -24 },   // 0x4F8B
  {   112,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs1[] = {
  {   112,  28,  31,    2,  -23 },   // 0x662F
  {   220,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs2[] = {
  {   220,  28,  31,    2,  -24 },   // 0x6C49
  {   332,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs3[] = {
  {   332,  27,  31,    2,  -24 },   // 0x5B57
  {   444,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs4[] = {
  {   444,  28,  31,    1,  -25 },   // 0x5E93
  {   560,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs5[] = {
  {   560,  25,  31,    3,  -23 },   // 0x56FE
  {   668,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs6[] = {
  {   668,  29,  31,    1,  -24 },   // 0x5F62
  {   780,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs7[] = {
  {   780,  25,  31,    3,  -24 },   // 0x7684
  {   892,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs8[] = {
  {   892,  28,  31,    1,  -24 },   // 0x4F8B
  {  1004,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_glyph_t g_sSimhei16pt1bGlyphs9[] = {
  {  1004,  29,  31,    1,  -25 },   // 0x7A0B
  {  1120,   0,   0,    0,    0 }
};

// This struct will be read by the CPU only
static const nema_font_range_t simhei16pt1b_ranges[] = {
  {0x00004f8b, 0x00004f8b, g_sSimhei16pt1bGlyphs0},
  {0x0000662f, 0x0000662f, g_sSimhei16pt1bGlyphs1},
  {0x00006c49, 0x00006c49, g_sSimhei16pt1bGlyphs2},
  {0x00005b57, 0x00005b57, g_sSimhei16pt1bGlyphs3},
  {0x00005e93, 0x00005e93, g_sSimhei16pt1bGlyphs4},
  {0x000056fe, 0x000056fe, g_sSimhei16pt1bGlyphs5},
  {0x00005f62, 0x00005f62, g_sSimhei16pt1bGlyphs6},
  {0x00007684, 0x00007684, g_sSimhei16pt1bGlyphs7},
  {0x00004f8b, 0x00004f8b, g_sSimhei16pt1bGlyphs8},
  {0x00007a0b, 0x00007a0b, g_sSimhei16pt1bGlyphs9},
  {0, 0, NULL}
};

// This struct will be read by the CPU only
nema_font_t simhei16pt1b = {
  {
    .base_virt = (void *) g_sSimhei16pt1bBitmaps,
    .base_phys = (uintptr_t) g_sSimhei16pt1bBitmaps,
    .size      = (int) sizeof(g_sSimhei16pt1bBitmaps)
  },
  simhei16pt1b_ranges,
  (int)sizeof(g_sSimhei16pt1bBitmaps),
  g_sSimhei16pt1bBitmaps,
  0,
  16, 36, 26, 1
};
#endif // SIMHEI16PT1B_C

