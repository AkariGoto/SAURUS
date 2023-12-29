/**
 *  ファイル名
 *		ViewSetting.h
 *  説明
 *		Viewクラスの設定
 *  日付
 *		作成日: 2007/06/06(WED)		更新日: 2007/06/11(MON)
 */

//  20200819  SCREEN_NUMBER変更

#ifndef __ViewSetting_h__
#define __ViewSetting_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\Utility\ColorNames.h"

/**
 *	----------------------------------------------------------------------
 *		定数宣言
 *	----------------------------------------------------------------------
 */
/// 描画するスクリーン数
//const int SCREEN_NUMBER = 4;
const int SCREEN_NUMBER = 1;  //  20200819  変更

/// FormViewの背景
const COLORREF DLG_BACK_GREEN_COLOR		= RGB(32, 192, 208);
const COLORREF DLG_BACK_BLUE_COLOR		= RGB(64, 128, 192);

/// FormViewのedit背景
const COLORREF EDIT_BACK_GREEN_COLOR	= RGB(32, 128, 160);
const COLORREF EDIT_BACK_BLUE_COLOR		= RGB(32, 48, 112);

/// staticテキスト色
const COLORREF TXT_YELLOW_COLOR_DARK	= RGB(192, 192, 0);
const COLORREF TXT_YELLOW_COLOR_LIGHT	= RGB(250, 240, 64);

/// editテキスト色
const COLORREF TXT_BLUE_COLOR_DARK		= RGB(32, 255, 255);
const COLORREF TXT_BLUE_COLOR_LIGHT		= RGB(64, 255, 255);


/// ボタン色
const COLORREF BTN_COLOR1				= RGB(0, 64, 128);
const COLORREF SELECTED_BTN_COLOR1		= RGB(255, 32, 160);

/// ボタンのキャプション色
const COLORREF BTN_CAPTION_COLOR1		= RGB(255, 255, 255);

/// スライダーのチャンネル色
const COLORREF SLDR_CHANNEL_COLOR		= RGB(32, 128, 160);
/// スライダーのTHUMB色
const COLORREF SLDR_THUMB_COLOR			= RGB(64, 255, 255);
/// スライダーのTHUMBのハイライト色
const COLORREF SLDR_THUMB_HILIGHT_COLOR	= RGB(250, 240, 64);


/**
 *		メニュー色
 */
const COLORREF MENU_SELECTED			= RGB( 192, 255, 255 );		/// 選択
const COLORREF MENU_SELBORDER			= RGB(   0,  50, 128 );		/// 選択の境界線
const COLORREF MENU_NORMAL_STR			= RGB(   0, 128,  64 );		/// 通常の文字列
const COLORREF MENU_SELECTED_STR		= RGB(  32,  64, 255 );		/// 選択状態の文字列
const COLORREF MENU_GRAYED_STR			= RGB( 140, 140, 140 );		/// 無効の文字列
const COLORREF MENU_SEPARATOR			= RGB( 128, 128, 128 );		/// セパレータ
const COLORREF MENU_BACKGROUND			= RGB( 224, 255, 240 );		/// 背景
const COLORREF MENU_BGICON				= RGB( 192, 240, 192 );		/// アイコンの背景
const COLORREF MENU_BGCHECK				= RGB( 224, 224, 255 );		/// チェックの背景
const COLORREF MENU_CHECKBORDER			= RGB(  64, 160, 255 );		/// チェックの境界線

/**
 *		列挙子
 */
typedef enum
{
	GREEN_THEMED,
	BLUE_THEMED,
}ViewStyle;

#endif	/// __ViewSetting_h__