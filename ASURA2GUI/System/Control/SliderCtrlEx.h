/**
 *  ファイル名
 *		CSliderCtrlEx.h
 *  説明
 *		スライダーコントロールの拡張（CSliderCtrlの拡張）
 *		 CMenuEXのインターフェイス
 *  日付
 *		作成日: 2007/06/06(WED)		更新日: 2007/06/11(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		CSliderCtrlExクラス
 *	----------------------------------------------------------------------
 */
class CSliderCtrlEx : public CSliderCtrl
{
	DECLARE_DYNAMIC(CSliderCtrlEx)

/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
	/// 背景のデバイスコンテキストハンドル
	HDC			BkgndDCHandle;
	/// 背景のBMPハンドル
	HBITMAP	BkgndBMPHandle;
	/// 以前の背景のBMPハンドル
	HBITMAP	BkgndBMPHandleOld;
	/// スライダーヘッドの色
	COLORREF	ThumbColor;
	/// 選ばれたスライダーバーの色
	COLORREF	ThumbColorSelected;
	/// 再描画のフラグ
	bool			isRedraw;
	
	/// チャンネルの塗りつぶし
	HBRUSH channelBkColorBrush;


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
	CSliderCtrlEx(); // 標準コンストラクタ
	virtual ~CSliderCtrlEx();

	DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *	------------------------------------------------------------
 */
protected:
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	/**
	 *		スライダのバー部分の色決定
	 */
	void SetThumbColors(COLORREF face, COLORREF highlight);
	/**
	 *		トランスペアレントな描画
	 */
	void DrawTransparent(BOOL bRepaint);

private:
	/// チャンネルの描画
	void DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd);
	/// スライダバーの描画
	void DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd);
};