#ifndef VIRMAT_PARSER_H
#define VIRMAT_PARSER_H
#include <regex.h>

#define SYNTACTIC_HASHTABLE_SIZE (8)
#define SYNTACTIC_VAR_HASHTABLE_SIZE (8)

#define PRETREATMENT_MASK ('#')  /* 预处理标识符 字符 */

struct vt_Token {
    enum {
        token_st = 0,
        token_str,
    } data_type;

    char *type;  // 类型标识符
    union {
        char *str;
        struct vt_Statement *st;
    };

    int enter_count;  // 该token前的换行符个数
    int space_count;  // 该token前的非空白符个数
    int other_count;  // json指定忽略的其他符号

    struct vt_Token *previous;
    struct vt_Token *next;
};

struct vt_TokenFlow {
    struct vt_Token *head;  // 头部 (词法分析器从此端处理token)
    struct vt_Token *tail;  // 尾部 (语法分析器从此端处理token)
    int len;  // 长度
};

struct vt_LexAction {  // 词法分析器: 匹配动作
    char *pattern;  // 正则表达式 字符串
    regex_t *reg;  // 正则表达式 结构体
    struct vt_LexAction *next;
};

struct vt_Lex {
    char *type;  // 生成的token type
    bool is_valid;  // 有效匹配器
    struct vt_LexAction *action;
    struct vt_Lex *next;

    // 匹配状态和信息
    enum {
        ls_normal,  // 正常情况
        ls_finished,  // 匹配完成
        ls_fail,  // 匹配失败
    } status;
    struct vt_LexAction *n_act;  // 当前执行的动作
    int index;  // 匹配到的单词 vt_CodeFile中addUpChar的索引, 表示匹配内容为addUpChar[0-index]
};

struct vt_SyntacticAction {
    enum {
        // 简单指令
        sat_getStrToken,  // 获得一个文本token
        sat_getCodeToken,  // 获得指定type的token
        sat_getNextToken,  // 调用下一调用对象获得token
        sat_arg,  // 生成参数
        sat_st,  // 生成st
        sat_stop,  // 停止(失败)
        sat_push,  // 压入token
        sat_return,  // 返回(成功)
        sat_error,  // 错误
        sat_del,  // 删除变量

        // 复合指令
        sat_exist_stop,  // 变量不存在则执行stop
        sat_exist_error,  // 变量不存在则执行error
        sat_token_check,  // 检查文本token
        sat_operator,  // 表达式匹配
        sat_postfix_opt,  // 后缀表达式匹配
        sat_code_block,  // 语法块匹配

        // 分支指令
        sat_if,  // 条件分支
        sat_while,  // 循环分支
    } type;

    struct {  // 简单指令(复合指令)的参数: 字符串 \ 分支指令exp表达式参数
        int argc;
        char **argv;
    };

    /* type为sat_if时_do和_else均可设置  type为sat_while仅可设置_do和_else其中一者 */
    struct vt_SyntacticAction *_do;  //分支指令 exp 成立时执行
    struct vt_SyntacticAction *_else;  // 分支指令 exp 不成立执行

    struct vt_SyntacticAction *next;
};

struct vt_Syntactic {
    char *type;
    struct vt_SyntacticAction *action;  // 动作链
    struct vt_Syntactic *next;  // 哈希表中的链表, 平行语法匹配器会放在一起
    struct vt_Syntactic *to;  // 呼叫链 下一呼叫对象
};

struct vt_SyntacticHashTable {
    int count;  // 个数
    struct vt_Syntactic (*syn)[SYNTACTIC_HASHTABLE_SIZE];  // 根据匹配器的type, 将匹配器记录在哈希表中
};

struct vt_SyntacticVar {  // 语法匹配器执行时的变量
    char *name;
    struct vt_Token *associate;  // 关联token (可无关联)
    enum {
        svt_str,
        svt_st,
        svt_pt
    } type;
    union {
        char *str;
        struct vt_Statement *st;
        struct vt_Parameter *pt;
    };

    struct vt_SyntacticVar *next;
};

struct vt_SyntacticRunner {
    /* 变量信息 */
    int count;
    struct vt_SyntacticVar (*var)[SYNTACTIC_VAR_HASHTABLE_SIZE];

    /* Token流信息 */
    struct vt_Token *tk;  // 记录所有读入的token
};

struct vt_CodeFile {
    /* 文件相关信息 */
    enum {
        cft_file,
        cft_str,
    } type;  // 文件类型

    union {
        struct {
            FILE *file;
            char *mode;  // 文件读取方式
            char *back;  // 回退区, 每次匹配完成回退多余内容时都回退到此处
            char *back_index;  // 回退区索引  每读取一个字符则指针+1
        };

        struct {
            char *str;  // 字符串
            char *index;  // 索引  每读取一个字符则指针+1
        };
    };

    char *longestSequence;  // 最长字符序列
    char *addUpChar;  // 累计字符

    fline line;  // 行号
    fpath path;  // 文件路径

    /* 指定忽略字符 */
    char *ignore;  // 指定需要忽略的字符
    int enter_count;  // 忽略的换行符个数（每次生成Token都会自动清0, 下同）
    int space_count;  // 忽略的非空白符个数
    int other_count;  // json指定忽略的其他符号

    /* 预处理 */
    bool start_of_line;  // 是否为行首
    int mask_index;  // 匹配指针, 匹配行首的内容是否为char *mask指定

    /* 语法分析 & 词法分析 */
    struct vt_Lex *lex;  // 词法分析器
    struct vt_SyntacticHashTable *syn;  // 语法分析器
};

#endif //VIRMAT_PARSER_H
