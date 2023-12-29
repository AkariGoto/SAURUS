/**
 *  ファイル名
 *		AxisControlDialog.cpp
 *  説明
 *		軸制御のためのダイアログクラス
 *  日付
 *		作成日: 2007/06/11(Mon)		更新日: 2007/06/18(Mon)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\stdafx.h"

#include "AxisControlDialog.h"
#include "..\Utility\ColorNames.h"
#include <iostream>
#include "..\System\WinUserMessage.h"
#include "..\Plan\PlanParameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;
using namespace Data;


/**
 *	----------------------------------------------------------------------
 *		定数宣言
 *	----------------------------------------------------------------------
 */
const int SLDR_RANGE_MIN[2] = {-180, -800};
const int SLDR_RANGE_MAX[2] = {180, 800};
const int SLDR_PAGE_SIZE[2] = {25, 50};
const int SLDR_TIC_FREQ[2] = {100, 400};
const int SLDR_SCALE = 10;

/**
 *	----------------------------------------------------------------------
 *		CAxisControlDialogクラス
 *	----------------------------------------------------------------------
 */

/**
 *		フレームワークが実行時にクラス名やクラス階層でのクラスの位置を知るために
 *		CObject クラスの動的派生オブジェクトに必要な C++ のコードを生成する．
 *
 *		第一引数は派生クラス（このクラス：CAxisControlDialog），第二引数は基本クラス（CDialog）をとる．
 */
IMPLEMENT_DYNAMIC(CAxisControlDialog, CDialog)

BEGIN_MESSAGE_MAP(CAxisControlDialog, CDialog)

	/// Windows予約
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_WM_TIMER()

	/// Windows管理
	ON_BN_CLICKED(IDC_CHECK_LEG1, &CAxisControlDialog::OnBnClickedCheckLeg1)
	ON_BN_CLICKED(IDC_CHECK_LEG2, &CAxisControlDialog::OnBnClickedCheckLeg2)
	ON_BN_CLICKED(IDC_CHECK_LEG3, &CAxisControlDialog::OnBnClickedCheckLeg3)
	ON_BN_CLICKED(IDC_CHECK_LEG4, &CAxisControlDialog::OnBnClickedCheckLeg4)
	ON_BN_CLICKED(IDC_CHECK_POSITION, &CAxisControlDialog::OnBnClickedCheckPosition)
	ON_BN_CLICKED(IDC_CHECK_VELOCITY, &CAxisControlDialog::OnBnClickedCheckVelocity)

	/// カスタムメッセージ
	ON_MESSAGE(WM_PLAN_SENDING_AC_DATA, OnRecieveData)
	ON_MESSAGE(WM_DLG_AC_DISPLAY, OnSwitchDisplayMode)

END_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		CAxisControlDialogクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
 CAxisControlDialog::CAxisControlDialog(CWnd* pParent /*=NULL*/)
{
	/// メンバ変数の初期化
	pParentWindow = pParent;
	isBrushCreated = false;
	motionPlanThreadID = 0;
	selectedCtrlMode = 0;
	timerID = 0;
}

CAxisControlDialog::~CAxisControlDialog()
{
}

void CAxisControlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_JOINT1, jointSlider1);
	DDX_Control(pDX, IDC_SLIDER_JOINT2, jointSlider2);
	DDX_Control(pDX, IDC_SLIDER_JOINT3, jointSlider3);
	DDX_Control(pDX, IDC_SLIDER_FOOTX, footSliderX);
	DDX_Control(pDX, IDC_SLIDER_FOOTY, footSliderY);
	DDX_Control(pDX, IDC_SLIDER_FOOTZ, footSliderZ);
	DDX_Control(pDX, IDC_CHECK_LEG1, m_CheckLeg1);
	DDX_Control(pDX, IDC_CHECK_LEG2, m_CheckLeg2);
	DDX_Control(pDX, IDC_CHECK_LEG3, m_CheckLeg3);
	DDX_Control(pDX, IDC_CHECK_LEG4, m_CheckLeg4);
	DDX_Control(pDX, IDC_CHECK_POSITION, m_CheckPosition);
	DDX_Control(pDX, IDC_CHECK_VELOCITY, m_CheckVelocity);
}

