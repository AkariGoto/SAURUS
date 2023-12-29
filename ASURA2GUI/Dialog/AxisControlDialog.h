/**
 *  ファイル名
 *		AxisControlDialog.h
 *  説明
 *		軸制御のためのダイアログクラス
 *  日付
 *		作成日: 2007/06/11(Mon)		更新日: 2007/06/18(Mon)
 */

#pragma once

// CAxisControlDialog ダイアログ

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\resource.h"
#include "afxcmn.h"

#include "..\Data\AsuraData.h"

#include "..\System\ViewSetting.h"
#include "..\System\Control\RoundButtonStyle.h"
#include "..\System\Control\RoundButton.h"
#include "..\System\Control\SliderCtrlEx.h"


/**
 *	----------------------------------------------------------------------
 *		CAxisControlDialogクラス
 *	----------------------------------------------------------------------
 */
class CAxisControlDialog : public CDialog
{
	DECLARE_DYNAMIC(CAxisControlDialog)

/**
 *	------------------------------------------------------------
 *		定数
 *	------------------------------------------------------------
 */
	static const int SLIDER_NUM = 6;

/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	DDX変数
 *	----------------------------------------
 */
/**
 *	ラウンドボタンクラス
 */
	CRoundButtonStyle m_DefaultRoundButtonStyle;

	CRoundButton* pCheckLeg[4];
	CRoundButton* pCheckMode[2];

	CRoundButton m_CheckLeg1;
	CRoundButton m_CheckLeg2;
	CRoundButton m_CheckLeg3;
	CRoundButton m_CheckLeg4;

	CRoundButton m_CheckPosition;
	CRoundButton m_CheckVelocity;

/**
 *	スライダーコントロール
 */
	CSliderCtrlEx jointSlider1;
	CSliderCtrlEx jointSlider2;
	CSliderCtrlEx jointSlider3;
	CSliderCtrlEx footSliderX;
	CSliderCtrlEx footSliderY;
	CSliderCtrlEx footSliderZ;

	CSliderCtrlEx* pSliderCtrl[SLIDER_NUM];	

/**
 *	親ウィンドウ
 */
	CWnd* pParentWindow;

/**
 *	メッセージを送るスレッドのID
 */
	DWORD motionPlanThreadID;

/**
 *	タイマ
 */
	/// タイマ識別子
	UINT_PTR	timerID;

/**
 *	Viewのスタイル
 */
	ViewStyle viewStyle;

/**
 *	色
 */
	/// エディットテキスト色
	COLORREF editColor;
	/// スタティックテキスト色
	COLORREF staticColor;

	/// エディットボックス背景色
	COLORREF editBkColor;
	/// スタティックテキスト背景色
	COLORREF staticBkColor;

/**
 *	ブラシ
 */
	/// ダイアログ背景
	HBRUSH backColorBrush;
	/// エディット背景
	HBRUSH editBkColorBrush;
	/// テキスト背景
	HBRUSH staticBkColorBrush;

/**
 *	フラグ
 */
	bool isBrushCreated;
	int selectedLegNo;
	int selectedCtrlMode;

/**
 *	データ
 */
	/// Asuraデータ
	Data::AsuraData dlgAsuraData;

	/// スライダデータ
	int sldrPosition[SLIDER_NUM];

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
	CAxisControlDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAxisControlDialog();

// ダイアログ データ
	enum { IDD = IDD_DLG_AXIS_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *	------------------------------------------------------------
 */
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedCheckLeg1();
	afx_msg void OnBnClickedCheckLeg2();
	afx_msg void OnBnClickedCheckLeg3();
	afx_msg void OnBnClickedCheckLeg4();
	afx_msg void OnBnClickedCheckPosition();
	afx_msg void OnBnClickedCheckVelocity();

	afx_msg LRESULT OnRecieveData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSwitchDisplayMode(WPARAM wParam, LPARAM lParam);

/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
public:
	virtual BOOL OnInitDialog();
	virtual BOOL Create(LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);

protected:
	virtual void PostNcDestroy();
	virtual void OnCancel();

/**
 *	------------------------------------------------------------
 *		自作関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	ダイアログのスタイルの設定
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		ダイアログの背景などを設定
	 *	引数
	 *		style: 背景のスタイル
	 */
	void setDialogStyle(ViewStyle style);
	/**
	 *	説明
	 *		スライダの設定
	 */
	void setSliderControlStyle(void);
	/**
	 *	説明
	 *		ボタンの設定
	 */
	void setButtonControlStyle(void);

	/**
	 *	説明
	 *		メッセージを送るスレッドIDの設定
	 *	引数
	 *		id: スレッドID
	 */
	void setMotionPlanThreadID(DWORD id);
	/**
	 *	説明
	 *		メッセージを送るスレッドIDの取得
	 */
	DWORD getMotionPlanThreadID(void) const{return motionPlanThreadID;}

private:
	/// ボタン選択のヘルプ関数
	void markSelectedLegButton(void);
	void markSelectedModeButton(void);

	/// 表示の更新
	void updateDlgItem(void);
};

