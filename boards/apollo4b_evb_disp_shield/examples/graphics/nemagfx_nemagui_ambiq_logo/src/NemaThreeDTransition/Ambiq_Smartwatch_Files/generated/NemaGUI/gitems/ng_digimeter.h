#ifndef NG_GITEM_DIGIMETER_H__
#define NG_GITEM_DIGIMETER_H__
#include "ng_gitem.h"
#include "ng_utils.h"
#include "ng_draw_prim.h"

typedef struct _gitem_digimeter_t{
    BASE_STRUCT;
    uint32_t     text_color;
    float        value;
    float        max_value;
    float        min_value;
    float        step;
    nema_font_t *font;
    uint32_t     alignment;
    uint8_t      precision;
    char        *suffix;
} gitem_digimeter_t;


DRAW_FUNC(ng_digimeter_draw);

void ng_digimeter_set_value(gitem_base_t *git, float value);
void ng_digimeter_set_percent(gitem_base_t *git, float percent);
void ng_digimeter_count_up(gitem_base_t *git);
void ng_digimeter_count_down(gitem_base_t *git);
void ng_digimeter_set_text_color(gitem_base_t *git, uint32_t rgba);

#define NG_DIGIMETER(object) SAFE_CAST((object), gitem_base_t *, gitem_digimeter_t *)

#endif //NG_GITEM_SCREEN_H__
