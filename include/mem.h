/*
 * 文件名: mem.h
 * 目标: vtc内内存管理的头文件
 */

#ifndef VIRMAT_MEM_H
#define VIRMAT_MEM_H

#include "stdlib.h"

// 开关
#define BUILD_VTMEM 1

// 默认情况
#define safeCalloc(n, size) (calloc((n), (size)))
#define safeFree(p) ((((p)!=NULL) ? free(p) : NULL), (p)=NULL)
#define print_memInfo() NULL
#define safeFree_ free

#if BUILD_VTMEM
#undef safeCalloc

void *safeCalloc(size_t n, size_t size);

#if DEBUG_VTMEM
#undef safeFree
#undef safeFree_
#undef print_memInfo

void safeFreeDebug(void *p);
int print_memInfo();

#define safeFree safeFreeDebug
#define safeFree_ safeFreeDebug
#else

#undef safeFree_
void safeFree_(void *p);

#endif  // DEBUG_VTMEM
#elif DEBUG_VTMEM
#error "The option \"debug vtmem\" can be turned on only when the option \"build vtmem\" is turned on"
#endif  // BUILD_VTMEM

#endif  // VIRMAT_MEM_H