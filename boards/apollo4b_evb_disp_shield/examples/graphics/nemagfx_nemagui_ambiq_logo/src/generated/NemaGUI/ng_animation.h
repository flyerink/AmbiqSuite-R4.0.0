#ifndef __NG_ANIMATION__
#define __NG_ANIMATION__

#include "ng_tree.h"
#include "ng_gitem.h"
#include "nema_utils.h"

#ifndef EXTERN
#define EXTERN extern
#define REMEMBER_TO_UNDEFINE_EXTERN
#endif

typedef enum {
    NG_DIRECTION_LEFT,
    NG_DIRECTION_RIGHT,
    NG_DIRECTION_TOP,
    NG_DIRECTION_BOTTOM
} ng_animation_direction_e;

typedef enum {
    NG_SHOW,
    NG_HIDE
} ng_animation_e;

typedef struct _ng_animation_data_t {
    tree_node_t   *node;
    float (*ez_func)(float p);
    int            back_buffer_index;
    void          *ext_data;
    ng_animation_e action;
} ng_animation_data_t;

#define NG_ANIMATION_DATA(object) SAFE_CAST((object), void *, ng_animation_data_t *)

void ng_animation_callback(ng_event_base_t *event, void *data);
void ng_animation_draw(tree_node_t *node, int x_min, int y_min, int x_max, int y_max);
bool ng_animation_init(ng_animation_data_t *data);


void ng_animation_fade     (ng_animation_data_t *data);
void ng_animation_fly      (ng_animation_data_t *data);
void ng_animation_fade_zoom(ng_animation_data_t *data);
void ng_animation_cube_face(ng_animation_data_t *data);
void ng_animation_flip     (ng_animation_data_t *data);

#ifdef REMEMBER_TO_UNDEFINE_EXTERN
#undef EXTERN
#undef REMEMBER_TO_UNDEFINE_EXTERN
#endif

#endif // __NG_ANIMATION__
