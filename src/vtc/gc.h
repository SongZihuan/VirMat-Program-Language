#ifndef VIRMAT_GC_H
#define VIRMAT_GC_H

typedef unsigned long long GcCount;

#define GC_CHAIN(type) struct type *next; struct type *previous /* 定义双向链表 */
#define SET_GC(type) struct vt_Gc##type gc
#define SET_GC_BASE(type) struct vt_##type gc_##type
#define GET_GC_BASE(type) gc_##type

struct vt_GcStatus {
    GcCount *reference;  // 引用计数
    bool reachable;  // 可达标记
};

struct vt_GcValue {
    struct vt_GcStatus status;
    enum {
        gc_value_not_clean,  // 不清除
        gc_value_del,  // 需要执行析构
        gc_value_clean,  // 可清除
    } clean_status;
    bool have_del;  // 已经析构
    GC_CHAIN(vt_Value);
};

struct vt_GcLinkValue {
    struct vt_GcStatus status;
    GC_CHAIN(vt_LinkValue);
};

struct vt_GcVarSpace {
    struct vt_GcStatus status;
    GC_CHAIN(vt_VarSpace);
};

#endif //VIRMAT_GC_H
