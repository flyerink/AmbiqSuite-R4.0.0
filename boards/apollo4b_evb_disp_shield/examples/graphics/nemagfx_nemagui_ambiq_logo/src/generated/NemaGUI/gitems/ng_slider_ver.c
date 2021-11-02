#include "ng_slider.h"
#include "ng_slider_ver.h"
#include "ng_globals.h"

void ng_slider_vertical_set_indicator_y(gitem_base_t *git, int y)
{
    // <-----------   SLIDER   ------------->
    // foreground   indicator   background
    // ================[X]-------------------
    gitem_slider_t *slider    = NG_SLIDER(git);
    gitem_base_t *background  = slider->track;
    gitem_base_t *foreground  = slider->fill;
    gitem_base_t *indicator   = slider->indicator;

    //indicator radius
    int indic_r = indicator->h>>1;

    //set indicator position
    y -= indic_r;
    indicator->y = y <= indic_r ? indic_r : y;
    indicator->y = y >= slider->h - indic_r ? slider->h - indic_r : y;

    //clip indicator position
    if ((indicator->y) < 0) {
        indicator->y = 0;
    }
    else if ((indicator->y) > slider->h-indicator->h) {
        indicator->y = slider->h-indicator->h;
    }

    //set filled_rect from start to indicator center
    background->y = indic_r;
    background->h = indicator->y;
    //set empty_rect from indicator center to finish
    foreground->y = indicator->y + indic_r;
    foreground->h = slider->h - indic_r - indicator->y - indic_r;
}

static GESTURE_FUNC_PRESS(press) {
    (void)x;
    gitem_slider_t *slider     = NG_SLIDER(node->this_);
    gitem_base_t   *indicator  = slider->indicator;
    gitem_base_t   *foreground = slider->fill;

    ng_gitem_set_flag(indicator , GITEMF_HIGHLIGHTED);
    ng_gitem_set_flag(foreground, GITEMF_HIGHLIGHTED);

    float indicator_y = (float) (y - indicator->h/2);
    float         val = 1.f - indicator_y/(slider->h - indicator->h);

    ng_slider_set_percent(NG_GITEM(slider), val);
}

static GESTURE_FUNC_RELEASE(release) {
    gitem_slider_t *slider   = NG_SLIDER(node->this_);
    gitem_base_t *indicator  = slider->indicator;
    gitem_base_t *foreground = slider->fill;

    ng_gitem_unset_flag(indicator , GITEMF_HIGHLIGHTED);
    ng_gitem_unset_flag(foreground, GITEMF_HIGHLIGHTED);
}


static GESTURE_FUNC_SWIPE(swipe) {
    (void)dx;
    (void)dy;

    press(node, x, y);
}

gitem_gestures_t gestures_slider_ver = {press, release, swipe, NULL};
