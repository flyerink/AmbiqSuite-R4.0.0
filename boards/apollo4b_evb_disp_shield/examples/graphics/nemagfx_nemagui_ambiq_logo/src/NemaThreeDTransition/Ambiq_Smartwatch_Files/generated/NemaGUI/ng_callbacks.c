#include "ng_globals.h"

void ng_animate_uint32(ng_event_base_t *event, void *data) {
    ng_git_uint32_uint32_ez_t *animation_data = (ng_git_uint32_uint32_ez_t*)(data);

    gitem_base_t            *target = animation_data->git;
    uint32_t                  start = animation_data->val_0;
    uint32_t                    end = animation_data->val_1;
    float                  progress = NG_TRANSITION(event)->progress;
    float (*easing_function)(float) = animation_data->easing;

    uint32_t val = (uint32_t) nema_ez(start, end, 1.f, progress, easing_function);

    event->action->act_gitptr_uint(target, val);
}

void ng_animate_float(ng_event_base_t *event, void *data) {
    ng_git_float_float_ez_t *retrieved_data = (ng_git_float_float_ez_t*)(data);

    gitem_base_t            *target = retrieved_data->git;
    float                     start = retrieved_data->val_0;
    float                       end = retrieved_data->val_1;
    float                  progress = NG_TRANSITION(event)->progress;
    float (*easing_function)(float) = retrieved_data->easing;

    float  val = nema_ez(start, end, 1.f, progress, easing_function);

    event->action->act_gitptr_float(target, val);
}

void ng_set_uint32(ng_event_base_t *event, void *data) {
    ng_git_uint32_t *color_data = (ng_git_uint32_t*)(data);

    gitem_base_t *target = color_data->git;
    uint32_t val = color_data->val;

    event->action->act_gitptr_uint(target, val);
}

void ng_set_float(ng_event_base_t *event, void *data) {
    ng_git_float_t *retrieved_data = (ng_git_float_t*)(data);

    gitem_base_t *target = retrieved_data->git;
    float val = retrieved_data->val;

    event->action->act_gitptr_float(target, val);
}

void ng_set_ptr(ng_event_base_t *event, void *data) {
    ng_git_ptr_t *retrieved_data = (ng_git_ptr_t*)(data);

    gitem_base_t *target = retrieved_data->git;
    void            *ptr = retrieved_data->ptr;
    event->action->act_gitptr_ptr(target, ptr);
}

void ng_update_gitem(ng_event_base_t *event, void *data) {
    ng_gitptr_t *retrieved_data = (ng_gitptr_t*)(data);
    gitem_base_t        *target = retrieved_data->git;
    event->action->act_gitptr(target);
}

void ng_animate_int_int_pair(ng_event_base_t *event, void *data) {
    ng_git_int_int_pair_ez_t *retrieved_data = (ng_git_int_int_pair_ez_t*)(data);

    gitem_base_t            *target = retrieved_data->git;
    int                     start_a = retrieved_data->a0;
    int                       end_a = retrieved_data->a1;
    int                     start_b = retrieved_data->b0;
    int                       end_b = retrieved_data->b1;
    float                  progress = NG_TRANSITION(event)->progress;
    float (*easing_function)(float) = retrieved_data->easing;

    int a = (int) nema_ez(start_a, end_a, 1.f, progress, easing_function);
    int b = (int) nema_ez(start_b, end_b, 1.f, progress, easing_function);

    event->action->act_gitptr_int_int(target, a, b);
}

void ng_set_int_int(ng_event_base_t *event, void *data) {
    ng_git_int_int_t *retrieved_data = (ng_git_int_int_t*)(data);

    gitem_base_t *target = retrieved_data->git;
    int                a = retrieved_data->a;
    int                b = retrieved_data->b;

    event->action->act_gitptr_int_int(target, a, b);
}

void ng_set_tree_node(ng_event_base_t *event, void *data) {
    ng_tree_node_ptr_t *retrieved_data = (ng_tree_node_ptr_t*)(data);
    tree_node_t *node = retrieved_data->node;
    event->action->act_nodeptr(node);
}

void ng_set_node_to_node(ng_event_base_t *event, void *data) {
    ng_node_node_t *retrieved_data = (ng_node_node_t*)(data);
    tree_node_t *node_a = retrieved_data->node0;
    tree_node_t *node_b = retrieved_data->node1;
    event->action->act_nodeptr_nodeptr(node_a, node_b);
}

void ng_set_percent(ng_event_base_t *event, void *data) {
    ng_git_float_t *retrieved_data = (ng_git_float_t*)(data);
    gitem_base_t *target = retrieved_data->git;

    float percent = ng_gitem_get_value(event->src_gitem);

    event->action->act_gitptr_float( target, percent);
}
