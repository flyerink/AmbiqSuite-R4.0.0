#include "ng_globals.h"
#include "ng_draw_prim.h"

static inline void set_blend_fill(uint32_t color) {
    if((color & 0xff000000U) == 0xff000000U){
        nema_set_blend_fill(NEMA_BL_SRC);
    }
    else{
        nema_set_blend_fill(NEMA_BL_SIMPLE);
    }
}

void ng_fill_rect(int x, int y, int w, int h, uint32_t color, int override_blend) {
    if ((color & 0xff000000U) == 0U){
        return;
    }

    if (override_blend == 0) {
        set_blend_fill(color);
    }

    nema_fill_rect(x, y, w, h, color);
}

void ng_draw_primitive_rect(int x, int y, int w, int h, uint32_t color, int pen_width, int override_blend){
    //skip operation if gitem is completely transparent
    if((color & 0xff000000U) == 0){
        return;
    }

    if (override_blend == 0) {
        set_blend_fill(color);
    }

    if(pen_width <= 1){
        nema_draw_rect(x, y, w, h, color);
    }
    else{
        ng_fill_rect(x, y, w, pen_width, color, 0);
        ng_fill_rect(x, y + h - pen_width, w, pen_width, color, 1);
        ng_fill_rect(x, y + pen_width, pen_width, h-2*pen_width, color, 1);
        ng_fill_rect(x + w - pen_width, y + pen_width, pen_width, h - 2*pen_width, color, 1);
    }
}

void ng_draw_primitive_rounded_rect(int x, int y, int w, int h, uint32_t color, int radius,int override_blend){
    if ((color & 0xff000000U) == 0U){
        return;
    }

    if (override_blend == 0) {
        set_blend_fill(color);
    }

    nema_draw_rounded_rect(x, y, w, h, radius, color);
}

void ng_fill_primitive_rounded_rect(int x, int y, int w, int h, uint32_t color, int radius,int override_blend){
    if ((color & 0xff000000U) == 0U){
        return;
    }

    if (override_blend == 0) {
        set_blend_fill(color);
    }

    nema_fill_rounded_rect(x, y, w, h, radius, color);
}

void ng_draw_primitive_circle(int x, int y, int r, uint32_t color, int override_blend) {
    if ((color & 0xff000000U) == 0U){
        return;
    }

    if (override_blend == 0) {
        set_blend_fill(color);
    }

    nema_draw_circle(x, y, r, color);
}

void ng_fill_primitive_circle(int x, int y, int r, uint32_t color, int override_blend) {
    if ((color & 0xff000000U) == 0U){
        return;
    }

    if (override_blend == 0) {
        set_blend_fill(color);
    }

    nema_fill_circle(x, y, r, color);
}

void ng_blit_rect_fit(img_obj_t *img,
        int x, int y,
        int w, int h,
        int override_blend,
        uint8_t opacity
    ) {

    if( (opacity == 0) || (img == NULL)){
        return;
    }

    if (override_blend == 0) {
        if((opacity & 0xffU) == 0xffU){
            nema_set_blend_blit(NEMA_BL_SIMPLE);
        }else{
            nema_set_blend_blit(NEMA_BL_SIMPLE|NEMA_BLOP_MODULATE_A);
            nema_set_const_color(opacity << 24);
        }
    }

    nema_bind_src_tex (img->bo.base_phys, img->w,      img->h,
                       img->format,       img->stride, img->sampling_mode);


    if(img->format == NEMA_A8){
        nema_set_tex_color(img->color);
    }

    // set_transform_image(x, y, w,h);
    nema_blit_rect_fit(x, y, w, h);
}

void ng_blit_quad_fit(img_obj_t *img,
        float x0, float y0,
        float x1, float y1,
        float x2, float y2,
        float x3, float y3,
        int override_blend,
        uint8_t opacity
    ) {

    if( (opacity == 0) || (img == NULL)){
        return;
    }

    if (override_blend == 0) {
        if((opacity & 0xffU) == 0xffU){
            nema_set_blend_blit(NEMA_BL_SIMPLE);
        }else{
            nema_set_blend_blit(NEMA_BL_SIMPLE|NEMA_BLOP_MODULATE_A);
            nema_set_const_color(opacity << 24);
        }
    }

    if(img->format == NEMA_A8){
        nema_set_tex_color(img->color);
    }

    nema_bind_src_tex (img->bo.base_phys, img->w,      img->h,
                    img->format,       img->stride, img->sampling_mode);

    nema_blit_quad_fit( x0, y0,
                        x1, y1,
                        x2, y2,
                        x3, y3);

}

void ng_fill_quad( float x0, float y0,
                   float x1, float y1,
                   float x2, float y2,
                   float x3, float y3,
                   int override_blend,
                   uint32_t color
    ) {

    if( (color & 0xff000000U) == 0){
        return;
    }

    set_blend_fill(color);

    nema_fill_quad((int) x0, (int) y0,
                   (int) x1, (int) y1,
                   (int) x2, (int) y2,
                   (int) x3, (int) y3,
                   color);
}

