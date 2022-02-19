/**
 * @file shell_cmd_group.c
 * @author Letter(nevermindzzt@gmail.com)
 * @brief shell command group support
 * @version 0.1
 * @date 2020-10-18
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "shell_cmd_group.h"
#include "string.h"
#include "stdio.h"

unsigned int shellRunCommand(Shell *shell, ShellCommand *command);

static void shellCmdGroupHelp(Shell *shell, char *name, ShellCommand *group);

/**
 * @brief shell锻戒护缁勮繍琛? * 
 * @param group 锻戒护鏁扮粍
 * @param argc 鍙傛暟鏁伴噺
 * @param argv 鍙傛暟
 * 
 * @return unsigned int 镓ц锻戒护杩斿洖链? */
unsigned int shellCmdGroupRun(ShellCommand *group, int argc, char *argv[])
{
    ShellCommand *item = group;
    Shell *shell = shellGetCurrent();

    if (argc < 2 || !shell)
    {
        return -1;
    }
    if (strcmp("-h", argv[1]) == 0)
    {
        shellCmdGroupHelp(shell, argv[0], group);
        return 0;
    }
    while (item->data.cmd.name)
    {
        if (strcmp(item->data.cmd.name, argv[1]) == 0)
        {
            for (short i = 0; i < argc - 1; i++)
            {
                argv[i] = argv[i + 1];
            }
            shell->parser.paramCount--;
            return shellRunCommand(shell, item);
            break;
        }
        item++;
    }
    shellWriteString(shell, "sub command not found\r\n");
    return -1;
}

/**
 * @brief shell 锻戒护缁勫府锷? * 
 * @param shell shell瀵硅薄
 * @param name 锻戒护缁勫悕
 * @param group 锻戒护鏁扮粍
 */
static void shellCmdGroupHelp(Shell *shell, char *name, ShellCommand *group)
{
    ShellCommand *item = group;

    shellWriteString(shell, "command group help of ");
    shellWriteString(shell, name);
    shellWriteString(shell, "\r\n");

    while (item->data.cmd.name)
    {
        shellWriteString(shell, item->data.cmd.name);
        shellWriteString(shell, ": ");
        if (item->data.cmd.desc) {
            shellWriteString(shell, item->data.cmd.desc);
        }
        shellWriteString(shell, "\r\n");
        item++;
    }
}
