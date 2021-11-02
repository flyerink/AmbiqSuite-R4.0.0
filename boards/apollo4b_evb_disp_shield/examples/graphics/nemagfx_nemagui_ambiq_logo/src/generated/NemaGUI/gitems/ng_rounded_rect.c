#include "ng_rounded_rect.h"
#include "ng_draw_prim.h"

DRAW_FUNC(ng_rounded_rect_draw){
    gitem_rounded_rect_t *rrect = NG_ROUNDED_RECT(git);

    if(rrect->flags & GITEMF_FILL_COLOR){
        ng_fill_primitive_rounded_rect(rrect->x + x_off, rrect->y + y_off, rrect->w, rrect->h, rrect->color, rrect->radius, 0);
    }
    else{
        ng_draw_primitive_rounded_rect(rrect->x + x_off, rrect->y + y_off, rrect->w, rrect->h, rrect->color, rrect->radius, 0);
    }
}
