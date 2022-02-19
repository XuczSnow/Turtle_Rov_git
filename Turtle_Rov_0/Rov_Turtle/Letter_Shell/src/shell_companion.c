/**
 * @file shell_companion.c
 * @author Letter (nevermindzzt@gmail.com)
 * @brief shell companion object support
 * @version 3.0.3
 * @date 2020-07-22
 * 
 * @copyright (c) 2020 Letter
 * 
 */
 #include "shell.h"
 
#if SHELL_USING_COMPANION == 1
/**
 * @brief shell娣诲姞浼寸敓瀵硅薄
 * 
 * @param shell shell瀵硅薄
 * @param id 浼寸敓瀵硅薄ID
 * @param object 浼寸敓瀵硅薄
 * @return signed char 0 娣诲姞鎴愬姛 -1 娣诲姞澶辫触
 */
signed char shellCompanionAdd(Shell *shell, int id, void *object)
{
    ShellCompanionObj *companions = shell->info.companions;
    ShellCompanionObj *node = SHELL_MALLOC(sizeof(ShellCompanionObj));
    SHELL_ASSERT(node, return -1);
    node->id = id;
    node->obj = object;
    node->next = companions;
    shell->info.companions = node;
    return 0;
}

/**
 * @brief shell鍒犻櫎浼寸敓瀵硅薄
 * 
 * @param shell shell瀵硅薄
 * @param id 浼寸敓瀵硅薄ID
 * @return signed char 0 鍒犻櫎鎴愬姛 -1 镞犲尮閰嶅璞? */
signed char shellCompanionDel(Shell *shell, int id)
{
    ShellCompanionObj *companions = shell->info.companions;
    ShellCompanionObj *front = companions;
    while (companions)
    {
        if (companions->id == id)
        {
            if (companions == shell->info.companions && !(companions->next)) 
            {
                shell->info.companions = (void *)0;
            }
            else
            {
                front->next = companions->next;
            }
            SHELL_FREE(companions);
            return 0;
        }
        front = companions;
        companions = companions->next;
    }
    return -1;
}

/**
 * @brief shell銮峰彇浼寸敓瀵硅薄
 * 
 * @param shell shell瀵硅薄
 * @param id 浼寸敓瀵硅薄ID
 * @return void* 浼寸敓瀵硅薄锛屾棤鍖归厤瀵硅薄镞惰繑锲濶ULL
 */
void *shellCompanionGet(Shell *shell, int id)
{
    SHELL_ASSERT(shell, return (void *)0);
    ShellCompanionObj *companions = shell->info.companions;
    while (companions)
    {
        if (companions->id == id)
        {
            return companions->obj;
        }
        companions = companions->next;
    }
    return (void *)0;
}
#endif /** SHELL_USING_COMPANION == 1 */
