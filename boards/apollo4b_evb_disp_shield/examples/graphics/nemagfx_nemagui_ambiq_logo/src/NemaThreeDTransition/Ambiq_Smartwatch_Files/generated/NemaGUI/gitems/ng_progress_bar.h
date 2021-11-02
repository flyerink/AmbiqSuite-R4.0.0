#ifndef NG_GITEM_PROGRESS_BAR_H__
#define NG_GITEM_PROGRESS_BAR_H__
#include "ng_gitem.h"
#include "ng_utils.h"

typedef struct _gitem_progress_bar_t{
    BASE_STRUCT;
    uint32_t foreground_color;
    img_obj_t *background_image;
    img_obj_t *foreground_image;
    float      value;
    uint16_t   pen_width;
} gitem_progress_bar_t;

DRAW_FUNC(ng_horizontal_progress_bar_draw);
DRAW_FUNC(ng_vertical_progress_bar_draw);

void ng_progress_bar_set_percent(gitem_base_t *git, float percent);
void ng_progress_bar_set_background_image(gitem_base_t *git, void *asset_ptr);
void ng_progress_bar_set_foreground_image(gitem_base_t *git, void *asset_ptr);
void ng_progress_bar_set_foreground_color(gitem_base_t *git, uint32_t rgba);

#define NG_PROGRESS_BAR(object) SAFE_CAST((object), gitem_base_t *, gitem_progress_bar_t *)

#endif //NG_GITEM_SCREEN_H__
