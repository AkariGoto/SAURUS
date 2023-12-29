/**
 *  ファイル名
 *		AviMaker.h
 *  説明
 *		AVIを作成するクラス
 *  日付
 *		作成日: 2008/09/02(TUE)		更新日: 2008/09/03(WED)
 */

#ifndef __AviMaker_h__
#define __AviMaker_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <vfw.h>
#include <TChar.h>

/**
 *	----------------------------------------------------------------------
 *		リンカへの通知
 *	----------------------------------------------------------------------
 */
#pragma comment (lib, "vfw32.lib")

/**
 *	----------------------------------------------------------------------
 *		AviMakerクラス
 *	----------------------------------------------------------------------
 */
class AviMaker
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
 
 /**
 *	----------------------------------------
 *	ビットマップの操作に関連するもの
 *	----------------------------------------
 */ 
	/// 描画領域のデバイスコンテキスト
	HDC					memDCHandle;
	/// ヒープへのハンドル
	HANDLE				heapHandle;
	/**
	 *	DIB（ビットマップ）のビット配列をコピーするバッファへのポインタ
	 *	DIB（ビットマップ）のコピーに利用する
	 */
	LPVOID				lpDIBPixelBits;

/**
 *	----------------------------------------
 *	AVIファイルの操作に関連するもの
 *	----------------------------------------
 */ 
	/// AVIファイルのハンドル	
	PAVIFILE			pAviFile;
	PAVISTREAM			pAviStream;
	PAVISTREAM			pAviCompressedStream;

	AVISTREAMINFO		aviStreamInfo;
	AVICOMPRESSOPTIONS	aviCompressOptions;

	/**
	 *	開始サンプル番号
	 *	現在のフレームインデックスのトラックを保持する
	 */
	LONG				frameCounter;
	
/**
 *	----------------------------------------
 *	AVIファイルの設定に関連するもの
 *	----------------------------------------
 */
	/// フレームレート（FPS）
	DWORD				frameRate;
	/// ビデオコーデック（FOURCC コード用）
	DWORD				fccHandler;
	/// 出力ファイル名
	TCHAR				outputFileName[MAX_PATH];
	/// 最終エラーメッセージ
	TCHAR				lastErrMsg[MAX_PATH];
	/**
	 *	追加関数のセレクタ
	 *	0=Dummy	1=FirstTime	2=Usual
	 */
	int					appendFuncSelector;

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
	/**
	 *	説明
	 *		デフォルトコンストラクタ
	 *	引数
	 *		pFileName: 作成するファイル名
	 *		codec_:	圧縮に使用するビデオコーデックのFOURCC コード（インストール済みのコーデックを選択すること）
	 *				デフォルトはWMV3。例えば、DIVXなら mmioFOURCC('D','I','V','X') とする
	 *				0ならば無圧縮
	 *		frameRate_: フレームレート
	 */
	explicit AviMaker(LPCTSTR pFileName = _T("Output.avi"), DWORD codec_ = mmioFOURCC('W','M','V','3'), DWORD frameRate_ = 1);

	/**
	 *	説明
	 *		デストラクタ
	 *	引数
	 *		動画ファイルを閉じて、全フレームを消去
	 */
	virtual ~AviMaker(void);

/**
 *	説明
 *		初期化
 *	引数
 *		pFileName: 作成するファイル名
 *		codec_:	圧縮に使用するビデオコーデックのFOURCC コード（インストール済みのコーデックを選択すること）
 *				デフォルトはWMV3。例えば、DIVXなら mmioFOURCC('D','I','V','X') とする
 *				0ならば無圧縮
 *		frameRate_: フレームレート
 */
	void initialize(LPCTSTR pFileName = _T("Output.avi"), DWORD codec_ = mmioFOURCC('W','M','V','3'), DWORD frameRate_ = 1);
/**
 *	説明
 *		動画作成のための終了処理
 */
	void fianalize(void);

/**
 *	説明
 *		初期化
 *	引数
 *		pFileName: 作成するファイル名
 */
	void setOutputFileName(LPCTSTR pFileName);

/**
 *	----------------------------------------
 *	動画の作成
 *	オーバーロード
 *	----------------------------------------
 */	
	/**
	 *	説明
	 *		動画の最後に指定したHBitmapを新しいフレームとして挿入する
	 *	引数
	 *		hBitmap: 挿入するHBITMAP型オブジェクト
	 */
	HRESULT	appendNewFrame(HBITMAP hBitmap);

	/**
	 *	説明
	 *		動画の最後に指定したBitmapのビットを新しいフレームとして挿入する
	 *	引数
	 *		width: 挿入するビット列の高さ
	 *		height: 挿入するビット列の幅
	 *		pBits: 指定するビット列へのポインタ
	 *		bitsPerPixel: 1ピクセル当たりのビット
	 */	
	HRESULT	appendNewFrame(int width, int height, LPVOID pBits,int bitsPerPixel);

/**
 *	説明
 *		最終エラーの取得
 */
	LPCTSTR getLastErrorMessage(void) const {return lastErrMsg;}

private:
/**
 *	説明
 *		動画作成のための初期化
 *		メモリ、ストリーム、圧縮のオプションの設定をする
 *	引数
 *		farameWidth: フレーム幅
 *		frameHeight: フレーム高さ
 *		bitsPerPixel: ピクセル単位でのビット列
 *	返却値
 *		S_OK以外: 失敗
 *		S_OK: 成功	 
 */
	HRESULT initializeEngine(int frameWidth, int frameHeight, int bitsPerPixel);

/**
 *	説明
 *		メモリや関連ハンドルの解放
 */
	void releaseMemory();

/**
 *	説明
 *		最終エラーの設定
 *	引数
 *		pLastErrMsg: エラー内容
 */
	void setLastErrMsg(LPCTSTR pLastErrMsg);

/**
 *	----------------------------------------
 *	動画の作成のためのヘルプ関数
 *	オーバーロード
 *	----------------------------------------
 */	
	HRESULT	appendFrameFirstTime(HBITMAP hBitmap);
	HRESULT	appendFrameUsual(HBITMAP hBitmap);
	HRESULT	appendDummy(HBITMAP hBitmap);
	HRESULT	(AviMaker::*pAppendFrame[3])(HBITMAP hBitmap);

	HRESULT	appendFrameFirstTime(int width, int height, LPVOID pBits,int bitsPerPixel);
	HRESULT	appendFrameUsual(int width, int height, LPVOID pBits,int bitsPerPixel);
	HRESULT	appendDummy(int nWidth, int nHeight, LPVOID pBits,int bitsPerPixel);
	HRESULT	(AviMaker::*pAppendFrameBits[3])(int nWidth, int nHeight, LPVOID pBits,int bitsPerPixel);
};

#endif