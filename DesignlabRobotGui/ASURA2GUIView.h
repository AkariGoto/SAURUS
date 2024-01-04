
#pragma once

#include "Data/asura_data.h"
#include "Data/plan_data.h"
#include "Graphic/AsuraGraphic.h"


class CASURA2GUIView final : public CFormView
{
    using AsuraData = designlab_robot_gui::data::AsuraData;
    using PlanData = designlab_robot_gui::data::PlanData;

protected: // シリアル化からのみ作成します。
    CASURA2GUIView() noexcept;
    DECLARE_DYNCREATE(CASURA2GUIView)

public:
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ASURA2GUI_FORM };
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

private:

    AsuraData viewAsuraXData;
    PlanData viewPlanData;

    Math::Vector* localFootPosition; //脚座標系による脚先位置
    Math::Vector* worldFootPosition; //ワールド座標系による脚先位置

    Graphic::AsuraGraphic* asuraXGraphic;

    DWORD motionPlanThreadID;
    DWORD AsuraUDPThreadID;

    /// タイマ識別子
    UINT_PTR	timerID;
    /// 現在時間
    double		presentTime;



    /**
     *	グラフィックを出力するピクチャボックスへのウィンドウ
     */
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

    //脚先の座標系のフラグ
    bool legCoordinateSystem;


    CStdioFile txtFile;

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
{
    return reinterpret_cast<CASURA2GUIDoc*>(m_pDocument);
}
#endif

