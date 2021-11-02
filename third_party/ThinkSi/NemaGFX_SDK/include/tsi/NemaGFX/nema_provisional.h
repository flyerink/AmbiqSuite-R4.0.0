/*******************************************************************************
 * Copyright (c) 2019 Think Silicon S.A.
 *
   Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this header file and/or associated documentation files to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Materials, and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * MODIFICATIONS TO THIS FILE MAY MEAN IT NO LONGER ACCURATELY REFLECTS
 * NEMAGFX API. THE UNMODIFIED, NORMATIVE VERSIONS OF THINK-SILICON NEMAGFX
 * SPECIFICATIONS AND HEADER INFORMATION ARE LOCATED AT:
 *   https://think-silicon.com/products/software/nemagfx-api
 *
 *  The software is provided 'as is', without warranty of any kind, express or
 *  implied, including but not limited to the warranties of merchantability,
 *  fitness for a particular purpose and noninfringement. In no event shall
 *  Think Silicon S.A. be liable for any claim, damages or other liability, whether
 *  in an action of contract, tort or otherwise, arising from, out of or in
 *  connection with the software or the use or other dealings in the software.
 ******************************************************************************/


#ifndef NEMA_PROVISIONAL_H__
#define NEMA_PROVISIONAL_H__

#include "nema_sys_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Fill a triangle strip with color (float coordinates)
 *
 * \param vertices pointer to vertices coordinated (first x coordinate of vertex,
 *                 then y coordinate of vertex)
 * \param num_vertices number of vertices
 * \param stride Distance between two vertices
 * \param rgba8888 Color to be used
 *  *
 */
void nema_fill_triangle_strip_f(float* vertices, int num_vertices, int stride, uint32_t rgba8888);

/** \brief Fill a traingle fan with color (float coordinates)
 *
 * \param vertices pointer to vertices coordinated (first x coordinate of vertex,
 *                 then y coordinate of vertex)
 * \param num_vertices number of vertices
 * \param stride Distance between two vertices
 * \param rgba8888 Color to be used
 *  *
 */
void nema_fill_triangle_fan_f(float* vertices, int num_vertices, int stride, uint32_t rgba8888);


#ifdef __cplusplus
}
#endif

#endif // NEMA_PROVISIONAL_H__
