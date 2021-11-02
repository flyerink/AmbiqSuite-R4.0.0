#include "ng_globals.h"
#include "ng_slider.h"
#include "ng_slider_hor.h"

void ng_slider_horizontal_set_indicator_x(gitem_base_t *git, int x) {
    // <-----------   SLIDER   ------------->
    // filled_rect   indicator   empty_rect
    // ================[X]-------------------
    gitem_slider_t *slider     = NG_SLIDER(git);
    gitem_base_t   *background = slider->track;
    gitem_base_t   *foreground = slider->fill;
    gitem_base_t   *indicator  = slider->indicator;

    //indicator radius
    int indic_r = indicator->w>>1;

    //set indicator position
    x -= indic_r;
    indicator->x = x <= indic_r ? indic_r : x;
    indicator->x = x >= slider->w - indic_r ? slider->w - indic_r : x;

    //clip indicator position
    if ((indicator->x) < 0)
        indicator->x = 0;
    else if ((indicator->x) > slider->w-indicator->w)
        indicator->x = slider->w-indicator->w;

    //set filled_rect from start to indicator center
    foreground->x = indic_r;
    foreground->w = indicator->x; //+ indic_r - filled_rect->x;
    //set empty_rect from indicator center to finish
    background->x = indicator->x + indic_r;
    background->w = (slider->w-indic_r) - background->x;
}

static GESTURE_FUNC_PRESS(press) {
    (void)y;
    gitem_slider_t *slider  = NG_SLIDER(node->this_);
    gitem_base_t *indicator = slider->indicator;


    float indicator_x = (float) (x - indicator->w/2);
    float         val = indicator_x/(slider->w - indicator->w);

    ng_slider_set_percent(NG_GITEM(slider), val);
}

static GESTURE_FUNC_SWIPE(swipe) {
    (void)dx;
    (void)dy;
	press(node, x, y);
}

gitem_gestures_t gestures_slider_hor = {press, NULL, swipe, NULL};
