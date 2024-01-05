
// ASURA2GUIView.cpp : CASURA2GUIView クラスの実装
//

//  20200819  OpenGL関連追加
//  20200820  ViewSetting.hを使用しない
//  20200821  スレッド関連
//  20200824
//  20200928
//  20201005  1周期で終了
//  20201013  第4関節
//  20201019  脚先のワールド座標と脚座標の切り替え
//  20221026  ログ追加


#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "ASURA2GUI.h"
#endif

#include "ASURA2GUIDoc.h"
#include "ASURA2GUIView.h"

/**
 *		追加ヘッダ
 */
#include "Plan/plan_parameter.h"  //20200824
#include "System\Console.h"


using namespace Const;  //20200819
using namespace Graphic;  //  20200819
using namespace Plan;  //20200824

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 *	------------------------------------------------------------
 *		ASURA2GUIViewクラスの変数の初期化
 *	------------------------------------------------------------
 */




 // CASURA2GUIView

IMPLEMENT_DYNCREATE(CASURA2GUIView, CFormView)

BEGIN_MESSAGE_MAP(CASURA2GUIView, CFormView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()

    ON_BN_CLICKED(IDC_CHECK_DRIVE, &CASURA2GUIView::OnClickedCheckDrive)
    ON_BN_CLICKED(IDC_CHECK_COMM, &CASURA2GUIView::OnClickedCheckComm)
    ON_BN_CLICKED(IDC_CHECK_LOG, &CASURA2GUIView::OnClickedCheckLog)

    /// カスタムメッセージ
    ON_MESSAGE(WM_PLAN_SENDING_VIEW_DATA, &CASURA2GUIView::OnRecieveViewData)
    ON_BN_CLICKED(IDC_BUTTON_SET, &CASURA2GUIView::OnClickedButtonSet)
END_MESSAGE_MAP()

// CASURA2GUIView コンストラクション/デストラクション

CASURA2GUIView::CASURA2GUIView()  noexcept
    : CFormView(IDD_ASURA2GUI_FORM)
{
    // TODO: 構築コードをここに追加します。

}

CASURA2GUIView::~CASURA2GUIView()
{
}

void CASURA2GUIView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}

BOOL CASURA2GUIView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
    //  修正してください。

    return CFormView::PreCreateWindow(cs);
}

void CASURA2GUIView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();
    ResizeParentToFit();

    //20201019
    /*
    CButton* radioWorld = (CButton*)GetDlgItem(IDC_RADIO_WORLD);
    radioWorld->SetCheck(1);
    */
    CButton* radioLeg = (CButton*)GetDlgItem(IDC_RADIO_LEG);
    radioLeg->SetCheck(1);

    CButton* chkbox1 = (CButton*)GetDlgItem(IDC_CHECK_1_CYCLE);
    chkbox1->SetCheck(1);

    /// グラフィックの初期化
    initializeGraphics();  //  20200819
}


// CASURA2GUIView の診断

#ifdef _DEBUG
void CASURA2GUIView::AssertValid() const
{
    CFormView::AssertValid();
}

void CASURA2GUIView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}

CASURA2GUIDoc* CASURA2GUIView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CASURA2GUIDoc)));
    return (CASURA2GUIDoc*)m_pDocument;
}
#endif //_DEBUG


/**
 *	------------------------------------------------------------
 *		追加メンバ関数
 *	------------------------------------------------------------
 */
 /**
  *	初期化
  */