/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *		CAxisControlDialog メッセージ ハンドラ
 *	------------------------------------------------------------
 */
/**
 *	Windows予約
 */
/// WM_CTLCOLORに対応
HBRUSH CAxisControlDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
    switch  ( nCtlColor )
	{
		case CTLCOLOR_DLG:	/// 背景色の設定
		{
			if (*pWnd == *this)
				hbr = backColorBrush;

			break;
		}	/// case CTLCOLOR_DLG:
		case CTLCOLOR_MSGBOX:
		case CTLCOLOR_EDIT:
		{
			/// 文字色
			pDC->SetTextColor( editColor );
			/// 背景色
			pDC->SetBkColor( editBkColor );
				
			hbr = editBkColorBrush;

			break;
		}
		case CTLCOLOR_STATIC:
		{
			int id = pWnd->GetDlgCtrlID();

			if ( IDC_TEXT_JOINT == id || 
				IDC_TEXT_FOOTHOLD == id || 
				IDC_TEXT_BODY == id ||
				IDC_TEXT_GAIT == id ||
				IDC_TEXT_TIME == id )
			{
				/// 文字色
				pDC->SetTextColor( staticColor );
				/// 背景色
				pDC->SetBkColor( staticBkColor );
				
				hbr = staticBkColorBrush;
			}
			else
			{
				/// 文字色
				pDC->SetTextColor( staticColor );
				/// 背景色
				pDC->SetBkColor( staticBkColor );
				
				hbr = staticBkColorBrush;
			}

			break;
		}	/// end of case CTLCOLOR_STATIC:
		default:
			break;
	}	/// switch  ( nCtlColor )

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

