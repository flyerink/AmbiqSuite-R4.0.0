#include "ng_label.h"
#include "ng_rect.h"
#include "ng_draw_prim.h"

DRAW_FUNC(ng_label_draw){
	gitem_label_t *label = NG_LABEL(git);

    if (git->flags & GITEMF_FILL_COLOR){
        ng_fill_rect(label->x, label->y, label->w, label->h, label->color, 0);
    }

    nema_bind_font(label->text->fonts[label->text->index]);
	nema_set_blend_fill(NEMA_BL_SRC);

    int font_x = git->x + x_off;
    int font_y = git->y + y_off;

    nema_print(label->text->texts[label->text->index], font_x, font_y, git->w, git->h, label->text_color, label->text->alignment);
}

void ng_label_set_text_color(gitem_base_t *git, uint32_t rgba) {
    gitem_label_t *label = NG_LABEL(git);
	label->text_color = rgba;
}
