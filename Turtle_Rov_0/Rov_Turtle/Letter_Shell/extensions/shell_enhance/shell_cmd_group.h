/**
 * @file shell_cmd_group.h
 * @author Letter(nevermindzzt@gmail.com)
 * @brief shell command group support
 * @version 0.1
 * @date 2020-10-18
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "shell.h"

/**
 * @brief shell 锻戒护缁勫嚱鏁板悕
 */
#define     SHELL_CMD_GROUP_FUNC_NAME(_group)   agency##_group

/**
 * @brief shell锻戒护缁勫嚱鏁板畾涔? * 
 * @param _group 锻戒护鏁扮粍
 */
#define     SHELL_CMD_GROUP_FUNC(_group) \
            void SHELL_CMD_GROUP_FUNC_NAME(_group)(int p1, int p2) \
            { shellCmdGroupRun(&_group, p1, p2); }


/**
 * @brief shell 锻戒护缁勫畾涔? * 
 * @param _attr 灞炴€? * @param _name 锻戒护缁勫悕
 * @param _group 锻戒护鏁扮粍
 * @param _desc 锻戒护缁勬弿杩? */
#define SHELL_EXPORT_CMD_GROUP(_attr, _name, _group, _desc) \
        SHELL_CMD_GROUP_FUNC(_group) \
        SHELL_EXPORT_CMD(_attr, _name, SHELL_CMD_GROUP_FUNC_NAME(_group), _desc)

/**
 * @brief shell 锻戒护缁刬tem瀹氢箟
 * 
 * @param _type 锻戒护绫诲瀷(SHELL_TYPE_CMD_MAIN or SHELL_TYPE_CMD_FUNC)
 * @param _func 锻戒护鍑芥暟
 * @param _desc 锻戒护鎻忚堪
 */
#define SHELL_CMD_GROUP_ITEM(_type, _name, _func, _desc) \
        { \
            .attr.value = SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(_type)|SHELL_CMD_DISABLE_RETURN, \
            .data.cmd.name = #_name, \
            .data.cmd.function = (int (*)())_func, \
            .data.cmd.desc = #_desc \
        }

/**
 * @brief shell 锻戒护缁勫畾涔夌粨灏? * 
 * @note 闇€瑕佹坊锷犲湪姣忎釜锻戒护鏁扮粍镄勬渶鍚庝竴鏉? */
#define SHELL_CMD_GROUP_END()   {0}

unsigned int shellCmdGroupRun(ShellCommand *group, int argc, char *argv[]);