/// WM_HSCROLLに対応
void CAxisControlDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	/// ローカル変数
	int jointNo = 0;	/// 関節番号... 1, 2, 3
	int footDirection = 0;	/// 運動方向... 1, 2, 3
	int sldrNum = 0;		/// スライダ番号

	/// カウンタ
	int i;

	/// スライダーの判定
	CSliderCtrlEx* pSliderBar  = (CSliderCtrlEx*)pScrollBar;

	for (i=0; i< SLIDER_NUM; i++)
	{
		if ( pSliderBar == pSliderCtrl[i] )
		{
			sldrNum = i+1; 
			break;
		}
	}

	switch (sldrNum)
	{
		case 1:
			jointNo = 1; break;
		case 2:
			jointNo = 2; break;
		case 3:
			jointNo = 3; break;
		case 4:
			footDirection = 1; break;
		case 5:
			footDirection = 2; break;
		case 6:
			footDirection = 3; break;
		default:
			break;
	}

	if ( 0 < sldrNum )
	{
		/// スライダーの生データ
		sldrPosition[sldrNum-1] = 0;
		/// スライダのレンジ
		int min = 0;
		int max = 0;

		/// 関節値
		double sldrInfo = 0.0;

		/// スライダの現在値を取得
		sldrPosition[sldrNum-1] = pSliderBar->GetPos();
		
		pSliderBar->GetRange(min, max);

		switch (nSBCode) 
		{
			case SB_PAGELEFT:		/// スライダの左側をクリックした場合（またはPageUpキー）
				if ( sldrNum < 4)
					sldrPosition[sldrNum-1] -= SLDR_PAGE_SIZE[0];
				else
					sldrPosition[sldrNum-1] -= SLDR_PAGE_SIZE[1];

				/// 最小値判定
				if( sldrPosition[sldrNum-1] < min )
					sldrPosition[sldrNum-1] = min;
				
				break;
		
			case SB_PAGERIGHT:		/// スライダの右側をクリックした場合（またはPageDownキー）
				if ( sldrNum < 4)
					sldrPosition[sldrNum-1] += SLDR_PAGE_SIZE[0];
				else
					sldrPosition[sldrNum-1] += SLDR_PAGE_SIZE[1];

				if( sldrPosition[sldrNum-1] > max )
					sldrPosition[sldrNum-1] = max;
		
				break;
			
			case SB_THUMBPOSITION:	/// スライダを絶対移動させた場合
			case SB_THUMBTRACK:		/// スライダバーをドラッグ移動させた場合
				sldrPosition[sldrNum-1] = pSliderBar->GetPos();

				break;

			default:
				break;
		}

		// 現在位置にスライダバー更新
		pSliderBar->SetPos( sldrPosition[sldrNum-1] );

		/// スライダ値の処理
		sldrInfo = (double)sldrPosition[sldrNum-1] / (double)SLDR_SCALE;

		/// editボックスに表示
		CString data;
		data.Format( TEXT("%5.1lf"), sldrInfo );

		/// スライダ番号により処理
		/// editボックスへの表示とMotionPlanThreadへの通知
		switch (sldrNum)
		{
			case 1:
				SetDlgItemText(IDC_EDIT_JOINT1, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 2:
				SetDlgItemText(IDC_EDIT_JOINT2, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 3:
				SetDlgItemText(IDC_EDIT_JOINT3, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 4:
				SetDlgItemText(IDC_EDIT_FOOTX, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 5:
				SetDlgItemText(IDC_EDIT_FOOTY, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 6:
				SetDlgItemText(IDC_EDIT_FOOTZ, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			default:
				break;
		}
	}	/// end of if ( 0 < sldrNum )

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/// WM_TIMERに対応
void CAxisControlDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_REQUEST_DATA, (WPARAM)this->m_hWnd, NULL);

	CDialog::OnTimer(nIDEvent);
}

/**
 *	Windows管理イベント
 */
void CAxisControlDialog::OnBnClickedCheckLeg1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (selectedLegNo == 1) selectedLegNo = 0;
	else selectedLegNo = 1;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckLeg2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (selectedLegNo == 2) selectedLegNo = 0;
	else selectedLegNo = 2;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckLeg3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (selectedLegNo == 3)	selectedLegNo = 0;
	else selectedLegNo = 3;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckLeg4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (selectedLegNo == 4) selectedLegNo = 0;
	else selectedLegNo = 4;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckPosition()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (selectedCtrlMode == 1) selectedCtrlMode = 0;
	else selectedCtrlMode = 1;

	PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_CTRL_MODE, (WPARAM)selectedCtrlMode, NULL);

	markSelectedModeButton();
}

void CAxisControlDialog::OnBnClickedCheckVelocity()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (selectedCtrlMode == 2) selectedCtrlMode = 0;
	else selectedCtrlMode = 2;

	PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_CTRL_MODE, (WPARAM)selectedCtrlMode, NULL);

	markSelectedModeButton();
}

/**
 *	カスタムメッセージ
 */
/// データの受信と表示更新
LRESULT CAxisControlDialog::OnRecieveData(WPARAM wParam, LPARAM lParam)
{
	/// 送信データをキャストして保持
	dlgAsuraData = *( (AsuraData*)wParam );

	/// 表示データ更新
	updateDlgItem();

	return 1;
}

/// ON/OFFの切替
LRESULT CAxisControlDialog::OnSwitchDisplayMode(WPARAM wParam, LPARAM lParam)
{
	if ( (int)wParam )
	{
		timerID = 1;
		SetTimer(timerID, 80, NULL);

		ShowWindow(SW_SHOW);

		return 1;
	}
	else
	{
		if (timerID == 1)
			KillTimer(timerID);
		
		timerID = 0;

		ShowWindow(SW_HIDE);

		return 0;
	}

	return 2;
}

/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
BOOL CAxisControlDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	SetWindowText( TEXT("Axis Control") );

	/// Viewスタイルの設定
	setDialogStyle(GREEN_THEMED);

	/// ボタンの設定
	setButtonControlStyle();

