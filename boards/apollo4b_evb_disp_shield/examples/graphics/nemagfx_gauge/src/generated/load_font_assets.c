//*****************************************************************************
//
//! @file load_font_assets.c
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

#include "nema_font.h"
#include "nema_utils.h"

#define NEMA_FONT_IMPLEMENTATION
#include "assets/fonts/arial8pt8b.h"
#include "assets/fonts/arial6pt8b.h"
#include "assets/fonts/arial14pt8b.h"
#undef NEMA_FONT_IMPLEMENTATION

nema_font_t *font_assets[3];

void load_font_assets(void)
{
#if 0
    arial8pt8b.bo = nema_load_file("assets/fonts/arial8pt8b.bin", -1, 0);
    font_assets[0] = &arial8pt8b;
    arial6pt8b.bo = nema_load_file("assets/fonts/arial6pt8b.bin", -1, 0);
    font_assets[1] = &arial6pt8b;
    arial14pt8b.bo = nema_load_file("assets/fonts/arial14pt8b.bin", -1, 0);
    font_assets[2] = &arial14pt8b;
#else
    arial8pt8b.bo = LOAD_ARRAY(arial8pt8bBitmaps);
    font_assets[0] = &arial8pt8b;
    arial6pt8b.bo = LOAD_ARRAY(arial6pt8bBitmaps);
    font_assets[1] = &arial6pt8b;
    arial14pt8b.bo = LOAD_ARRAY(arial14pt8bBitmaps);
    font_assets[2] = &arial14pt8b;
#endif
}