void CASURA2GUIView::initializeCASURA2GUIView(void)
{
    //20201019
    localFootPosition = new Math::Vector[Asura::LEG_NUM];
    worldFootPosition = new Math::Vector[Asura::LEG_NUM];
    for (int i = 0; i < Asura::LEG_NUM; i++)
    {
        localFootPosition[i].setSize(DH_DIMENSION);
        worldFootPosition[i].setSize(DH_DIMENSION);
    }

    /// フラグ類の初期化
    isScreenDrawn = true;

    isDriving = false;
    isCommunicating = false;
    isLogging = false;

    legCoordinateSystem = false;  //20201019  最初はワールド座標系に設定

    // timerIDの決定
    timerID = 1;

    /// コンソール作成
    Console::createConsole();

    /// Windowsのタイマセット（スクリーン更新用タイマ）
    //SetTimer(timerID, 100, NULL);      //100msごとにOnTimerを実行
    SetTimer(timerID, 20, NULL);      //20msごとにOnTimerを実行  20221026]

    CASURA2GUIDoc* pDoc = GetDocument();
    motionPlanThreadID = pDoc->pMotionPlanThread->m_nThreadID;

    AsuraUDPThreadID = pDoc->pUDPThread->m_nThreadID; //20210617 実験的にUDPThreadID to AsuraUDPThreadID

    //20221026
    //書き込みモード
    if (txtFile.Open(L"log.txt", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite) == NULL)
    {
    }
}

/**
 *	終了処理
 */
void CASURA2GUIView::finalizeCASURA2GUIView(void)
{
    //20201019  脚先の座標系
    delete[] localFootPosition;
    delete[] worldFootPosition;

    //ファイルクローズ
    txtFile.Close();  //20221026

    /// グラフィックの終了処理
    finalizeGraphics();

    /// Windowsのタイマ破棄
    KillTimer(timerID);

    /// コンソール破棄
    Console::destroyConsole();
}

/**
 *	グラフィックの初期化
 */
void CASURA2GUIView::initializeGraphics(void)
{
    /**
     *		ピクチャコントロール（スクリーン）の設定
     */
     //CWnd* pictWnd[SCREEN_NUMBER];
    CWnd* pictWnd;  //20200820
    /// メインスクリーン
    //pictWnd[0] = (CWnd*)GetDlgItem(IDC_MAIN_SCREEN);
    pictWnd = (CWnd*)GetDlgItem(IDC_MAIN_SCREEN);  //20200820
    //pictWndHandle[0] = pictWnd[0]->GetSafeHwnd();
    pictWndHandle = pictWnd->GetSafeHwnd();  //20200820
    /*  20200820
    if (::IsWindow(pictWndHandle[0]))
    {
      /// メインスクリーン作成
      asuraXGraphic[0] = new AsuraGraphic(pictWndHandle[0], &viewAsuraXData, PERSPECTIVE);
      asuraXGraphic[0]->startRecording();
    }
    */
    if (::IsWindow(pictWndHandle))
    {
        /// メインスクリーン作成
        asuraXGraphic = new AsuraGraphic(pictWndHandle, &viewAsuraXData, PERSPECTIVE);
        asuraXGraphic->startRecording();
    }
    //20200820

    /**
     *	ピクチャコントロールのサブクラス化
     */
     /**
      *	メイン
      */
      /*  20200820
      orgWndProcedure[0] = (WNDPROC)(UINT_PTR)GetWindowLongPtr(pictWndHandle[0], GWLP_WNDPROC);
      /// ウィンドウハンドルとCAsuraWareViewを結びつける
      SetWindowLongPtr(pictWndHandle[0], GWLP_USERDATA, (LONG)(LONG_PTR)this);
      /// Windowプロシージャの置き換え
      SetWindowLongPtr(
        pictWndHandle[0],						/// 指定するWindowハンドル
        GWLP_WNDPROC,							/// 変更する属性を表す定数
        (LONG)(LONG_PTR)drawMainScrWndProc		/// 新しく設定する値
      );
      */

    orgWndProcedure = (WNDPROC)(UINT_PTR)GetWindowLongPtr(pictWndHandle, GWLP_WNDPROC);
    /// ウィンドウハンドルとCAsuraWareViewを結びつける
    SetWindowLongPtr(pictWndHandle, GWLP_USERDATA, (LONG)(LONG_PTR)this);
    /// Windowプロシージャの置き換え
    SetWindowLongPtr(
      pictWndHandle,						/// 指定するWindowハンドル
      GWLP_WNDPROC,							/// 変更する属性を表す定数
      (LONG)(LONG_PTR)drawMainScrWndProc		/// 新しく設定する値
    );
    //20200820

    return;
}

