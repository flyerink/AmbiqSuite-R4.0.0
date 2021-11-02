#include "ng_gitem.h"
#include "ng_event.h"
#include "ng_window.h"
#include "ng_globals.h"


static GESTURE_FUNC_PRESS(press) {
    (void)x;
    (void)y;
	ng_gitem_unset_flag(node->this_, GITEMF_STOP_RECUR);
}

static bool swipe_momentum = true;
static bool swipe_snap     = true;
int step_x = -1;
int step_y = 31;

static void ng_window_snap_y(tree_node_t *window){
    if(step_y < 0){
        return;
    }

    int diff      = -window->first_child->this_->y % step_y;
    int half_step = step_y / 2;

    if((window->first_child->this_->y != 0) && (window->first_child->this_->y != -window->first_child->this_->h + window->this_->h)){
        if(-window->first_child->this_->y % step_y > half_step){
            window->first_child->this_->y = window->first_child->this_->y - step_y + diff;
        }else{
            window->first_child->this_->y = window->first_child->this_->y + diff;
        }
    }
}

static void ng_window_snap_x(tree_node_t *window){
    if(step_x < 0){
        return;
    }

    int diff      = -window->first_child->this_->x % step_x;
    int half_step = step_x / 2;

    if((window->first_child->this_->x != 0) && (window->first_child->this_->x != -window->first_child->this_->w + window->this_->w)){
        if(-window->first_child->this_->x % step_x > half_step){
            window->first_child->this_->x = window->first_child->this_->x - step_x + diff;
        }else{
            window->first_child->this_->x = window->first_child->this_->x + diff;
        }
    }
}

static GESTURE_FUNC_RELEASE(release) {
    ng_gitem_unset_flag(node->this_, GITEMF_STOP_RECUR);
}

static GESTURE_FUNC_SWIPE(swipe) {
    (void)x;
    (void)y;

    ng_gitem_set_flag(node->this_, GITEMF_STOP_RECUR);
    gitem_base_t *subwindow = node->first_child->this_;

    if (subwindow->h > node->this_->h) {
        int cont_y_min = subwindow->y;
        int cont_y_max = subwindow->h + cont_y_min;
        int cont_y_min_new = cont_y_min + dy;
        int cont_y_max_new = cont_y_max + dy;

        if (cont_y_max_new < node->this_->h && cont_y_min_new > 0)
            //don't do anything
            cont_y_min_new = cont_y_min;
        else if (cont_y_min_new > 0)
            cont_y_min_new = 0;
        else if (cont_y_max_new < node->this_->h)
            cont_y_min_new = node->this_->h - subwindow->h;

        subwindow->y = cont_y_min_new;
    }

    if (subwindow->w > node->this_->w) {
        int cont_x_min = subwindow->x;
        int cont_x_max = subwindow->w + cont_x_min;
        int cont_x_min_new = cont_x_min + dx;
        int cont_x_max_new = cont_x_max + dx;

        if (cont_x_max_new < node->this_->w && cont_x_min_new > 0)
            //don't do anything
            cont_x_min_new = cont_x_min;
        else if (cont_x_min_new > 0)
            cont_x_min_new = 0;
        else if (cont_x_max_new < node->this_->w)
            cont_x_min_new = node->this_->w - subwindow->w;

        subwindow->x = cont_x_min_new;
    }
}

gitem_gestures_t gestures_window = {press, release, swipe, NULL};

void ng_window_set_source(tree_node_t *window, tree_node_t *source){
    window->first_child->first_child = source;
    window->first_child->this_->w    = source->this_->w;
    window->first_child->this_->h    = source->this_->h;
}
