#ifndef NG_GITEM_RADIO_BUTTON_H__
#define NG_GITEM_RADIO_BUTTON_H__
#include "ng_gitem.h"
#include "ng_gestures.h"
#include "ng_utils.h"

extern gitem_gestures_t gestures_radio_button;

typedef struct _gitem_radio_button_t{
    BASE_STRUCT;
    uint32_t   secondary_color;
    uint16_t   radius;
} gitem_radio_button_t;

DRAW_FUNC(ng_radio_button_draw);

void ng_radio_button_toggle(tree_node_t *node);
void ng_radio_button_set_secondary_color(gitem_base_t *git, uint32_t rgba);

#define NG_RADIO_BUTTON(object) SAFE_CAST((object), gitem_base_t *, gitem_radio_button_t *)

#endif //NG_GITEM_SCREEN_H__
