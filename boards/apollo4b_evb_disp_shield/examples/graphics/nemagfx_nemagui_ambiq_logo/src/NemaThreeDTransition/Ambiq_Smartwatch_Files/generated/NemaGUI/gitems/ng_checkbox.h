#ifndef NG_GITEM_CHECKBOX_H__
#define NG_GITEM_CHECKBOX_H__
#include "ng_gitem.h"
#include "ng_tree.h"
#include "ng_gestures.h"
#include "ng_utils.h"

extern gitem_gestures_t gestures_checkbox;

typedef struct _gitem_checkbox_t{
    BASE_STRUCT;
    uint32_t   secondary_color;
    img_obj_t *image;
    uint16_t   pen_width;
    uint16_t   radius;
} gitem_checkbox_t;

DRAW_FUNC(ng_checkbox_draw);

void ng_checkbox_set_image(gitem_base_t *git, void *asset_ptr);
void ng_checkbox_set_secondary_color(gitem_base_t *git, uint32_t rgba);

#define NG_CHECKBOX(object) SAFE_CAST((object), gitem_base_t *, gitem_checkbox_t *)

#endif //NG_GITEM_CHECKBOX_H__