/**
 *		スライダの設定
 */
	setSliderControlStyle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CAxisControlDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	return CDialog::Create(IDD, pParentWindow);

	/*
	return CDialog::Create(lpszTemplateName, pParentWnd);
	*/
}

void CAxisControlDialog::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	DeleteObject(backColorBrush);
	DeleteObject(editBkColorBrush);
	DeleteObject(staticBkColorBrush);

	delete this;

	CDialog::PostNcDestroy();
}

void CAxisControlDialog::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	DestroyWindow();
	/// EndDialogを呼び出すため，ダイアログボックスは破棄されず非表示になるだけ
	/*
	CDialog::OnCancel();
	*/
}
/**
 *	------------------------------------------------------------
 *		自作関数
 *	------------------------------------------------------------
 */

/**
 *	ダイアログのスタイルを決定
 */
void CAxisControlDialog::setDialogStyle(ViewStyle style)
{
	/// スタイルの決定
	viewStyle = style;

	/// 既に作成されていたらブラシ破棄
	if ( isBrushCreated )
	{
		DeleteObject(backColorBrush);
		DeleteObject(editBkColorBrush);
		DeleteObject(staticBkColorBrush);
	}
	
	switch ( viewStyle )
	{
		case GREEN_THEMED:
		{
			/// 色決定
			editColor = TXT_BLUE_COLOR_LIGHT;
			staticColor = Color::YELLOW;
			editBkColor = EDIT_BACK_GREEN_COLOR;
			staticBkColor = DLG_BACK_GREEN_COLOR /* EDIT_BACK_GREEN_COLOR */;

			///ブラシ作成
			backColorBrush			= CreateSolidBrush( DLG_BACK_GREEN_COLOR );
			editBkColorBrush		= CreateSolidBrush( EDIT_BACK_GREEN_COLOR );
			staticBkColorBrush	= CreateSolidBrush( DLG_BACK_GREEN_COLOR /* EDIT_BACK_GREEN_COLOR */ );

			break;
		}
		case BLUE_THEMED:
		{
			/// 色決定
			editColor = TXT_BLUE_COLOR_LIGHT;
			staticColor = TXT_YELLOW_COLOR_LIGHT;
			editBkColor = EDIT_BACK_BLUE_COLOR;
			staticBkColor = EDIT_BACK_BLUE_COLOR;

			///ブラシ作成
			backColorBrush			= CreateSolidBrush( DLG_BACK_BLUE_COLOR );
			editBkColorBrush		= CreateSolidBrush( EDIT_BACK_BLUE_COLOR );
			staticBkColorBrush	= CreateSolidBrush( EDIT_BACK_BLUE_COLOR );

			break;
		}
		default:
			break;
	}

	/// ブラシ作成完了
	isBrushCreated = true;

	return;
}

/**
 *		スライダを設定する
 */
