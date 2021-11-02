#ifndef NG_GITEM_H__
#define NG_GITEM_H__

#include "nema_core.h"
#include "nema_utils.h"
#include "nema_font.h"

#include "ng_typedefs.h"

//GITEM FLAGS
#define GITEMF_ALWAYS            0x00000001U //(1<< 0)
#define GITEMF_PRESS             0x00000002U //(1<< 1)
#define GITEMF_RELEASE           0x00000004U //(1<< 2)
#define GITEMF_HOLD              0x00000008U //(1<< 3)

#define GITEMF_DRAG              0x00000010U //(1<< 4)
#define GITEMF_FILL_COLOR        0x00000020U //(1<< 5)
#define GITEMF_FILL_IMAGE        0x00000040U //(1<< 6)
#define GITEMF_FILL_OUTLINE      0x00000080U //(1<< 7)

#define GITEMF_HIGHLIGHTED       0x00000100U //(1<< 8)
#define GITEMF_CHECKED           0x00000200U //(1<< 9)
#define GITEMF_STOP_RECUR        0x00000400U //(1<< 10)
#define GITEMF_HIDDEN            0x00000800U //(1<< 11)

#define GITEMF_ANIMATED          0x00001000U //(1<< 12)
// #define GITEMF_WINDOW          0x00002000U //(1<< 13)

//GITEM TYPE
typedef enum
{
    GITEM_MAIN_SCREEN,             //0
    GITEM_CONTAINER,               //1
    GITEM_CIRCLE,                  //2
    GITEM_RECT,                    //3
    GITEM_ROUNDED_RECT,            //4
    GITEM_IMAGE,                   //5
    GITEM_LABEL,                   //6
    GITEM_LABEL_BUTTON,            //7
    GITEM_CHECKBOX,                //8
    GITEM_RADIO_BUTTON,            //9
    GITEM_HORIZONTAL_SLIDER,       //10
    GITEM_VERTICAL_SLIDER,         //11
    GITEM_DIGITAL_METER,           //12
    GITEM_WINDOW_SCREEN,           //13
    GITEM_ICON,                    //14
    GITEM_TABLE,                   //15
    GITEM_HORIZONTAL_PROGRESS_BAR, //16
    GITEM_GAUGE,                   //17
    GITEM_NEEDLE,                  //18
    GITEM_ICON_BUTTON,             //19
    GITEM_WINDOW,                  //20
    GITEM_CIRCULAR_PROGRESS,       //21
    GITEM_WATCH_FACE,              //22
    //watchface hand
    //digital clock
    GITEM_VERTICAL_PROGRESS_BAR,
    //scroll bar
    //scrolling window
    //swipe window
    //container array
    //toggle button
    GITEM_TYPE_COUNT          //23
} gitem_type_e;

struct _ng_event_base_t;

//GITEM BASE STRUCTURE
#define BASE_STRUCT                                                     \
    struct _ng_event_base_t *event;                                     \
    void (*draw)(struct _gitem_base_t* gitem, int x_off, int y_off);    \
    struct _gitem_gestures_t   *gestures;                               \
    uint32_t          flags;                                            \
    int               x;                                                \
    int               y;                                                \
    uint16_t          w;                                                \
    uint16_t          h;                                                \
    int               id;                                               \
    gitem_type_e      type;                                             \
    uint32_t          color

typedef struct _gitem_base_t{
    BASE_STRUCT;
} __gitem_base_t;

typedef struct _attr_text_t{
    uint8_t        index; // index to current text-font (multitext-multilanguage support)
    nema_font_t  **fonts;
    char         **texts;
    uint32_t     alignment;
} attr_text_t;

float ng_gitem_get_value(gitem_base_t *git);

//GITEM GENERIC FUNCTIONS
void ng_gitem_set_flag(gitem_base_t *git, uint32_t flag);
void ng_gitem_unset_flag(gitem_base_t *git, uint32_t flag);
void ng_gitem_set_visible(gitem_base_t *git);
void ng_gitem_set_hidden(gitem_base_t *git);

void ng_gitem_set_alpha(gitem_base_t *git, uint32_t alpha);
void ng_gitem_set_color(gitem_base_t *git, uint32_t rgba);
void ng_gitem_set_position(gitem_base_t *git, int x, int y);
void ng_gitem_set_size(gitem_base_t *git, int w, int h);

// void ng_gitem_scale(gitem_base_t *git, float scale_x, float scale_y, )

#define _G_TYPE_CIC(ip, gt, ct)       ((ct*) ip)
#define G_TYPE_CHECK_INSTANCE_CAST(instance, g_type, c_type)    (_G_TYPE_CIC ((instance), (g_type), c_type))
#define NG_GITEM(object)            (G_TYPE_CHECK_INSTANCE_CAST ((object), gitem_type_e, gitem_base_t))

// #define NG_GITEM2(object) SAFE_CAST((object), void *, gitem_base_t *)

#define DRAW_FUNC(NAME) void NAME (gitem_base_t *git, int x_off, int y_off)

#define ID(v)         (v)
#define X(v)          (v)
#define Y(v)          (v)
#define W(v)          (v)
#define H(v)          (v)
#define COLOR(v)      (v)
#define EVENT(v)      (v)
#define IMAGE(v)      (v)
#define CUR_VAL(v)    (v)
#define MAX_VAL(v)    (v)
#define MIN_VAL(v)    (v)
#define FLAGS(v)      (v)
#define GESTURES(v)   (v)
#define PEN_WIDTH(v)  (v)
#define PRECISION(v)  (v)
#define TEXT_COLOR(v) (v)
#define SEC_COLOR(v)  (v)
#define SEC_IMAGE(v)  (v)
#define ANGLE(v)      (v)
#define MAX_ANGLE(v)  (v)
#define MIN_ANGLE(v)  (v)
#define STEP(v)       (v)
#define X_ROT(v)      (v)
#define Y_ROT(v)      (v)
#define NEEDLE(v)     (v)
#define HOUR(v)       (v)
#define MINUTE(v)     (v)
#define SECOND(v)     (v)
#define RADIUS(v)     (v)
#define SUFFIX(v)     (v)

#endif // NG_GITEM_H__
