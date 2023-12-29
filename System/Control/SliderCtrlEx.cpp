/**
 *  ファイル名
 *		CSliderCtrlEx.cpp
 *  説明
 *		スライダーコントロールの拡張（CSliderCtrlの拡張）
 *		 CMenuEXのインターフェイス
 *  日付
 *		作成日: 2007/06/06(WED)		更新日: 2007/06/11(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\..\stdafx.h"

#include "SliderCtrlEx.h"
#include "..\ViewSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 *	----------------------------------------------------------------------
 *		CSliderCtrlExクラス
 *	----------------------------------------------------------------------
 */
IMPLEMENT_DYNAMIC(CSliderCtrlEx, CSliderCtrl)

BEGIN_MESSAGE_MAP(CSliderCtrlEx, CSliderCtrl)

		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
		ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		CSliderCtrlExクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// コンストラクタ
CSliderCtrlEx::CSliderCtrlEx()
{

	BkgndDCHandle = NULL;
	ThumbColor = NULL;
	ThumbColorSelected = NULL;
	isRedraw = false;
	channelBkColorBrush	= CreateSolidBrush( SLDR_CHANNEL_COLOR );
}

/// デストラクタ
CSliderCtrlEx::~CSliderCtrlEx()
{	
	/// 指定されたデバイスコンテキストのオブジェクトを選択
	::SelectObject(BkgndDCHandle, BkgndBMPHandleOld);
	
	/// オブジェクトの削除
	::DeleteObject(BkgndBMPHandle);
	
	/// デバイスコンテキストの削除
	::DeleteDC(BkgndDCHandle);			
	
	/// ブラシの削除
	DeleteObject(channelBkColorBrush);
}

/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *		CSliderCtrlExのメッセージハンドラ
 *	------------------------------------------------------------
 */
/**
 *		スライダーコントロールにより送られる親ウィンドウに通知する描画作業
 *		この通知はWM_NOTIFYメッセージ
 *		引数：
 *			pNMHDR:  NM_CUSTOMDRAW構造体へのポインタ
 *			pResult: 現在の描画状態による値
 */
void CSliderCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/// LPNMCUSTOMDRAWにキャスト
	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;
	
	/// HDCからCDCを作成
	CDC *pDC = CDC::FromHandle(lpcd->hdc);

	/// 現在の描画ステージにより動作を決定
	/**
	 *		CDDS_POSTERASE 消去サイクルが完了後 
	 *		CDDS_POSTPAINT 描画サイクル完了後 
	 *		CDDS_PREERASE 消去サイクルが始まる前 
	 *		CDDS_PREPAINT 描画サイクルが始まる前 
	 *		CDDS_ITEM dwItemSpec, uItemState, lItemlParamメンバが有効 
	 *		CDDS_ITEMPOSTERASE アイテムが消去された後 
	 *		CDDS_ITEMPOSTPAINT アイテムが描画された後 
	 *		CDDS_ITEMPREERASE アイテムが消去される前 
	 *		CDDS_ITEMPREPAINT アイテムが描画される前 
	 *		CDDS_SUBITEM Ver.4.71以降で有効
	 */
	switch (lpcd->dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		}
		case CDDS_ITEMPREPAINT:
		{
			/// アイテム番号により動作を決定
			switch(lpcd->dwItemSpec)
			{
				case TBCD_TICS:	/// スライダの目盛
				{
					*pResult = CDRF_DODEFAULT;
					break;
				}
				case TBCD_THUMB:	/// スライダのバー
				{
					DrawThumb(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				}
				case TBCD_CHANNEL:	/// スライダのチャンネル
				{
					DrawChannel(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				}
			}
			default:
				break;
		}	/// end of case CDDS_ITEMPREPAINT:
	}	/// end of switch (lpcd->dwDrawStage)

	return;
}

/**
 *		背景の消去
 */
BOOL CSliderCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

/**
 *		スライダのヘッド部分の色決定
 */
void CSliderCtrlEx::SetThumbColors(COLORREF face, COLORREF highlight)
{
	ThumbColor = face;
	ThumbColorSelected = highlight;
}

/**
 *		透過的な（トランスペアレント）な描画
 */
void CSliderCtrlEx::DrawTransparent(BOOL bRepaint)
{
	/// HDCとHBMPが有効かどうかのチェック
	if ( BkgndDCHandle != NULL && BkgndBMPHandleOld != NULL )
	{
		::SelectObject(BkgndDCHandle, BkgndBMPHandleOld);
	}

	/// HBMPとHDCの削除
	::DeleteObject(BkgndBMPHandle);
	::DeleteDC(BkgndDCHandle);

	BkgndDCHandle = NULL;
	BkgndBMPHandle = NULL;

	if (bRepaint == TRUE) 
	{
		Invalidate();
		EnableWindow(FALSE);
		EnableWindow(TRUE);
	}
}

/**
 *	------------------------------------------------------------
 *		CSliderCtrlExクラスのprivateなメンバ関数
 *		OnCustomDrawで呼ばれるヘルプ関数
 *	------------------------------------------------------------
 */
/**
 *		チャンネルの描画
 */
