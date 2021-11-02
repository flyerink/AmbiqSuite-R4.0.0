#ifndef NG_GITEM_BUTTON_H__
#define NG_GITEM_BUTTON_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_button_t{
    BASE_STRUCT;
    uint32_t   secondary_color;
    img_obj_t *primary_image;
    img_obj_t *secondary_image;
    uint16_t   pen_width;
} gitem_button_t;

DRAW_FUNC(ng_button_draw);

void ng_button_set_primary_image  (gitem_base_t *git, void *asset_ptr);
void ng_button_set_secondary_image(gitem_base_t *git, void *asset_ptr);
void ng_button_set_secondary_color(gitem_base_t *git, uint32_t rgba);

#define NG_BUTTON(object) SAFE_CAST((object), gitem_base_t *, gitem_button_t *)

#endif //NG_GITEM_SCREEN_H__
