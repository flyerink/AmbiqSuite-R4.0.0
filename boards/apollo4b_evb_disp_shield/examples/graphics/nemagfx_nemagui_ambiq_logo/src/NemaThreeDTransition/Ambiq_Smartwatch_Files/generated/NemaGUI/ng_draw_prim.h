#ifndef NG_DRAW_PRIM_H_
#define NG_DRAW_PRIM_H_

#include "nema_sys_defs.h"

void ng_fill_rect(int x, int y, int w, int h, uint32_t color, int override_blend);
void ng_draw_primitive_rect(int x, int y, int w, int h, uint32_t color, int pen_width,int override_blend);
void ng_draw_primitive_rounded_rect(int x, int y, int w, int h, uint32_t color, int radius,int override_blend);
void ng_fill_primitive_rounded_rect(int x, int y, int w, int h, uint32_t color, int radius,int override_blend);

void ng_draw_primitive_circle(int x, int y, int r, uint32_t color, int override_blend);
void ng_fill_primitive_circle(int x, int y, int r, uint32_t color, int override_blend);

void ng_blit_rect_fit(img_obj_t *img,
        int x, int y,
        int w, int h,
        int override_blend,
        uint8_t opacity
    );

void ng_blit_quad_fit(img_obj_t *img,
        float x0, float y0,
        float x1, float y1,
        float x2, float y2,
        float x3, float y3,
        int override_blend,
        uint8_t opacity
    );

void ng_fill_quad( float x0, float y0,
                   float x1, float y1,
                   float x2, float y2,
                   float x3, float y3,
                   int override_blend,
                   uint32_t color
    );

#endif //NG_DRAW_PRIM_H_