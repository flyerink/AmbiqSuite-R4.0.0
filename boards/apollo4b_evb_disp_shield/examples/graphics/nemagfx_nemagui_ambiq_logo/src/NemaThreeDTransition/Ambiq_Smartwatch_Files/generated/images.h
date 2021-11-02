#ifndef __NG_IMAGES_H__
#define __NG_IMAGES_H__

#include "nema_utils.h"

#include "ng_utils.h"

#ifndef EXTERN
#define EXTERN extern
#define REMEMBER_TO_UNDEFINE_EXTERN
#endif

EXTERN img_obj_t _AmbiqAddress_tsc6a;
EXTERN img_obj_t _AmbiqArrow_tsc6a;
EXTERN img_obj_t _AmbiqMinuteHand_tsc6a;
EXTERN img_obj_t _AmbiqHourHand_tsc6a;
EXTERN img_obj_t _Ambiq_WatchFace_tsc4;
EXTERN img_obj_t _AmbiqDescrip_tsc6a;
EXTERN img_obj_t _ScottyCircle_tsc6a;
EXTERN img_obj_t _white_ambiq_logo_tsc6a;
EXTERN img_obj_t _AmbiqSecondHand_tsc6a;
EXTERN img_obj_t _ClockCircle_tsc6a;
EXTERN img_obj_t _Star_tsc6a;
EXTERN img_obj_t _ScottyFly_tsc6a;

void load_image_assets(void);

#ifdef REMEMBER_TO_UNDEFINE_EXTERN
#undef EXTERN
#undef REMEMBER_TO_UNDEFINE_EXTERN
#endif

#endif  //__NG_IMAGES_H__ 
