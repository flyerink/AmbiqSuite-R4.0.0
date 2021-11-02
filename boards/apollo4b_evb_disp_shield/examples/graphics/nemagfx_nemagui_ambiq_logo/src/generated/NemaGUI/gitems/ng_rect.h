#ifndef NG_GITEM_RECT_H__
#define NG_GITEM_RECT_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_rect_t{
    BASE_STRUCT;
    uint16_t  pen_width;
} gitem_rect_t;

DRAW_FUNC(ng_rect_draw);

#define NG_RECT(object) SAFE_CAST((object), gitem_base_t *, gitem_rect_t *)

#endif //NG_GITEM_SCREEN_H__
