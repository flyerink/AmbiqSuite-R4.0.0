#include "ng_needle.h"
#include "ng_draw_prim.h"

void nema_set_raster_color(uint32_t rgba8888);

DRAW_FUNC(ng_needle_draw){
    gitem_needle_t *needle = NG_NEEDLE(git);

    if((needle->color & 0xff000000) == 0){
        return;
    }

    //  P0-------P1
    //  | .rot    |
    //  P3-------P2
    //move rotation center to (0, 0)
    float x0 = -needle->x_rot;
    float y0 = -needle->y_rot;
    float x1 = x0 + git->w;
    float y1 = y0;
    float x2 = x1;
    float y2 = y1 + git->h;
    float x3 = x0;
    float y3 = y2;

    //translate rotation center to final coords
    int cx = git->x + x_off;
    int cy = git->y + y_off;

    //calculate rotation matrix
    nema_matrix3x3_t m;
    nema_mat3x3_load_identity(m);
    nema_mat3x3_rotate(m, needle->angle);
    nema_mat3x3_translate(m, cx, cy);

    //rotate points
    nema_mat3x3_mul_vec(m, &x0, &y0);
    nema_mat3x3_mul_vec(m, &x1, &y1);
    nema_mat3x3_mul_vec(m, &x2, &y2);
    nema_mat3x3_mul_vec(m, &x3, &y3);

    if ( git->flags & GITEMF_FILL_IMAGE ) {
        if(needle->image == NULL){
            return;
        }

        ng_blit_quad_fit( needle->image,
                          x0, y0,
                          x1, y1,
                          x2, y2,
                          x3, y3, 0,
                          (needle->color & 0xff000000) >> 24);
    } else {
        ng_fill_quad(x0, y0,
                     x1, y1,
                     x2, y2,
                     x3, y3, 0,
                     needle->color);
    }
}

void ng_needle_set_image(gitem_base_t *git, void *asset_ptr) {
    gitem_needle_t *needle = NG_NEEDLE(git);
	needle->image = (img_obj_t*) asset_ptr;
}
