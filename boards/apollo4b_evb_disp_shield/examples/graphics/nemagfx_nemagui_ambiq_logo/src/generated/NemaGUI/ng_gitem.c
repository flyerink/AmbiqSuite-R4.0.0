#include "ng_tree.h"
#include "ng_gitem.h"
#include "ng_globals.h"

void ng_gitem_set_flag(gitem_base_t *git, uint32_t flag) {
    git->flags |= flag;
}

void ng_gitem_unset_flag(gitem_base_t *git, uint32_t flag) {
    git->flags &= ~flag;
}

// Return the value from gitems that support it
float ng_gitem_get_value(gitem_base_t *git) {
    float value = 0.f;

    switch (git->type) {
    case GITEM_VERTICAL_SLIDER:
    case GITEM_HORIZONTAL_SLIDER:
        value = NG_SLIDER(git)->value;
        break;
    case GITEM_DIGITAL_METER:
        value = NG_DIGIMETER(git)->value;
        break;
    case GITEM_HORIZONTAL_PROGRESS_BAR:
        value = NG_PROGRESS_BAR(git)->value;
        break;
    case GITEM_GAUGE:
        value = NG_GAUGE(git)->value;
        break;
    case GITEM_CIRCULAR_PROGRESS:
        value = NG_CIRCULAR_PROGRESS(git)->value;
        break;
    default:
        break;
    }

    return value;
}

void ng_gitem_set_visible(gitem_base_t *git) {
    ng_gitem_unset_flag(git, GITEMF_HIDDEN);
}

void ng_gitem_set_hidden(gitem_base_t *git) {
    ng_gitem_set_flag(git, GITEMF_HIDDEN);
}

void ng_gitem_set_alpha(gitem_base_t *git, uint32_t alpha) {
    git->color  = git->color & 0x00ffffffU;
    git->color |= alpha << 24;
}

void ng_gitem_set_color(gitem_base_t *git, uint32_t rgba) {
    git->color = rgba;
}

void ng_gitem_set_position(gitem_base_t *git, int x, int y) {
    git->x = x;
    git->y = y;
}

void ng_gitem_set_size(gitem_base_t *git, int w, int h) {
    git->w = w;
    git->h = h;
}
