#ifndef NG_GITEM_LABEL_H__
#define NG_GITEM_LABEL_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_label_t{
    BASE_STRUCT;
    attr_text_t *text;
    uint32_t    text_color;
} gitem_label_t;

DRAW_FUNC(ng_label_draw);

void ng_label_set_text_color(gitem_base_t *git, uint32_t rgba);

#define NG_LABEL(object) SAFE_CAST((object), gitem_base_t *, gitem_label_t *)

#endif //NG_GITEM_SCREEN_H__
