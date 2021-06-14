#ifndef VIRMAT_STATEMENT_H
#define VIRMAT_STATEMENT_H

typedef struct vt_Statement Statement, *pSt;
typedef struct vt_Parameter Parameter, *pPt;
typedef struct vt_Backtracking Backtracking;

struct vt_Statement {
    enum {
        stt_null = 0,
        stt_value,
        stt_var,
        stt_call,
        stt_func,
        stt_model
    } type;

    union {
        struct {
            char *model;
            char *data;
        } value;

        struct {
            enum {
                var_lim_pub = 0,  // 仅访问公开权限变量
                var_lim_pro,  // 仅访问非私有权限变量
                var_lim_pri,  // 访问任意变量
            } limited;
            char *var;
        } var;

        struct {
            enum {
                call_lim_c = 0,  // 只回调C函数
                call_lim_v,  // 只回调v函数
                call_not_lim,  // 无限制
            } limited;
            struct vt_Statement *func;
            bool is_protect_func;  // 是否为保护空间的函数
            struct vt_Parameter *ap;  // actual parameter 实参
        } call;

        struct {
            struct vt_Statement *func;  // 函数对象
            struct vt_Statement *body;  // 函数体
            struct vt_Parameter *fp;  // formal parameter 形参
        } func;

        struct {
            struct vt_Statement *func;  // 模板对象
            struct vt_Statement *body;  // 模板定义内容
            struct vt_Parameter *inherit;
        } model;
    };

    struct vt_Statement *next;
};

struct vt_Parameter {
    enum {
        pt_value = 0,  // 仅value
        pt_key,  // key-value形式
        pt_values,  // values形式, 即需要调用量API得到变长 Parameter(内容均为value)
        pt_keys,  // keys形式, 即需要调用量API得到变长 Parameter(内容均为key-value)
    } type;

    char *key;
    struct vt_Statement *value;
    struct vt_Parameter *next;
};

struct vt_Backtracking {  // 结果回溯
    fline line;  // 代码行号
    fpath file;  // 代码文件路径
    struct vt_Backtracking *next;
};

#endif //VIRMAT_STATEMENT_H
