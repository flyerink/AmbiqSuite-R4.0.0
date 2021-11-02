#ifndef __NEMAGUI_WINDOW__
#define __NEMAGUI_WINDOW__
#include "ng_gitem.h"
#include "ng_gestures.h"

typedef struct _gitem_window_t{
    BASE_STRUCT;
} gitem_window_t;

extern gitem_gestures_t gestures_window;

void ng_window_set_source(tree_node_t *window, tree_node_t *source);

#define NG_WINDOW(object) SAFE_CAST((object), gitem_base_t *, gitem_window_t *)

#endif //__NEMAGUI_WINDOW__
