#ifndef NG_TIMER_H__
#define NG_TIMER_H__

#include "nema_core.h" //bool
#include "ng_event.h"

int  ng_timer_create();
void ng_timer_set_period(int ms);
void ng_timer_start();
void ng_timer_stop();
void ng_timer_handler();
int  ng_timer_get();
bool ng_timer_is_running();
int ng_timer_get_period();
float ng_timer_get_frequency(); //returns frequency in Hz

void ng_timer_set_event_list(ng_event_base_t** list, int size);

#endif //NG_TIMER_H__