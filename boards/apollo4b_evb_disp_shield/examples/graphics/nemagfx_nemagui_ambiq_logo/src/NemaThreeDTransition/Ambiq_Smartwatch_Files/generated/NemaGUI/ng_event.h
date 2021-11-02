#ifndef __NG_EVENT_H__
#define __NG_EVENT_H__

#include "ng_gitem.h"
#include "ng_typedefs.h"
#include "ng_animation.h"

// typedef void  (*NG_CALLBACK)     (void *data);
// #define	NG_CALLBACK(f)			 ((NG_CALLBACK) (f))
#define	NG_CALLBACK_DATA(data)	 (data)

typedef enum {
    EV_TRIGGER_NULL,          //0
    EV_TRIGGER_PRESS,         //1
    EV_TRIGGER_RELEASE,       //2
    EV_TRIGGER_HOLD,          //3
    EV_TRIGGER_DRAG,          //4
    EV_TRIGGER_VALUE_CHANGED, //5
    EV_TRIGGER_STATE_CHANGED, //6
    EV_TRIGGER_SCREEN_ENTERED,//7
    EV_TRIGGER_APP_START,     //8
    EV_TRIGGER_CUSTOM,        //9
    EV_TRIGGER_TIMER          //10
} event_trigger_e;

typedef enum {
    EV_RETRIGGER_IGNORE       = 0x0,  // Ignore retrigger event
    EV_RETRIGGER_PAUSE        = 0x1,  // Pause a running event
    EV_RETRIGGER_RESUME       = 0x2,  // Resume a paused event
    EV_RETRIGGER_PAUSE_TOGGLE = EV_RETRIGGER_PAUSE | EV_RETRIGGER_RESUME,
    EV_RETRIGGER_REVERSE      = 0x4,  // Return to the initial state by running the event in reverse order (cur_progress to 0.f)
    EV_RETRIGGER_RESET        = 0x8, // Reset the event to its initial state
    EV_RETRIGGER_FINISH       = 0x10, // Go to final state (transition)
    EV_RETRIGGER_RESTART      = 0x20, // On retrigger, restart periodic/transitional event
    EV_RETRIGGER_STOP         = 0x40, // Go to final state and stop (periodic transition)
} event_retrigger_e;

typedef enum {
    EV_STATUS_STOPPED   = 0x0,
    EV_STATUS_RUNNING   = 0x1,
    EV_STATUS_PAUSED    = 0x2,
    EV_STATUS_MASK      = 0x3,
    EV_STATUS_REVERSED  = 0x4, //a backwards transition (from progress: p1 to 0)
    EV_STATUS_FIRST_RUN = 0x8,
    EV_STATUS_LAST_RUN  = 0x10
} event_status_e;

struct _gitem_base_t;

typedef union _ng_act_ptr{
  void (*act_gitptr_float   )(struct _gitem_base_t *gitem, float value);
  void (*act_gitptr_ptr     )(struct _gitem_base_t *gitem, void *ptr);
  void (*act_gitptr_int     )(struct _gitem_base_t *gitem, int value);
  void (*act_gitptr_uint    )(struct _gitem_base_t *gitem, uint32_t value);
  void (*act_gitptr         )(struct _gitem_base_t *gitem);
  void (*act_gitptr_int_int )(struct _gitem_base_t *gitem, int a, int b);
  void (*act_nodeptr        )(struct _tree_node_t  *node);
  void (*act_nodeptr_nodeptr)(struct _tree_node_t  *node0, struct _tree_node_t  *node1);
  void (*act_animptr      )(ng_animation_data_t *data);
  void (*act_void         )(void);
} ng_act_ptr;

//EVENT BASE STRUCT: contains field common among the most event types (onehsot, transition, periodic, periodic transition)
#define EVENT_BASE_STRUCT       \
    event_trigger_e trigger;                                           \
    event_retrigger_e retrigger;                                       \
    struct _gitem_base_t *src_gitem;                                   \
    void (*callback)(struct _ng_event_base_t* event, void *data);      \
    union _ng_act_ptr (*action);                                       \
    void *action_data;                                                 \
    event_status_e status;                                             \
    void (*handler) (struct _ng_event_base_t *event, float cur_time, event_trigger_e trigger); \
    void (*start) (struct _ng_event_base_t *event);                    \
    void (*stop) (struct _ng_event_base_t *event, bool force_finish);  \
    void (*pause_toggle) (struct _ng_event_base_t *event, bool pause); \
    struct _ng_event_base_t *next

typedef struct _ng_event_base_t{
    EVENT_BASE_STRUCT;
} __ng_event_base_t;

void ng_event_init (void);
void ng_event_handle(ng_event_base_t *event, event_trigger_e trigger_event);
void ng_event_run_callback(ng_event_base_t *event, int status_flags);

void ng_event_set_status(ng_event_base_t *event, event_status_e status);
bool ng_event_check_retrigger_flag(ng_event_base_t *event, int flag);
bool ng_event_check_status_flag(ng_event_base_t *event, int flag);
void ng_event_set_status_flag(ng_event_base_t *event, int flag);
void ng_event_unset_status_flag(ng_event_base_t *event, int flag);
void ng_event_flip_status_flag(ng_event_base_t *event, int flag);

#define ng_event_is_stopped(ev_ptr) \
    (((ev_ptr)->status & EV_STATUS_MASK) == EV_STATUS_STOPPED)

#define ng_event_is_running(ev_ptr) \
    (((ev_ptr)->status & EV_STATUS_MASK) == EV_STATUS_RUNNING)

#define ng_event_is_paused(ev_ptr) \
    (((ev_ptr)->status & EV_STATUS_MASK) == EV_STATUS_PAUSED)

#define NG_EVENT(object) SAFE_CAST((object), void *, ng_event_base_t *)

#define PROGRESS(t)     (t)
#define DURATION(t)     (t)
#define PERIOD(t)       (t)
#define START_TIME(t)   (t)

#endif //__NG_EVENT_H__
