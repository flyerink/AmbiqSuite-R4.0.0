//*****************************************************************************
//
//! @file load_objects.c
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

#define FRAME_BUFFERS 4
#define BACK_BUFFERS 2

#define RESX        224
#define RESY        224

img_obj_t fb[FRAME_BUFFERS];
nemadc_layer_t layer[FRAME_BUFFERS] = {{0}};

void load_objects(void)
{
    for (int i = 0; i < FRAME_BUFFERS; ++i)
    {
        int format = i < (FRAME_BUFFERS - BACK_BUFFERS) ? NEMA_TSC4 : NEMA_RGB565;

        fb[i].stride = nema_stride_size(format, 0, RESX);
        fb[i].w = RESX;
        fb[i].h = RESY;
        fb[i].format = format;

        if (format == NEMA_TSC4)
        {
            fb[i].bo = nema_buffer_create(RESX * RESY / 2);
        }
        else
        {
            fb[i].bo = nema_buffer_create(fb[i].stride * fb[i].h);
        }

        nema_buffer_map(&fb[i].bo);

        if (i < FRAME_BUFFERS - BACK_BUFFERS)
        {
            layer[i].sizex         = layer[i].resx = fb[i].w;
            layer[i].sizey         = layer[i].resy = fb[i].h;
            layer[i].stride        = -1;
            layer[i].format        = NEMADC_TSC4;
            layer[i].blendmode     = NEMADC_BL_SRC;
            layer[i].baseaddr_phys = fb[i].bo.base_phys;
            layer[i].baseaddr_virt = fb[i].bo.base_virt;
        }

        PRINT_I("FB[%d]: V:%p P:0x%08x\n ", i, (void *)fb[i].bo.base_virt, fb[i].bo.base_phys);
    }
}
