#include "ng_digimeter.h"
#include "ng_event.h"
#include "ng_utils.h"
#include "nema_font.h"
// const gitem_gestures_t template_gestures = {NULL, NULL, NULL, NULL};

DRAW_FUNC(ng_digimeter_draw)
{
	gitem_digimeter_t *digimeter = NG_DIGIMETER(git);

    if (git->flags & GITEMF_FILL_COLOR){
    	ng_fill_rect(digimeter->x + x_off, digimeter->y + y_off, digimeter->w, digimeter->h, digimeter->color, 0);
    }

    nema_bind_font(digimeter->font);
    nema_set_blend_fill(NEMA_BL_SRC);

    char text[30];
    float2str(text, 30, digimeter->value, digimeter->precision);
    //Concatenate value and suffix
    concatenate_strings(text, digimeter->suffix);

    nema_print(text, digimeter->x + x_off, digimeter->y + y_off, digimeter->w, digimeter->h, digimeter->text_color, digimeter->alignment);
}

static void ng_digimeter_change_value(gitem_digimeter_t *digimeter, float val) {
    float val_init = digimeter->value;
    val = CLAMP(val, digimeter->min_value, digimeter->max_value);
    digimeter->value = val;
    
    if ( ( val != val_init ) && ( digimeter->event != NULL ) ) {
        ng_event_handle(digimeter->event, EV_TRIGGER_VALUE_CHANGED);
    }
}

void ng_digimeter_count_up(gitem_base_t *git) {
    gitem_digimeter_t *digimeter = NG_DIGIMETER(git);
    ng_digimeter_change_value(digimeter, digimeter->value + digimeter->step);
}

void ng_digimeter_count_down(gitem_base_t *git) {
    gitem_digimeter_t *digimeter = NG_DIGIMETER(git);
    ng_digimeter_change_value(digimeter, digimeter->value - digimeter->step);
}

void ng_digimeter_set_value(gitem_base_t *git, float value) {
    gitem_digimeter_t *digimeter = NG_DIGIMETER(git);
    ng_digimeter_change_value(digimeter, value);
}

void ng_digimeter_set_percent(gitem_base_t *git, float percent) {
    gitem_digimeter_t *digimeter = NG_DIGIMETER(git);
    ng_digimeter_change_value(digimeter, percent*(digimeter->max_value - digimeter->min_value) + digimeter->min_value);
}

void ng_digimeter_set_text_color(gitem_base_t *git, uint32_t rgba) {
    gitem_digimeter_t *digimeter = NG_DIGIMETER(git);
	digimeter->text_color = rgba;
}
