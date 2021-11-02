#include "ng_gestures.h"
#include "ng_tree.h"
#include "ng_display.h"
#include "ng_screen_trans.h"
#include "ng_timer.h"
#include "nema_sys_defs.h"
#include "ng_timer.h"

// this is the node that the original press was assigned to
static tree_node_t *pressed_node  = NULL;

static int     cur_node_x         = 0;
static int     cur_node_y         = 0;
static int     mouse_dxy          = 0;
// swiped screens per sec
// e.g. 1 means the total transition would take 1 second
// e.g. 0.1 means we the total transition is fast, it would take 0.1 second
static float   swipe_velocity     = 0.f;

static void
ng_reset_swipe_velocity(void) {
    swipe_velocity = 0.f;
}

// calculates swipe_velocity (screens/sec)
// takes current velocity and averages with previous velocities to avoid spikes
static void
ng_update_swipe_velocity(float velocity_screens_per_frame) {
    float tmp_swipe_velocity = velocity_screens_per_frame*ng_timer_get_frequency();
    swipe_velocity = swipe_velocity*0.8f + tmp_swipe_velocity*0.2f;
}

static void abort_popup(){
    ng_display_set_mode(DISPLAY_SCREEN);
}

//returns true if popup is active and point (x,y) is inside the popup
bool 
ng_gestures_is_inside_popup(int x, int y) {
    if ( ng_display_get_mode() != DISPLAY_POPUP ) {
        return false;
    }

    if (x >= popup_node->this_->x + popup_off_x &&
            y >= popup_node->this_->y + popup_off_y &&
            x <  popup_node->this_->x + popup_off_x + popup_node->this_->w &&
            y <  popup_node->this_->y + popup_off_y + popup_node->this_->h) {
            return true;
    } else {
        return false;
    }
}

static GESTURE_FUNC_ABORT_PRESS(abort_press_generic)
{
    ng_gitem_unset_flag(node->this_, GITEMF_PRESS);
    ng_gitem_unset_flag(node->this_, GITEMF_HIGHLIGHTED);

    if ((node->this_->gestures != NULL) && (node->this_->gestures->abort != NULL)) {
        node->this_->gestures->abort(node);
    }
    pressed_node = NULL;
}

static GESTURE_FUNC_PRESS(press_generic)
{
    (void)x;
    (void)y;
    ng_gitem_set_flag(node->this_, GITEMF_HIGHLIGHTED);

    if ( (node->this_->gestures == NULL) || (node->this_->gestures->press == NULL) ) {
        // Generic press
        ng_event_handle(node->this_->event, EV_TRIGGER_PRESS);
    } else {
        // Item specific press
        node->this_->gestures->press(node, cur_node_x, cur_node_y);
    }
}

static GESTURE_FUNC_RELEASE(release_generic)
{
    ng_gitem_unset_flag(node->this_, GITEMF_HIGHLIGHTED);

    //Generic release
    if ( (node->this_->gestures == NULL) || (node->this_->gestures->release == NULL) ) {
        ng_event_handle(node->this_->event, EV_TRIGGER_RELEASE);
    //Item specific release
    } else if ((node->this_->gestures != NULL) && (node->this_->gestures->release != NULL)) {
        node->this_->gestures->release(node);
    }
}

static GESTURE_FUNC_SWIPE(swipe_generic)
{
    //Generic release
    if ( (node->this_->gestures == NULL) || (node->this_->gestures->swipe == NULL) ) {
        ng_event_handle(node->this_->event, EV_TRIGGER_DRAG);
    //Item specific swipe
    } else if ((node->this_->gestures != NULL) && (node->this_->gestures->swipe != NULL)) {
        node->this_->gestures->swipe(node, x, y, dx, dy);
    }
}

tree_node_t*
ng_gestures_press(nema_event_t *event, int event_press_x, int event_press_y) {
    (void)event;

    ng_reset_swipe_velocity();

    pressed_node    = NULL;

    // if doing screen transition and mouse is pressed, pause transition
    if ( DOING_SCREEN_TRANSITION ) {
        pressed_node = NG_CUR_SCREEN_GROUP_NODES[NG_CUR_SCREEN_NODE_INDEX];
        ng_screen_trans_pause(NG_SCREEN_TRANSITION_EVENT);

    } else {
        int x_off = 0;
        int y_off = 0;

        (ng_gestures_is_inside_popup(event_press_x, event_press_y) == true ) ? x_off+=popup_off_x : 0;
        (ng_gestures_is_inside_popup(event_press_x, event_press_y) == true ) ? y_off+=popup_off_y : 0;

        // find a node under cursor that supports PRESS or RELEASE
        pressed_node = ng_tree_get_node_under_cursor( NULL, GITEMF_PRESS | GITEMF_RELEASE, event_press_x, event_press_y, x_off, y_off, &cur_node_x  , &cur_node_y);

        //at worst case we have pressed a screen or a popup, thus pressed_not != NULL
        // Press Selected Node
        if ( pressed_node != NULL ) {
            ng_gitem_set_flag(pressed_node->this_, GITEMF_HIGHLIGHTED);

            // if selected_node supports PRESS (it may support only release), press it
            if (pressed_node->this_->flags & GITEMF_PRESS) {
                press_generic(pressed_node, cur_node_x, cur_node_y);
            }

            global_force_display = true;
        } 
    }

    return pressed_node;
}