/**
 *	グラフィックの終了処理
 */
void CASURA2GUIView::finalizeGraphics(void)
{

    /**
     *	ウィンドウプロシージャを元に戻す
     *		サブクラス化解除
     */

     /*
     int i;
     for (i = 0; i < SCREEN_NUMBER; i++)
       SetWindowLongPtr(pictWndHandle[i], GWLP_WNDPROC, (LONG)(LONG_PTR)orgWndProcedure[i]);
     */
    SetWindowLongPtr(pictWndHandle, GWLP_WNDPROC, (LONG)(LONG_PTR)orgWndProcedure);  //20200820

    /// グラフィックオブジェクトの破棄
    //asuraXGraphic[0]->stopRecording();
    asuraXGraphic->stopRecording();  //20200820
    /*
    int j;
    for (j = 0; j < SCREEN_NUMBER; j++)
      delete[] asuraXGraphic[j];
    */
    delete asuraXGraphic;
}

/**
 *		表示更新
 */
void CASURA2GUIView::updateFormView(void)
{
    //20201019
    CButton* radioWorld = (CButton*)GetDlgItem(IDC_RADIO_WORLD);
    CButton* radioLeg = (CButton*)GetDlgItem(IDC_RADIO_LEG);
    if (radioWorld->GetCheck())
    {
        legCoordinateSystem = false;
        asuraXGraphic->legCoordinateOff();
    }
    else if (radioLeg->GetCheck())
    {
        legCoordinateSystem = true;
        asuraXGraphic->legCoordinateOn();
    }

    /// 画面の更新
    CWnd* pWnd0 = (CWnd*)GetDlgItem(IDC_MAIN_SCREEN);
    if (::IsWindow(pWnd0->GetSafeHwnd()))
        pWnd0->Invalidate(FALSE);


    CString data;

    //パラメータを表示
    //重心ストライド
    data.Format(TEXT("%5.1lf"), TRIPODGAIT_STRIDE);
    SetDlgItemText(IDC_EDIT_STRIDE, data);
    //歩行速度
    data.Format(TEXT("%5.1lf"), TRIPODGAIT_WALKING_SPEED);
    SetDlgItemText(IDC_EDIT_WALKING_SPEED, data);
    //振り上げ
    data.Format(TEXT("%5.1lf"), TRIPODGAIT_SWING_UP[2]);
    SetDlgItemText(IDC_EDIT_SWING_UP, data);
    //振り下げ  20200928
    data.Format(TEXT("%5.1lf"), TRIPODGAIT_SWING_DOWN[2]);
    SetDlgItemText(IDC_EDIT_SWING_DOWN, data);
    //歩行周期
    data.Format(TEXT("%5.3lf"), TRIPODGAIT_STRIDE / TRIPODGAIT_WALKING_SPEED / TRIPODGAIT_DUTY_FACTOR);
    SetDlgItemText(IDC_EDIT_CYCLE_TIME, data);


    /// 時間を更新
    data.Format(TEXT("%5.3lf"), viewPlanData.elapsed_time); SetDlgItemText(IDC_ELAPSED_TIME, data);

    //ログ取得
    if (isDriving)
    {
        txtFile.WriteString(data);
        txtFile.WriteString(L",");
    }

    //関節の表示
    double joint4 = 0;
    data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_joint_angle[1](1) * RAD2DEG);
    SetDlgItemText(IDC_JOINT1, data);

    //ログ取得
    if (isDriving)
    {
        txtFile.WriteString(data);
        txtFile.WriteString(L",");
    }

    data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_joint_angle[1](2) * RAD2DEG);
    SetDlgItemText(IDC_JOINT2, data);

    //ログ取得
    if (isDriving)
    {
        txtFile.WriteString(data);
        txtFile.WriteString(L",");
    }

    data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_joint_angle[1](3) * RAD2DEG);
    SetDlgItemText(IDC_JOINT3, data);

    //ログ取得
    if (isDriving)
    {
        txtFile.WriteString(data);
        txtFile.WriteString(L",");
    }

    //joint4 = Const::PI / 2 - viewAsuraXData.getLegJointAngle(2)(2) - viewAsuraXData.getLegJointAngle(2)(3);
    //data.Format(TEXT("%5.1lf"), joint4 * RAD2DEG);
    //SetDlgItemText(IDC_JOINT4, data);
    data.Format(TEXT("%5.1lf"), viewAsuraXData.foot_joint_angle[1] * RAD2DEG);
    SetDlgItemText(IDC_JOINT4, data);

    //ログ取得
    if (isDriving)
    {
        txtFile.WriteString(data);
        txtFile.WriteString(L",");
    }

    //アクチュエータ

    double shaft_diameter = 11.5;

    data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_actuator_position[1](1) / (shaft_diameter * Const::PI) * 360 * 3 * 18);
    //data.Format(TEXT("%5.1lf"), viewAsuraXData.getLegActuatorPosition(2)(1));
    SetDlgItemText(IDC_ACT1, data);
    data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_actuator_position[1](2) / (shaft_diameter * Const::PI) * 360 * 3 * 18);
    SetDlgItemText(IDC_ACT2, data);
    data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_actuator_position[1](3) / (shaft_diameter * Const::PI) * 360 * 3 * 18);
    SetDlgItemText(IDC_ACT3, data);
    data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_actuator_position[1](4) / (shaft_diameter * Const::PI) * 360 * 3 * 18);
    SetDlgItemText(IDC_ACT4, data);

    //脚先位置の表示
    //20201019
    if (legCoordinateSystem)
    {
        worldFootPosition[2](1) = viewAsuraXData.leg_foot_position[1](1);
        worldFootPosition[2](2) = viewAsuraXData.leg_foot_position[1](2);
        worldFootPosition[2](3) = viewAsuraXData.leg_foot_position[1](3);
        worldFootPosition[2](4) = 1;
        localFootPosition[2] = viewAsuraXData.leg_base_transformation[1].inversion() * worldFootPosition[2];
        data.Format(TEXT("%5.1lf"), localFootPosition[2](1));
        SetDlgItemText(IDC_FOOTX, data);

        //ログ取得
        if (isDriving)
        {
            txtFile.WriteString(data);
            txtFile.WriteString(L",");
        }

        data.Format(TEXT("%5.1lf"), localFootPosition[2](2));
        SetDlgItemText(IDC_FOOTY, data);

        //ログ取得
        if (isDriving)
        {
            txtFile.WriteString(data);
            txtFile.WriteString(L",");
        }

        data.Format(TEXT("%5.1lf"), localFootPosition[2](3));
        SetDlgItemText(IDC_FOOTZ, data);

        //ログ取得
        if (isDriving)
        {
            txtFile.WriteString(data);
            txtFile.WriteString(L",\n");
        }

    }
    else
    {
        data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_foot_position[1](1));
        SetDlgItemText(IDC_FOOTX, data);

        //ログ取得
        if (isDriving)
        {
            txtFile.WriteString(data);
            txtFile.WriteString(L",");
        }

        data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_foot_position[1](2));
        SetDlgItemText(IDC_FOOTY, data);

        //ログ取得
        if (isDriving)
        {
            txtFile.WriteString(data);
            txtFile.WriteString(L",");
        }

        data.Format(TEXT("%5.1lf"), viewAsuraXData.leg_foot_position[1](3));
        SetDlgItemText(IDC_FOOTZ, data);

        //ログ取得
        if (isDriving)
        {
            txtFile.WriteString(data);
            txtFile.WriteString(L",\n");
        }
    }

    UpdateData(FALSE);

    return;
}


