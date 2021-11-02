#include "ng_event_periodic.h"
#include "ng_timer.h"
static float cur_time = 0.f;

static inline void reset(ng_periodic_t *periodic) {
    periodic->start_time = cur_time;
    ng_event_run_callback(NG_EVENT(periodic), 0);
    ng_event_set_status(NG_EVENT(periodic) ,EV_STATUS_STOPPED);
}

void ng_periodic_start(ng_event_base_t *event) {
    ng_periodic_t *periodic = NG_PERIODIC(event);
    cur_time = nema_get_time();
    periodic->start_time = cur_time;
    ng_event_set_status(event, EV_STATUS_RUNNING);
    ng_event_run_callback(event, EV_STATUS_FIRST_RUN);
    ng_timer_start();
}

void ng_periodic_stop(ng_event_base_t *event, bool force_finish) {
    ng_event_set_status(event, EV_STATUS_STOPPED);
}

void ng_periodic_pause_toggle(ng_event_base_t *event, bool pause) {
    if ( pause == true ) {
        ng_event_set_status(event, EV_STATUS_PAUSED);
    } else if (event->status != EV_STATUS_RUNNING) {
        ng_periodic_t *periodic = NG_PERIODIC(event);
        cur_time = nema_get_time();
        periodic->start_time = cur_time;
        ng_event_set_status(event, EV_STATUS_RUNNING);
    }
}

void ng_periodic_handler(ng_event_base_t *event, float current_time, event_trigger_e trigger) {
    ng_periodic_t *periodic = NG_PERIODIC(event);
    cur_time = current_time;
    if ( trigger == EV_TRIGGER_TIMER ) {
        // If period has expired, execute and schedule the next execution one period later
        if( (ng_event_is_running(event)) && (cur_time >= periodic->start_time + periodic->period) ) {
            ng_event_run_callback(event, 0);
            periodic->start_time += periodic->period;
        }
    } else {
        if ( ng_event_is_stopped(event) ) {
            // user trigger
            ng_periodic_start(event);
        } else {
            // retrigger
            if ( ng_event_check_retrigger_flag(event, EV_RETRIGGER_RESET) ) {
                reset(periodic);
            }

            if ( ng_event_is_running(event) ) {
                if ( ng_event_check_retrigger_flag(event, EV_RETRIGGER_PAUSE) ) {
                    ng_periodic_pause_toggle(event, true);
                }
            } else if ( ng_event_is_paused(event) ) {
                if ( ng_event_check_retrigger_flag(event, EV_RETRIGGER_RESUME) ) {
                    ng_periodic_pause_toggle(event, false);
                }
            }
        }
    }
}