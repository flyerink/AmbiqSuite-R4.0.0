/*
 * "Gestures" is responsible for handling user gestures on the display.
 * a) handle press, release, swipe on an item
 * b) start, pause, update a screen transition
 * c) provide default function definitions that can be adopted in widgets
 */
#ifndef NG_GESTURES_H
#define NG_GESTURES_H

#include "ng_tree.h"
#include "ng_globals.h"
#include "nema_event.h"

#define GESTURE_FUNC_ABORT_PRESS(NAME)  void NAME (tree_node_t *node)
#define GESTURE_FUNC_RELEASE(    NAME)  void NAME (tree_node_t *node)
#define GESTURE_FUNC_PRESS(      NAME)  void NAME (tree_node_t *node, int x, int y)
#define GESTURE_FUNC_SWIPE(      NAME)  void NAME (tree_node_t *node, int x, int y, int dx, int dy)

typedef GESTURE_FUNC_RELEASE(release_gesture_func_t);
typedef GESTURE_FUNC_PRESS(press_gesture_func_t);
typedef GESTURE_FUNC_SWIPE(swipe_gesture_func_t);
typedef GESTURE_FUNC_ABORT_PRESS(abort_gesture_func_t);

typedef struct _gitem_gestures_t {
    press_gesture_func_t   *press;
    release_gesture_func_t *release;
    swipe_gesture_func_t   *swipe;
    abort_gesture_func_t   *abort;
} gitem_gestures_t;


tree_node_t* ng_gestures_press   (nema_event_t *event, int event_press_x, int event_press_y);
void         ng_gestures_release (nema_event_t *event);
void         ng_gestures_swipe   (nema_event_t *event, int mouse_dx, int mouse_dy);

bool ng_gestures_is_inside_popup(int x, int y);

void ng_gestures_set_screen_transition_event(ng_event_base_t *event);

#endif // NG_GESTURES_H
