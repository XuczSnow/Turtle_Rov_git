/**
 * @file log.h
 * @author Letter (nevermindzzt@gmail.com)
 * @brief log
 * @version 1.0.0
 * @date 2020-07-30
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#ifndef __LOG_H__
#define __LOG_H__

#include "shell.h"

#define     LOG_VERSION        "1.0.1"

#define     SHELL_COMPANION_ID_LOG          -2

#define     LOG_BUFFER_SIZE    256              /**< log杈揿嚭缂揿啿澶у皬 */
#define     LOG_USING_COLOR    1                /**< 鏄惁浣跨敤棰滆壊 */
#define     LOG_MAX_NUMBER     5                /**< 鍏佽娉ㄥ唽镄勬渶澶og瀵硅薄鏁伴噺 */
#define     LOG_AUTO_TAG       1                /**< 鏄惁镊姩娣诲姞TAG */
#define     LOG_END            "\r\n"           /**< log淇℃伅缁揿熬 */
#define     LOG_TAG            __FUNCTION__     /**< 镊畾娣诲姞镄凾AG */
#define     LOG_TIME_STAMP     0                /**< 璁剧疆銮峰彇绯荤粺镞堕棿鎴?*/

#ifndef     LOG_ENABLE
    #define LOG_ENABLE         1                /**< 浣胯兘log */
#endif

#define     LOG_ALL_OBJ        ((Log *)-1)      /**< 镓€链夊凡娉ㄥ唽镄刲og瀵硅薄 */

/**
 * 缁堢瀛椾綋棰滆壊浠ｇ爜
 */
#define     CSI_BLACK           30              /**< 榛戣壊 */
#define     CSI_RED             31              /**< 绾㈣壊 */
#define     CSI_GREEN           32              /**< 缁胯壊 */
#define     CSI_YELLOW          33              /**< 榛勮壊 */
#define     CSI_BLUE            34              /**< 钃濊壊 */
#define     CSI_FUCHSIN         35              /**< 鍝佺孩 */
#define     CSI_CYAN            36              /**< 闱掕壊 */
#define     CSI_WHITE           37              /**< 鐧借壊 */
#define     CSI_BLACK_L         90              /**< 浜粦 */
#define     CSI_RED_L           91              /**< 浜孩 */
#define     CSI_GREEN_L         92              /**< 浜豢 */
#define     CSI_YELLOW_L        93              /**< 浜粍 */
#define     CSI_BLUE_L          94              /**< 浜掴 */
#define     CSI_FUCHSIN_L       95              /**< 浜搧绾?*/
#define     CSI_CYAN_L          96              /**< 浜潚 */
#define     CSI_WHITE_L         97              /**< 浜槠 */
#define     CSI_DEFAULT         39              /**< 榛樿 */

#define     CSI(code)           "\033["#code"m" /**< ANSI CSI鎸囦护 */

/**
 * log绾у埆瀛楃(鍖呭惈棰滆壊)
 */
#if LOG_USING_COLOR == 1
#define     ERROR_TEXT          CSI(31)"E(%d) %s:"CSI(39)       /**< 阌栾镙囩 */
#define     WARNING_TEXT        CSI(33)"W(%d) %s:"CSI(39)       /**< 璀﹀憡镙囩 */
#define     INFO_TEXT           CSI(32)"I(%d) %s:"CSI(39)       /**< 淇℃伅镙囩 */
#define     DEBUG_TEXT          CSI(34)"D(%d) %s:"CSI(39)       /**< 璋冭瘯镙囩 */
#define     VERBOSE_TEXT        CSI(36)"V(%d) %s:"CSI(39)       /**< 鍐椾綑淇℃伅镙囩 */
#else
#define     ERROR_TEXT          "E(%d) %s:"
#define     WARNING_TEXT        "W(%d) %s:"
#define     INFO_TEXT           "I(%d) %s:"
#define     DEBUG_TEXT          "D(%d) %s:"
#define     VERBOSE_TEXT        "V(%d) %s:"
#endif


/**
 * @brief 镞ュ织绾у埆瀹氢箟
 * 
 */