void
ng_gestures_release(nema_event_t *event) {
    // if doing swipe-transition, finish it
    if ( ng_event_is_paused(NG_SCREEN_TRANSITION_EVENT) ) {
        float duration = SCREEN_TRANSITION_DURATION_SECS/swipe_velocity;

        if ( duration < 0.f ) {
            duration = -duration;
        }

        if ( duration > SCREEN_TRANSITION_DURATION_SECS ) {
            duration = SCREEN_TRANSITION_DURATION_SECS;
        }

        bool abort = NG_TRANSITION(NG_SCREEN_TRANSITION_EVENT)->progress < 0.33f;
        ng_screen_trans_resume(NG_SCREEN_TRANSITION_EVENT, duration, abort);
    }
    
    // if a node is pressed, release it
    else if ( pressed_node != NULL ) {

        if ( (ng_display_get_mode() == DISPLAY_POPUP) && (ng_gestures_is_inside_popup(event->mouse_x, event->mouse_y) == false) ) {
                abort_popup();
        } else if (pressed_node->this_->flags & GITEMF_RELEASE) {
            release_generic(pressed_node);
        }
        
        global_force_display = true;
    }

    pressed_node  = NULL;
}

void
ng_gestures_swipe(nema_event_t *event, int mouse_dx, int mouse_dy){
    cur_node_x += mouse_dx;
    cur_node_y += mouse_dy;

    uint8_t layout = NG_LAYOUT_PER_GROUP[NG_CUR_SCREEN_GROUP_INDEX];
    mouse_dxy = layout != 0 ? mouse_dy : mouse_dx;

    // if doing a transition, update the transition
    if ( DOING_SCREEN_TRANSITION ) {
        if ( mouse_dxy != 0 ) {
            //velocity: screens/frame
            float velocity  = layout ? (float)mouse_dy/NG_RESY : (float)mouse_dx/NG_RESX;
            ng_screen_trans_swipe(velocity);
            //velocity: screens/second
            ng_update_swipe_velocity(velocity);
        }

    } else {
        int x_off       = 0;
        int y_off       = 0;
        //(cur_node_x, cur_node_y) refer to the pressed node. We might need these values, 
        // 
        int cur_node2_x = cur_node_x;
        int cur_node2_y = cur_node_y;

        (ng_gestures_is_inside_popup(cur_node_x, cur_node_y) == true ) ? x_off+=popup_off_x : 0;
        (ng_gestures_is_inside_popup(cur_node_x, cur_node_y) == true ) ? y_off+=popup_off_y : 0;

        //get the tree node under cursor
        tree_node_t *cursor_node = ng_tree_get_node_under_cursor( NULL, GITEMF_DRAG, event->mouse_x, event->mouse_y, x_off, y_off, &cur_node2_x, &cur_node2_y);

        if ( (cursor_node == NULL) && (pressed_node != NULL) )  {
            abort_press_generic(pressed_node);
            global_force_display = true;
            pressed_node         = NULL;
            return;
        }

        if ( ng_display_get_mode() == DISPLAY_POPUP ) {
            // ignore swiping inside popup
            // if ( (pressed_node == cursor_node) && (cursor_node == cur_group_tree_nodes[cur_screen_node]) ) {
            //     abort_popup();
            //     pressed_node = NULL;
            // }
        } else {
            // Start screen transition
            if ( (pressed_node == cursor_node) && (cursor_node->this_->type == GITEM_MAIN_SCREEN) && (mouse_dxy != 0) ) {
                //velocity: screens/frame
                float velocity  = layout ? (float)mouse_dy/NG_RESY : (float)mouse_dx/NG_RESX;
                NG_TRANSITION(NG_SCREEN_TRANSITION_EVENT)->progress = 0.f;
                ng_screen_trans_swipe(velocity);
                //velocity: screens/second
                ng_update_swipe_velocity(velocity);
            } else {
                //if the pressed_node supports swipe, swipe it (eg swipe pressed slider)
                if ( (pressed_node != NULL) && (pressed_node->this_->flags & GITEMF_DRAG) ) {
                    swipe_generic(pressed_node, cur_node_x, cur_node_y, mouse_dx, mouse_dy);
                } else if ( (cursor_node != NULL) && (cursor_node->this_->flags & GITEMF_DRAG) ) {
                    //if cursor_node supports swipe, swipe it (eg swipe inside window)
                    swipe_generic(cursor_node, cur_node2_x, cur_node2_y, mouse_dx, mouse_dy);
                    if ( pressed_node != NULL ) {
                        abort_press_generic(pressed_node);
                    }
                    cur_node_x = cur_node2_x;
                    cur_node_y = cur_node2_y;
                }

                global_force_display = true;
            }
        }
    }
}
