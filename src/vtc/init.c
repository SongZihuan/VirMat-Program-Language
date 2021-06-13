/*
 * VirMat 初始化函数
 */

#include <locale.h>
#include "virmat.h"

#include "statement.h"
#include "value.h"
#include "var.h"
#include "inter.h"

int vtcInit(void) {
    atexit(dlcExit);
    cJsonInit();
    if (setlocale(LC_ALL, "") == NULL)  // locale: "", 表示自动识别
        return 1;
    return 0;
}