#ifndef NG_GITEM_WATCHFACE_H__
#define NG_GITEM_WATCHFACE_H__
#include "ng_gitem.h"

// typedef struct _gitem_prim_needle_t{
//     img_obj_t *image;
//     float      value;
//     float      max_value;
//     float      min_value;
//     float      angle;
//     float      max_angle;
//     float      min_angle;
//     int        x_rot;
//     int        y_rot;
//     uint16_t   pen_width;
//     uint16_t   radius;
// } prim_needle_t;

typedef struct _gitem_watchface_t{
    BASE_STRUCT;
    img_obj_t    *image;
    gitem_base_t *hour;
    gitem_base_t *minute;
    gitem_base_t *sec;
    uint16_t      pen_width;
} gitem_watchface_t;

DRAW_FUNC(ng_watchface_draw);

void ng_watchface_set_time(gitem_base_t *git, float hour, float min, float sec);

extern const gitem_gestures_t watchface_gestures;

#define NG_WATCHFACE(object) SAFE_CAST((object), gitem_base_t *, gitem_watchface_t *)

#endif //NG_GITEM_SCREEN_H__
