#include "ng_event_periodic_transition.h"
#include "ng_timer.h"

#define INIT_PROGRESS 0.f
#define FINAL_PROGRESS 1.f

static float cur_time = 0.f;

inline static void set_start_time_from_progress(ng_event_base_t *event) {
    ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    float progress = transition->progress;

    transition->start_time = cur_time - transition->duration*progress;
}

inline static void update_start_time(ng_event_base_t *event) {
    ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    float time_diff = cur_time - transition->start_time;

    int num_of_periods_to_add = (int)(time_diff/transition->period);

    transition->start_time += transition->period*num_of_periods_to_add;
}

inline static void update_progress(ng_event_base_t *event) {
    ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    float progress = (cur_time - transition->start_time)/transition->duration;
    if (progress > 1.f) {
        progress = 1.f;
    }

    transition->progress = progress;
}

inline static void reset(ng_event_base_t *event) {
    ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    transition->progress = INIT_PROGRESS;
    set_start_time_from_progress(event);
    ng_event_run_callback(event, 0);
}

inline static void finish(ng_event_base_t *event) {
    ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    ng_event_unset_status_flag(event, EV_STATUS_REVERSED);
    transition->progress = FINAL_PROGRESS;
    set_start_time_from_progress(event);
    ng_event_run_callback(event, EV_STATUS_LAST_RUN);
}

void ng_periodic_transition_start(ng_event_base_t *event) {
    ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    cur_time = nema_get_time();
    transition->progress = 0.f;
    transition->start_time = cur_time;
    ng_event_set_status(event, EV_STATUS_RUNNING);
    ng_event_run_callback(event, EV_STATUS_FIRST_RUN);
    ng_timer_start();
}

void ng_periodic_transition_stop(ng_event_base_t *event, bool force_finish) {
    if ( force_finish == true ) {
        finish(event);
    } else {
        reset(event);
    }
    ng_event_set_status(event, EV_STATUS_STOPPED);
}

void ng_periodic_transition_pause_toggle(ng_event_base_t *event, bool pause) {
     ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    if ( pause == true ) {
        ng_event_set_status(event, EV_STATUS_PAUSED);
    } else if ( event->status != EV_STATUS_RUNNING ) {
           // resume not running events
        cur_time = nema_get_time();
        set_start_time_from_progress(event);
        ng_event_set_status(event, EV_STATUS_RUNNING);
        ng_timer_start();
    }
}

void ng_periodic_transition_handler(ng_event_base_t *event, float current_time, event_trigger_e trigger) {
    ng_periodic_transition_t *transition = NG_PERIODIC_TRANSITION(event);
    cur_time = current_time;

    if ( trigger == EV_TRIGGER_TIMER ) {
        float duration_end      = transition->start_time + transition->duration;
        float period_end        = transition->start_time + transition->period;

        if ( cur_time > period_end ) {
            update_start_time(event);
            update_progress(event);
            ng_event_run_callback(event, 0);
        } else if ( cur_time < duration_end ) {
            update_progress(event);
            ng_event_run_callback(event, 0);
        } else if ( transition->progress != FINAL_PROGRESS ) {
            // set progress to 1.f in order to force draw final state
            transition->progress = FINAL_PROGRESS;
            ng_event_run_callback(event, EV_STATUS_LAST_RUN);
        } else {
            // duration has ended
            // progress 1.f has been drawn
            // waiting for new period
            // do nothing
        }
    } else {
        if ( ng_event_is_stopped(event) ) {
            ng_periodic_transition_start(event);
        } else {
            //retrigger
            if ( ng_event_check_retrigger_flag(event, EV_RETRIGGER_RESTART) ) {
                ng_periodic_transition_stop(event, false);
                ng_periodic_transition_start(event);
            }  else if ( ng_event_check_retrigger_flag(event, EV_RETRIGGER_FINISH) ) {
                finish(event);
            } else if ( ng_event_check_retrigger_flag(event, EV_RETRIGGER_RESET) ) {
                ng_periodic_transition_stop(event, false);
            }

            if ( ng_event_is_running(event) && ng_event_check_retrigger_flag(event, EV_RETRIGGER_PAUSE) ) {
                ng_periodic_transition_pause_toggle(event, true);
            } else if ( ng_event_is_paused(event) && ng_event_check_retrigger_flag(event, EV_RETRIGGER_RESUME) ) {
                ng_periodic_transition_pause_toggle(event, false);
            }
        }
    }
}
