#ifndef NOTOSANS_REGULAR12PT8B_H
#define NOTOSANS_REGULAR12PT8B_H

/*
    Do this:
        #define NEMA_FONT_IMPLEMENTATION
    before you include this file in *one* C file to create the implementation

    OR

    add:
        NotoSans_Regular12pt8b.c
    to your C_SRCS to be compiled
*/

#include "nema_font.h"

extern nema_font_t NotoSans_Regular12pt8b;

#ifdef NEMA_FONT_IMPLEMENTATION
#include "NotoSans_Regular12pt8b.c"
#endif // NEMA_FONT_IMPLEMENTATION

#endif // NOTOSANS_REGULAR12PT8B_H

