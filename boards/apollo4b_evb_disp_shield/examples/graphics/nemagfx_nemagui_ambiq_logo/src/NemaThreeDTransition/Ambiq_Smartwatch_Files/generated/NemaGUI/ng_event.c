// #define DEBUG_PROC_ENTRY

#include "nema_event.h"

#include "ng_event.h"
#include "ng_event_transition.h"
#include "ng_event_periodic.h"
#include "ng_event_periodic_transition.h"
#include "ng_globals.h"
#include "ng_display.h"
#include "ng_timer.h"
#include "ng_screen_trans.h"

//asign events to "source" gitems
void ng_event_init() {

    int master_timer = ng_timer_create(33);
    event_list_init();

    for (int i = 0; i < NG_EVENT_LIST_SIZE; ++i) {
        if(NG_EVENT_LIST[i]->src_gitem != NULL){
            ng_event_base_t **event = &NG_EVENT_LIST[i]->src_gitem->event;

            while (*event != NULL) {
                event = &(*event)->next;
            }

            *event = NG_EVENT_LIST[i];
        }
    }

    // 
}

void ng_event_set_status(ng_event_base_t *event, event_status_e status) {
    event->status &= (~EV_STATUS_MASK);
    event->status |= status;
}

void ng_event_set_status_flag(ng_event_base_t *event, int flag) {
    event->status |= flag;
}

void ng_event_unset_status_flag(ng_event_base_t *event, int flag) {
    event->status &= (~flag);
}

bool ng_event_check_status_flag(ng_event_base_t *event, int flag) {
    return (event->status & flag) == 0 ? false : true;
}

void ng_event_flip_status_flag(ng_event_base_t *event, int flag) {
    // flag is set, unset it
    if ( ng_event_check_status_flag(event, flag) ) {
        ng_event_unset_status_flag(event, flag);
    }
    // flag is not set, set it
    else {
        ng_event_set_status_flag(event, flag);
    }
}

bool ng_event_check_retrigger_flag(ng_event_base_t *event, int flag) {
    if ( (event->retrigger & flag) == 0 ) {
        return false;
    } else {
        return true;
    }
}

void ng_event_run_callback(ng_event_base_t *event, int status_flags) {
        if (status_flags != 0) {
            ng_event_set_status_flag(event, status_flags);
        }

        event->callback(event, event->action_data);
        global_force_display = true;

        if (status_flags != 0) {
            ng_event_unset_status_flag(event, status_flags);
        }
}

static inline bool needs_handling(ng_event_base_t *event, event_trigger_e trigger) {
    bool result = false;

    //if trigger matches
    //and the event is not running (it is paused or stopped) or can be re-triggered
    if ( ( event->trigger == trigger ) && ( (!ng_event_is_running(event)) || ( !ng_event_check_retrigger_flag(event, EV_RETRIGGER_IGNORE) ) ) ) {
        result = true;
    } else if (( trigger == EV_TRIGGER_TIMER ) && ( ng_event_is_running(event) )) {
        //if the trigger is timer and the event is running
        result = true;
    }

    return result;
}

void ng_event_handle(ng_event_base_t *event, event_trigger_e trigger_event) {
    float cur_time = nema_get_time();

    while ( (event != NULL) && (event->handler != NULL) ) {
        if ( needs_handling(event, trigger_event) == true ) {
            event->handler(event, cur_time, trigger_event);
        }

        if ( (ng_event_is_running(event)) && (ng_timer_is_running() == false) ) {
            ng_timer_start();
        }

        event = event->next;

        if ( trigger_event == EV_TRIGGER_TIMER ) {
            return;
        }
    }
}
