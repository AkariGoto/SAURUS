
// ASURA2GUIDoc.h : CASURA2GUIDoc クラスのインターフェイス
//

//20200821  スレッド関連

#pragma once


#include "System/Thread/MotionPlanThread.h"
#include "UDP/asura_udp_thread.h"

class CASURA2GUIDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
    CASURA2GUIDoc() noexcept;
    DECLARE_DYNCREATE(CASURA2GUIDoc)

    // 属性
public:

    // 操作
public:

    // オーバーライド
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

    // 実装
public:
    virtual ~CASURA2GUIDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // 生成された、メッセージ割り当て関数
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    // 検索ハンドラーの検索コンテンツを設定するヘルパー関数
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


    /**
     *	------------------------------------------------------------
     *		追加メンバ変数
     *	------------------------------------------------------------
     */
     //20200821
public:
    /**
     *	歩容計画を行うためのスレッド
     */
    designlab_robot_gui::system::MotionPlanThread* pMotionPlanThread;
    /**
     *	UDP通信を行うためのスレッド
     */

    designlab_robot_gui::udp::AsuraUdpThread* pUDPThread;


    //private:
public:
    /**
     *	動作計画
     */
     /// 前回の計画
    designlab_robot_gui::plan::Strategy lastStrategy;

    /// 選択した計画
    designlab_robot_gui::plan::Strategy currentStrategy;

    ///MODE_CHANGE後
    designlab_robot_gui::plan::Strategy nextStrategy;



    /**
     *	------------------------------------------------------------
     *		追加メンバ関数
     *	------------------------------------------------------------
     */
     //20200821
public:
    /**
     *	初期化処理
     */
    void initializeCASURA2GUIDoc(void);

    /**
     *	終了処理
     */
    void finalizeCASURA2GUIDoc(void);


};
