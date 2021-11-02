#ifndef NG_UTILS_H__
#define NG_UTILS_H__

#include "nema_core.h"
#include "nema_utils.h"

#define SAFE_CAST(x, srcType, dstType) \
   (((union {srcType a; dstType b;}){.a=x}).b)

#ifndef CLAMP
    #define CLAMP(x, low, high) ( (x) < (low) ? (low) : ( (x) > (high) ? (high) : (x) ) )
#endif

void float2str(char *str, int size, float f, int precision);
void concatenate_strings(char *str1, char *str2);
int  round_up(int numToRound, int multiple);

static nema_buffer_t create_bo_from_pointer(const void *ptr, size_t size) {
    nema_buffer_t bo;

#ifdef UNIFIED_MEMORY
    bo.base_virt = (void *)ptr;
    bo.base_phys = (uintptr_t)ptr;
    bo.size      = size;
#else
    bo = nema_buffer_create(size);
    (void)nema_buffer_map(&bo);
    nema_memcpy(bo.base_virt, ptr, size);
#endif

    return bo;
}

#define NG_LOAD_ARRAY(array) create_bo_from_pointer((void *)(array), sizeof(array))

#endif //NG_UTILS_H_