// CASURA2GUIView メッセージ ハンドラー

int CASURA2GUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFormView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: ここに特定な作成コードを追加してください。
    initializeCASURA2GUIView();

    return 0;
}


void CASURA2GUIView::OnDestroy()
{
    CFormView::OnDestroy();

    // TODO: ここにメッセージ ハンドラー コードを追加します。
    finalizeCASURA2GUIView();
}

HBRUSH CASURA2GUIView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: ここで DC の属性を変更してください。

    // TODO: 既定値を使用したくない場合は別のブラシを返します。
    return hbr;
}

void CASURA2GUIView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    //20200821
    /// 表示データ要求
    PostThreadMessage(motionPlanThreadID, WM_PLAN_REQUEST_VIEW_DATA, (WPARAM)this->m_hWnd, NULL);

    CFormView::OnTimer(nIDEvent);
}


void CASURA2GUIView::OnClickedCheckDrive()
{
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
    //20200821
    /// XORでフラグ反転
    isDriving ^= true;

    if (isDriving)
    {
        /// 計画スレッドへ動作開始通知
        PostThreadMessage(motionPlanThreadID, WM_PLAN_START_MOTION, (WPARAM)this->m_hWnd, NULL);
    }
    else
    {
        /// 計画スレッドへ動作停止通知
        PostThreadMessage(motionPlanThreadID, WM_PLAN_STOP_MOTION, (WPARAM)this->m_hWnd, NULL);
    }
}


