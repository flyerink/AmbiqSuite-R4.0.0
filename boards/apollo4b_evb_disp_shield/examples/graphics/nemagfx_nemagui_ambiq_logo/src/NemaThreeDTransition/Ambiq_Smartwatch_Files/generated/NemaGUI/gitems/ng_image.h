#ifndef NG_GITEM_IMAGE_H__
#define NG_GITEM_IMAGE_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_image_t{
    BASE_STRUCT;
    img_obj_t *image;
} gitem_image_t;

DRAW_FUNC(ng_image_draw);

void ng_image_set_asset(gitem_base_t *git, void *asset_ptr);

#define NG_IMAGE(object) SAFE_CAST((object), gitem_base_t *, gitem_image_t *)

#endif //NG_GITEM_SCREEN_H__
