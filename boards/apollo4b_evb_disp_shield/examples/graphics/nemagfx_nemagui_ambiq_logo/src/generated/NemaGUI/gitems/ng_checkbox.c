#include "ng_checkbox.h"
#include "ng_draw_prim.h"
#include "ng_event.h"

static GESTURE_FUNC_PRESS(press) {
    (void)x;
    (void)y;
    ng_gitem_set_flag(node->this_, GITEMF_HIGHLIGHTED);
}

static GESTURE_FUNC_RELEASE(release) {
    ng_gitem_unset_flag(node->this_, GITEMF_HIGHLIGHTED);

    if(node->this_->flags & GITEMF_CHECKED){
    	ng_gitem_unset_flag(node->this_, GITEMF_CHECKED);
    }else{
    	ng_gitem_set_flag(node->this_, GITEMF_CHECKED);
    }

    if(node->this_->event){
    	ng_event_handle(node->this_->event, EV_TRIGGER_RELEASE);
    }
}

gitem_gestures_t gestures_checkbox = {press, release, NULL, NULL};

DRAW_FUNC(ng_checkbox_draw)
{
    gitem_checkbox_t *checkbox = SAFE_CAST(git, gitem_base_t *, gitem_checkbox_t *);
    const uint32_t color = (checkbox->flags & GITEMF_HIGHLIGHTED) ? checkbox->secondary_color : checkbox->color;

    if(checkbox->flags & GITEMF_CHECKED){
        if(checkbox->flags & GITEMF_CHECKED){
            if(git->flags & GITEMF_FILL_IMAGE){
                ng_blit_rect_fit(checkbox->image, checkbox->x + x_off, checkbox->y + y_off, checkbox->w, checkbox->h, 0, (checkbox->color >> 24) & 0xffU);
            }else{
                ng_fill_rect(checkbox->x + x_off + 4, checkbox->y + y_off + 4, checkbox->w - 8, checkbox->h - 8, color, 0);
            }
        }
    }

    //Draw border
    ng_draw_primitive_rect(checkbox->x + x_off, checkbox->y + y_off, checkbox->w, checkbox->h, color, checkbox->pen_width, 0);
}

void ng_checkbox_set_image(gitem_base_t *git, void *asset_ptr) {
    gitem_checkbox_t *checkbox = NG_CHECKBOX(git);
	checkbox->image = (img_obj_t*) asset_ptr;
}

void ng_checkbox_set_secondary_color(gitem_base_t *git, uint32_t rgba) {
    gitem_checkbox_t *checkbox = NG_CHECKBOX(git);
	checkbox->secondary_color = rgba;
}
