#ifndef NG_GITEM_GAUGE_H__
#define NG_GITEM_GAUGE_H__
#include "ng_gitem.h"
#include "ng_gestures.h"
#include "ng_utils.h"

extern gitem_gestures_t gestures_gauge;

typedef struct _gitem_gauge_t{
    BASE_STRUCT;
    img_obj_t    *image;
    gitem_base_t *needle;
    float         value;
    float         max_value;
    float         min_value;
    float         angle;
    float         max_angle;
    float         min_angle;
    int           x_rot;
    int           y_rot;
    uint16_t      pen_width;
} gitem_gauge_t;

DRAW_FUNC(ng_gauge_draw);

void ng_gauge_set_value(gitem_base_t *git, float value);
void ng_gauge_set_percent(gitem_base_t *git, float percent);
void ng_gauge_set_image(gitem_base_t *git, void *asset_ptr);

#define NG_GAUGE(object) SAFE_CAST((object), gitem_base_t *, gitem_gauge_t *)
#endif //NG_GITEM_SCREEN_H__
