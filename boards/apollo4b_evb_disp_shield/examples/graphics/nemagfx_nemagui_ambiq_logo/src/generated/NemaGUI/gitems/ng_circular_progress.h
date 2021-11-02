#ifndef NG_GITEM_CIRCULAR_PROGRESS_H__
#define NG_GITEM_CIRCULAR_PROGRESS_H__
#include "ng_gitem.h"
#include "ng_utils.h"

extern gitem_gestures_t gestures_circular_progress;

typedef struct _gitem_circular_progress_t{
    BASE_STRUCT;
    img_obj_t *background_image;
    img_obj_t *foreground_image;
    float      value;
    float      max_angle;
    float      min_angle;
} gitem_circular_progress_t;

void ng_circular_progress_set_percent(gitem_base_t *git, float percent);

DRAW_FUNC(ng_circular_progress_draw);

void ng_circular_progress_set_background_image(gitem_base_t *git, void *asset_ptr);
void ng_circular_progress_set_foreground_image(gitem_base_t *git, void *asset_ptr);

#define NG_CIRCULAR_PROGRESS(object) SAFE_CAST((object), gitem_base_t *, gitem_circular_progress_t *)

#endif //NG_GITEM_SCREEN_H__
