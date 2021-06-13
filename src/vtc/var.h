#ifndef VIRMAT_VAR_H
#define VIRMAT_VAR_H
#include "gc.h"

#define VAR_HASHTABLE_SIZE (8)

typedef enum vt_VarAuthority VarAuthority;
typedef struct vt_Var Var;
typedef struct vt_VarInfo VarInfo;
typedef struct vt_VarSpace VarSpace;
typedef struct vt_FreezeVarSpace FreezeVarSpace;

enum vt_VarAuthority {
    var_pub = 0,  // public 公开
    var_pro,  // protect 保护
    var_pri,  // private 私有
};

struct vt_Var {
    enum vt_VarAuthority authority;
    char *var_name;
    struct vt_LinkValue *value;

    struct vt_Var *next;
};

struct vt_VarInfo {
    enum {
        vi_global = 0,  // 全局变量
        vi_nonlocal,  // 非本地变量
    } type;
    char *var_name;
    struct vt_VarSpace *var_space;  // 指定变量寻找的变量空间, 若为NULL则每次都动态寻找

    struct vt_VarInfo *next;
};

struct vt_VarSpace {
    bool is_attr;  // 是否为量的变量空间
    struct vt_Var (*var)[VAR_HASHTABLE_SIZE];

    struct vt_VarInfo *info;  // 用于变量空间为活动变量空间链顶层变量空间时，变量寻找定位
    struct vt_VarSpace *next;
    SET_GC(VarSpace);
};

struct vt_FreezeVarSpace {  // 记录冻结的变量空间链的链表
    struct vt_VarSpace *var_space;  // 冻结的变量空间链
    struct vt_FreezeVarSpace *next;
};

#endif //VIRMAT_VAR_H
