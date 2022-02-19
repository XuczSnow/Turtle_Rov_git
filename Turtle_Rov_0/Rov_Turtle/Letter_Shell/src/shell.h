/**
 * @file shell.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief letter shell
 * @version 3.0.0
 * @date 2019-12-30
 * 
 * @copyright (c) 2020 Letter
 * 
 */

#ifndef     __SHELL_H__
#define     __SHELL_H__

#include "shell_cfg.h"

#define     SHELL_VERSION               "3.1.0"                 /**< 鐗堟湰鍙?*/


/**
 * @brief shell 鏂█
 * 
 * @param expr 琛ㄨ揪寮? * @param action 鏂█澶辫触鎿崭綔
 */
#define     SHELL_ASSERT(expr, action) \
            if (!(expr)) { \
                action; \
            }

#if SHELL_USING_LOCK == 1
#define     SHELL_LOCK(shell)           shell->lock(shell)
#define     SHELL_UNLOCK(shell)         shell->unlock(shell)
#else
#define     SHELL_LOCK(shell)
#define     SHELL_UNLOCK(shell)
#endif /** SHELL_USING_LOCK == 1 */
/**
 * @brief shell 锻戒护鏉冮檺
 * 
 * @param permission 鏉冮檺绾у埆
 */
#define     SHELL_CMD_PERMISSION(permission) \
            (permission & 0x000000FF)

/**
 * @brief shell 锻戒护绫诲瀷
 * 
 * @param type 绫诲瀷
 */
#define     SHELL_CMD_TYPE(type) \
            ((type & 0x0000000F) << 8)

/**
 * @brief 浣胯兘锻戒护鍦ㄦ湭镙￠獙瀵嗙爜镄勬儏鍐典笅浣跨敤
 */
#define     SHELL_CMD_ENABLE_UNCHECKED \
            (1 << 12)

/**
 * @brief 绂佺敤杩斿洖链兼墦鍗? */
#define     SHELL_CMD_DISABLE_RETURN \
            (1 << 13)

/**
 * @brief 鍙灞炴€?浠呭鍙橀噺鐢熸晥)
 */
#define     SHELL_CMD_READ_ONLY \
            (1 << 14)

/**
 * @brief 锻戒护鍙傛暟鏁伴噺
 */
#define     SHELL_CMD_PARAM_NUM(num) \
            ((num & 0x0000000F)) << 16

#ifndef SHELL_SECTION
    #if defined(__CC_ARM) || defined(__CLANG_ARM)
        #define SHELL_SECTION(x)                __attribute__((section(x)))
    #elif defined (__IAR_SYSTEMS_ICC__)
        #define SHELL_SECTION(x)                @ x
    #elif defined(__GNUC__)
        #define SHELL_SECTION(x)                __attribute__((section(x)))
    #else
        #define SHELL_SECTION(x)
    #endif
#endif

#ifndef SHELL_USED
    #if defined(__CC_ARM) || defined(__CLANG_ARM)
        #define SHELL_USED                      __attribute__((used))
    #elif defined (__IAR_SYSTEMS_ICC__)
        #define SHELL_USED                      __root
    #elif defined(__GNUC__)
        #define SHELL_USED                      __attribute__((used))
    #else
        #define SHELL_USED
    #endif
#endif

/**
 * @brief shell float鍨嫔弬鏁拌浆鎹? */
#define     SHELL_PARAM_FLOAT(x)            (*(float *)(&x))

/**
 * @brief shell 浠ｇ悊鍑芥暟鍚? */
#define     SHELL_AGENCY_FUNC_NAME(_func)   agency##_func

/**
 * @brief shell浠ｇ悊鍑芥暟瀹氢箟
 * 
 * @param _func 琚唬鐞嗙殑鍑芥暟
 * @param ... 浠ｇ悊鍙傛暟
 */
#define     SHELL_AGENCY_FUNC(_func, ...) \
            void SHELL_AGENCY_FUNC_NAME(_func)(int p1, int p2, int p3, int p4, int p5, int p6, int p7) \
            { _func(__VA_ARGS__); }

