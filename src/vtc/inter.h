#ifndef VIRMAT_INTER_H
#define VIRMAT_INTER_H
#include "gc.h"

struct vt_VirMatCore *vtc;

/*
 * global的生成
 * 执行.vtcc时, 所有的LinkValue其father设置为null
 * 然后调用object生成对象global
 * 再修改所有的LinkValue的值为global
 *
 * 在初始化时调用API makeLinkValue允许father为null
 */

struct vt_VirMatCore {
    /* 量gc机制 */
    SET_GC_BASE(Value);
    SET_GC_BASE(LinkValue);
    SET_GC_BASE(VarSpace);

    /* 初始化 */
    bool init_mode;  // 初始化模式, 退出初始化模式后则不可再进入
    struct vt_VarSpace *protect;  // 保护空间

    /* 变量空间 */
    struct vt_VarSpace *activity;  // 活动变量空间链
    struct vt_FreezeVarSpace *freeze;  // 冻结变量空间链

    /* 顶级量和空值 */
    struct vt_LinkValue *object;  // 量object
    struct vt_LinkValue *global;  // 量global
    struct vt_LinkValue *null_model;  // 空值
};

#endif //VIRMAT_INTER_H
