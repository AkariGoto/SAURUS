
// ASURA2GUIView.h : CASURA2GUIView クラスのインターフェイス
//

#pragma once

//  20200819  OpenGL関連追加
//  20200820  ViewSetting.hを使用しない
//  20200821  スレッド関連
//  20201019  脚先のワールド座標と脚座標の切り替え

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "Graphic\AsuraGraphic.h"

#include "Data\AsuraData.h"
#include "Data\PlanData.h"

//#include "System\ViewSetting.h"  20200820


class CASURA2GUIView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CASURA2GUIView() noexcept;
	DECLARE_DYNCREATE(CASURA2GUIView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_ASURA2GUI_FORM };
#endif

// 属性
public:
	CASURA2GUIDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後に初めて呼び出されます。

// 実装
public:
	virtual ~CASURA2GUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnClickedCheckDrive();
	afx_msg void OnClickedCheckComm();
	afx_msg void OnClickedCheckLog();

	/// カスタムメッセージ
	afx_msg LRESULT OnRecieveViewData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		追加メンバ変数
 *	------------------------------------------------------------
 */

private:
/**
 *	----------------------------------------
 *	クラスオブジェクト
 *	----------------------------------------
 */

 /**
  *		表示データ
  */
	Data::AsuraData viewAsuraXData;
	Data::PlanData viewPlanData;

	//20201019
	Math::Vector* localFootPosition; //脚座標系による脚先位置
	Math::Vector* worldFootPosition; //ワールド座標系による脚先位置

	/**
	 *	グラフィックオブジェクト
	 */
	/// アニメーション
	//Graphic::AsuraGraphic* asuraXGraphic[SCREEN_NUMBER];  //20200819
	Graphic::AsuraGraphic* asuraXGraphic;  //20200820

	/**
	 *	スレッド
	 */
	//20200821
	DWORD motionPlanThreadID;
	DWORD AsuraUDPThreadID; //20210617 実験的にUDPThreadID to AsuraUDPThreadID

/**
 *	----------------------------------------
 *	タイマ
 *	----------------------------------------
 */
	 /// タイマ識別子
	UINT_PTR	timerID;
	/// 現在時間
	double		presentTime;


/**
 *	----------------------------------------
 *	GUI（FormView）関連
 *	----------------------------------------
 */
	 /**
	  *	グラフィックを出力するピクチャボックスへのウィンドウ
	  */
	//HWND pictWndHandle[SCREEN_NUMBER];  //  20200819
	HWND pictWndHandle;  //20200820

	/**
	 *	(変更前の)ウィンドウプロシージャアドレス
	 */
	//WNDPROC orgWndProcedure[SCREEN_NUMBER];  //  20200819
	WNDPROC orgWndProcedure;  //20200820


/**
 *	----------------------------------------
 *	フラグ
 *	----------------------------------------
 */
 /// 画面を描画するか
	bool isScreenDrawn;
	/// ブラシが作成されているか
	bool isBrushCreated;

	/// それぞれの動作が開始されたか（ボタンが押されているか）
	bool isDriving;
	bool isCommunicating;
	bool isLogging;

	//20201019
	//脚先の座標系のフラグ
	bool legCoordinateSystem;


	CStdioFile txtFile;  //20221026
/**
 *	------------------------------------------------------------
 *		追加メンバ関数
 *	------------------------------------------------------------
 */
public:

/**
 *	初期化・終了処理
 */
	/**
	 *	本クラスの初期化・終了処理
	 */
	void initializeCASURA2GUIView(void);
	void finalizeCASURA2GUIView(void);

	/**
	 *	グラフィック
	 */
	void initializeGraphics(void);
	void finalizeGraphics(void);

/**
 *	画面の描画に関係するもの
 */
	 /// 画面の更新
	void updateFormView(void);

private:
/**
 *	サブクラス化したコントロールのウィンドウプロシージャ  //  20200819
 */
	static LRESULT CALLBACK drawMainScrWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClickedButtonSet();
};

#ifndef _DEBUG  // ASURA2GUIView.cpp のデバッグ バージョン
inline CASURA2GUIDoc* CASURA2GUIView::GetDocument() const
   { return reinterpret_cast<CASURA2GUIDoc*>(m_pDocument); }
#endif

