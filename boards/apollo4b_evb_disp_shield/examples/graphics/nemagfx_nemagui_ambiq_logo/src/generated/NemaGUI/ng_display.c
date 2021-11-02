#include "nema_core.h"
#include "nema_event.h"
#include "nema_utils.h"

#include "ng_globals.h"
#define EXTERN
#include "ng_display.h"
#undef EXTERN
#include "ng_draw.h"
#include "ng_draw_prim.h"
#include "ng_tree.h"
#include "ng_screen_trans.h"

#define CURRENT_SCREEN_NODE     NG_CUR_SCREEN_GROUP_NODES[NG_CUR_SCREEN_NODE_INDEX]

static display_mode_e display_mode = DISPLAY_SCREEN;
static ng_event_base_t *screen_transition_event_ = NULL;
static int fb_locked_flags   = 0;
static img_obj_t *cur_fb_img = NULL;
static uintptr_t cur_fb_base_phys;

nema_transition_t global_screen_trans_effect = 0;

static inline img_obj_t* ng_display_get_cur_fb_img(void) {
    for (int l = 0; l < NG_FRAMEBUFFER_COUNT; ++l) {
        if ( NG_DC_LAYER[l].baseaddr_phys == cur_fb_base_phys ) {
            return &NG_FRAMEBUFFER[l];
        }
    }

    return 0;
}

static inline void
ng_display_bind_fb_img(img_obj_t *fb_img) {
    nema_bind_dst_tex(fb_img->bo.base_phys, fb_img->w, fb_img->h, fb_img->format, -1);
}

void
ng_display_screen_node_to_fb(img_obj_t *fb_img, tree_node_t *screen_node, int x_off, int y_off) {
    PROC_ENTRY;

    //draw screen backtround
    nema_cl_rewind(&cl_screen);
    nema_cl_bind(&cl_screen);
    ng_display_bind_fb_img(fb_img);
    ng_draw_tree_node(screen_node, x_off, y_off, 0, 0, NG_RESX, NG_RESY);
    nema_cl_submit_no_irq(&cl_screen);

    //draw screen gitems
    nema_cl_rewind(&cl);
    nema_cl_bind(&cl);
    ng_display_bind_fb_img(fb_img);
    ng_draw_tree(screen_node->first_child, x_off, y_off, 0, 0, NG_RESX, NG_RESY);
    nema_cl_submit(&cl);
    nema_cl_wait(&cl);
}

static void
ng_display_popup(void) {
    ng_display_screen_node_to_fb(cur_fb_img, CURRENT_SCREEN_NODE, 0, 0);

    nema_cl_rewind(&cl);
    nema_cl_bind(&cl);
    //Bind Framebuffer
    ng_display_bind_fb_img(cur_fb_img);

    nema_set_clip (0, 0, NG_RESX, NG_RESY);
    ng_fill_rect(0, 0, NG_RESX, NG_RESY, 0xd0000000U, 0);

    ng_draw_tree(popup_node, popup_off_x, popup_off_y, 0, 0, NG_RESX, NG_RESY);

    nema_cl_submit(&cl);
    nema_cl_wait(&cl);
}

void
ng_display_screen_clear(int wait) {
    nema_cl_rewind(&cl_screen);
    nema_cl_bind(&cl_screen);
        //Bind Framebuffer
        ng_display_bind_fb_img(cur_fb_img);

        nema_set_clip (0, 0, NG_RESX, NG_RESY);
        ng_fill_rect(0, 0, NG_RESX, NG_RESY, 0xff000000U, 0);

    if (wait) {
        nema_cl_submit(&cl_screen);
        nema_cl_wait(&cl_screen);
    }
    else {
        nema_cl_submit_no_irq(&cl_screen);
    }
}

extern bool goto_right_screen;

