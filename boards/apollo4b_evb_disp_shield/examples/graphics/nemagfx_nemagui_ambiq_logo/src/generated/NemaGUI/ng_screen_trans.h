#ifndef NG_SCREEN_TRANS_H__
#define NG_SCREEN_TRANS_H__

#ifndef SCREEN_TRANSITION_DURATION_SECS
#define SCREEN_TRANSITION_DURATION_SECS 1.f
#endif

#include "ng_globals.h"
#include "ng_event_transition.h"
#include "event_list.h"
#include "ng_utils.h"
#include "nema_transitions.h"

// initialize screen transition
void ng_screen_trans_initialize(ng_event_base_t *event, tree_node_t *to_screen, tree_node_t *from_screen, nema_transition_t effect, int go_right, float initial_progress);
// update paused screen transition
void ng_screen_trans_swipe(float progress_diff);

// starts timer for transition
void ng_screen_trans_resume(ng_event_base_t *event, float duration, int abort);

// pauses on-going transition
void ng_screen_trans_pause();

//callback functions
void ng_screen_trans_swipable(ng_event_base_t *event, void *data);
void ng_callback_show_screen (ng_event_base_t *event, void *data);
void ng_callback_set_screen  (ng_event_base_t *event, void *data);

#endif
