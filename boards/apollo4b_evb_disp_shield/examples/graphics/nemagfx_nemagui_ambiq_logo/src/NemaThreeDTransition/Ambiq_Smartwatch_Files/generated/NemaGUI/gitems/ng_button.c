#include "ng_button.h"
#include "ng_draw_prim.h"

DRAW_FUNC(ng_button_draw){
	gitem_button_t *button = NG_BUTTON(git);

    if ( button->flags & GITEMF_FILL_IMAGE ) {
    	img_obj_t *image = (button->flags & GITEMF_HIGHLIGHTED) ? button->secondary_image : button->primary_image;
        ng_blit_rect_fit(image, button->x + x_off, button->y + y_off, button->w, button->h, 0, (button->color >> 24) & 0xffU);
    } else {
    	uint32_t color = (button->flags & GITEMF_HIGHLIGHTED) ? button->secondary_color : button->color;
        	if ( button->flags & GITEMF_FILL_COLOR ) {
        		ng_fill_rect(button->x + x_off, button->y + y_off, button->w, button->h, color, 0);
        	} else {
        		ng_draw_primitive_rect(button->x + x_off, button->y + y_off, button->w, button->h, color, button->pen_width, 0);
        	}
    }
}

void ng_button_set_primary_image(gitem_base_t *git, void *asset_ptr) {
	gitem_button_t *button = NG_BUTTON(git);
	button->primary_image = (img_obj_t*) asset_ptr;
}

void ng_button_set_secondary_image(gitem_base_t *git, void *asset_ptr) {
	gitem_button_t *button = NG_BUTTON(git);
	button->secondary_image = (img_obj_t*) asset_ptr;
}

void ng_button_set_secondary_color(gitem_base_t *git, uint32_t rgba) {
	gitem_button_t *button = NG_BUTTON(git);
	button->secondary_color = rgba;
}
