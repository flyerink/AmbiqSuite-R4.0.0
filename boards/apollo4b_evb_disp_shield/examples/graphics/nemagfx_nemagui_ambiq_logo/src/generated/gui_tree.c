#undef EXTERN
#include "gui_tree.h"
#undef EXTERN

#include "ng_globals.h"
#include "fonts.h"
#include "images.h"

gitem_screen_t _1Screen1 = {EVENT(NULL), ng_screen_draw, GESTURES(NULL), FLAGS(0x00000036U), X(0), Y(0), W(456), H(456), ID(1), GITEM_MAIN_SCREEN , COLOR(0xff000000U) , IMAGE(NULL)};
	gitem_watchface_t _2Watch_Face1 = {EVENT(NULL), ng_watchface_draw, GESTURES(NULL), FLAGS(0x00000040U), X(0), Y(0), W(454), H(454), ID(2), GITEM_WATCH_FACE , COLOR(0xff808080U) , IMAGE(&_Ambiq_WatchFace_tsc4 ), NG_GITEM(NULL), NG_GITEM(NULL), NG_GITEM(NULL), PEN_WIDTH(1)};
		gitem_image_t _7Description = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(11), Y(140), W(424), H(147), ID(7), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_AmbiqDescrip_tsc6a )};
		gitem_image_t _10Arrow = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(301), Y(272), W(100), H(100), ID(10), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_AmbiqArrow_tsc6a)};
		gitem_image_t _22AmbiqLogo = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(175), Y(381), W(100), H(29), ID(22), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_white_ambiq_logo_tsc6a )};

gitem_screen_t _11Screen2 = {EVENT(NULL), ng_screen_draw, GESTURES(NULL), FLAGS(0x00000036U), X(0), Y(0), W(456), H(456), ID(11), GITEM_MAIN_SCREEN , COLOR(0xff000000U) , IMAGE(NULL)};
	gitem_watchface_t _12Watch_Face2 = {EVENT(NULL), ng_watchface_draw, GESTURES(NULL), FLAGS(0x00000040U), X(0), Y(0), W(454), H(454), ID(12), GITEM_WATCH_FACE , COLOR(0xff808080U) , IMAGE(&_Ambiq_WatchFace_tsc4 ), NG_GITEM(&_15HourHand), NG_GITEM(&_14MinuteHand), NG_GITEM(&_13SecondHand), PEN_WIDTH(1)};
		gitem_image_t _27Image1 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(175), Y(296), W(100), H(100), ID(27), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_ScottyCircle_tsc6a )};
		gitem_image_t _28Image2 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(215), Y(215), W(25), H(25), ID(28), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_ClockCircle_tsc6a )};
		gitem_image_t _38Image4 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(291), Y(49), W(20), H(79), ID(38), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_ScottyFly_tsc6a )};
		gitem_needle_t _15HourHand = {EVENT(NULL), ng_needle_draw, GESTURES(NULL), FLAGS(0x00000040U), X(227), Y(227), W(172), H(9), ID(15), GITEM_NEEDLE , COLOR(0xff000000U) , IMAGE(&_AmbiqHourHand_tsc6a ), ANGLE(0.000f), X_ROT(167), Y_ROT(4), PEN_WIDTH(1)};
		gitem_needle_t _14MinuteHand = {EVENT(NULL), ng_needle_draw, GESTURES(NULL), FLAGS(0x00000040U), X(227), Y(227), W(207), H(9), ID(14), GITEM_NEEDLE , COLOR(0xff000000U) , IMAGE(&_AmbiqMinuteHand_tsc6a ), ANGLE(0.000f), X_ROT(201), Y_ROT(4), PEN_WIDTH(1)};
		gitem_needle_t _13SecondHand = {EVENT(NULL), ng_needle_draw, GESTURES(NULL), FLAGS(0x00000040U), X(227), Y(227), W(207), H(9), ID(13), GITEM_NEEDLE , COLOR(0xff000000U) , IMAGE(&_AmbiqSecondHand_tsc6a ), ANGLE(0.000f), X_ROT(202), Y_ROT(4), PEN_WIDTH(1)};
	gitem_image_t _31Image3 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(31), Y(220), W(15), H(15), ID(31), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_Star_tsc6a )};
	gitem_image_t _34Image3 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(403), Y(220), W(15), H(15), ID(34), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_Star_tsc6a )};
	gitem_image_t _36Image3 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(220), Y(31), W(15), H(15), ID(36), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_Star_tsc6a )};
	gitem_image_t _37Image3 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(220), Y(403), W(15), H(15), ID(37), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_Star_tsc6a )};

