#ifndef VIRMAT_VALUE_H
#define VIRMAT_VALUE_H
#include "tool.h"
#include "gc.h"

#define API_HASHTABLE_SIZE (8)

typedef void pFuncC();
NEW_DLC_SYMBOL(pFuncC, pFuncC);

typedef void pValueAPI();
NEW_DLC_SYMBOL(pValueAPI, pAPIFUNC);

struct vt_LinkValue Value;  // 含从属关系的量
typedef struct vt_Value BaseValue;  // 量
typedef struct vt_Inherit Inherit;
typedef struct vt_ValueAPI ValueAPI;

struct vt_Value {
    char *id;  // 量的类型标识符
    void *data;  // 量的值
    struct vt_ValueAPI *API;  // 量的API哈希表
    struct vt_VarSpace *attr;  // 属性
    struct vt_Inherit *inherit;  // 继承链

    /* 模板属性 */
    bool is_model;  // 模板属性开关

    /* 函数属性 */
    bool is_func;  // 函数属性开关
    bool is_inline;  // 内联函数
    bool is_embed;   // 内嵌函数
    bool is_expand;  // 拓展函数
    bool is_quote;  // 引用函数

    enum {
        func_c,
        func_v,
    } func_type;

    union {
        struct {
            struct vt_Statement *body;
            struct vt_Parameter *fp;
            struct VarSpace *base;  // 回调基底
        } func_v;

        struct {
            int prototype;  // 函数原型序号 (可以选择不同的函数原型, 引用函数时有：传入匿名函数为参数、传入语法树为参数等不同原型)
            DLC_SYMBOL(pFuncC) func;  // C函数实际的函数指针
        } func_c;
    };

    SET_GC(Value);
};

struct vt_LinkValue {
    struct vt_LinkValue *father;
    struct vt_Value *value;
    SET_GC(LinkValue);
};

struct vt_Inherit {
    bool is_top;  // 顶级: object
    struct vt_LinkValue *model;
    struct vt_Inherit *next;
};

struct vt_ValueAPINode {
    char *api_name;  // api名字
    DLC_SYMBOL(pAPIFUNC) api;  // api函数
    struct vt_ValueAPINode *next;
};

struct vt_ValueAPI {
    int count;  // api个数记录
    struct vt_ValueAPINode (*node)[API_HASHTABLE_SIZE];
};

#endif //VIRMAT_VALUE_H
