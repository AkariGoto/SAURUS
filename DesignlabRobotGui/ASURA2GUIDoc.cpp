
// ASURA2GUIDoc.cpp : CASURA2GUIDoc クラスの実装
//

//20200821  スレッド関連

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "ASURA2GUI.h"
#endif

#include "ASURA2GUIDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CASURA2GUIDoc

IMPLEMENT_DYNCREATE(CASURA2GUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CASURA2GUIDoc, CDocument)
END_MESSAGE_MAP()


// CASURA2GUIDoc コンストラクション/デストラクション

CASURA2GUIDoc::CASURA2GUIDoc() noexcept
{
    // TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。
    initializeCASURA2GUIDoc();  //20200821
}

CASURA2GUIDoc::~CASURA2GUIDoc()
{
    finalizeCASURA2GUIDoc();  //20200821
}

BOOL CASURA2GUIDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: この位置に再初期化処理を追加してください。
    // (SDI ドキュメントはこのドキュメントを再利用します。

    return TRUE;
}




// CASURA2GUIDoc のシリアル化

void CASURA2GUIDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: 格納するコードをここに追加してください。
    }
    else
    {
        // TODO: 読み込むコードをここに追加してください。
    }
}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void CASURA2GUIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
    // このコードを変更してドキュメントのデータを描画します
    dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

    CString strText = _T("TODO: implement thumbnail drawing here");
    LOGFONT lf;

    CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
    pDefaultGUIFont->GetLogFont(&lf);
    lf.lfHeight = 36;

    CFont fontDraw;
    fontDraw.CreateFontIndirect(&lf);

    CFont* pOldFont = dc.SelectObject(&fontDraw);
    dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
    dc.SelectObject(pOldFont);
}

// 検索ハンドラーのサポート
void CASURA2GUIDoc::InitializeSearchContent()
{
    CString strSearchContent;
    // ドキュメントのデータから検索コンテンツを設定します。
    // コンテンツの各部分は ";" で区切る必要があります

    // 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
    SetSearchContent(strSearchContent);
}

void CASURA2GUIDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl* pChunk = nullptr;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != nullptr)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}

#endif // SHARED_HANDLERS

// CASURA2GUIDoc の診断

#ifdef _DEBUG
void CASURA2GUIDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CASURA2GUIDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


void CASURA2GUIDoc::initializeCASURA2GUIDoc()
{
    /// ドキュメントのポインタを取得
    theApp.pASURA2GUIDoc = this;

    /// 動作計画の初期化
    lastStrategy = designlab_robot_gui::plan::Strategy::NO_STRATEGY;
    currentStrategy = designlab_robot_gui::plan::Strategy::NO_STRATEGY;
    // lastStrategy = designlab_robot_gui::plan::TRIPOD;
    // currentStrategy = designlab_robot_gui::plan::TRIPOD;

    /// スレッドの作成 MotionPlanThread
    pMotionPlanThread = (MotionPlanThread*)AfxBeginThread
    (
      RUNTIME_CLASS(MotionPlanThread),		/// CWinThread から派生されたオブジェクトの RUNTIME_CLASS。
      THREAD_PRIORITY_NORMAL,				/// スレッドの優先度
      0,											/// 新しいスレッドのスタックサイズをバイト数で指定．0 のときはスレッドを作成したプロセスのスタックサイズと同じ
      CREATE_SUSPENDED,							/// スレッドの作成を制御するフラグ CREATE_SUSPENDED or 0
      NULL

    );

    /// スレッドの開始
    /// CWinThread オブジェクトのメンバ データは初期化済
    pMotionPlanThread->ResumeThread();

    /// スレッドの生成待機
    while (pMotionPlanThread->isRunning()) {}

    /// CAsuraWare内にスレッドIDを保持
    theApp.motionPlanThreadID = pMotionPlanThread->m_nThreadID;


    /// スレッドの作成 AsuraUDPThread
    pUDPThread = (AsuraUDPThread*)AfxBeginThread
    (
      RUNTIME_CLASS(AsuraUDPThread),		/// CWinThread から派生されたオブジェクトの RUNTIME_CLASS。
      THREAD_PRIORITY_NORMAL,				/// スレッドの優先度
      0,											/// 新しいスレッドのスタックサイズをバイト数で指定．0 のときはスレッドを作成したプロセスのスタックサイズと同じ
      CREATE_SUSPENDED,						/// スレッドの作成を制御するフラグ CREATE_SUSPENDED or 0
      NULL
    );

    /// スレッドの開始
    /// CWinThread オブジェクトのメンバ データは初期化済
    pUDPThread->ResumeThread();

    /// スレッドの生成待機
    while (pUDPThread->isRunning()) {}

    /// CAsuraWare内にスレッドIDを保持
    theApp.AsuraUDPThreadID = pUDPThread->m_nThreadID;

    //
    pMotionPlanThread->acquireAsuraUDPThread(pUDPThread);

    PostThreadMessage(pMotionPlanThread->m_nThreadID, WM_PLAN_SETUP_MOTION, (WPARAM)lastStrategy, (LPARAM)currentStrategy);  //20200821


    return;
}

/**
 *	終了処理
 */
 //20200821
void CASURA2GUIDoc::finalizeCASURA2GUIDoc(void)
{
    pMotionPlanThread->PostThreadMessage(WM_PLAN_END_THREAD, NULL, NULL);

    /// スレッドの破棄待機
    while (!pMotionPlanThread->isRunning()) {}

    pUDPThread->PostThreadMessage(WM_DLG_UDPTHREAD_END, NULL, NULL);

    /// スレッドの破棄待機
    while (!pUDPThread->isRunning()) {}


    return;
}