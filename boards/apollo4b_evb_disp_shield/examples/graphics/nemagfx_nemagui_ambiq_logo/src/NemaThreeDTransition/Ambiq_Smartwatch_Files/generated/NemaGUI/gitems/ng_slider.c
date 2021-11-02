#include "ng_slider.h"
#include "ng_container.h"
#include "ng_event.h"

static void update_indicator_position(gitem_slider_t *slider) {
    if ( slider->type == GITEM_HORIZONTAL_SLIDER ) {
        int x = slider->value*slider->w;
        ng_slider_horizontal_set_indicator_x(NG_GITEM(slider), x);
    } else {
        int y = slider->h - slider->value*slider->h;
        ng_slider_vertical_set_indicator_y(NG_GITEM(slider), y);
    }
}

void ng_slider_set_percent(gitem_base_t *git, float percent) {
    gitem_slider_t *slider = NG_SLIDER(git);
    float val_init = slider->value;
    slider->value = percent;

    if ( val_init != percent ) {
        update_indicator_position(slider);
        if (slider->event) {
            ng_event_handle(slider->event, EV_TRIGGER_VALUE_CHANGED);
        }
    }
}