/**
 * @file shell_cfg.h
 * @author Letter (nevermindzzt@gmail.com)
 * @brief shell config
 * @version 3.0.0
 * @date 2019-12-31
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#ifndef __SHELL_CFG_H__
#define __SHELL_CFG_H__


/**
 * @brief 鏄惁浣跨敤榛樿shell浠诲姟while寰幆锛屼娇鑳藉畯`SHELL_USING_TASK`鍚庢瀹忔湁镒忎箟
 *        浣胯兘姝ゅ畯锛屽垯`shellTask()`鍑芥暟浼氢竴鐩村惊鐜鍙栬緭鍏ワ紝涓€鑸娇鐢ㄦ搷浣灭郴缁熷缓绔媠hell
 *        浠诲姟镞朵娇鑳芥瀹忥紝鍏抽棴姝ゅ畯镄勬儏鍐典笅锛屼竴鑸€傜敤浜庢棤鎿崭綔绯荤粺锛屽湪涓诲惊鐜腑璋幂敤`shellTask()`
 */
#define     SHELL_TASK_WHILE            0

/**
 * @brief 鏄惁浣跨敤锻戒护瀵煎嚭鏂瑰纺
 *        浣胯兘姝ゅ畯鍚庯紝鍙互浣跨敤`SHELL_EXPORT_CMD()`绛夊鍑哄懡浠? *        瀹氢箟shell锻戒护锛屽叧闂瀹忕殑鎯呭喌涓嬶紝闇€瑕佷娇鐢ㄥ懡浠よ〃镄勬柟寮? */
#define     SHELL_USING_CMD_EXPORT      1

/**
 * @brief 鏄惁浣跨敤shell浼寸敓瀵硅薄
 *        涓€浜涙墿灞旷殑缁勪欢(鏂囦欢绯荤粺鏀寔锛屾棩蹇楀伐鍏风瓑)闇€瑕佷娇鐢ㄤ即鐢熷璞? */
#define     SHELL_USING_COMPANION       0

/**
 * @brief 鏀寔shell灏捐妯″纺
 */
#define     SHELL_SUPPORT_END_LINE      0

/**
 * @brief 鏄惁鍦ㄨ緭鍑哄懡浠ゅ垪琛ㄤ腑鍒楀嚭鐢ㄦ埛
 */
#define     SHELL_HELP_LIST_USER        0

/**
 * @brief 鏄惁鍦ㄨ緭鍑哄懡浠ゅ垪琛ㄤ腑鍒楀嚭鍙橀噺
 */
#define     SHELL_HELP_LIST_VAR         0

/**
 * @brief 鏄惁鍦ㄨ緭鍑哄懡浠ゅ垪琛ㄤ腑鍒楀嚭鎸夐敭
 */
#define     SHELL_HELP_LIST_KEY         0

/**
 * @brief 鏄惁鍦ㄨ緭鍑哄懡浠ゅ垪琛ㄤ腑灞旷ず锻戒护鏉冮檺
 */
#define     SHELL_HELP_SHOW_PERMISSION  1

/**
 * @brief 浣跨敤LF浣滀负锻戒护琛屽洖杞﹁Е鍙? *        鍙互鍜孲HELL_ENTER_CR鍚屾椂寮€鍚? */
#define     SHELL_ENTER_LF              1

/**
 * @brief 浣跨敤CR浣滀负锻戒护琛屽洖杞﹁Е鍙? *        鍙互鍜孲HELL_ENTER_LF鍚屾椂寮€鍚? */
#define     SHELL_ENTER_CR              1

/**
 * @brief 浣跨敤CRLF浣滀负锻戒护琛屽洖杞﹁Е鍙? *        涓嶅彲浠ュ拰SHELL_ENTER_LF鎴朣HELL_ENTER_CR鍚屾椂寮€鍚? */
#define     SHELL_ENTER_CRLF            0

/**
 * @brief 浣跨敤镓ц链鍑哄嚱鏁扮殑锷熻兘
 *        鍚敤鍚庯紝鍙互阃氲绷`exec [addr] [args]`鐩存帴镓ц瀵瑰簲鍦板潃镄勫嚱鏁? * @attention 濡傛灉鍦板潃阌栾锛屽彲鑳戒细鐩存帴寮曡捣绋嫔簭宕╂簝
 */
#define     SHELL_EXEC_UNDEF_FUNC       0

