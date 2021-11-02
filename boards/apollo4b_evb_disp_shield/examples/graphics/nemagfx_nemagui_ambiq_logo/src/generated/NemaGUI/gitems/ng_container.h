#ifndef NG_GITEM_CONTAINER_H__
#define NG_GITEM_CONTAINER_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_container_t{
    BASE_STRUCT;
    img_obj_t *image;
    uint16_t pen_width;
} gitem_container_t;

#define NG_CONTAINER(object) SAFE_CAST((object), gitem_base_t *, gitem_container_t *)

void ng_container_set_image(gitem_base_t *git, void *asset_ptr);

DRAW_FUNC(ng_container_draw);
#endif //NG_GITEM_SCREEN_H__
