/*
 * 文件名: macro.h
 * 目标: 定义vtc中使用的公共宏
 */

#ifndef VIRMAT_MACRO_H_2
#define VIRMAT_MACRO_H_2
#include <stdbool.h>

#ifndef __bool_true_false_are_defined
#define bool int
#define true (1)
#define false (0)
#endif

#define NUL ((char)0)
#define W_NUL ((wchar_t)0)

typedef int fline;
typedef int Layer;
typedef char *fpath;

#endif //VIRMAT_MACRO_H_2