static void
ng_display_screen_transition(void) {
    // PROC_ENTRY;

    // use animation buffers
    if ( NG_BACKBUFFER_COUNT >= 2 ) {
        nema_cl_rewind(&cl);
        nema_cl_bind(&cl);
        //Bind Framebuffer
        ng_display_bind_fb_img(cur_fb_img);

        nema_set_clip (0, 0, NG_RESX, NG_RESY);

        float progress = goto_right_screen ? NG_TRANSITION(screen_transition_event_)->progress : 1.f - NG_TRANSITION(screen_transition_event_)->progress;

        nema_transition(global_screen_trans_effect, NEMA_TEX1, NEMA_TEX2, NEMA_BL_SRC, nema_ez_linear(progress), NG_FRAMEBUFFER[0].w, NG_FRAMEBUFFER[0].h);

        nema_cl_submit(&cl);
        nema_cl_wait(&cl);
    }
    // no animation buffers available
    // do linear transition
    else {
        float progress = goto_right_screen ? NG_TRANSITION(screen_transition_event_)->progress : 1.f - NG_TRANSITION(screen_transition_event_)->progress;

        bool is_vert = NG_LAYOUT_PER_GROUP[NG_CUR_SCREEN_GROUP_INDEX] == 1;

        int res = is_vert ? cur_fb_img->h : cur_fb_img->w;
        int off = progress*res;

        if ( !goto_right_screen ) {
            ng_display_screen_node_to_fb(cur_fb_img, NG_CUR_SCREEN_GROUP_NODES[NG_CUR_SCREEN_NODE_INDEX+1],
                !is_vert ? off : 0,
                 is_vert ? off : 0);
            ng_display_screen_node_to_fb(cur_fb_img, CURRENT_SCREEN_NODE,
                !is_vert ? off-NG_RESX : 0,
                 is_vert ? off-NG_RESY : 0);
        }
        else {
            ng_display_screen_node_to_fb(cur_fb_img, CURRENT_SCREEN_NODE,
                !is_vert ? off : 0,
                 is_vert ? off : 0);
            ng_display_screen_node_to_fb(cur_fb_img, NG_CUR_SCREEN_GROUP_NODES[NG_CUR_SCREEN_NODE_INDEX-1],
                !is_vert ? off-NG_RESX : 0,
                 is_vert ? off-NG_RESY : 0);
        }
    }
}

extern nemadc_layer_t layer[];

// main display function
void ng_display(void) {
    ng_display_screen_clear(0);

    if ( (display_mode == DISPLAY_SCREEN_TRANSITION) && (screen_transition_event_ != NULL)) {
        ng_display_screen_transition();
    } else if (display_mode == DISPLAY_POPUP) {
        ng_display_popup();
    } else {
        ng_display_screen_node_to_fb(cur_fb_img, CURRENT_SCREEN_NODE, 0, 0);
    }

    cur_fb_base_phys = nema_swap_fb(0);
    cur_fb_img       = ng_display_get_cur_fb_img();

    nemadc_set_layer(0, &layer[0]);
    dsi_send_frame_single(NEMADC_OUTP_OFF);
    

    // #ifdef CALCULATE_FPS
    nema_calculate_fps();
    // #endif
}

void ng_display_bind_transition_buffers(void) {
    if ( NG_BACKBUFFER_COUNT >=2 ) {
        nema_cl_rewind(&cl);
        nema_cl_bind(&cl);
        nema_bind_tex(1, NG_BACKBUFFER[0].bo.base_phys, NG_BACKBUFFER[0].w, NG_BACKBUFFER[0].h, NG_BACKBUFFER[0].format, -1, NEMA_FILTER_BL);
        nema_bind_tex(2, NG_BACKBUFFER[1].bo.base_phys, NG_BACKBUFFER[1].w, NG_BACKBUFFER[1].h, NG_BACKBUFFER[1].format, -1, NEMA_FILTER_BL);
        nema_cl_submit(&cl);
        nema_cl_wait(&cl);
    }
}

void
ng_display_init(){
    uintptr_t l0 = NG_DC_LAYER[0].baseaddr_phys;
    uintptr_t l1 = 2 > NG_FRAMEBUFFER_COUNT ? 0U : NG_DC_LAYER[1].baseaddr_phys;
    uintptr_t l2 = 3 > NG_FRAMEBUFFER_COUNT ? 0U : NG_DC_LAYER[2].baseaddr_phys;
    cur_fb_base_phys = nema_init_triple_fb(0, l0,
                                              l1,
                                              l2);

    cur_fb_img = ng_display_get_cur_fb_img();

    cl  = nema_cl_create();
    cl_screen = nema_cl_create();

    // //Bind Command List
    nema_cl_bind(&cl);

    // //Set Clipping Rectangle
    nema_set_clip(0, 0, NG_RESX, NG_RESY);

    nema_cl_submit(&cl);
    nema_wait_irq();

    nemadc_cursor_enable(1);

    ng_display();
}

void ng_display_set_event(ng_event_base_t *event) {
    screen_transition_event_ = event;
}

void ng_display_set_mode(display_mode_e mode) {
    display_mode = mode;
}

display_mode_e ng_display_get_mode() {
    return display_mode;
}

bool ng_back_buffer_is_locked(int index) {
    bool val = true;
    int mask = (1 << index);

    if ( (fb_locked_flags & mask) == 0 ) {
        val = false;
    }

    return val;
}

void ng_back_buffer_lock(int index) {
    int bit_to_set = (1 << index);
    fb_locked_flags |= bit_to_set;
}

void ng_back_buffer_unlock(int index) {
    int bit_to_set = (1 << index);
    fb_locked_flags &= ~bit_to_set;
}

void ng_display_set_popup(tree_node_t *node) {
    display_mode = DISPLAY_POPUP;
    ng_tree_set_current_popup(node);
}
