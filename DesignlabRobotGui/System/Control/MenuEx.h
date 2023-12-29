/**
 *  ファイル名
 *		MenuEx.h
 *  説明
 *		拡張メニュー（CMenuの拡張）
 *		 CMenuEXのインターフェイス
 *  日付
 *		作成日: 2007/06/11(MON)		更新日: 2007/06/18(MON)
 */

#pragma once

/**
 *	----------------------------------------------------------------------
 *		クラスの前方宣言
 *	----------------------------------------------------------------------
 */
class CMenuEx;

/**
 *	----------------------------------------------------------------------
 *		マクロ、型宣言
 *	----------------------------------------------------------------------
 */
/**
 *		MENUEXBITMAP 構造体
 */
typedef struct tagMENUEXBITMAP
{
	UINT	nID;			/// コマンドID
	UINT	nIDBmpRes;	/// ビットマップリソースID
} MENUEXBITMAP;

/**
 *		WMMENUITEM 構造体
 */
typedef struct tagMENUEXITEM
{
	CMenuEx*	pThisMenu;			/// MeasureItem でポップアップがサブメニューのハンドルになってしまうので自分へのポインタを持っておく
	CMenuEx*	pSubMenu;			/// サブメニューへのポインタ
	CString		sItemString;		/// アイテムの文字列
	CString		sItemAccelerator;	/// アイテムのショートカット文字列
	HBITMAP	hBitmap;				/// アイコン用のビットマップ
} MENUEXITEM;

/**
 *	----------------------------------------------------------------------
 *		CMenuExクラス
 *	----------------------------------------------------------------------
 */
class CMenuEx : public CMenu
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */

////////// アトリビュート //////////
private:
	UINT		m_cxMaxStr;				/// メニューの文字部分の最大幅
	UINT		m_cxMaxAccelerator;		/// メニューのショートカットキー部分の最大幅

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
	CMenuEx();
	virtual ~CMenuEx();

/**
 *	----------------------------------------
 *	メニューのロード／破棄
 *	----------------------------------------
 */
	/// メニューリソースのロード
	BOOL LoadMenu( LPCTSTR lpszResourceName, MENUEXBITMAP* pMenuBmps=NULL, int nBmps=0 );
	/// メニューリソースのロード
	BOOL LoadMenu( UINT nIDResource, MENUEXBITMAP* pMenuBmps=NULL, int nBmps=0 );
	/// メニュー破棄
	virtual BOOL DestroyMenu( CMenuEx* pMenu=NULL );

protected:
/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
	virtual void DrawItem( LPDRAWITEMSTRUCT pDis );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

private:
/**
 *	------------------------------------------------------------
 *		自作関数
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *		オペレーション関数
 *	----------------------------------------
 */
	/**
	 *		メニューアイテムにビットマップを関連付ける
	 */
	BOOL SetItemBitmaps( MENUEXBITMAP* pMenuBmps, int nBmps );
	/**
	 *		メニュー全てをオーナー描画にする
	 */
	void ToOwnerDraw( CMenu* pMenu, MENUEXBITMAP* pMenuBmp, int nBmps );

	/// アイコン
	/**
	 *		無効アイコンを描画するためのビットマップを生成
	 */	
	HBITMAP CreateGrayedBitmap( CDC* pDC, int w, int h );

	// メニュー描画
	/**
	 *		選択を描画
	 */
	void DrawSelected( CDC* pDC, CRect rItem, UINT itemState );
	
	/**
	 *		メニューの文字列部分を描画
	 */
	void DrawString( CDC* pDC, LPCTSTR pszStr, LPCTSTR pszAcl, CRect rItem, UINT itemState );
	
	/**
	 *		チェックマークを描画
	 */
	void DrawCheckMark( CDC* pDC, CRect rItem, UINT itemState );
	
	/**
	 *		セパレータを描画
	 */
	void DrawSeparator( CDC* pDC, CRect rItem );
	
	/**
	 *		ビットマップを描画
	 */
	void DrawBitmap( CDC* pDC, HBITMAP hMenuBmp, CRect rItem, UINT itemState );
};