void CSliderCtrlEx::DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd)
{
	/// DCの取得
	/// GetParent(): 指定されたウィンドウの親ウィンドウまたはオーナーウィンドウのハンドルを取得
	CClientDC clientDC( GetParent() );


	CRect crect;
	CRect wrect;

	/// ウィンドウのクライアント領域の座標を取得
	GetClientRect(crect);
	/// ウィンドウの座標をスクリーン座標系で取得
	GetWindowRect(wrect);
	/// スクリーン上の点の座標をスクリーン座標から指定されたウィンドウのクライアント座標に変換
	GetParent()->ScreenToClient(wrect);

	/// HDCが無効だったら
	if (BkgndDCHandle == NULL)
	{
		BkgndDCHandle = CreateCompatibleDC(clientDC.m_hDC);
		BkgndBMPHandle = CreateCompatibleBitmap(clientDC.m_hDC, crect.Width(), crect.Height());
		BkgndBMPHandleOld = (HBITMAP)::SelectObject(BkgndDCHandle, BkgndBMPHandle);
		
		/// 画像のビットブロック転送を行う
		::BitBlt(BkgndDCHandle, 0, 0, crect.Width(), crect.Height(), clientDC.m_hDC, wrect.left, wrect.top, SRCCOPY);
		/**
		 *		これによりチェックマークを透過的に作成する
		 *		dcのメモリを作成しチェックマークを含む古いbmpデータのコピーを保持する
		 *		これはチェックマークを失であろう背景にチェックマークを加える時に備えてである
		 */
	}

	HDC hSaveHDC;
	HBITMAP hSaveBmp;

	int iWidth = crect.Width();
	int iHeight = crect.Height();
	
	hSaveHDC = ::CreateCompatibleDC(pDC->m_hDC);
	hSaveBmp = ::CreateCompatibleBitmap(hSaveHDC, iWidth, iHeight);
	
	HBITMAP hSaveCBmpOld = (HBITMAP)::SelectObject(hSaveHDC, hSaveBmp);			
	
	/// 単色マスクbmpのための色を設定する
	COLORREF crOldBack = ::SetBkColor(pDC->m_hDC, RGB(0,0,0));
	COLORREF crOldText = ::SetTextColor(pDC->m_hDC, RGB(255,255,255));		
	
	/// マスクを使ってイメージを転送する
	::BitBlt(hSaveHDC, 0, 0, iWidth, iHeight, pDC->m_hDC, crect.left, crect.top, SRCCOPY);
	::BitBlt(pDC->m_hDC, 0, 0, iWidth, iHeight, BkgndDCHandle, 0, 0, SRCCOPY);
	::BitBlt(pDC->m_hDC, 0, 0, iWidth, iHeight, hSaveHDC, 0, 0, SRCAND);
	
	/// 値の保持
	::SetBkColor(pDC->m_hDC, crOldBack);
	::SetTextColor(pDC->m_hDC, crOldText);
	::SelectObject(hSaveHDC, hSaveCBmpOld);
	
	/// オブジェクトの削除
	::DeleteObject(hSaveBmp);
	::DeleteDC(hSaveHDC);
	crect = lpcd->rc;
	
	if ( (crect.bottom - crect.top) > (crect.right - crect.left) )
		crect.InflateRect(10, 0, 10, 0);
	else
		crect.InflateRect(0, 10, 0, 10);

	/// 境界の描画
	DrawEdge(pDC->m_hDC, &crect, EDGE_BUMP, BF_MONO | BF_ADJUST);
	/// チャンネルの塗りつぶし
	FillRect(pDC->m_hDC, crect, channelBkColorBrush);

}

/**
 *		スライダバーの描画
 */
void CSliderCtrlEx::DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd)
{
	CRect crect;
	GetThumbRect(&crect);
	COLORREF col;

	if (lpcd->uItemState & CDIS_SELECTED)
		col = ThumbColorSelected;
	else
		col = ThumbColor;
	
	if (col == NULL && lpcd->uItemState & CDIS_SELECTED)
		col = GetSysColor(COLOR_3DHIGHLIGHT);
	else if (col == NULL && !(lpcd->uItemState & CDIS_SELECTED))
		col = GetSysColor(COLOR_3DFACE);
	
	/// ブラシの作成と選択
	HBRUSH hbrush = CreateSolidBrush(col);
    HBRUSH hbOld = (HBRUSH)SelectObject(pDC->m_hDC, hbrush);

	/// ダイアモンド形のTHUMBを描画
	int xx, yy, dx, dy, cx, cy;
	/// THUMBの外形
	xx = lpcd->rc.left;
	yy = lpcd->rc.top;
	/// 差分
	dx = 3;
	dy = 3;

	cx = lpcd->rc.right - xx - 1;
	cy = lpcd->rc.bottom - yy - 1;
	
	/// 頂点をPOINT構造体に格納
	POINT pts[8] = { {xx + dx, yy}, 
							{xx, yy + dy},
							{xx, yy + cy - dy}, 
							{xx + dx, yy + cy},
							{xx + cx - dx, yy + cy}, 
							{xx + cx, yy + cy - dy}, 
							{xx + cx, yy + dy}, 
							{xx + cx -dx, yy} };

	/// ポリゴンでTHUMBの描画
	pDC->Polygon( pts, 8 );
   
	SelectObject(pDC->m_hDC, hbOld);
    
	DeleteObject(hbrush);
}
