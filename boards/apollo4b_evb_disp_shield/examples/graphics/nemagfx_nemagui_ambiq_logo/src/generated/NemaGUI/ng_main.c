#include "am_bsp.h"
#include "am_util.h"

#include "nema_core.h"
#include "nema_event.h"
#include "nema_utils.h"

#include "nema_dc_hal.h"
#include "nema_dc.h"

#include "ng_globals.h"
#include "ng_display.h"
#include "ng_event.h"
#include "ng_callbacks.h"
#include "ng_gestures.h"
#include "ng_timer.h"
#include "ng_screen_trans.h"

#include "images.h"
#include "fonts.h"
#include "framebuffers.h"
#include "screen_groups.h"

#include "am_devices_dsi_rm67162.h"
#include "am_devices_nemadc_rm67162.h"

#define RESX        456
#define RESY        456

static void ng_main_loop(const int run_once) {
    static nema_event_t event = {0};
    static tree_node_t *pressed_node = NULL;

    // x,y coordinates of the last press event
    int mouse_press_x      = -1;
    int mouse_press_y      = -1;
    global_force_display   = false;
    const int ng_timer_id  = ng_timer_get();

    do {
        bool press_event       = false;
        bool release_event     = false;
        bool swipe_event       = false;
        bool press_r_event     = false;
        bool timer_event       = false;

        int mouse_dx           =  0;
        int mouse_dy           =  0;

        //wait for press/release/swipe/timer event
        bool block_until_event = (run_once == 0) && (!global_force_display);
        if ( nema_event_wait(&event, block_until_event) > 0) {
            do {

                // get mouse/touch event
                switch (event.mouse_event) {
                    case MOUSE_EVENT_LEFT_CLICK:
                        press_event   = true;
                        mouse_press_x = event.mouse_x;
                        mouse_press_y = event.mouse_y;
                        break;
                    case MOUSE_EVENT_LEFT_RELEASE:
                        release_event = true;
                        break;
                    case MOUSE_EVENT_RIGHT_CLICK:
                        press_r_event = true;
                        break;
                    default:
                        break;
                }

                // check if the user is swipping
                // if yes, set swipe_event = true
                if ( (event.mouse_state == MOUSE_STATE_LEFT_CLICKED) ) {
                    //accumulate drag events
                    mouse_dx += event.mouse_dx;
                    mouse_dy += event.mouse_dy;

                    int swipe_margin = 5;

                    if(pressed_node != NULL){
                        // if the pressed node doesn't support SWIPE , don't swipe immediately
                        // give it some margin
                        swipe_margin = ((pressed_node->this_->flags & GITEMF_DRAG) != 0U) ? 0 : swipe_margin;
                    }

                    if ( (event.mouse_dx != 0 || event.mouse_dy != 0 ) &&
                         (nema_abs(event.mouse_x - mouse_press_x) > swipe_margin  ||
                          nema_abs(event.mouse_y - mouse_press_y) > swipe_margin ) ) {
                            swipe_event = true;
                    }
                }

                if ( event.timer_id == ng_timer_id ) {
                    timer_event = true;
                }

            } while ( nema_event_wait(&event, 0) );
        } else {
            //no events, nothing to do in this iteration
            if ( global_force_display == false ) {
                continue;
            }
        }

        //Process event
        if ( press_event ){
            pressed_node = ng_gestures_press(&event, mouse_press_x, mouse_press_y);
        }

        if( swipe_event ) {
            ng_gestures_swipe(&event, mouse_dx, mouse_dy);
        }

        if ( release_event ) {
            ng_gestures_release(&event);
        }

        if( timer_event ) {
            ng_timer_handler();
        }

        if ( press_r_event ) {
            //On right click, change the transition effect
            NG_EFFECT_PER_GROUP[NG_CUR_SCREEN_GROUP_INDEX] = (NG_EFFECT_PER_GROUP[NG_CUR_SCREEN_GROUP_INDEX]+1)%NEMA_TRANS_MAX;
            global_force_display = true;
        }


        extern gitem_needle_t _15HourHand;
	    extern gitem_needle_t _14MinuteHand;
        extern gitem_needle_t _13SecondHand;
        float *second = &_13SecondHand.angle;
        float *minute = &_14MinuteHand.angle;
        float *hour = &_15HourHand.angle;
        
        float current_time;
        int32_t current_time_int_current;
        int32_t current_time_int_last;
        
        current_time = nema_get_time();
        current_time_int_current = current_time;
        
        if (current_time_int_current != current_time_int_last)
        {
            *second += 6;
            if (*second >= 360){
                *second = 0;
                *minute += 6;
                if (*minute >= 360){
                    *minute = 0;
                }

                if ((int)(*minute) % 12 == 0){
                    *hour += 1;
                }
            }
        }
        
        current_time_int_last = current_time_int_current;

        
        //if(global_force_display == true){
        //    global_force_display = false;
            ng_display();
        //}

    } while(run_once == 0);
}

