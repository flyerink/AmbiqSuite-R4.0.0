#ifndef __NEMAGUI_DISPLAY_H__
#define __NEMAGUI_DISPLAY_H__

#include "nema_core.h"
#include "nema_transitions.h"
#include "ng_gitem.h"

#ifndef EXTERN
#define EXTERN extern
#define REMEMBER_TO_UNDEFINE_EXTERN
#endif

typedef enum {
    DISPLAY_SCREEN,            //0
    DISPLAY_SCREEN_TRANSITION, //1
    DISPLAY_POPUP              //2
} display_mode_e;

void ng_display_screen_node_to_fb(img_obj_t *fb_img, tree_node_t *screen_node, int x_off, int y_off);
void ng_display_screen_clear(int wait);
void ng_display_bind_transition_buffers(void);
void ng_display(void);
void ng_display_init(void);
void ng_display_set_event(ng_event_base_t *event);
void ng_display_set_mode(display_mode_e mode);
display_mode_e ng_display_get_mode();
void ng_display_set_popup(tree_node_t *node);

bool ng_back_buffer_is_locked(int index);
void ng_back_buffer_lock(int index);
void ng_back_buffer_unlock(int index);

EXTERN nema_transition_t global_screen_trans_effect;
EXTERN nema_cmdlist_t cl, cl_screen;

#ifdef REMEMBER_TO_UNDEFINE_EXTERN
#undef EXTERN
#undef REMEMBER_TO_UNDEFINE_EXTERN
#endif

#endif //__NEMAGUI_DISPLAY_H__
