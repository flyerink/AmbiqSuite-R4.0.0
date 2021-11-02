#include "ng_screen.h"
#include "ng_draw_prim.h"
#include "ng_globals.h"

DRAW_FUNC(ng_screen_draw){
	gitem_screen_t *screen = NG_SCREEN(git);
    if(git->flags & GITEMF_FILL_IMAGE){
        ng_blit_rect_fit(screen->image, screen->x + x_off, screen->y + y_off, screen->w, screen->h, 0, 0xffU);
    }else{
        ng_fill_rect(screen->x + x_off, screen->y + y_off, screen->w, screen->h, screen->color, 0);
    }
}

void ng_screen_set_image(gitem_base_t *git, void *asset_ptr) {
    gitem_screen_t *screen = NG_SCREEN(git);
	screen->image = (img_obj_t*) asset_ptr;
}
