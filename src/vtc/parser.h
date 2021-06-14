#ifndef VIRMAT_PARSER_H
#define VIRMAT_PARSER_H

#define SYNTACTIC_HASHTABLE_SIZE (8)
#define SYNTACTIC_VAR_HASHTABLE_SIZE (8)

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
    enum {
        la_exact = 0,  // 完全匹配
        le_long,  // 变长匹配
    } type;

    union {
        char *prefix;  // 完全匹配: 排除前缀
        struct {  // 变长匹配:
            int size[2];  // 长度 第一个元素表示最短长度、第二个元素表示最长长度(0表示无限长)
            bool is_exclude;  // 反向匹配(排除匹配)
        };
    };

    union {
        struct {  // 匹配字符串
            char *str;
            int index;
        };

        bool (*func)(char);  // 由vtc指定检查函数
    };

    struct vt_LexAction *next;
};

struct vt_Lex {
    char *type;  // 生成的token type
    bool is_valid;  // 有效匹配器
    int priority;  // 优先级
    struct vt_LexAction *action;
    struct vt_Lex *next;
};

struct vt_LexPriority {  // 同一个优先级的所有 Lexer
    int priority;  // 优先级
    struct vt_Lex *lexer;
    struct vt_LexPriority *next;
};

struct vt_SyntacticAction {
    enum {
        // 简单指令
        sat_getStrToken,
        sat_getCodeToken,
        sat_arg,
        sat_st,
        sat_stop,
        sat_push,
        sat_return,
        sat_error,
        sat_del,

        // 复合指令
        sat_exist_stop,
        sat_exist_error,
        sat_token_check,

        // 分支指令
        sat_if,
        sat_while,
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
    struct vt_Syntactic *next;
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
        };

        struct {
            char *str;  // 字符串
            int index;  // 索引
        };
    };

    fline line;  // 行号
    fpath path;  // 文件路径

    /* 读取回退 */
    bool is_back;  // 是否有回退
    char back;  // 回退字符 (只能回退一个)

    /* 指定忽略字符 */
    char *ignore;  // 指定需要忽略的字符
    int enter_count;  // 忽略的换行符个数（每次生成Token都会自动清0, 下同）
    int space_count;  // 忽略的非空白符个数
    int other_count;  // json指定忽略的其他符号

    /* 预处理 */
    char *mask;  // 预处理标识符
    bool start_of_line;  // 是否为行首
    int mask_index;  // 匹配指针, 匹配行首的内容是否为char *mask指定

    /* 语法分析 & 词法分析 */
    struct vt_LexPriority *lex;  // 词法分析器
    struct vt_SyntacticHashTable *syn;  // 语法分析器
};

#endif //VIRMAT_PARSER_H