void CAxisControlDialog::setSliderControlStyle(void)
{
	/// コントロールのメンバ変数を代入
	pSliderCtrl[0] = &jointSlider1;
	pSliderCtrl[1] = &jointSlider2;
	pSliderCtrl[2] = &jointSlider3;
	pSliderCtrl[3] = &footSliderX;
	pSliderCtrl[4] = &footSliderY;
	pSliderCtrl[5] = &footSliderZ;

	/// 初期位置と範囲の決定
	int i;
	for (i=0; i<3; i++)
	{
		pSliderCtrl[i]->SetRange(SLDR_RANGE_MIN[0]*SLDR_SCALE, SLDR_RANGE_MAX[0]*SLDR_SCALE);
		pSliderCtrl[i]->SetPageSize(SLDR_PAGE_SIZE[0]);
		pSliderCtrl[i]->SetTicFreq(SLDR_TIC_FREQ[0]);
		pSliderCtrl[i]->SetPos(0);
	}

	int j;
	for (j=3; j<6; j++)
	{
		pSliderCtrl[j]->SetRange(SLDR_RANGE_MIN[1]*SLDR_SCALE, SLDR_RANGE_MAX[1]*SLDR_SCALE);
		pSliderCtrl[j]->SetPageSize(SLDR_PAGE_SIZE[1]);
		pSliderCtrl[j]->SetTicFreq(SLDR_TIC_FREQ[1]);
		pSliderCtrl[j]->SetPos(0);
	}

	CString data;
	/// editの表示
	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[0]->GetPos() );
	SetDlgItemText(IDC_EDIT_JOINT1, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[1]->GetPos() );
	SetDlgItemText(IDC_EDIT_JOINT2, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[2]->GetPos() );
	SetDlgItemText(IDC_EDIT_JOINT3, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[3]->GetPos() );
	SetDlgItemText(IDC_EDIT_FOOTX, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[4]->GetPos() );
	SetDlgItemText(IDC_EDIT_FOOTY, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[5]->GetPos() );
	SetDlgItemText(IDC_EDIT_FOOTZ, data);


	UpdateData(FALSE);

	/// スライダの色決定
	int k;
	for (k=0; k<6; k++)
	{
		pSliderCtrl[k]->SetThumbColors(SLDR_THUMB_COLOR, SLDR_THUMB_HILIGHT_COLOR);
		sldrPosition[k] = 0;
	}

	return;
}

/// ボタンの設定
void CAxisControlDialog::setButtonControlStyle(void)
{
	int i;
	selectedLegNo = 0;
	selectedCtrlMode = 0;

	/// アドレス代入
	pCheckLeg[0] = &m_CheckLeg1;
	pCheckLeg[1] = &m_CheckLeg2;
	pCheckLeg[2] = &m_CheckLeg3;
	pCheckLeg[3] = &m_CheckLeg4;

	pCheckMode[0] = &m_CheckPosition;
	pCheckMode[1] = &m_CheckVelocity;

/**
 *	----------------------------------------
 *	形状の決定
 *	----------------------------------------
 */
	/// スタイルデータ
	tButtonStyle tStyle;

	/// 現在のボタンスタイルの設定をtStyleに格納
	m_DefaultRoundButtonStyle.GetButtonStyle(&tStyle);
	/**
	 *	スタイル変更
	 */
	tStyle.m_dRadius = 12.0;
	tStyle.m_tColorFace.m_tEnabled = RGB(64, 128, 255);
	tStyle.m_tColorFace.m_tClicked = RGB(255, 223, 128);
	tStyle.m_tColorBorder.m_tEnabled = RGB(255, 223, 128);
	tStyle.m_tColorBorder.m_tClicked = RGB(64, 128, 255);
	tStyle.m_tColorBack.m_tEnabled = DLG_BACK_GREEN_COLOR;
	tStyle.m_tColorBack.m_tClicked = DLG_BACK_GREEN_COLOR;
	/// 変更をtStyleから読み込む
	m_DefaultRoundButtonStyle.SetButtonStyle(&tStyle);

	/// 設定を反映
	for (i=0;i<4;i++)
	{
		pCheckLeg[i]->SetRoundButtonStyle(&m_DefaultRoundButtonStyle);
		pCheckLeg[i]->SetCheck(false);
	}

	for (i=0;i<2;i++)
	{
		pCheckMode[i]->SetRoundButtonStyle(&m_DefaultRoundButtonStyle);
		pCheckMode[i]->SetCheck(false);
	}

/**
 *	----------------------------------------
 *	フォントの決定
 *	----------------------------------------
 */
	/// フォントオブジェクト
	LOGFONT tFont;
	/// 現在の設定を格納
	for (i=0;i<4;i++) pCheckLeg[i]->GetFont(&tFont);
	for (i=0;i<2;i++) pCheckMode[i]->GetFont(&tFont);

	/**
	 *	設定を変更する
	 */
	strcpy((char*)tFont.lfFaceName,"Verdana");
	tFont.lfHeight = 12;
	/// 設定を反映する
	for (i=0;i<4;i++) pCheckLeg[i]->SetFont(&tFont);
	for (i=0;i<2;i++) pCheckMode[i]->SetFont(&tFont);

/**
 *	----------------------------------------
 *	フォントカラーの決定
 *	----------------------------------------
 */
	/// カラーオブジェクト
	tColorScheme tColor;
	/// 現在の設定を格納
	for (i=0;i<4;i++) pCheckLeg[i]->GetTextColor(&tColor);
	for (i=0;i<2;i++) pCheckMode[i]->GetTextColor(&tColor);

	/**
	 *	設定を変更する
	 */
	tColor.m_tEnabled = RGB(255, 255, 128);
	tColor.m_tClicked = RGB(16, 64, 255);
	/// 設定を反映する
	for (i=0;i<4;i++) pCheckLeg[i]->SetTextColor(&tColor);
	for (i=0;i<2;i++) pCheckMode[i]->SetTextColor(&tColor);
}

