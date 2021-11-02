#include "nema_graphics.h"
#include "nema_interpolators.h"
#include "nema_raster.h"

#include "ng_draw.h"
#include "ng_globals.h"
#include "ng_animation.h"

#define FRAME_BUFFERS 4
extern img_obj_t fb[FRAME_BUFFERS];

void ng_draw_tree_node(tree_node_t *node, int x_off, int y_off, int x_min, int y_min, int x_max, int y_max)
{
    if(node->this_->draw) {
        nema_set_clip(x_min, y_min, x_max-x_min, y_max-y_min);
        node->this_->draw(node->this_, x_off, y_off);
    }
}

//draws tree
void ng_draw_tree(tree_node_t *node, int x_off, int y_off, int x_min, int y_min, int x_max, int y_max)
{
    while (node && node->this_) {
        // a) Simple draw operations
        if ( (node->this_->flags & GITEMF_HIDDEN) == 0) {
            //draw current gitem
            ng_draw_tree_node(node, x_off, y_off, x_min, y_min, x_max, y_max);

            //draw children if this is a container of some kind
            if (node->first_child) {
                int cur_x_min = x_off     + node->this_->x;
                int cur_y_min = y_off     + node->this_->y;
                int cur_x_max = cur_x_min + node->this_->w;
                int cur_y_max = cur_y_min + node->this_->h;

                if (x_min > cur_x_min) cur_x_min = x_min;
                if (y_min > cur_y_min) cur_y_min = y_min;
                if (x_max < cur_x_max) cur_x_max = x_max;
                if (y_max < cur_y_max) cur_y_max = y_max;

                ng_draw_tree(node->first_child, x_off+node->this_->x, y_off+node->this_->y,
                             cur_x_min, cur_y_min,
                             cur_x_max, cur_y_max);
            }

        // b) Draw animated items (animated items are hidden and replaced by a back_buffer)
        } else if ( node->this_->flags & GITEMF_ANIMATED) {
            ng_animation_draw(node, x_min, y_min, x_max, y_max);
        }

        //got to next item at the same level;
        node = node->next;
    }
}


//Draws a tree_node and its children inside the destination buffer at its native resolution
void ng_draw_to_buffer(tree_node_t *node, int x_off, int y_off, int x_min, int y_min, int x_max, int y_max){
    //Draw target tree node
    if ( node ) {
        nema_set_clip(0, 0, node->this_->w, node->this_->h);
        // nema_set_clip(x_min, y_min, x_max-x_min, y_max-y_min);
        //Clear
        nema_set_blend_fill(NEMA_BL_SRC);
        ng_fill_rect(0, 0, node->this_->w, node->this_->h, 0U, 1);

        //Draw current node
        if(node->this_->draw){
            node->this_->draw(node->this_, x_off, y_off);
        }

        node = node->first_child;
    }

    //Draw its children
    while (node && node->this_) {
        if ( (node->this_->flags & GITEMF_HIDDEN) == 0) {
            //draw current gitem
            if (node->this_->draw) {
                x_min = x_min > 0 ? x_min : 0;
                y_min = y_min > 0 ? y_min : 0;
                // node->this_->color = node->this_->color_const;
                if (x_min < x_max && y_min < y_max) {
                    nema_set_clip(x_min, y_min, x_max-x_min, y_max-y_min);
                    node->this_->draw(node->this_, x_off, y_off);
                }
            }

            //draw children if this is a container of some kind
            if (node->first_child) {

                int cur_x_min = x_off     + node->this_->x;
                int cur_y_min = y_off     + node->this_->y;
                int cur_x_max = cur_x_min + node->this_->w;
                int cur_y_max = cur_y_min + node->this_->h;

                if (x_min > cur_x_min) cur_x_min = x_min;
                if (y_min > cur_y_min) cur_y_min = y_min;
                if (x_max < cur_x_max) cur_x_max = x_max;
                if (y_max < cur_y_max) cur_y_max = y_max;

                ng_draw_to_buffer(node->first_child, x_off + node->this_->x, y_off + node->this_->y, cur_x_min, cur_y_min, cur_x_max, cur_y_max);
            }
        }

        //go to next item at the same level
        node = node->next;
    }
}