void CASURA2GUIView::OnClickedCheckComm()
{
    // TODO: ここにコントロール通知ハンドラー コードを追加します。


    //PostThreadMessage(UDPThreadID, WM_DLG_COMM_START_LISTEN, (WPARAM)this->m_hWnd, NULL);

    // TODO: ここにコントロール通知ハンドラ コードを追加します。
    /// XORでフラグ反転

    isCommunicating ^= true;

    /// 通信の開始/停止
    if (isCommunicating)
    {
        /// UDP通信スレッドに通知
        PostThreadMessage(AsuraUDPThreadID, WM_DLG_COMM_START_LISTEN, (WPARAM)this->m_hWnd, NULL); //20210617 実験的にUDPThreadID to AsuraUDPThreadID

        /// 計画スレッドに通知
        PostThreadMessage(theApp.motionPlanThreadID, WM_PLAN_COMM_START, NULL, NULL);

        /// ボタンの色変更
        //m_CheckComm.SetCheck(true);
    }
    else
    {
        /// UDP通信スレッドに通知
        PostThreadMessage(AsuraUDPThreadID, WM_DLG_COMM_STOP_LISTEN, (WPARAM)this->m_hWnd, NULL); //20210617 実験的にUDPThreadID to AsuraUDPThreadID

        /// 計画スレッドに通知
        PostThreadMessage(theApp.motionPlanThreadID, WM_PLAN_COMM_STOP, NULL, NULL);

        /// ボタンの色を元に戻す
        //m_CheckComm.SetCheck(false);
    }




}


void CASURA2GUIView::OnClickedCheckLog()
{
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
}

/**
 *	------------------------------------------------------------
 *		CAsuraWareViewの privateなメンバ関数
 *	------------------------------------------------------------
 */
 /**
  *	サブクラス化したコントロールのウィンドウプロシージャ
  */
