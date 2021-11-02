#ifndef __NG_EVENT_PERIODIC_TRANSITION_H__
#define __NG_EVENT_PERIODIC_TRANSITION_H__

#include "ng_event.h"
#include "ng_utils.h"

typedef struct _ng_periodic_transition_t{
    EVENT_BASE_STRUCT;
    float start_time;
    float duration;
    float progress;
    float period;
} ng_periodic_transition_t;

void ng_periodic_transition_handler       (ng_event_base_t *event, float current_time, event_trigger_e trigger);
void ng_periodic_transition_start         (ng_event_base_t *event);
void ng_periodic_transition_stop          (ng_event_base_t *event, bool force_finish);
void ng_periodic_transition_pause_toggle  (ng_event_base_t *event, bool pause);

#define NG_PERIODIC_TRANSITION(object) SAFE_CAST((object), ng_event_base_t *, ng_periodic_transition_t *)

#endif //__NG_EVENT_PERIODIC_TRANSITION_H__
