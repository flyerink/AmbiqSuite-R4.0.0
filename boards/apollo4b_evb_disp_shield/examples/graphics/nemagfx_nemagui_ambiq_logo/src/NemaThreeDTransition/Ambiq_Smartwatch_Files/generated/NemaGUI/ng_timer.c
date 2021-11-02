#include "ng_timer.h"
#include "ng_globals.h"
#include "nema_event.h"
#include "nema_utils.h"


static int master_timer   = -1;
static int timer_period   = 1000;
static bool is_running    = false;
static float frequency_hz = 1;

int ng_timer_create(int ms) {
    if (master_timer < 0 ) {
        master_timer = nema_timer_create();
    }
    
    timer_period = ms;
    frequency_hz = 1000.f/ms;
    
    return master_timer;
}

void ng_timer_set_period(int ms) {
    timer_period = ms;
    frequency_hz = 1000.f/ms;
}

void ng_timer_start() {
    if ( is_running == false ) {
        nema_timer_set_periodic(master_timer, timer_period);
        is_running = true;
    }
}

void ng_timer_stop() {
    nema_timer_stop(master_timer);
    is_running = false;
}

int ng_timer_get(){
    return master_timer;
}

bool ng_timer_is_running() {
    return is_running;
}

void ng_timer_handler(void) {
    bool activate_timer = false;

    for ( int i = 0; i < NG_EVENT_LIST_SIZE; ++i ) {
        ng_event_base_t *event = NG_EVENT_LIST[i];

        if ( ng_event_is_running(event) ){
            //Set timer as trigger (to be handled at execution)
            event_trigger_e trigger = event->trigger;
            event->trigger = EV_TRIGGER_TIMER;
            ng_event_handle(event, EV_TRIGGER_TIMER);
            //Reset trigger to its default value
            event->trigger = trigger;
            activate_timer = true;
        }
    }

    if( activate_timer == false ) {
        ng_timer_stop();
    }
}

int ng_timer_get_period() {
    return timer_period;
}

float ng_timer_get_frequency() {
    return frequency_hz;
}