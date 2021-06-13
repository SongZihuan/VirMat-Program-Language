/*
 * VirMat 初始化函数
 */

#include <locale.h>
#include "virmat.h"

int vtcInit(void) {
    atexit(dlcExit);
    cJsonInit();
    if (setlocale(LC_ALL, "") == NULL)  // locale: "", 表示自动识别
        return 1;
    return 0;
}