/// メッセージを送るスレッドIDの設定
void CAxisControlDialog::setMotionPlanThreadID(DWORD id)
{
	motionPlanThreadID = id;
	return;
}

/**
 *	------------------------------------------------------------
 *		CAxisControlDialogのprivateなメンバ関数
 *	------------------------------------------------------------
 */
void CAxisControlDialog::markSelectedLegButton(void)
{
	int i;
	for (i=0; i<4; i++)
	{
		if (i == selectedLegNo - 1) pCheckLeg[i]->SetCheck(true);
		else pCheckLeg[i]->SetCheck(false);
	}

	Invalidate(FALSE);
	return;
}

void CAxisControlDialog::markSelectedModeButton(void)
{
	int i;
	for (i=0; i<2; i++)
	{
		if (i == selectedCtrlMode -1) pCheckMode[i]->SetCheck(true);
		else pCheckMode[i]->SetCheck(false);
	}

	Invalidate(FALSE);
	return;
}

void CAxisControlDialog::updateDlgItem(void)
{
	if ( selectedLegNo == 0 )
		return;

	double data[AC_DLG_AXIS_NUM];

	/// 各種データの取得
	data[0] = ( dlgAsuraData.getLegJointAngle(selectedLegNo) )(1)*RAD2DEG;
	data[1] = ( dlgAsuraData.getLegJointAngle(selectedLegNo) )(2)*RAD2DEG;
	data[2] = ( dlgAsuraData.getLegJointAngle(selectedLegNo) )(3)*RAD2DEG;
	data[3] = ( dlgAsuraData.getLegFootPosition(selectedLegNo) )(1);
	data[4] = ( dlgAsuraData.getLegFootPosition(selectedLegNo) )(2);
	data[5] = ( dlgAsuraData.getLegFootPosition(selectedLegNo) )(3);


	/// editコントロールへの表示
	CString edit;

	edit.Format(TEXT( "%5.1lf"), data[0] ); SetDlgItemText(IDC_EDIT_JOINT1, edit);
	edit.Format(TEXT( "%5.1lf"), data[1] ); SetDlgItemText(IDC_EDIT_JOINT2, edit);
	edit.Format(TEXT( "%5.1lf"), data[2] ); SetDlgItemText(IDC_EDIT_JOINT3, edit);
	edit.Format(TEXT( "%5.1lf"), data[3] ); SetDlgItemText(IDC_EDIT_FOOTX, edit);
	edit.Format(TEXT( "%5.1lf"), data[4] ); SetDlgItemText(IDC_EDIT_FOOTY, edit);
	edit.Format(TEXT( "%5.1lf"), data[5] ); SetDlgItemText(IDC_EDIT_FOOTZ, edit);


	UpdateData(FALSE);

	/// SliderBarの変更
	int i;
	for ( i=0; i<AC_DLG_AXIS_NUM; i++ )
		pSliderCtrl[i]->SetPos( (int)(data[i]*SLDR_SCALE) );

	return;
}