#if SHELL_USING_CMD_EXPORT == 1

    /**
     * @brief shell 锻戒护瀹氢箟
     * 
     * @param _attr 锻戒护灞炴€?     * @param _name 锻戒护鍚?     * @param _func 锻戒护鍑芥暟
     * @param _desc 锻戒护鎻忚堪
     */
    #define SHELL_EXPORT_CMD(_attr, _name, _func, _desc) \
            const char shellCmd##_name[] = #_name; \
            const char shellDesc##_name[] = #_desc; \
            SHELL_USED const ShellCommand \
            shellCommand##_name SHELL_SECTION("shellCommand") =  \
            { \
                .attr.value = _attr, \
                .data.cmd.name = shellCmd##_name, \
                .data.cmd.function = (int (*)())_func, \
                .data.cmd.desc = shellDesc##_name \
            }

    /**
     * @brief shell 浠ｇ悊锻戒护瀹氢箟
     * 
     * @param _attr 锻戒护灞炴€?     * @param _name 锻戒护鍚?     * @param _func 锻戒护鍑芥暟
     * @param _desc 锻戒护鎻忚堪
     * @param ... 浠ｇ悊鍙傛暟
     */
    #define SHELL_EXPORT_CMD_AGENCY(_attr, _name, _func, _desc, ...) \
            SHELL_AGENCY_FUNC(_func, ##__VA_ARGS__) \
            SHELL_EXPORT_CMD(_attr, _name, SHELL_AGENCY_FUNC_NAME(_func), _desc)

    /**
     * @brief shell 鍙橀噺瀹氢箟
     * 
     * @param _attr 鍙橀噺灞炴€?     * @param _name 鍙橀噺鍚?     * @param _value 鍙橀噺链?     * @param _desc 鍙橀噺鎻忚堪
     */
    #define SHELL_EXPORT_VAR(_attr, _name, _value, _desc) \
            const char shellCmd##_name[] = #_name; \
            const char shellDesc##_name[] = #_desc; \
            SHELL_USED const ShellCommand \
            shellVar##_name SHELL_SECTION("shellCommand") =  \
            { \
                .attr.value = _attr, \
                .data.var.name = shellCmd##_name, \
                .data.var.value = (void *)_value, \
                .data.var.desc = shellDesc##_name \
            }

    /**
     * @brief shell 鐢ㄦ埛瀹氢箟
     * 
     * @param _attr 鐢ㄦ埛灞炴€?     * @param _name 鐢ㄦ埛鍚?     * @param _password 鐢ㄦ埛瀵嗙爜
     * @param _desc 鐢ㄦ埛鎻忚堪
     */
    #define SHELL_EXPORT_USER(_attr, _name, _password, _desc) \
            const char shellCmd##_name[] = #_name; \
            const char shellPassword##_name[] = #_password; \
            const char shellDesc##_name[] = #_desc; \
            SHELL_USED const ShellCommand \
            shellUser##_name SHELL_SECTION("shellCommand") =  \
            { \
                .attr.value = _attr|SHELL_CMD_TYPE(SHELL_TYPE_USER), \
                .data.user.name = shellCmd##_name, \
                .data.user.password = shellPassword##_name, \
                .data.user.desc = shellDesc##_name \
            }

    /**
     * @brief shell 鎸夐敭瀹氢箟
     * 
     * @param _attr 鎸夐敭灞炴€?     * @param _value 鎸夐敭阌€?     * @param _func 鎸夐敭鍑芥暟
     * @param _desc 鎸夐敭鎻忚堪
     */
    #define SHELL_EXPORT_KEY(_attr, _value, _func, _desc) \
            const char shellDesc##_value[] = #_desc; \
            SHELL_USED const ShellCommand \
            shellKey##_value SHELL_SECTION("shellCommand") =  \
            { \
                .attr.value = _attr|SHELL_CMD_TYPE(SHELL_TYPE_KEY), \
                .data.key.value = _value, \
                .data.key.function = (void (*)(Shell *))_func, \
                .data.key.desc = shellDesc##_value \
            }

    /**
     * @brief shell 浠ｇ悊鎸夐敭瀹氢箟
     * 
     * @param _attr 鎸夐敭灞炴€?     * @param _value 鎸夐敭阌€?     * @param _func 鎸夐敭鍑芥暟
     * @param _desc 鎸夐敭鎻忚堪
     * @param ... 浠ｇ悊鍙傛暟
     */
    #define SHELL_EXPORT_KEY_AGENCY(_attr, _value, _func, _desc, ...) \
            SHELL_AGENCY_FUNC(_func, ##__VA_ARGS__) \
            SHELL_EXPORT_KEY(_attr, _value, SHELL_AGENCY_FUNC_NAME(_func), _desc)
#else
    /**
     * @brief shell 锻戒护item瀹氢箟
     * 
     * @param _attr 锻戒护灞炴€?     * @param _name 锻戒护鍚?     * @param _func 锻戒护鍑芥暟
     * @param _desc 锻戒护鎻忚堪
     */
    #define SHELL_CMD_ITEM(_attr, _name, _func, _desc) \
            { \
                .attr.value = _attr, \
                .data.cmd.name = #_name, \
                .data.cmd.function = (int (*)())_func, \
                .data.cmd.desc = #_desc \
            }

    /**
     * @brief shell 鍙橀噺item瀹氢箟
     * 
     * @param _attr 鍙橀噺灞炴€?     * @param _name 鍙橀噺鍚?     * @param _value 鍙橀噺链?     * @param _desc 鍙橀噺鎻忚堪
     */
    #define SHELL_VAR_ITEM(_attr, _name, _value, _desc) \
            { \
                .attr.value = _attr, \
                .data.var.name = #_name, \
                .data.var.value = (void *)_value, \
                .data.var.desc = #_desc \
            }

    /**
     * @brief shell 鐢ㄦ埛item瀹氢箟
     * 
     * @param _attr 鐢ㄦ埛灞炴€?     * @param _name 鐢ㄦ埛鍚?     * @param _password 鐢ㄦ埛瀵嗙爜
     * @param _desc 鐢ㄦ埛鎻忚堪
     */
    #define SHELL_USER_ITEM(_attr, _name, _password, _desc) \
            { \
                .attr.value = _attr|SHELL_CMD_TYPE(SHELL_TYPE_USER), \
                .data.user.name = #_name, \
                .data.user.password = #_password, \
                .data.user.desc = #_desc \
            }

    /**
     * @brief shell 鎸夐敭item瀹氢箟
     * 
     * @param _attr 鎸夐敭灞炴€?     * @param _value 鎸夐敭阌€?     * @param _func 鎸夐敭鍑芥暟
     * @param _desc 鎸夐敭鎻忚堪
     */
    #define SHELL_KEY_ITEM(_attr, _value, _func, _desc) \
            { \
                .attr.value = _attr|SHELL_CMD_TYPE(SHELL_TYPE_KEY), \
                .data.key.value = _value, \
                .data.key.function = (void (*)(Shell *))_func, \
                .data.key.desc = #_desc \
            }

    #define SHELL_EXPORT_CMD(_attr, _name, _func, _desc)
    #define SHELL_EXPORT_CMD_AGENCY(_attr, _name, _func, _desc, ...)
    #define SHELL_EXPORT_VAR(_attr, _name, _value, _desc)
    #define SHELL_EXPORT_USER(_attr, _name, _password, _desc)
    #define SHELL_EXPORT_KEY(_attr, _value, _func, _desc)
    #define SHELL_EXPORT_KEY_AGENCY(_attr, _name, _func, _desc, ...)
#endif /** SHELL_USING_CMD_EXPORT == 1 */

/**
 * @brief shell command绫诲瀷
 */
typedef enum
{
    SHELL_TYPE_CMD_MAIN = 0,                                    /**< main褰㈠纺锻戒护 */
    SHELL_TYPE_CMD_FUNC,                                        /**< C鍑芥暟褰㈠纺锻戒护 */
    SHELL_TYPE_VAR_INT,                                         /**< int鍨嫔彉閲?*/
    SHELL_TYPE_VAR_SHORT,                                       /**< short鍨嫔彉閲?*/
    SHELL_TYPE_VAR_CHAR,                                        /**< char鍨嫔彉閲?*/
    SHELL_TYPE_VAR_STRING,                                      /**< string鍨嫔彉閲?*/
    SHELL_TYPE_VAR_POINT,                                       /**< 鎸囬拡鍨嫔彉閲?*/
    SHELL_TYPE_VAR_NODE,                                        /**< 鑺傜偣鍙橀噺 */
    SHELL_TYPE_USER,                                            /**< 鐢ㄦ埛 */
    SHELL_TYPE_KEY,                                             /**< 鎸夐敭 */
} ShellCommandType;


/**
 * @brief Shell瀹氢箟
 */
typedef struct shell_def
{
    struct
    {
        const struct shell_command *user;                       /**< 褰揿墠鐢ㄦ埛 */
        int activeTime;                                         /**< shell婵€娲绘椂闂?*/
        char *path;                                             /**< 褰揿墠shell璺缎 */
    #if SHELL_USING_COMPANION == 1
        struct shell_companion_object *companions;              /**< 浼寸敓瀵硅薄 */
    #endif
    } info;
    struct
    {
        unsigned short length;                                  /**< 杈揿叆鏁版嵁闀垮害 */
        unsigned short cursor;                                  /**< 褰揿墠鍏夋爣浣岖疆 */
        char *buffer;                                           /**< 杈揿叆缂揿啿 */
        char *param[SHELL_PARAMETER_MAX_NUMBER];                /**< 鍙傛暟 */
        unsigned short bufferSize;                              /**< 杈揿叆缂揿啿澶у皬 */
        unsigned short paramCount;                              /**< 鍙傛暟鏁伴噺 */
        int keyValue;                                           /**< 杈揿叆鎸夐敭阌€?*/
    } parser;
    struct
    {
        char *item[SHELL_HISTORY_MAX_NUMBER];                   /**< 铡嗗彶璁板綍 */
        unsigned short number;                                  /**< 铡嗗彶璁板綍鏁?*/
        unsigned short record;                                  /**< 褰揿墠璁板綍浣岖疆 */
        signed short offset;                                    /**< 褰揿墠铡嗗彶璁板綍锅忕Щ */
    } history;
    struct
    {
        void *base;                                             /**< 锻戒护琛ㄥ熀鍧€ */
        unsigned short count;                                   /**< 锻戒护鏁伴噺 */
    } commandList;
    struct
    {
        unsigned char isChecked : 1;                            /**< 瀵嗙爜镙￠獙阃氲绷 */
        unsigned char isActive : 1;                             /**< 褰揿墠娲诲姩Shell */
        unsigned char tabFlag : 1;                              /**< tab镙囧织 */
    } status;
    signed short (*read)(char *, unsigned short);               /**< shell璇诲嚱鏁?*/
    signed short (*write)(char *, unsigned short);              /**< shell鍐椤嚱鏁?*/
#if SHELL_USING_LOCK == 1
    int (*lock)(struct shell_def *);                              /**< shell 锷犻挛 */
    int (*unlock)(struct shell_def *);                            /**< shell 瑙ｉ挛 */
#endif
} Shell;


/**
 * @brief shell command瀹氢箟
 */
typedef struct shell_command
{
    union
    {
        struct
        {
            unsigned char permission : 8;                       /**< command鏉冮檺 */
            ShellCommandType type : 4;                          /**< command绫诲瀷 */
            unsigned char enableUnchecked : 1;                  /**< 鍦ㄦ湭镙￠獙瀵嗙爜镄勬儏鍐典笅鍙敤 */
            unsigned char disableReturn : 1;                    /**< 绂佺敤杩斿洖链艰緭鍑?*/
            unsigned char  readOnly : 1;                        /**< 鍙 */
            unsigned char reserve : 1;                          /**< 淇濈暀 */
            unsigned char paramNum : 4;                         /**< 鍙傛暟鏁伴噺 */
        } attrs;
        int value;
    } attr;                                                     /**< 灞炴€?*/
    union
    {
        struct
        {
            const char *name;                                   /**< 锻戒护鍚?*/
            int (*function)();                                  /**< 锻戒护镓ц鍑芥暟 */
            const char *desc;                                   /**< 锻戒护鎻忚堪 */
        } cmd;                                                  /**< 锻戒护瀹氢箟 */
        struct
        {
            const char *name;                                   /**< 鍙橀噺鍚?*/
            void *value;                                        /**< 鍙橀噺链?*/
            const char *desc;                                   /**< 鍙橀噺鎻忚堪 */
        } var;                                                  /**< 鍙橀噺瀹氢箟 */
        struct
        {
            const char *name;                                   /**< 鐢ㄦ埛鍚?*/
            const char *password;                               /**< 鐢ㄦ埛瀵嗙爜 */
            const char *desc;                                   /**< 鐢ㄦ埛鎻忚堪 */
        } user;                                                 /**< 鐢ㄦ埛瀹氢箟 */
        struct
        {
            int value;                                          /**< 鎸夐敭阌€?*/
            void (*function)(Shell *);                          /**< 鎸夐敭镓ц鍑芥暟 */
            const char *desc;                                   /**< 鎸夐敭鎻忚堪 */
        } key;                                                  /**< 鎸夐敭瀹氢箟 */
    } data; 
} ShellCommand;

/**
 * @brief shell鑺傜偣鍙橀噺灞炴€? */
typedef struct
{
    void *var;                                                  /**< 鍙橀噺寮旷敤 */
    int (*get)();                                               /**< 鍙橀噺get鏂规硶 */
    int (*set)();                                               /**< 鍙橀噺set鏂规硶 */
} ShellNodeVarAttr;


#define shellSetPath(_shell, _path)     (_shell)->info.path = _path
#define shellGetPath(_shell)            ((_shell)->info.path)

void shellInit(Shell *shell, char *buffer, unsigned short size);
unsigned short shellWriteString(Shell *shell, const char *string);
void shellPrint(Shell *shell, char *fmt, ...);
void shellScan(Shell *shell, char *fmt, ...);
Shell* shellGetCurrent(void);
void shellHandler(Shell *shell, char data);
void shellWriteEndLine(Shell *shell, char *buffer, int len);
void shellTask(void *param);
int shellRun(Shell *shell, const char *cmd);



#if SHELL_USING_COMPANION == 1
/**
 * @brief shell浼寸敓瀵硅薄瀹氢箟
 */
typedef struct shell_companion_object
{
    int id;                                                     /**< 浼寸敓瀵硅薄ID */
    void *obj;                                                  /**< 浼寸敓瀵硅薄 */
    struct shell_companion_object *next;                        /**< 涓嬩竴涓即鐢熷璞?*/
} ShellCompanionObj;


signed char shellCompanionAdd(Shell *shell, int id, void *object);
signed char shellCompanionDel(Shell *shell, int id);
void *shellCompanionGet(Shell *shell, int id);
#endif

#endif