LRESULT CALLBACK CASURA2GUIView::drawMainScrWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    /**
     *		ローカル変数
     */
     /// 戻り値
    LRESULT lResult = 0;
    /// 視点変更の基点
    int x = 0;
    int y = 0;

    int xDelta = 0;
    int yDelta = 0;
    int zDelta = 0;

    /// BeginPaintのための変数
    PAINTSTRUCT ps;
    HDC	hDC;
    CDC dc;

    /// CAsuraWareViewオブジェクトへのポインタを取得
    CASURA2GUIView* pView = (CASURA2GUIView*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    /// ポインタが有効だったら
    if (pView != NULL && ::IsWindow(hWnd))
    {
        switch (uMsg)		// ウィンドウズのメッセージにより各種動作を行う
        {
            case WM_PAINT:
                /// 目的のピクチャボックスをレンダリング
                hDC = ::BeginPaint(hWnd, &ps);
                dc.Attach(hDC);

                //pView->asuraXGraphic[0]->renderScenes();
                pView->asuraXGraphic->renderScenes();  //20200820

                dc.Detach();
                ::EndPaint(hWnd, &ps);

                //lResult = CallWindowProc(pView->orgWndProcedure[0], hWnd, uMsg, wParam, lParam);
                lResult = CallWindowProc(pView->orgWndProcedure, hWnd, uMsg, wParam, lParam);  //20200820
                break;

            case WM_LBUTTONDOWN:
                x = GET_X_LPARAM(lParam);
                y = GET_Y_LPARAM(lParam);
                //pView->asuraXGraphic[0]->beginCameraViewControl(CameraView::SPIN, x, y);
                pView->asuraXGraphic->beginCameraViewControl(CameraView::SPIN, x, y);  //20200820
                break;

            case WM_LBUTTONUP:
                //pView->asuraXGraphic[0]->endCameraViewControl();
                pView->asuraXGraphic->endCameraViewControl();  //20200820
                break;

            case WM_MBUTTONDOWN:
                x = GET_X_LPARAM(lParam);
                y = GET_Y_LPARAM(lParam);
                //pView->asuraXGraphic[0]->beginCameraViewControl(CameraView::PAN, x, y);
                pView->asuraXGraphic->beginCameraViewControl(CameraView::PAN, x, y);  //20200820
                break;

            case WM_MBUTTONUP:
                //pView->asuraXGraphic[0]->endCameraViewControl();
                pView->asuraXGraphic->endCameraViewControl();  //20200820
                break;

            case WM_MOUSEWHEEL:
                x = GET_X_LPARAM(lParam);
                y = GET_Y_LPARAM(lParam);
                //pView->asuraXGraphic[0]->beginCameraViewControl(CameraView::ZOOM, x, y);
                pView->asuraXGraphic->beginCameraViewControl(CameraView::ZOOM, x, y);  //20200820

                zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                xDelta = x - zDelta / 10;
                yDelta = y - zDelta / 10;
                //pView->asuraXGraphic[0]->doCameraViewControl(xDelta, yDelta);
                pView->asuraXGraphic->doCameraViewControl(xDelta, yDelta);  //20200820

                //pView->asuraXGraphic[0]->endCameraViewControl();
                pView->asuraXGraphic->endCameraViewControl();  //20200820
                break;

            case WM_MOUSEMOVE:
                x = GET_X_LPARAM(lParam);
                y = GET_Y_LPARAM(lParam);
                //pView->asuraXGraphic[0]->doCameraViewControl(x, y);
                pView->asuraXGraphic->doCameraViewControl(x, y);  //20200820
                break;

            default:
                /// デフォルトは置き換える前のウィンドウプロシージャを呼ぶ
                lResult = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
                break;
        }

    }

    return lResult;

}

/**
 *	------------------------------------------------------------
 *		Windows カスタムメッセージハンドラ
 *	------------------------------------------------------------
 */
 //20200821
afx_msg LRESULT CASURA2GUIView::OnRecieveViewData(WPARAM wParam, LPARAM lParam)
{
    /// データ更新
    if ((AsuraData*)wParam != NULL)
        viewAsuraXData = *((AsuraData*)wParam);

    if ((PlanData*)lParam != NULL)
        viewPlanData = *((PlanData*)lParam);

    /// 表示更新
    updateFormView();

    return 1;

    //return 0;
}

void CASURA2GUIView::OnClickedButtonSet()
{
    // TODO: ここにコントロール通知ハンドラー コードを追加します。
    if (!isDriving)
    {
        CASURA2GUIDoc* myDOC = (CASURA2GUIDoc*)GetDocument();
        myDOC->lastStrategy = myDOC->currentStrategy;
        //myDOC->currentStrategy = Plan::TRIPOD;  20201005
        //20201005
        CButton* chkbox1 = (CButton*)GetDlgItem(IDC_CHECK_1_CYCLE);
        if (chkbox1->GetCheck())
        {
            myDOC->currentStrategy = Plan::Strategy::TRIPOD_1_CYCLE;
        }
        else
        {
            myDOC->currentStrategy = Plan::Strategy::TRIPOD;
        }


        PostThreadMessage(motionPlanThreadID, WM_PLAN_SETUP_MOTION, (WPARAM)myDOC->lastStrategy, (LPARAM)myDOC->currentStrategy);
    }
}