/**
 * @brief shell锻戒护鍙傛暟链€澶ф暟閲? *        鍖呭惈锻戒护鍚嶅湪鍐咃紝瓒呰绷16涓弬鏁板苟涓斾娇鐢ㄤ简鍙傛暟镊姩杞崲镄勬儏鍐典笅锛岄渶瑕佷慨鏀规簮镰? */
#define     SHELL_PARAMETER_MAX_NUMBER  8

/**
 * @brief 铡嗗彶锻戒护璁板綍鏁伴噺
 */
#define     SHELL_HISTORY_MAX_NUMBER    5

/**
 * @brief 鍙屽向闂撮殧(ms)
 *        浣胯兘瀹廯SHELL_LONG_HELP`鍚庢瀹忕敓鏁堬紝瀹氢箟鍙屽向tab琛ュ叏help镄勬椂闂撮棿闅? */
#define     SHELL_DOUBLE_CLICK_TIME     200

/**
 * @brief 绠＄悊镄勬渶澶hell鏁伴噺
 */
#define     SHELL_MAX_NUMBER            5

/**
 * @brief shell镙煎纺鍖栬緭鍑虹殑缂揿啿澶у皬
 *        涓?镞朵笉浣跨敤shell镙煎纺鍖栬緭鍑? */
#define     SHELL_PRINT_BUFFER          128

/**
 * @brief shell镙煎纺鍖栬緭鍏ョ殑缂揿啿澶у皬
 *        涓?镞朵笉浣跨敤shell镙煎纺鍖栬緭鍏? * @note shell镙煎纺鍖栬緭鍏ヤ细阒诲shellTask, 浠呴€傜敤浜庡湪链夋搷浣灭郴缁熺殑鎯呭喌涓嬩娇鐢? */
#define     SHELL_SCAN_BUFFER          0

/**
 * @brief 銮峰彇绯荤粺镞堕棿(ms)
 *        瀹氢箟姝ゅ畯涓鸿幏鍙栫郴缁烼ick锛屽`HAL_GetTick()`
 * @note 姝ゅ畯涓嶅畾涔夋椂镞犳硶浣跨敤鍙屽向tab琛ュ叏锻戒护help锛屾棤娉曚娇鐢╯hell瓒呮椂阌佸畾
 */
#define     SHELL_GET_TICK()            0

/**
 * @brief 浣跨敤阌? * @note 浣跨敤shell阌佹椂锛岄渶瑕佸锷犻挛鍜岃В阌佽繘琛屽疄鐜? */
#define     SHELL_USING_LOCK            0

/**
 * @brief shell鍐呭瓨鍒嗛厤
 *        shell链韩涓嶉渶瑕佹鎺ュ彛锛岃嫢浣跨敤shell浼寸敓瀵硅薄锛岄渶瑕佽繘琛屽畾涔? */
#define     SHELL_MALLOC(size)          0

/**
 * @brief shell鍐呭瓨閲婃斁
 *        shell链韩涓嶉渶瑕佹鎺ュ彛锛岃嫢浣跨敤shell浼寸敓瀵硅薄锛岄渶瑕佽繘琛屽畾涔? */
#define     SHELL_FREE(obj)             0

/**
 * @brief 鏄惁鏄剧ずshell淇℃伅
 */
#define     SHELL_SHOW_INFO             0

/**
 * @brief 鏄惁鍦ㄧ橱褰曞悗娓呴櫎锻戒护琛? */
#define     SHELL_CLS_WHEN_LOGIN        1

/**
 * @brief shell榛樿鐢ㄦ埛
 */
#define     SHELL_DEFAULT_USER          "Turtle_OUC"

/**
 * @brief shell榛樿鐢ㄦ埛瀵嗙爜
 *        鑻ラ粯璁ょ敤鎴蜂笉闇€瑕佸瘑镰侊紝璁句负""
 */
#define     SHELL_DEFAULT_USER_PASSWORD ""

/**
 * @brief shell镊姩阌佸畾瓒呮椂
 *        shell褰揿墠鐢ㄦ埛瀵嗙爜链夋晥镄勬椂链欑敓鏁堬紝瓒呮椂鍚庝细镊姩閲嶆柊阌佸畾shell
 *        璁剧疆涓?镞跺叧闂嚜锷ㄩ挛瀹氩姛鑳斤紝镞堕棿鍗曚綅涓筚SHELL_GET_TICK()`鍗曚綅
 * @note 浣跨敤瓒呮椂阌佸畾蹇呴』淇濊瘉`SHELL_GET_TICK()`链夋晥
 */
#define     SHELL_LOCK_TIMEOUT          0 * 60 * 1000

#endif
