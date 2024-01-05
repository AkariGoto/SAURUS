/**
 *  ファイル名
 *		DebugOutput.h
 *  説明
 *		Windowsでのデバッグ用ツール
 *  日付
 *		作成日: 2007/05/19(SAT)		更新日: 2007/05/26(SAT)
 */

#ifndef __DebugOutput_h__
#define __DebugOutput_h__

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
#include <windows.h>
#include <winnt.h>
#include <tchar.h>
#include <stdarg.h>


  /**
   *	----------------------------------------------------------------------
   *		定数、マクロ宣言
   *	----------------------------------------------------------------------
   */
   /// Visual Studio のデバッグウィンドウの横幅に対応する文字数(バイト数ではない)
const int DEBUG_MESSAGE_LENGTH = 256;

#ifdef _DEBUG

/**
 *	デバッグモードの時のみ出力する（MFCのTRACEマクロと同様）
 */
inline void debugPrint(const _TCHAR* fmt, ...)
{
    /// ローカル変数
    va_list ap;
    TCHAR buffer[DEBUG_MESSAGE_LENGTH];

    /// 可変長文字列コピー
    va_start(ap, fmt);
    _vsntprintf(buffer, DEBUG_MESSAGE_LENGTH, fmt, ap);
    va_end(ap);

    /// デバッグウィンドウに出力
    ::OutputDebugString(buffer);
}

#define DEBUG_TRACE ::debugPrint
#define DEBUG_TRACE_EX ::debugPrint("%s (%d):", __FILE__, __LINE__), ::debugPrint
#define DEBUG_ASSERT(x) \
      if ( !(x) )	{\
        debugPrint(TEXT("Assertion failed! in %s (%d)\n"), __FILE__, __LINE__);\
        DebugBreak();\
      }
#else
#define DEBUG_TRACE		__noop
#define DEBUG_ASSERT	__noop
#endif	/// _DEBUG

#endif	/// __DebugOutput_h__