#ifndef NG_GITEM_ROUNDED_RECT_H__
#define NG_GITEM_ROUNDED_RECT_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_rounded_rect_t{
    BASE_STRUCT;
    uint16_t  radius;
} gitem_rounded_rect_t;

DRAW_FUNC(ng_rounded_rect_draw);

#define NG_ROUNDED_RECT(object) SAFE_CAST((object), gitem_base_t *, gitem_rounded_rect_t *)

#endif //NG_GITEM_SCREEN_H__
