#ifndef VIRMAT_VIRMAT_H
#define VIRMAT_VIRMAT_H
#include "macro.h"
#include "mem.h"
#include "tool.h"
#include "cjson.h"

#define R_NORMAL (0)
#define R_RETURN (1)

typedef struct vt_Result Result;

struct vt_Result {
    int type;  // 标识符
    int count;
    struct vt_LinkValue *value;  // 量, 在私有头文件 value.h
    struct vt_Backtracking *bt;  // 回溯链, 在私有头文件 statement.h
};

int vtcInit(void);

#endif //VIRMAT_VIRMAT_H
