/**
 * @file shell_cpp.h
 * @author Letter (nevermindzzt@gmail.com)
 * @brief shell cpp support
 * @version 1.0.0
 * @date 2021-01-09
 * 
 * @copyright (c) 2021 Letter
 * 
 */
#ifndef __SHELL_CPP_H__
#define __SHELL_CPP_H__

#ifdef __cplusplus
extern "C" {

#include "shell.h"

/**
 * @brief shell command cpp 鏀寔 cmd 瀹氢箟
 */
typedef struct shell_command_cpp_cmd
{
    int attr;                                                   /**< 灞炴€?*/
    const char *name;                                           /**< 锻戒护鍚?*/
    int (*function)();                                          /**< 锻戒护镓ц鍑芥暟 */
    const char *desc;                                           /**< 锻戒护鎻忚堪 */
} ShellCommandCppCmd;

/**
 * @brief shell command cpp 鏀寔 var 瀹氢箟
 */
typedef struct shell_command_cpp_var
{
    int attr;                                                   /**< 灞炴€?*/
    const char *name;                                           /**< 鍙橀噺鍚?*/
    void *value;                                                /**< 鍙橀噺链?*/
    const char *desc;                                           /**< 鍙橀噺鎻忚堪 */
} ShellCommandCppVar;

/**
 * @brief shell command cpp 鏀寔 user 瀹氢箟
 */
typedef struct shell_command_cpp_user
{
    int attr;                                                   /**< 灞炴€?*/
    const char *name;                                           /**< 鐢ㄦ埛鍚?*/
    const char *password;                                       /**< 鐢ㄦ埛瀵嗙爜 */
    const char *desc;                                           /**< 鐢ㄦ埛鎻忚堪 */
} ShellCommandCppUser;

/**
 * @brief shell command cpp 鏀寔 key 瀹氢箟
 */
typedef struct shell_command_cpp_key
{
    int attr;                                                   /**< 灞炴€?*/
    int value;                                                  /**< 鎸夐敭阌€?*/
    void (*function)(Shell *);                                  /**< 鎸夐敭镓ц鍑芥暟 */
    const char *desc;                                           /**< 鎸夐敭鎻忚堪 */
} ShellCommandCppKey;

#if SHELL_USING_CMD_EXPORT == 1

    #undef SHELL_EXPORT_CMD
    /**
     * @brief shell 锻戒护瀹氢箟
     * 
     * @param _attr 锻戒护灞炴€?     * @param _name 锻戒护鍚?     * @param _func 锻戒护鍑芥暟
     * @param _desc 锻戒护鎻忚堪
     */
    #define SHELL_EXPORT_CMD(_attr, _name, _func, _desc) \
            const char shellCmd##_name[] = #_name; \
            const char shellDesc##_name[] = #_desc; \
            extern "C" SHELL_USED const ShellCommandCppCmd \
            shellCommand##_name SHELL_SECTION("shellCommand") =  \
            { \
                _attr, \
                shellCmd##_name, \
                (int (*)())_func, \
                shellDesc##_name \
            }

    #undef SHELL_EXPORT_VAR
    /**
     * @brief shell 鍙橀噺瀹氢箟
     * 
     * @param _attr 鍙橀噺灞炴€?     * @param _name 鍙橀噺鍚?     * @param _value 鍙橀噺链?     * @param _desc 鍙橀噺鎻忚堪
     */
    #define SHELL_EXPORT_VAR(_attr, _name, _value, _desc) \
            const char shellCmd##_name[] = #_name; \
            const char shellDesc##_name[] = #_desc; \
            extern "C" SHELL_USED const ShellCommandCppVar \
            shellVar##_name SHELL_SECTION("shellCommand") =  \
            { \
                _attr, \
                shellCmd##_name, \
                (void *)_value, \
                shellDesc##_name \
            }

    #undef SHELL_EXPORT_USER
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
            extern "C" SHELL_USED const ShellCommandCppUser \
            shellUser##_name SHELL_SECTION("shellCommand") =  \
            { \
                _attr|SHELL_CMD_TYPE(SHELL_TYPE_USER), \
                shellCmd##_name, \
                shellPassword##_name, \
                shellDesc##_name \
            }

    #undef SHELL_EXPORT_KEY
    /**
     * @brief shell 鎸夐敭瀹氢箟
     * 
     * @param _attr 鎸夐敭灞炴€?     * @param _value 鎸夐敭阌€?     * @param _func 鎸夐敭鍑芥暟
     * @param _desc 鎸夐敭鎻忚堪
     */
    #define SHELL_EXPORT_KEY(_attr, _value, _func, _desc) \
            const char shellDesc##_value[] = #_desc; \
            extern "C" SHELL_USED const ShellCommandCppKey \
            shellKey##_value SHELL_SECTION("shellCommand") =  \
            { \
                _attr|SHELL_CMD_TYPE(SHELL_TYPE_KEY), \
                _value, \
                (void (*)(Shell *))_func, \
                shellDesc##_value \
            }
#endif /** SHELL_USING_CMD_EXPORT == 1 */

}
#endif /**< defined __cplusplus */

#endif /**< __SHELL_CPP_H__ */


