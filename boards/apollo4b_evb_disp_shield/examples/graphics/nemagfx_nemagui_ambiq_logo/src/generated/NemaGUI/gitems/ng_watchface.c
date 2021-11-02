#include "ng_globals.h"


#include "nema_utils.h"

#define WIDGET_NAME "watchface"

// static GESTURE_FUNC_ABORT_PRESS(abort) {

// }

// static GESTURE_FUNC_PRESS(press) {

// }

// static GESTURE_FUNC_RELEASE(release) {

// }

// static GESTURE_FUNC_SWIPE(swipe) {

// }

const gitem_gestures_t watchface_gestures = {NULL, NULL, NULL, NULL};

DRAW_FUNC(ng_watchface_draw) {
    gitem_watchface_t *watchface = NG_WATCHFACE(git);

    //Draw background
    if ( watchface->flags & GITEMF_FILL_IMAGE ) {
        ng_blit_rect_fit(watchface->image, watchface->x + x_off, watchface->y + y_off, watchface->w, watchface->h, 0, (watchface->color & 0xff000000U) >> 24);
    } else {
        if ( watchface->flags & GITEMF_FILL_COLOR ) {
            ng_fill_rect(watchface->x + x_off, watchface->y + y_off, watchface->w, watchface->h, watchface->color, 0);
        } else {
            ng_draw_primitive_rect(watchface->x + x_off, watchface->y + y_off, watchface->w, watchface->h, watchface->color, watchface->pen_width, 0);
        }
    }
}

void ng_watchface_set_image(gitem_base_t *git, void *asset_ptr) {
    gitem_watchface_t *watchface = NG_WATCHFACE(git);
	watchface->image = (img_obj_t*) asset_ptr;
}
