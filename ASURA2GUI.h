
// ASURA2GUI.h : ASURA2GUI アプリケーションのメイン ヘッダー ファイル
//

//20200821  スレッド関連

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CASURA2GUIApp:
// このクラスの実装については、ASURA2GUI.cpp を参照してください
//


/**
 *	----------------------------------------------------------------------
 *		追加でクラス前方宣言
 *	----------------------------------------------------------------------
 */
//20200821
class CASURA2GUIDoc;


class CASURA2GUIApp : public CWinApp
{
public:
	CASURA2GUIApp() noexcept;


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


/**
 *	------------------------------------------------------------
 *		追加メンバ変数
 *	------------------------------------------------------------
 */
public:
	/**
	 *	それぞれのクラスへのポインタ
	 */
	CASURA2GUIDoc* pASURA2GUIDoc{ 0 };  //20200821

	/// モーションプランスレッドのID
	DWORD motionPlanThreadID{ 0 };  //20200821
	//AsuraUDPThreadID
	DWORD AsuraUDPThreadID{ 0 };	
};

extern CASURA2GUIApp theApp;
