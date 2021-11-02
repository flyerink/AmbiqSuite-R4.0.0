#include "ng_event_oneshot.h"

void ng_oneshot_handler(ng_event_base_t *event, float current_time, event_trigger_e trigger) {
    event->callback(event, event->action_data);
}
