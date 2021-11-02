#ifndef NG_GITEM_SCREEN_H__
#define NG_GITEM_SCREEN_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_screen_t{
    BASE_STRUCT;
    img_obj_t        *image;
}gitem_screen_t;

DRAW_FUNC(ng_screen_draw);

void ng_screen_set_image(gitem_base_t *git, void *asset_ptr);

#define NG_SCREEN(object) SAFE_CAST((object), gitem_base_t *, gitem_screen_t *)

#endif //NG_GITEM_SCREEN_H__
