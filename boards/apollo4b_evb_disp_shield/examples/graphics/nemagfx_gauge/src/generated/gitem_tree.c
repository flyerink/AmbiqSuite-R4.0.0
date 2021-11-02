//*****************************************************************************
//
//! @file gitem_tree.c
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

#include "gitem.h"
#define RESX        224
#define RESY        224
#define NUM_GROUPS  1
#define NUM_POPUPS  0
#define TIMER_ANIM_PERIOD  16

int NUM_SCREENS[NUM_GROUPS] = {1};
int cur_screen_group = 0;
int cur_screen_node  = 0;

//Forward declaration of gitems and tree nodes
//---------------------------------
tree_node_t node_1149Gauge;
gitem_t _1149Gauge;
    tree_node_t node_1244Gauge1;
    gitem_t _1244Gauge1;
        tree_node_t node_1250Image1;
        gitem_t _1250Image1;
        tree_node_t node_1251Digital_Meter2;
        gitem_t _1251Digital_Meter2;
        tree_node_t node_1253Label1;
        gitem_t _1253Label1;
        tree_node_t node_1245Needle;
        gitem_t _1245Needle;
    tree_node_t node_1269Horizontal_Slider1;
    gitem_t _1269Horizontal_Slider1;
        tree_node_t node_1270Empty_Rect;
        gitem_t _1270Empty_Rect;
        tree_node_t node_1271Filled_Rect;
        gitem_t _1271Filled_Rect;
        tree_node_t node_1272Indicator;
        gitem_t _1272Indicator;

//---------------------------------

gitem_t _1149Gauge = {0, 0, 0, 0, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1149, 0xff000000, 0xff000000, 0xff4c4c4c, 0xff4c4c4c, 0, 0, 0, 0, 224, 224, 224, 224, 0, 0, -1, -1, -1, 1, 0, 0, 0, GITEM_SCREEN, 0, 0, 1, 0, 0, 0 };
    gitem_t _1244Gauge1 = {0, 0, 0, 0, 4.000, 0.000, 0.000, 8.000, -90.000, 166.000, 38.000, 1244, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0, 0, 0, 0, 224, 224, 224, 224, 0, 0, 1, -1, -1, 1, 112, 112, 0, GITEM_GAUGE, 0, 0, 0, 0, 0, 1 };
        gitem_t _1250Image1 = {0, 0, 0, 0, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1250, 0xff808080, 0xff808080, 0xff000000, 0xff000000, 99, 99, 80, 80, 30, 30, 30, 30, 0, 0, 2, -1, -1, 1, 0, 0, 0, GITEM_IMG, 0, 0, 0, 0, 0, 1 };
        char text_1251Digital_Meter2[] = "";
        gitem_t _1251Digital_Meter2 = {0, 0, 0, text_1251Digital_Meter2, 15.000, 1.000, 0.000, 30.000, 0.000, 0.000, 0.000, 1251, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 95, 95, 114, 114, 38, 38, 21, 21, 0, 0, -1, -1, 2, 1, 0, 0, 0, GITEM_DIGIMETER, 0, 1, 0, 0, 0, 0 };
        char text_1253Label1[] = "l/100 km";
        gitem_t _1253Label1 = {0, 0, 0, text_1253Label1, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1253, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 69, 69, 130, 130, 92, 92, 25, 25, 0, 0, -1, -1, 1, 1, 0, 0, 0, GITEM_LABEL, 10, 0, 0, 0, 0, 0 };
        gitem_t _1245Needle = {0, 0, 0, 0, 4.000, 0.000, 0.000, 8.000, -90.000, 166.000, 38.000, 1245, 0xff0000ff, 0xff0000ff, 0xff000000, 0xff000000, 112, 112, 112, 112, 58, 58, 19, 19, 0, 0, 0, -1, -1, 1, 112, 9, 0, GITEM_ROTATING_CONTAINER, 0, 0, 0, 0, 0, 1 };
    gitem_t _1269Horizontal_Slider1 = {0, 0, 0, 0, 50.000, 0.000, 0.000, 100.000, 0.000, 0.000, 0.000, 1269, 0x00ffffff, 0x00ffffff, 0xff000000, 0xff000000, 2, 2, 204, 204, 218, 218, 20, 20, 0, 0, -1, -1, -1, 1, 0, 0, 22, GITEM_H_SLIDER, 0, 0, 0, 0, 0, 0 };
        gitem_t _1270Empty_Rect = {0, 0, 0, 0, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1270, 0xffc8c8c8, 0xffc8c8c8, 0xff000000, 0xff000000, 109, 109, 8, 8, 99, 99, 5, 5, 0, 0, -1, -1, -1, 1, 0, 0, 0, GITEM_RECT, 0, 0, 1, 0, 0, 0 };
        gitem_t _1271Filled_Rect = {0, 0, 0, 0, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1271, 0xff505050, 0xff505050, 0xff000000, 0xff000000, 10, 10, 8, 8, 99, 99, 5, 5, 0, 0, -1, -1, -1, 1, 0, 0, 0, GITEM_RECT, 0, 0, 1, 0, 0, 0 };
        gitem_t _1272Indicator = {0, 0, 0, 0, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1272, 0xff969600, 0xff969600, 0xff000000, 0xff000000, 99, 99, 0, 0, 20, 20, 20, 20, 0, 0, 3, -1, -1, 1, 0, 0, 0, GITEM_CONT, 0, 0, 0, 0, 0, 1 };

//---------------------------------


//Build tree nodes
//---------------------------------
tree_node_t node_1149Gauge = { &_1149Gauge, NULL, &node_1244Gauge1, NULL};
    tree_node_t node_1244Gauge1 = { &_1244Gauge1, &node_1149Gauge, &node_1250Image1, &node_1269Horizontal_Slider1};
        tree_node_t node_1250Image1 = { &_1250Image1, &node_1244Gauge1, NULL, &node_1251Digital_Meter2};
        tree_node_t node_1251Digital_Meter2 = { &_1251Digital_Meter2, &node_1244Gauge1, NULL, &node_1253Label1};
        tree_node_t node_1253Label1 = { &_1253Label1, &node_1244Gauge1, NULL, &node_1245Needle};
        tree_node_t node_1245Needle = { &_1245Needle, &node_1244Gauge1, NULL, NULL};
    tree_node_t node_1269Horizontal_Slider1 = { &_1269Horizontal_Slider1, &node_1149Gauge, &node_1270Empty_Rect, NULL};
        tree_node_t node_1270Empty_Rect = { &_1270Empty_Rect, &node_1269Horizontal_Slider1, NULL, &node_1271Filled_Rect};
        tree_node_t node_1271Filled_Rect = { &_1271Filled_Rect, &node_1269Horizontal_Slider1, NULL, &node_1272Indicator};
        tree_node_t node_1272Indicator = { &_1272Indicator, &node_1269Horizontal_Slider1, NULL, NULL};

//---------------------------------


tree_node_t *group0_tree_nodes[] = {
    &node_1149Gauge,
};
tree_node_t **group_tree_nodes[NUM_GROUPS] = {group0_tree_nodes};

tree_node_t **screen_tree_nodes = group0_tree_nodes;

nema_transition_t group_effect[1] = {NEMA_TRANS_CUBE_H};

uint8_t group_layout[1] = {0};


//---------------------------------


tree_node_t *popup_tree_nodes[] = {
0};

//---------------------------------