typedef enum
{
    LOG_NONE = 0,                                  /**< 镞犵骇鍒?*/
    LOG_ERROR = 1,                                 /**< 阌栾 */
    LOG_WRANING = 2,                               /**< 璀﹀憡 */
    LOG_INFO = 3,                                  /**< 娑堟伅 */
    LOG_DEBUG = 4,                                 /**< 璋冭瘯 */
    LOG_VERBOSE = 5,                               /**< 鍐椾綑 */
    LOG_ALL = 6,                                   /**< 镓€链夋棩蹇?*/
} LogLevel;


/**
 * @brief log瀵硅薄瀹氢箟
 * 
 */
typedef struct
{
    void (*write)(char *, short);                   /**< 鍐檅uffer */
    char active;                                    /**< 鏄惁婵€娲?*/
    LogLevel level;                                 /**< 镞ュ织绾у埆 */
    Shell *shell;                                   /**< 鍏宠仈shell瀵硅薄 */
} Log;



/**
 * @brief log镓揿嵃(镊姩鎹㈣)
 * 
 * @param fmt 镙煎纺
 * @param ... 鍙傛暟
 */
#define logPrintln(format, ...) \
        logWrite(LOG_ALL_OBJ, LOG_NONE, format"\r\n", ##__VA_ARGS__)


/**
 * @brief 镞ュ织镙煎纺鍖栬緭鍑? * 
 * @param text 娑堟伅鏂囨湰
 * @param level 镞ュ织绾у埆
 * @param fmt 镙煎纺
 * @param ... 鍙傛暟
 */
#define logFormat(text, level, fmt, ...) \
        if (LOG_ENABLE) {\
            logWrite(LOG_ALL_OBJ, level, text" "fmt""LOG_END, \
                LOG_TIME_STAMP, LOG_TAG, ##__VA_ARGS__); }

/**
 * @brief 阌栾log杈揿嚭
 * 
 * @param fmt 镙煎纺
 * @param ... 鍙傛暟
 */
#define logError(fmt, ...) \
        logFormat(ERROR_TEXT, LOG_ERROR, fmt, ##__VA_ARGS__)

/**
 * @brief 璀﹀憡log杈揿嚭
 * 
 * @param fmt 镙煎纺
 * @param ... 鍙傛暟
 */
#define logWarning(fmt, ...) \
        logFormat(WARNING_TEXT, LOG_WRANING, fmt, ##__VA_ARGS__)

/**
 * @brief 淇℃伅log杈揿嚭
 * 
 * @param fmt 镙煎纺
 * @param ... 鍙傛暟
 */
#define logInfo(fmt, ...) \
        logFormat(INFO_TEXT, LOG_INFO, fmt, ##__VA_ARGS__)

/**
 * @brief 璋冭瘯log杈揿嚭
 * 
 * @param fmt 镙煎纺
 * @param ... 鍙傛暟
 */
#define logDebug(fmt, ...) \
        logFormat(DEBUG_TEXT, LOG_DEBUG, fmt, ##__VA_ARGS__)

/**
 * @brief 鍐椾綑log杈揿嚭
 * 
 * @param fmt 镙煎纺
 * @param ... 鍙傛暟
 */
#define logVerbose(fmt, ...) \
        logFormat(VERBOSE_TEXT, LOG_VERBOSE, fmt, ##__VA_ARGS__)

/**
 * @brief 鏂█
 * 
 * @param expr 琛ㄨ揪寮? * @param action 鏂█澶辫触鎿崭綔
 */
#define logAssert(expr, action) \
        if (!(expr)) { \
            logError("\"" #expr "\" assert failed at file: %s, line: %d", __FILE__, __LINE__); \
            action; \
        }

/**
 * @brief 16杩涘埗杈揿嚭鍒版墍链夌粓绔? * 
 * @param base 鍐呭瓨鍩哄潃
 * @param length 闀垮害
 */
#define logHexDumpAll(base, length) \
        logHexDump(LOG_ALL_OBJ, LOG_ALL, base, length)

void logRegister(Log *log, Shell *shell);
void logUnRegister(Log *log);
void logSetLevel(Log *log, LogLevel level);
void logWrite(Log *log, LogLevel level, char *fmt, ...);
void logHexDump(Log *log, LogLevel level, void *base, unsigned int length);

#endif
