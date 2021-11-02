#include "ng_circle.h"
#include "ng_draw_prim.h"

DRAW_FUNC(ng_circle_draw){
	gitem_circle_t *circle = SAFE_CAST(git, gitem_base_t *, gitem_circle_t *);

    const int r  = git->w>>1;
    const int x0 = git->x + x_off + r;
    const int y0 = git->y + y_off + r;

    if (circle->flags & GITEMF_FILL_COLOR){
        ng_fill_primitive_circle(x0, y0, r, circle->color, 0);
    }else{
        ng_draw_primitive_circle(x0, y0, r, circle->color, 0);
    }
}