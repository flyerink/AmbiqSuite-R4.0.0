#include "ng_rect.h"
#include "ng_draw_prim.h"

DRAW_FUNC(ng_rect_draw)
{
	gitem_rect_t *rect = NG_RECT(git);

	if(rect->flags & GITEMF_FILL_COLOR){
		ng_fill_rect(rect->x + x_off, rect->y + y_off, rect->w, rect->h, rect->color, 0);
	}else{
		ng_draw_primitive_rect(rect->x + x_off, rect->y + y_off, rect->w, rect->h, rect->color, rect->pen_width, 0);
	}
}
