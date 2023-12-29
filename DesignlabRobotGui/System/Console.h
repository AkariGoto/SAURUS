/**
 *  ファイル名
 *		Console.h
 *  説明
 *		コンソールに出力するためのクラス
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/19(SAT)
 */

#ifndef DESIGNLAB_ROBOT_GUI_SYSTEM_CONSOLE_H_
#define DESIGNLAB_ROBOT_GUI_SYSTEM_CONSOLE_H_

#include <windows.h>
#include <stdio.h>

 /**
   *	----------------------------------------------------------------------
   *		Consoleクラス
   *			MonoStateパターンで作成（コンストラクタ系はstatic, メンバはprivate）
   *			継承しないこと
   *			Console.iniファイルを含むSettingディレクトリを用意
   *	----------------------------------------------------------------------
   */
class Console final
{
public:
    Console() = delete;
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;
    Console(Console&&) = delete;
    ~Console() = delete;

    /**
     *	説明
     *		デバッグ出力オプションの初期化
     */
    static void initialize(LPCWSTR  lpszSection);

    /**
     *	説明
     *		コンソールの作成
     */
    static BOOL createConsole(void);

    /**
     *	説明
     *		デバッグレベルのチェック
     */
    static BOOL checkLevel(DWORD level);

    /**
     *	説明
     *		デバッグログファイルの作成
     */
    static FILE* openLogFile(void);

    /**
     *	説明
     *		結果をコンソールに出力
     */
    static void print(LPSTR msgString);

    /**
     *	説明
     *		詳細結果をコンソールに出力
     */
    static void	__cdecl output(DWORD level, PSTR fmtStrPointer, ...);

    /**
     *	説明
     *		デバッグダンプ出力
     *	引数
     *		DWORD  level		... 出力レベル
     *		PSTR  titleStrPtr	... タイトルを指すポインタ
     *		LPBYTE buffBytePtr	... バッファを指すポインタ
     *		DWORD  buffSize		... バッファサイズ
     */
    static void dump(DWORD level, PSTR titleStrPtr, LPBYTE buffBytePtr, DWORD buffSize);

    /**
     *	説明
     *		コンソールの破棄
     */
    static void destroyConsole();

private:



    /**
     *	説明
     *		コンソールの属性を設定
     */
    static BOOL setConsoleAttribute(HANDLE consoleHandle);

    /**
     *	説明
     *		コンソールの標準ファイルハンドルを得る
     */
    static FILE* getConsoleFileHandle(DWORD dwDevice);

    static const int	MAX_NAME = 256;
    static const int	ENTRYBUFFSIZE = 256;
    static const int	LINEBUFFSIZE = 256;

    /// ファイル名
    static const TCHAR	CONSOLENAME[MAX_PATH + 1];
    static const TCHAR	INIFILENAME[MAX_PATH + 1];
    static const TCHAR	INIDIRNAME[MAX_PATH + 1];
    static const TCHAR* YESNO[2];

    /// コンソール属性
    static const WORD	FOREGROUND_BLACK;
    static const WORD	FOREGROUND_CYAN;
    static const WORD	FOREGROUND_WHITE;
    static const WORD	FOREGROUND_CUSTOM;

    static const WORD	BACKGROUND_BLACK;
    static const WORD	BACKGROUND_CYAN;
    static const WORD	BACKGROUND_WHITE;
    static const WORD	BACKGROUND_CUSTOM;

    /// 出力レベル
    static const WORD	INFO;
    static const WORD	WARNING;
    static const WORD	ALART;
    static const WORD	EMERGENCY;

    static const WORD	DUMP;

    /// ログファイル名
    static TCHAR logFileName[ENTRYBUFFSIZE];

    /// iniファイルのセクション名
    static TCHAR sectionName[ENTRYBUFFSIZE];

    /// デバッグログを出力するか
    static bool 	isDebugMode;
    /// ログファイルを削除するか
    static bool	isDeleteOnStart;
    /// デバッグログ出力レベル
    static DWORD	debugLevel;
    /// ログファイルハンドル
    static FILE* logFile;

    /// クリティカルセクション ログ出力中
    static CRITICAL_SECTION	loggingCS;
};	/// end of class Console


#endif  // DESIGNLAB_ROBOT_GUI_SYSTEM_CONSOLE_H_
