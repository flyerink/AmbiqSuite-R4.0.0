#ifndef NG_GITEM_SLIDER_H__
#define NG_GITEM_SLIDER_H__
#include "ng_gitem.h"

typedef struct _gitem_slider_t{
    BASE_STRUCT;
    float   value;
    gitem_base_t *track;
    gitem_base_t *fill;
    gitem_base_t *indicator;
} gitem_slider_t;

void ng_slider_set_percent(gitem_base_t *git, float percent);
void ng_slider_horizontal_set_indicator_x(gitem_base_t *git, int x);
void ng_slider_vertical_set_indicator_y(gitem_base_t *git, int y);

#define NG_SLIDER(object) SAFE_CAST((object), gitem_base_t *, gitem_slider_t *)

#endif //NG_GITEM_SCREEN_H__
