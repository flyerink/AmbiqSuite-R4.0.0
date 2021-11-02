#include "ng_radio_button.h"
#include "ng_draw_prim.h"
#include "ng_event.h"

static GESTURE_FUNC_PRESS(press) {
    (void)x;
    (void)y;
	ng_gitem_set_flag(node->this_, GITEMF_HIGHLIGHTED);
}

static GESTURE_FUNC_RELEASE(release) {
    ng_gitem_unset_flag(node->this_, GITEMF_HIGHLIGHTED);

    tree_node_t *table = node;
    do {
        table = table->parent;
    } while(table->this_->type != GITEM_TABLE);

    ng_radio_button_toggle(table->first_child);
    ng_gitem_set_flag(node->this_, GITEMF_CHECKED);

    if(node->this_->event){
    	ng_event_handle(node->this_->event, EV_TRIGGER_RELEASE);
    }
}

gitem_gestures_t gestures_radio_button = {press, release, NULL, NULL};

DRAW_FUNC(ng_radio_button_draw){
    gitem_radio_button_t *radio_button = NG_RADIO_BUTTON(git);

    uint32_t color = radio_button->color;
    if (radio_button->flags & GITEMF_HIGHLIGHTED){
        color = radio_button->secondary_color;
    }

    if((color & 0xff000000) == 0){
        return;
    }

    if((color & 0xff000000) == 0xff000000 ){
        nema_set_blend_fill(NEMA_BL_SRC);
    }else{
        nema_set_blend_fill(NEMA_BL_SIMPLE);
    }

    const int r_out  = radio_button->w>>1;
    const int x0     = radio_button->x + x_off + r_out;
    const int y0     = radio_button->y + y_off + r_out;

    if(radio_button->flags & GITEMF_CHECKED){
        nema_fill_circle(x0, y0, radio_button->radius, color);
    }

    nema_draw_circle(x0, y0, r_out, color);
}

void ng_radio_button_toggle(tree_node_t *node){
	while (node && node->this_) {
        if (node->this_->type == GITEM_RADIO_BUTTON) {
            ng_gitem_unset_flag(node->this_, GITEMF_CHECKED);
        }

        if (node->first_child) {
            ng_radio_button_toggle(node->first_child);
        }

        //go to next item at the same level;
        node = node->next;
    }
}

void ng_radio_button_set_secondary_color(gitem_base_t *git, uint32_t rgba) {
    gitem_radio_button_t *radio_button = NG_RADIO_BUTTON(git);
	radio_button->secondary_color = rgba;
}
