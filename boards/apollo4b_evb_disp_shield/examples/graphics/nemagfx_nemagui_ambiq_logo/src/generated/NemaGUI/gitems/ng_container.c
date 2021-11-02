#include "ng_container.h"
#include "ng_draw_prim.h"

DRAW_FUNC(ng_container_draw){
	gitem_container_t *container = SAFE_CAST(git, gitem_base_t *, gitem_container_t *);

    if ( container->flags & GITEMF_FILL_IMAGE ) {
        ng_blit_rect_fit(container->image, container->x + x_off, container->y + y_off, container->w, container->h, 0, (container->color & 0xff000000U) >> 24);
    } else {
        if ( container->flags & GITEMF_FILL_COLOR ) {
            ng_fill_rect(container->x + x_off, container->y + y_off, container->w, container->h, container->color, 0);
        } else {
            ng_draw_primitive_rect(container->x + x_off, container->y + y_off, container->w, container->h, container->color, container->pen_width, 0);
        }
    }
}

void ng_container_set_image(gitem_base_t *git, void *asset_ptr) {
    gitem_container_t *container = NG_CONTAINER(git);
	container->image = (img_obj_t*) asset_ptr;
}
