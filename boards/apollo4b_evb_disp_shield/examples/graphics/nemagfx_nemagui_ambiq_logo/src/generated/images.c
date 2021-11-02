#include "nema_core.h"

#define EXTERN
#include "images.h"
#undef EXTERN

img_obj_t _AmbiqAddress_tsc6a;
img_obj_t _AmbiqArrow_tsc6a;
img_obj_t _AmbiqMinuteHand_tsc6a;
img_obj_t _AmbiqHourHand_tsc6a;
img_obj_t _Ambiq_WatchFace_tsc4;
img_obj_t _AmbiqDescrip_tsc6a;
img_obj_t _ScottyCircle_tsc6a;
img_obj_t _white_ambiq_logo_tsc6a;
img_obj_t _AmbiqSecondHand_tsc6a;
img_obj_t _ClockCircle_tsc6a;
img_obj_t _Star_tsc6a;
img_obj_t _ScottyFly_tsc6a;
#include "assets/images/AmbiqAddress_tsc6a.h"
#include "assets/images/AmbiqArrow_tsc6a.h"
#include "assets/images/AmbiqMinuteHand_tsc6a.h"
#include "assets/images/AmbiqHourHand_tsc6a.h"
#include "assets/images/Ambiq_WatchFace_tsc4.h"
#include "assets/images/AmbiqDescrip_tsc6a.h"
#include "assets/images/ScottyCircle_tsc6a.h"
#include "assets/images/white_ambiq_logo_tsc6a.h"
#include "assets/images/AmbiqSecondHand_tsc6a.h"
#include "assets/images/ClockCircle_tsc6a.h"
#include "assets/images/Star_tsc6a.h"
#include "assets/images/ScottyFly_tsc6a.h"

void load_image_assets(void) {
    _AmbiqAddress_tsc6a.bo = NG_LOAD_ARRAY(AmbiqAddress_tsc6a);
    _AmbiqAddress_tsc6a.w = 348;
    _AmbiqAddress_tsc6a.h = 116;
    _AmbiqAddress_tsc6a.format = NEMA_TSC6A;
    _AmbiqAddress_tsc6a.stride = -1;
    _AmbiqAddress_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _AmbiqAddress_tsc6a.color = 0xff000000;

    _AmbiqArrow_tsc6a.bo = NG_LOAD_ARRAY(AmbiqArrow_tsc6a);
    _AmbiqArrow_tsc6a.w = 100;
    _AmbiqArrow_tsc6a.h = 100;
    _AmbiqArrow_tsc6a.format = NEMA_TSC6A;
    _AmbiqArrow_tsc6a.stride = -1;
    _AmbiqArrow_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _AmbiqArrow_tsc6a.color = 0xff000000;

    _AmbiqMinuteHand_tsc6a.bo = NG_LOAD_ARRAY(AmbiqMinuteHand_tsc6a);
    _AmbiqMinuteHand_tsc6a.w = 208;
    _AmbiqMinuteHand_tsc6a.h = 12;
    _AmbiqMinuteHand_tsc6a.format = NEMA_TSC6A;
    _AmbiqMinuteHand_tsc6a.stride = -1;
    _AmbiqMinuteHand_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _AmbiqMinuteHand_tsc6a.color = 0xff000000;

    _AmbiqHourHand_tsc6a.bo = NG_LOAD_ARRAY(AmbiqHourHand_tsc6a);
    _AmbiqHourHand_tsc6a.w = 172;
    _AmbiqHourHand_tsc6a.h = 12;
    _AmbiqHourHand_tsc6a.format = NEMA_TSC6A;
    _AmbiqHourHand_tsc6a.stride = -1;
    _AmbiqHourHand_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _AmbiqHourHand_tsc6a.color = 0xff000000;

    _Ambiq_WatchFace_tsc4.bo = NG_LOAD_ARRAY(Ambiq_WatchFace_tsc4);
    _Ambiq_WatchFace_tsc4.w = 456;
    _Ambiq_WatchFace_tsc4.h = 456;
    _Ambiq_WatchFace_tsc4.format = NEMA_TSC4;
    _Ambiq_WatchFace_tsc4.stride = -1;
    _Ambiq_WatchFace_tsc4.sampling_mode = NEMA_FILTER_BL;
    _Ambiq_WatchFace_tsc4.color = 0xff000000;

    _AmbiqDescrip_tsc6a.bo = NG_LOAD_ARRAY(AmbiqDescrip_tsc6a);
    _AmbiqDescrip_tsc6a.w = 424;
    _AmbiqDescrip_tsc6a.h = 148;
    _AmbiqDescrip_tsc6a.format = NEMA_TSC6A;
    _AmbiqDescrip_tsc6a.stride = -1;
    _AmbiqDescrip_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _AmbiqDescrip_tsc6a.color = 0xff000000;

    _ScottyCircle_tsc6a.bo = NG_LOAD_ARRAY(ScottyCircle_tsc6a);
    _ScottyCircle_tsc6a.w = 100;
    _ScottyCircle_tsc6a.h = 100;
    _ScottyCircle_tsc6a.format = NEMA_TSC6A;
    _ScottyCircle_tsc6a.stride = -1;
    _ScottyCircle_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _ScottyCircle_tsc6a.color = 0xff000000;

    _white_ambiq_logo_tsc6a.bo = NG_LOAD_ARRAY(white_ambiq_logo_tsc6a);
    _white_ambiq_logo_tsc6a.w = 348;
    _white_ambiq_logo_tsc6a.h = 100;
    _white_ambiq_logo_tsc6a.format = NEMA_TSC6A;
    _white_ambiq_logo_tsc6a.stride = -1;
    _white_ambiq_logo_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _white_ambiq_logo_tsc6a.color = 0xff000000;

    _AmbiqSecondHand_tsc6a.bo = NG_LOAD_ARRAY(AmbiqSecondHand_tsc6a);
    _AmbiqSecondHand_tsc6a.w = 208;
    _AmbiqSecondHand_tsc6a.h = 12;
    _AmbiqSecondHand_tsc6a.format = NEMA_TSC6A;
    _AmbiqSecondHand_tsc6a.stride = -1;
    _AmbiqSecondHand_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _AmbiqSecondHand_tsc6a.color = 0xff000000;

    _ClockCircle_tsc6a.bo = NG_LOAD_ARRAY(ClockCircle_tsc6a);
    _ClockCircle_tsc6a.w = 28;
    _ClockCircle_tsc6a.h = 28;
    _ClockCircle_tsc6a.format = NEMA_TSC6A;
    _ClockCircle_tsc6a.stride = -1;
    _ClockCircle_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _ClockCircle_tsc6a.color = 0xff000000;

    _Star_tsc6a.bo = NG_LOAD_ARRAY(Star_tsc6a);
    _Star_tsc6a.w = 16;
    _Star_tsc6a.h = 16;
    _Star_tsc6a.format = NEMA_TSC6A;
    _Star_tsc6a.stride = -1;
    _Star_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _Star_tsc6a.color = 0xff000000;

    _ScottyFly_tsc6a.bo = NG_LOAD_ARRAY(ScottyFly_tsc6a);
    _ScottyFly_tsc6a.w = 20;
    _ScottyFly_tsc6a.h = 80;
    _ScottyFly_tsc6a.format = NEMA_TSC6A;
    _ScottyFly_tsc6a.stride = -1;
    _ScottyFly_tsc6a.sampling_mode = NEMA_FILTER_BL;
    _ScottyFly_tsc6a.color = 0xff000000;

}
