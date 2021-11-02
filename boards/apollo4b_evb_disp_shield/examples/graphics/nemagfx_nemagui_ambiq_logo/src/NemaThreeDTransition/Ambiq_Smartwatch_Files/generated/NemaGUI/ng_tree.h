#ifndef NG_TREE_H__
#define NG_TREE_H__

#include "ng_typedefs.h"
#include "nema_sys_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _tree_node_t{
    gitem_base_t *this_;
    struct _tree_node_t *parent;
    struct _tree_node_t *first_child;
    struct _tree_node_t *next;
} tree_node_t;

void ng_tree_set_current_screen(tree_node_t *node);
void ng_tree_set_current_popup(tree_node_t *node);
tree_node_t* ng_tree_get_current_screen();

tree_node_t* ng_tree_get_node_under_cursor(tree_node_t *node, uint32_t gesture, int x, int y, int x_off, int y_off, int *click_x, int *click_y);

#define NG_TREE_NODE(object) SAFE_CAST((object), void *, tree_node_t *)

#define _G_TYPE_CIC(ip, gt, ct)       ((ct*) ip)
#define G_TYPE_CHECK_INSTANCE_CAST(instance, g_type, c_type)    (_G_TYPE_CIC ((instance), (g_type), c_type))
#define NG_TREE_NODE_OBJ(object)            (G_TYPE_CHECK_INSTANCE_CAST ((object), gitem_type_e, tree_node_t))


#define PARENT_NODE(v)      (v)
#define FIRST_CHILD_NODE(v) (v)
#define NEXT_NODE(v)        (v)


#ifdef __cplusplus
}
#endif

#endif // NG_TREE_H__
