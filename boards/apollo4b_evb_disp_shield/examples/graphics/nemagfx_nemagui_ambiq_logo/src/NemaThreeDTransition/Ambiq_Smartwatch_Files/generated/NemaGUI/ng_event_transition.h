#ifndef __NG_EVENT_TRANSITION_H__
#define __NG_EVENT_TRANSITION_H__

#include "ng_event.h"
#include "ng_utils.h"

typedef struct _ng_transition_t{
    EVENT_BASE_STRUCT;
    float start_time;
    float duration;
    float progress;
} ng_transition_t;

void ng_transition_handler       (ng_event_base_t *event, float current_time, event_trigger_e trigger);
void ng_transition_start         (ng_event_base_t *event);
void ng_transition_stop          (ng_event_base_t *event, bool force_finish);
void ng_transition_pause_toggle  (ng_event_base_t *event, bool pause);

void ng_transition_revert        (ng_event_base_t *event);
void ng_transition_revert_force  (ng_event_base_t *event, int set);

#define NG_TRANSITION(object) SAFE_CAST((object), ng_event_base_t *, ng_transition_t *)

#endif //__NG_EVENT_TRANSITION_H__
