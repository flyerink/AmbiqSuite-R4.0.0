#ifndef __GITEM_DRAW_H__
#define __GITEM_DRAW_H__
#include "ng_gitem.h"
#include "ng_tree.h"

void ng_draw_tree_node (tree_node_t *node, int x_off, int y_off, int x_min, int y_min, int x_max, int y_max);
void ng_draw_tree      (tree_node_t *node, int x_off, int y_off, int x_min, int y_min, int x_max, int y_max);
void ng_draw_to_buffer (tree_node_t *node, int x_off, int y_off, int x_min, int y_min, int x_max, int y_max);
#endif //GITEM_DRAW_H__
