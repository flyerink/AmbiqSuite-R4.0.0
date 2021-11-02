#ifndef __GUI_TREE_H__   
#define __GUI_TREE_H__   
                         
#include "ng_globals.h"
                         
#ifndef EXTERN
#define EXTERN extern
#define REMEMBER_TO_UNDEFINE_EXTERN
#endif

//GItem Declaration
//---------------------------------
EXTERN gitem_screen_t _1Screen1;
	EXTERN gitem_watchface_t _2Watch_Face1;
		EXTERN gitem_image_t _7Description;
		EXTERN gitem_image_t _10Arrow;
		EXTERN gitem_image_t _22AmbiqLogo;
EXTERN gitem_screen_t _11Screen2;
	EXTERN gitem_watchface_t _12Watch_Face2;
		EXTERN gitem_image_t _27Image1;
		EXTERN gitem_image_t _28Image2;
		EXTERN gitem_image_t _38Image4;
		EXTERN gitem_needle_t _15HourHand;
		EXTERN gitem_needle_t _14MinuteHand;
		EXTERN gitem_needle_t _13SecondHand;
	EXTERN gitem_image_t _31Image3;
	EXTERN gitem_image_t _34Image3;
	EXTERN gitem_image_t _36Image3;
	EXTERN gitem_image_t _37Image3;
EXTERN gitem_screen_t _18Screen3;
	EXTERN gitem_watchface_t _21Watch_Face3;
		EXTERN gitem_image_t _26AmbiqLogo;
	EXTERN gitem_image_t _20Image10;

//---------------------------------

//Tree Node Declaration 
EXTERN tree_node_t node_1Screen1;
	EXTERN tree_node_t node_2Watch_Face1;
		EXTERN tree_node_t node_7Description;
		EXTERN tree_node_t node_10Arrow;
		EXTERN tree_node_t node_22AmbiqLogo;
EXTERN tree_node_t node_11Screen2;
	EXTERN tree_node_t node_12Watch_Face2;
		EXTERN tree_node_t node_27Image1;
		EXTERN tree_node_t node_28Image2;
		EXTERN tree_node_t node_38Image4;
		EXTERN tree_node_t node_15HourHand;
		EXTERN tree_node_t node_14MinuteHand;
		EXTERN tree_node_t node_13SecondHand;
	EXTERN tree_node_t node_31Image3;
	EXTERN tree_node_t node_34Image3;
	EXTERN tree_node_t node_36Image3;
	EXTERN tree_node_t node_37Image3;
EXTERN tree_node_t node_18Screen3;
	EXTERN tree_node_t node_21Watch_Face3;
		EXTERN tree_node_t node_26AmbiqLogo;
	EXTERN tree_node_t node_20Image10;

//---------------------------------

#ifdef REMEMBER_TO_UNDEFINE_EXTERN
#undef EXTERN
#undef REMEMBER_TO_UNDEFINE_EXTERN
#endif

#endif //__GUI_TREE_H__
