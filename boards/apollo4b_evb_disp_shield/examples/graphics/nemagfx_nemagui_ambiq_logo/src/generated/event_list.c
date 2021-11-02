#include "gui_tree.h"
#include "event_list.h"
#include "ng_screen_trans.h"
#include "ng_globals.h"
#include "ng_timer.h"
#include "ng_display.h"
#include "ng_callbacks.h"
#include "nema_easing.h"
#include "custom_callbacks.h"
#include "images.h"

#define EVENT_LIST_SIZE   5
#define TEMP_ANIMATIONS   5

#define TIMER_PERIOD_MS 16

//Temprary animations - events reserved by the project (do not edit)
//----------------------------------------------------------------------------------------------------------------
static ng_node_effect_direction_t transition_data_0 = {NULL, NEMA_TRANS_LINEAR_H, 0};
#define transition_dummy {EV_TRIGGER_NULL, EV_RETRIGGER_IGNORE, NULL, ng_screen_trans_swipable, NULL, NG_CALLBACK_DATA(&transition_data_0), EV_STATUS_STOPPED, ng_transition_handler, ng_transition_start, ng_transition_stop, ng_transition_pause_toggle, NULL, 0.f, 1.f, 2.f}
static ng_transition_t temp_anim_0  = transition_dummy;   //reserved for screen transition
static ng_transition_t temp_anim_1  = transition_dummy;   //reserved for temporary animation
static ng_transition_t temp_anim_2  = transition_dummy;   //reserved for temporary animation
static ng_transition_t temp_anim_3  = transition_dummy;   //reserved for temporary animation
static ng_transition_t temp_anim_4  = transition_dummy;   //reserved for temporary animation
//----------------------------------------------------------------------------------------------------------------


static ng_event_base_t* event_list[EVENT_LIST_SIZE];

void event_list_init() {
    event_list[0] = (NG_EVENT(&temp_anim_0));
    event_list[1] = (NG_EVENT(&temp_anim_1));
    event_list[2] = (NG_EVENT(&temp_anim_2));
    event_list[3] = (NG_EVENT(&temp_anim_3));
    event_list[4] = (NG_EVENT(&temp_anim_4));


    ng_globals_register_screen_transition_event(event_list[0]);
    ng_globals_register_event_list(event_list, EVENT_LIST_SIZE);
    ng_timer_set_period(TIMER_PERIOD_MS);
}
