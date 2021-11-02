#ifndef __PREDEFINED_CALLBACKS_H__
#define __PREDEFINED_CALLBACKS_H__

#include "ng_event.h"

void ng_animate_uint32(ng_event_base_t *event, void *data);
void ng_animate_float(ng_event_base_t *event, void *data);
void ng_set_uint32(ng_event_base_t *event, void *data);
void ng_set_float(ng_event_base_t *event, void *data);
void ng_set_ptr(ng_event_base_t *event, void *data);
void ng_update_gitem(ng_event_base_t *event, void *data);

void ng_set_int_int(ng_event_base_t *event, void *data);
void ng_animate_int_int_pair(ng_event_base_t *event, void *data);

void ng_set_tree_node(ng_event_base_t *event, void *data);
void ng_set_node_to_node(ng_event_base_t *event, void *data);
void ng_set_percent(ng_event_base_t *event, void *data);

#endif //__PREDEFINED_CALLBACKS_H__
