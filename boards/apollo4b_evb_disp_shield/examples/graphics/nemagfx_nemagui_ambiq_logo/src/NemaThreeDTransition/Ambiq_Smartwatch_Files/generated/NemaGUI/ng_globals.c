#define EXTERN
#include "ng_globals.h"
#undef EXTERN

int NG_RESX = -1;
int NG_RESY = -1;

ng_event_base_t *NG_SCREEN_TRANSITION_EVENT = NULL;
ng_event_base_t** NG_EVENT_LIST          = NULL;
int NG_EVENT_LIST_SIZE                   = 0;

void ng_globals_register_screen_transition_event(ng_event_base_t *event) {
    NG_SCREEN_TRANSITION_EVENT = event;
}

void ng_globals_register_event_list(ng_event_base_t **event_list, int list_size) {
    NG_EVENT_LIST_SIZE = list_size;
    NG_EVENT_LIST      = event_list;
}

int                NG_SCREEN_GROUPS_COUNT = -1;
int                NG_POPUP_COUNT = -1;
int               *NG_SCREENS_PER_GROUP = NULL;
tree_node_t     ***NG_NODES_PER_GROUP = NULL;
nema_transition_t *NG_EFFECT_PER_GROUP = NULL;
uint8_t           *NG_LAYOUT_PER_GROUP = NULL;
tree_node_t      **NG_POPUP_NODES = NULL;
int                NG_CUR_SCREEN_GROUP_INDEX = -1;
int                NG_CUR_SCREEN_NODE_INDEX = -1;
tree_node_t      **NG_CUR_SCREEN_GROUP_NODES = NULL;

void ng_globals_register_screen_groups(int group_count, int popup_count, int *screens_per_group, tree_node_t ***nodes_per_group, nema_transition_t *effect_per_group, uint8_t *layout_per_group, tree_node_t **popup_nodes, int cur_group, int cur_screen, tree_node_t **cur_group_nodes){
    NG_SCREEN_GROUPS_COUNT    = group_count;
    NG_POPUP_COUNT            = popup_count;
    NG_SCREENS_PER_GROUP      = screens_per_group;
    NG_NODES_PER_GROUP        = nodes_per_group;
    NG_EFFECT_PER_GROUP       = effect_per_group;
    NG_LAYOUT_PER_GROUP       = layout_per_group;
    NG_POPUP_NODES            = popup_nodes;
    NG_CUR_SCREEN_GROUP_INDEX = cur_group;
    NG_CUR_SCREEN_NODE_INDEX  = cur_screen;
    NG_CUR_SCREEN_GROUP_NODES = cur_group_nodes;
}

int NG_FRAMEBUFFER_COUNT     = -1;
int NG_BACKBUFFER_COUNT      = -1;
img_obj_t *NG_FRAMEBUFFER   = NULL;
img_obj_t *NG_BACKBUFFER    = NULL;
nemadc_layer_t *NG_DC_LAYER = NULL;

void ng_globals_register_framebuffers(int frame_buffer_count, img_obj_t *frame_buffers, int back_buffer_count, img_obj_t *back_buffers, nemadc_layer_t *layers) {
    NG_FRAMEBUFFER_COUNT = frame_buffer_count;
    NG_BACKBUFFER_COUNT  = back_buffer_count;

    NG_FRAMEBUFFER = frame_buffers;
    NG_BACKBUFFER  = back_buffers;
    NG_DC_LAYER    = layers;
}

void ng_globals_set_resolution(int resx, int resy) {
    NG_RESX = resx;
    NG_RESY = resy;
}

int ng_globals_sanity_check() {
    int val = 0;

    if ( NG_SCREEN_TRANSITION_EVENT == NULL ) {
        val++;
    }

    if ( (NG_EVENT_LIST_SIZE <= 0) || (NG_EVENT_LIST == NULL) ) {
        val++;
    }

    if ( (NG_SCREEN_GROUPS_COUNT < 0) ) {
        val++;
    }

    if ( (NG_RESX < 0) || (NG_RESY < 0)) {
        val++;
    }

    return val;
}