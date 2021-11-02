#ifndef NG_GLOBALS_H
#define NG_GLOBALS_H

//NemaGFX
#include "nema_core.h"
#include "nema_font.h"
#include "nema_dc_hal.h"
#include "nema_dc.h"
#include "nema_easing.h"
#include "nema_transitions.h"
#include "nema_utils.h"

//NemaGUI
#include "ng_gitem.h"
#include "ng_tuples.h"
#include "ng_gestures.h"
#include "ng_tree.h"
#include "ng_progress_bar.h"
#include "event_list.h"
#include "ng_utils.h"
#include "ng_animation.h"
#include "ng_callbacks.h"

#include "ng_event.h"
#include "ng_event_transition.h"
#include "ng_event_oneshot.h"
#include "ng_event_periodic.h"
#include "ng_event_periodic_transition.h"

//All Gitems/Widgets
#include "ng_button.h"
#include "ng_checkbox.h"
#include "ng_circle.h"
#include "ng_circular_progress.h"
#include "ng_container.h"
#include "ng_digimeter.h"
#include "ng_gauge.h"
#include "ng_image.h"
#include "ng_label.h"
#include "ng_needle.h"
#include "ng_progress_bar.h"
#include "ng_radio_button.h"
#include "ng_rect.h"
#include "ng_rounded_rect.h"
#include "ng_screen.h"
#include "ng_slider.h"
#include "ng_slider_hor.h"
#include "ng_slider_ver.h"
#include "ng_watchface.h"
#include "ng_icon.h"
#include "ng_window.h"

#ifndef EXTERN
#define EXTERN extern
#define REMEMBER_TO_UNDEFINE_EXTERN
#endif

#ifdef DEBUG_PROC_ENTRY
#define PROC_ENTRY printf("%s:%s\r\n", __FILE__, __func__)
#else
#define PROC_ENTRY
#endif

EXTERN int NG_RESX;
EXTERN int NG_RESY;

// POPUP specific globals
EXTERN tree_node_t *popup_node;
EXTERN int popup_off_x;
EXTERN int popup_off_y;

// draw at end of frame
EXTERN bool global_force_display;

//=====================================================================================
//screen transition event
EXTERN ng_event_base_t *NG_SCREEN_TRANSITION_EVENT;

#define SCREEN_TRANSITION_PAUSED \
    ( ng_event_is_paused(NG_SCREEN_TRANSITION_EVENT) )

#define SCREEN_TRANSITION_RUNNING \
    ( ng_event_is_running(NG_SCREEN_TRANSITION_EVENT) )

#define SCREEN_TRANSITION_STOPPED \
    ( ng_event_is_stopped(NG_SCREEN_TRANSITION_EVENT) )

#define DOING_SCREEN_TRANSITION \
     ( !SCREEN_TRANSITION_STOPPED )

//event list
EXTERN int               NG_EVENT_LIST_SIZE;
EXTERN ng_event_base_t **NG_EVENT_LIST;
//screen groups
EXTERN int                NG_SCREEN_GROUPS_COUNT;
EXTERN int                NG_POPUP_COUNT;
EXTERN int               *NG_SCREENS_PER_GROUP;
EXTERN tree_node_t     ***NG_NODES_PER_GROUP;
EXTERN nema_transition_t *NG_EFFECT_PER_GROUP;
EXTERN uint8_t           *NG_LAYOUT_PER_GROUP;
EXTERN tree_node_t      **NG_POPUP_NODES;
EXTERN int                NG_CUR_SCREEN_GROUP_INDEX;
EXTERN int                NG_CUR_SCREEN_NODE_INDEX;
EXTERN tree_node_t      **NG_CUR_SCREEN_GROUP_NODES;


//framebuffers
EXTERN int             NG_FRAMEBUFFER_COUNT;
EXTERN int             NG_BACKBUFFER_COUNT;
EXTERN img_obj_t      *NG_FRAMEBUFFER;
EXTERN img_obj_t      *NG_BACKBUFFER;
EXTERN nemadc_layer_t *NG_DC_LAYER;

void ng_globals_set_resolution(int resx, int resy);
void ng_globals_register_screen_transition_event(ng_event_base_t *event);
void ng_globals_register_event_list(ng_event_base_t **event_list, int list_size);
void ng_globals_register_screen_groups(int group_count, int popup_count, int *screens_per_group, tree_node_t ***nodes_per_group, nema_transition_t *effect_per_group, uint8_t *layout_per_group, tree_node_t **popup_nodes, int cur_group, int cur_screen, tree_node_t **cur_group_nodes);
void ng_globals_register_framebuffers(int frame_buffer_count, img_obj_t *frame_buffers, int back_buffer_count, img_obj_t *back_buffers, nemadc_layer_t *layers);

//Perform a basic test that the global variables needed by the API have been initialized properly
int ng_globals_sanity_check();

#ifdef REMEMBER_TO_UNDEFINE_EXTERN
#undef EXTERN
#undef REMEMBER_TO_UNDEFINE_EXTERN
#endif

#endif // NG_GLOBALS_