static int ng_init() {
    int ret;
    uint32_t ui32MipiCfg = MIPICFG_8RGB888_OPT0; // default config
    //Initialize NemaGFX
    ret = nema_init();
    if (ret != 0)
    {
        return ret;
    }
    //Initialize Nema|dc
    ret = nemadc_init();
    if (ret != 0)
    {
        return ret;
    }

    if ((g_sDispCfg[g_eDispType].eInterface == IF_DSI) || (g_sDispCfg[g_eDispType].bUseDPHYPLL == true))
    {
        uint8_t ui8LanesNum = g_sDsiCfg.ui8NumLanes;
        uint8_t ui8DbiWidth = g_sDsiCfg.eDbiWidth;
        uint32_t ui32FreqTrim = g_sDsiCfg.eDsiFreq;
        pixel_format_t eFormat = FMT_RGB888;
        if ( am_hal_dsi_para_config(ui8LanesNum, ui8DbiWidth, ui32FreqTrim) != 0 )
        {
            return -3;
        }
        switch ( eFormat )
        {
            case FMT_RGB888:
                if ( ui8DbiWidth == 16 )
                {
                    ui32MipiCfg = MIPICFG_16RGB888_OPT0;
                }
                if ( ui8DbiWidth ==  8 )
                {
                    ui32MipiCfg = MIPICFG_8RGB888_OPT0;
                }
                break;

            case FMT_RGB565:
                if ( ui8DbiWidth == 16 )
                {
                    ui32MipiCfg = MIPICFG_16RGB565_OPT0;
                }
                if ( ui8DbiWidth ==  8 )
                {
                    ui32MipiCfg = MIPICFG_8RGB565_OPT0;
                }
                break;

            default:
                //
                // invalid color component index
                //
                return -3;
        }
    }

    uint16_t panel_resx = g_sDispCfg[g_eDispType].ui32PanelResX; //panel's max resolution
    uint16_t panel_resy = g_sDispCfg[g_eDispType].ui32PanelResY; //panel's max resolution

    uint16_t minx, miny;

    //Set the display region to center
    if ( RESX > panel_resx )
    {
        minx = 0;   // set the minimum value to 0
    }
    else
    {
        minx = (panel_resx - RESX) >> 1;
        minx = (minx >> 1) << 1;
    }

    if ( RESY > panel_resy )
    {
        miny = 0;   // set the minimum value to 0
    }
    else
    {
        miny = (panel_resy - RESY) >> 1;
        miny = (miny >> 1) << 1;
    }

    //Initialize the display
    switch (g_sDispCfg[g_eDispType].eInterface)
    {
        case IF_SPI4:
            am_devices_nemadc_rm67162_init(MIPICFG_SPI4, MIPICFG_1RGB565_OPT0, RESX, RESY, minx, miny);
            break;
        case IF_DSPI:
            am_devices_nemadc_rm67162_init(MIPICFG_DSPI | MIPICFG_SPI4, MIPICFG_2RGB888_OPT0, RESX, RESY, minx, miny);
            break;
        case IF_QSPI:
            am_devices_nemadc_rm67162_init(MIPICFG_QSPI | MIPICFG_SPI4, MIPICFG_4RGB565_OPT0, RESX, RESY, minx, miny);
            break;
        case IF_DSI:
            am_devices_dsi_rm67162_init(ui32MipiCfg, RESX, RESY, minx, miny);
            break;
        default:
            ; //NOP
    }


    load_framebuffers();
    load_image_assets();
    load_font_assets();
    init_screen_groups();

    nema_event_init(1, 0, 0, NG_RESX, NG_RESY);
    ng_event_init();

    //Check if globals (project generated) have been forwarded to the API
    ret = ng_globals_sanity_check();
    if (ret) return ret;

    //Format        | Pixclock | RESX | FP | SYNC | BP | RESY | FP | SYNC | BP
    //800x600, 60Hz | 40.000   | 800  | 40 | 128  | 88 | 600  | 1  | 4    | 23
    // nemadc_timing(800, 40, 128, 88, 600, 1, 4, 23);
    // nemadc_set_layer(0, &NG_DC_LAYER[0]);

    //Initialize display module
    ng_display_init();

    //Trigger events
    for (int i = 0; i < NG_EVENT_LIST_SIZE; ++i) {
        ng_event_base_t *event = NG_EVENT_LIST[i];

        if ( event->trigger == EV_TRIGGER_APP_START ) {
            event->start(event);
        }
    }
    return 0;
}

int guage()
{
#if defined(BAREMETAL) && defined(APOLLO4_FPGA) && defined(AM_PART_APOLLO4P)
    am_hal_stimer_config(AM_HAL_STIMER_HFRC_6MHZ | AM_HAL_STIMER_CFG_RUN);
#endif
    // Init NemaGFX, NemaDC, GUI environment (GItems, assets, events)
    int ret = ng_init();
    if (ret)
    {
        return ret;
    }


    //Enter application's main loop
    ng_main_loop(0);

    nema_cl_destroy(&cl);
    nema_cl_destroy(&cl_screen);

    return 0;
}

