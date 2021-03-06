/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 */
#include "cc_pal_log.h"
#include "stdlib.h"

void CC_PalAbort(const char * exp)
{
    CC_PAL_LOG_ERR("ASSERT:%s:%d: %s", __FILE__, __LINE__, exp);
    CC_UNUSED_PARAM(exp); /* to avoid compilation error in case DEBUG isn't defined*/
    //abort();
    while (1);
}