gitem_screen_t _18Screen3 = {EVENT(NULL), ng_screen_draw, GESTURES(NULL), FLAGS(0x00000036U), X(0), Y(0), W(456), H(456), ID(18), GITEM_MAIN_SCREEN , COLOR(0xff000000U) , IMAGE(NULL)};
	gitem_watchface_t _21Watch_Face3 = {EVENT(NULL), ng_watchface_draw, GESTURES(NULL), FLAGS(0x00000040U), X(0), Y(0), W(454), H(454), ID(21), GITEM_WATCH_FACE , COLOR(0xff808080U) , IMAGE(&_Ambiq_WatchFace_tsc4 ), NG_GITEM(NULL), NG_GITEM(NULL), NG_GITEM(NULL), PEN_WIDTH(1)};
		gitem_image_t _26AmbiqLogo = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(52), Y(162), W(346), H(100), ID(26), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_white_ambiq_logo_tsc6a )};
	gitem_image_t _20Image10 = {EVENT(NULL), ng_image_draw, GESTURES(NULL), FLAGS(0x00000040U), X(52), Y(304), W(345), H(113), ID(20), GITEM_IMAGE , COLOR(0xff808080U) , IMAGE(&_AmbiqAddress_tsc6a )};


//---------------------------------


//Build tree nodes
//---------------------------------
tree_node_t node_1Screen1 = { NG_GITEM(&_1Screen1), PARENT_NODE(NULL), FIRST_CHILD_NODE(&node_2Watch_Face1), NEXT_NODE(NULL)};
	tree_node_t node_2Watch_Face1 = { NG_GITEM(&_2Watch_Face1), PARENT_NODE(&node_1Screen1), FIRST_CHILD_NODE(&node_7Description), NEXT_NODE(NULL)};
		tree_node_t node_7Description = { NG_GITEM(&_7Description), PARENT_NODE(&node_2Watch_Face1), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_10Arrow)};
		tree_node_t node_10Arrow = { NG_GITEM(&_10Arrow), PARENT_NODE(&node_2Watch_Face1), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_22AmbiqLogo)};
		tree_node_t node_22AmbiqLogo = { NG_GITEM(&_22AmbiqLogo), PARENT_NODE(&node_2Watch_Face1), FIRST_CHILD_NODE(NULL), NEXT_NODE(NULL)};

//Build tree nodes
//---------------------------------
tree_node_t node_11Screen2 = { NG_GITEM(&_11Screen2), PARENT_NODE(NULL), FIRST_CHILD_NODE(&node_12Watch_Face2), NEXT_NODE(NULL)};
	tree_node_t node_12Watch_Face2 = { NG_GITEM(&_12Watch_Face2), PARENT_NODE(&node_11Screen2), FIRST_CHILD_NODE(&node_27Image1), NEXT_NODE(&node_31Image3)};
		tree_node_t node_27Image1 = { NG_GITEM(&_27Image1), PARENT_NODE(&node_12Watch_Face2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_28Image2)};
		tree_node_t node_28Image2 = { NG_GITEM(&_28Image2), PARENT_NODE(&node_12Watch_Face2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_38Image4)};
		tree_node_t node_38Image4 = { NG_GITEM(&_38Image4), PARENT_NODE(&node_12Watch_Face2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_15HourHand)};
		tree_node_t node_15HourHand = { NG_GITEM(&_15HourHand), PARENT_NODE(&node_12Watch_Face2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_14MinuteHand)};
		tree_node_t node_14MinuteHand = { NG_GITEM(&_14MinuteHand), PARENT_NODE(&node_12Watch_Face2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_13SecondHand)};
		tree_node_t node_13SecondHand = { NG_GITEM(&_13SecondHand), PARENT_NODE(&node_12Watch_Face2), FIRST_CHILD_NODE(NULL), NEXT_NODE(NULL)};
	tree_node_t node_31Image3 = { NG_GITEM(&_31Image3), PARENT_NODE(&node_11Screen2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_34Image3)};
	tree_node_t node_34Image3 = { NG_GITEM(&_34Image3), PARENT_NODE(&node_11Screen2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_36Image3)};
	tree_node_t node_36Image3 = { NG_GITEM(&_36Image3), PARENT_NODE(&node_11Screen2), FIRST_CHILD_NODE(NULL), NEXT_NODE(&node_37Image3)};
	tree_node_t node_37Image3 = { NG_GITEM(&_37Image3), PARENT_NODE(&node_11Screen2), FIRST_CHILD_NODE(NULL), NEXT_NODE(NULL)};

//Build tree nodes
//---------------------------------
tree_node_t node_18Screen3 = { NG_GITEM(&_18Screen3), PARENT_NODE(NULL), FIRST_CHILD_NODE(&node_21Watch_Face3), NEXT_NODE(NULL)};
	tree_node_t node_21Watch_Face3 = { NG_GITEM(&_21Watch_Face3), PARENT_NODE(&node_18Screen3), FIRST_CHILD_NODE(&node_26AmbiqLogo), NEXT_NODE(&node_20Image10)};
		tree_node_t node_26AmbiqLogo = { NG_GITEM(&_26AmbiqLogo), PARENT_NODE(&node_21Watch_Face3), FIRST_CHILD_NODE(NULL), NEXT_NODE(NULL)};
	tree_node_t node_20Image10 = { NG_GITEM(&_20Image10), PARENT_NODE(&node_18Screen3), FIRST_CHILD_NODE(NULL), NEXT_NODE(NULL)};
