#include "ng_image.h"
#include "ng_draw_prim.h"

DRAW_FUNC(ng_image_draw)
{
	gitem_image_t *im = NG_IMAGE(git);
	uint8_t opacity = (im->color & 0xff000000) >> 24;
	ng_blit_rect_fit(im->image, im->x + x_off, im->y + y_off, im->w, im->h, 0, opacity);
}

void ng_image_set_asset(gitem_base_t *git, void *asset_ptr) {
	gitem_image_t *image = NG_IMAGE(git);
	image->image = (img_obj_t*) asset_ptr;
}
