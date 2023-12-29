///**
// *  ファイル名
// *		BasicDirectX.h
// *  説明
// *		DirectXの定義
// *  日付
// *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
// */
//#ifndef __BasicDirectX_h__
//#define __BasicDirectX_h__
//
// /**
//  *	----------------------------------------------------------------------
//  *		ヘッダファイルインクルード
//  *	----------------------------------------------------------------------
//  */
//#include "DebugOutput.h"
//
//
//  /**
//   *	------------------------------------------------------------
//   *		DirectInput
//   *	------------------------------------------------------------
//   */
//   /// DirectXのバージョン宣言（ヘッダより前で宣言する）
//#define DIRECTINPUT_VERSION 0x800
//#include <dinput.h>
//
///**
// *	ウィンドウズヘッダ。WinNT4.0、Win98以降
// */
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x400
//#endif
//#include <Windows.h>
//
// /**
//  *	------------------------------------------------------------
//  *		DirectXライブラリの追加
//  *	------------------------------------------------------------
//  */
//  /// DirectXGUID
//#pragma comment(lib, "dxguid.lib")
//
///// DirectInput
//#pragma comment(lib, "dinput8.lib")
//
///**
// *	------------------------------------------------------------
// *		DirectXマクロ
// *	------------------------------------------------------------
// */
//
// /**
//  *	----------------------------------------
//  *	リリース時にも実行
//  *	----------------------------------------
//  */
//  /**
//   *	DirectXの成功
//   *
//   *	リリース時にも実行される
//   *	引数
//   *		dxStatus DirectXのステータス。成功していればtrueを返す
//   */
//#define DX_SUCCEEDED(dxStatus) ( (dxStatus) >= 0 )
//
//   /**
//    * 	DirectXの失敗
//    *
//    * 	リリース時にも実行される
//    *	引数
//    *		dxStatus DirectXのステータス。失敗していればtrueを返す
//    */
//#define DX_FAILED(dxStatus) ( (dxStatus) < 0 )
//
//    /**
//     *	----------------------------------------
//     *	デバッグ時のみ実行
//     *	----------------------------------------
//     */
//#ifdef _DEBUG
//     /**
//      *	DirectXのアサート
//      *
//      *	リリース時には実行されない
//      *	引数
//      *		expression 失敗だとアサートが発生
//      */
//#define DX_ASSERT(expression) \
//	if ( DX_FAILED(expression) ) {\
//		debugPrint(TEXT("DirectX Assertion in %s (%d)\n"), __FILE__, __LINE__);\
//		DebugBreak();\
//	}
//
//      /**
//       *	DirectXのメッセージ付アサート
//       *
//       *	リリース時には実行されません
//       *	引数
//       *		expression 失敗だとアサートが発生する
//       *		message アサート時に出力するメッセージ
//       */
//#define DX_MSG_ASSERT(expression, message) \
//	if ( DX_FAILED(expression) ) {\
//		debugPrint(TEXT("DirectX Assertion %s\n in %s (%d)\n"), message, __FILE__, __LINE__);\
//		DebugBreak();\
//	}
//
//       /**
//        *	DirectXのチェック
//        *
//        *	リリース時にチェックは実行されないが、引数の構文は実行される
//        *	引数
//        *		expression 失敗だとアサートが発生する
//        */
//#define DX_CHECK(expression) DX_ASSERT(expression)
//
//        /**
//         *	DirectXのメッセージ付チェック
//         *
//         *	リリース時にチェックは実行されないが、引数の構文は実行される
//         *	引数
//         *		expression 失敗だとアサートが発生する
//         *		message アサート時に出力するメッセージ
//         */
//#define DX_MSG_CHECK(expression, message) \
//	DX_MSG_ASSERT(expression, message)
//
//         /**
//          *	----------------------------------------
//          *	デバッグ時マクロのリリース時のダミー
//          *	----------------------------------------
//          */
//#else /// _DEBUG
//
//#define DX_ASSERT(expression)
//#define DX_MSG_ASSERT(expression, message)
//#define DX_CHECK(expression)
//#define DX_MSG_CHECK(expression, message)
//
//#endif	/// End of _DEBUG
//
//#endif	/// __BasicDirectX_h__