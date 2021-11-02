#ifndef NG_GITEM_NEEDLE_H__
#define NG_GITEM_NEEDLE_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_needle_t{
    BASE_STRUCT;
    img_obj_t *image;
    float      angle;
    int        x_rot;
    int        y_rot;
    uint16_t   pen_width;
} gitem_needle_t;

DRAW_FUNC(ng_needle_draw);

void ng_needle_set_image(gitem_base_t *git, void *asset_ptr);

#define NG_NEEDLE(object) SAFE_CAST((object), gitem_base_t *, gitem_needle_t *)

#endif //NG_GITEM_SCREEN_H__
