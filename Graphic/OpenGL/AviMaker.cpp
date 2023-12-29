/**
 *  ファイル名
 *		AviMaker.cpp
 *  説明
 *		AVIを作成するクラス
 *  日付
 *		作成日: 2008/09/02(TUE)		更新日: 2008/09/03(WED)
 */
/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "AviMaker.h"

/**
 *	----------------------------------------------------------------------
 *		マクロ、型の定義
 *	----------------------------------------------------------------------
 */
#ifndef __countof
#define __countof(x)	((sizeof(x)/sizeof(x[0])))
#endif

/**
 *	----------------------------------------------------------------------
 *		AviMakerクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		AviMakerクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
AviMaker:: AviMaker(LPCTSTR pFileName /* =_T("Output.avi") */, 
			DWORD codec_ /* = mmioFOURCC('M','P','G','4') */,
			DWORD frameRate_ /* = 1 */)
{
	initialize(pFileName, codec_, frameRate_);

}

AviMaker::~AviMaker(void)
{
	fianalize();
}

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
void AviMaker::initialize(LPCTSTR pFileName, DWORD codec_, DWORD frameRate_)
{
/**
 *	プラットフォーム SDK 関数
 *		AVIFile ライブラリを初期化
 *			・AVIFile ライブラリは初期化された回数のカウントを管理するが、解放された回数のカウントは管理しまない。
 *			・AVIFile ライブラリを解放してカウントを 1 つ減らすには、AVIFileExit 関数を使う。
 *			・AVIFileInit 関数は、他の AVIFile 関数を使う前に呼び出なくてはならない。
 *		プログラムの初期化時に一度だけ呼び出す
 */
	AVIFileInit();

/**
 *	メンバ変数の初期化
 */ 
	heapHandle = NULL;
	memDCHandle = NULL;
	lpDIBPixelBits = NULL;
	frameCounter = NULL;

	pAviFile = NULL;
	pAviStream = NULL;
	pAviCompressedStream = NULL;

	fccHandler = codec_;
	frameRate = frameRate_;

/**
 *	文字列の初期化
 */
	_tcscpy(outputFileName, pFileName);
	_tcscpy(lastErrMsg, _T("Method Succeeded"));
	lastErrMsg[__countof(lastErrMsg)-1] = _T('\0');

/**
 *	以下の処理はVC8以降で必要。（関数ポインタ用）
 *	コンパイラに警告を受けたら、削除しても良い
 */ 
	pAppendFrame[0] = &AviMaker::appendDummy;
	pAppendFrame[1] = &AviMaker::appendFrameFirstTime;
	pAppendFrame[2] = &AviMaker::appendFrameUsual;

	pAppendFrameBits[0] = &AviMaker::appendDummy;
	pAppendFrameBits[1] = &AviMaker::appendFrameFirstTime;
	pAppendFrameBits[2] = &AviMaker::appendFrameUsual;

	appendFuncSelector=1;		//0=Dummy	1=FirstTime	2=Usual

	return;
}

/**
 *	説明
 *		動画作成のための終了処理
 */
void AviMaker::fianalize(void)
{
	releaseMemory();

/**
 *	プラットフォーム SDK 関数
 *		AVIFile ライブラリの終了処理
 */	
	AVIFileExit();
}

/**
 *	説明
 *		初期化
 *	引数
 *		pFileName: 作成するファイル名
 */
void AviMaker::setOutputFileName(LPCTSTR pFileName)
{
	_tcscpy(outputFileName, pFileName);
}

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
HRESULT AviMaker::appendNewFrame(HBITMAP hBitmap)
{
	return (this->*pAppendFrame[appendFuncSelector])((HBITMAP)hBitmap);
}

/**
 *	説明
 *		動画の最後に指定したBitmapのビットを新しいフレームとして挿入する
 *	引数
 *		width: 挿入するビット列の高さ
 *		height: 挿入するビット列の幅
 *		pBits: 指定するビット列へのポインタ
 *		bitsPerPixel: 1ピクセル当たりのビット
 */
HRESULT	AviMaker::appendNewFrame(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	return (this->*pAppendFrameBits[appendFuncSelector])(width, height, pBits, bitsPerPixel);
}
/**
 *	------------------------------------------------------------
 *		AviMaker クラスの private なメンバ関数
 *	------------------------------------------------------------
 */
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
HRESULT AviMaker::initializeEngine(int frameWidth, int frameHeight, int bitsPerPixel)
{
	/// 返却値
	HRESULT hResult;

	/// AVI圧縮オプション構造体
	LPAVICOMPRESSOPTIONS pOptions[1] = {&aviCompressOptions};

	/**
	 *	画面構成を取得
	 */
	int	maxWidth = GetSystemMetrics(SM_CXSCREEN); 
	int maxHeight = GetSystemMetrics(SM_CYSCREEN);

/**
 *	STEP 0
 *		メモリ領域の確保
 */
	/**
	 *	メモリデバイスコンテキスト（メモリの論理的なデバイス）を作成
	 */
	memDCHandle = CreateCompatibleDC(NULL);
	/// 失敗判定
	if (memDCHandle == NULL)	
	{
		setLastErrMsg(_T("Unable to Create Compatible DC"));
		return E_FAIL;
	}
	
	/// 幅と高さの範囲設定
	if (frameWidth > maxWidth)	maxWidth= frameWidth;
	if (frameHeight > maxHeight)	maxHeight = frameHeight;

	/**
	 *	パフォーマンスのためプライベートヒープの作成
	 *		予めある程度の仮想メモリ領域を予約しておき、プログラムから要求され次第、
	 *		その予約された領域からメモリを切り出してきて効率良くメモリブロックを確保する。
	 */
	heapHandle = HeapCreate(HEAP_NO_SERIALIZE, maxWidth * maxHeight * 4, 0);
	/// 失敗判定
	if(heapHandle==NULL)
	{
		setLastErrMsg(_T("Unable to Create Heap"));
		return E_FAIL;
	}
	
	/**
	 *	メモリブロックをHeapCreate 関数で作成したヒープから割り当てる
	 *	これにより、DIB（デバイス独立ビットマップ）用ピクセル列を作成
	 *	1ピクセルあたり4バイト。
	 */ 
	lpDIBPixelBits = HeapAlloc(heapHandle, HEAP_ZERO_MEMORY|HEAP_NO_SERIALIZE, maxWidth * maxHeight * 4);
	/// 失敗判定
	if (lpDIBPixelBits == NULL)	
	{	
		setLastErrMsg(_T("Unable to Allocate Memory on Heap"));
		return E_FAIL;
	}

/**
 *	STEP 1
 *		AVIの設定に使用する各種変数、構造体を決定
 */
/**
 *	AVIStreamSetFormat 関数で使用する BITMAPINFO 構造体の設定
 *		typedef struct tagBITMAPINFO {
 *							BITMAPINFOHEADER bmiHeader;				色フォーマットとサイズに関する情報を保持している、BITMAPINFOHEADER 構造体
 *							RGBQUAD          bmiColors[1];			次のうちのどれかを保持。
 *																	RGBQUAD の配列。配列の要素は色テーブル（注：パレット）を構成する。 
 *																	現在使用されている論理パレットへのインデックスを指定する、16ビット符号無しの整数の配列。
 *						} BITMAPINFO, *PBITMAPINFO;
 *
 *		typedef struct tagBITMAPINFOHEADER{
 *							DWORD  biSize;							構造体に必要なバイト数( 必ず sizeof(BITMAPINFOHEADER) で指定。)
 *							LONG   biWidth;							ビットマップの幅をピクセル単位で指定			
 *							LONG   biHeight;						ビットマップの高さをピクセル単位で指定
 *							WORD   biPlanes;						ターゲット デバイスの面の数を指定。この値は 1
 *							WORD   biBitCount;						1 ピクセルあたりのビット数 (bits-per-pixel) を指定。これは0、1、4、8、16、24、32のどれかを指定。
 *							DWORD  biCompression;					圧縮された "bottom-up" ビットマップの圧縮形式を指定。BI_***を指定。（***は、RGB、RLE8、RLE4、BITFIELDS、JPEG、PNG）
 *							DWORD  biSizeImage;						画像サイズをバイト単位で指定
 *							LONG   biXPelsPerMeter;					ビットマップのターゲット デバイスの水平方向の解像度を、pixels-per-meter で指定
 *							LONG   biYPelsPerMeter;					ビットマップのターゲット デバイスの垂直方向の解像度を、pixels-per-meter で指定
 *							DWORD  biClrUsed;						実際にビットマップによって使用される色テーブル中の色インデックスの数を指定
 *							DWORD  biClrImportant;					ビットマップを表示するのに必要な色インデックスの数を指定
 *						} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
 *						
 */
	/// 構造体の初期化
	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
	/// 構造体の設定
	bmpInfo.bmiHeader.biPlanes		= 1;
	bmpInfo.bmiHeader.biWidth		= frameWidth;
	bmpInfo.bmiHeader.biHeight		= frameHeight;
	bmpInfo.bmiHeader.biCompression	= BI_RGB;
	bmpInfo.bmiHeader.biBitCount	= bitsPerPixel;
	bmpInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biSizeImage	= ((bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biBitCount+31)/32*4)*bmpInfo.bmiHeader.biHeight;//bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biHeight*bmpInfo.bmiHeader.biBitCount/32*4;


/**
 *	AVIファイルフォーマットを決定する AVISTREAMINFO 構造体を設定する
 *
 *	typedef struct {
 *				DWORD fccType;					開くストリームタイプを示す。規定値は streamtype***。（***はAUDIO、MIDI、TEXT、VIDEOのどれか）
 *				DWORD fccHandler;				ビデオストリームを保存する時に圧縮するハンドラの4文字コード
 *				DWORD dwFlags;					ストリームフラグ
 *				DWORD dwCaps;					機能フラグ
 *				WORD  wPriority;				ストリームの優先順位
 *				WORD  wLanguage;				ストリームの言語
 *				DWORD dwScale;					ストリームの時間単位
 *				DWORD dwRate;					ストリームのレート
 *				DWORD dwStart;					AVIファイルの最初のフレームのサンプル番号。dwScaleとdwRateで単位が定義される。通常は0
 *				DWORD dwLength;					ストリームの長さ。dwScaleとdwRateで単位が定義される。
 *				DWORD dwInitialFrames;			オーティオ スキュー。インターリーブファイル内のビデオストリーム前方へオーディオストリームをどの程度ずらすかを指定する。通常は0.75秒。
 *				DWORD dwSuggestedBufferSize;	ストリームを読み込むときに適したバッファサイズ。この値は0のときもある。
 *				DWORD dwQuality;				ビデオストリームの品質。0～10000の範囲で-1のときはデフォルトの品質。
 *				DWORD dwSampleSize;				1つのデータサンプルのバイトサイズ。0の場合、サンプルのサイズは可変。
 *				RECT  rcFrame;					表示を行う矩形。ビデオストリームでのみ使われる。
 *	 			DWORD dwEditCount;				ファイルを編集（追加または削除）した回数
 *				DWORD dwFormatChangeCount;		フォーマットの変更回数
 *				char  szName[64];				ストリームを説明するNULLで終わる文字列
 *			} AVISTREAMINFO;
 */
	/// 構造体の初期化
	ZeroMemory(&aviStreamInfo,sizeof(AVISTREAMINFO));
	/// 構造体の設定
	aviStreamInfo.fccType				= streamtypeVIDEO;
	aviStreamInfo.fccHandler			= fccHandler;
	aviStreamInfo.dwScale				= 1;
	aviStreamInfo.dwRate				= frameRate;
	aviStreamInfo.dwQuality				= (DWORD)-1;				
	aviStreamInfo.dwSuggestedBufferSize = maxWidth*maxHeight*4;
    SetRect(&aviStreamInfo.rcFrame, 0, 0, frameWidth, frameHeight);
	_tcscpy(aviStreamInfo.szName, _T("Video Stream"));

/**
 *	圧縮オプションを格納するAVICOMPRESSOPTIONS構造体の設定
 *	typedef struct { 
				DWORD  fccType;					開くストリームタイプを示す。規定値は streamtype***。（***はAUDIO、MIDI、TEXT、VIDEOのどれか）
				DWORD  fccHandler;				ビデオストリームを保存する時に圧縮するハンドラの4文字コード
				DWORD  dwKeyFrameEvery;			キーフレームの周期。AVICOMPRESSF_KEYFRAMESフラグを指定したときのみこの値は使われる。それ以外ではすべてのフレームがキーフレーム。
				DWORD  dwQuality;				ビデオストリームの品質
				DWORD  dwBytesPerSecond;		データ比率。AVICOICMF_CHOOSE_PREVIEWMPRESSF_DATARATEフラグを指定したときのみこの値は使われる。
				DWORD  dwFlags;					圧縮フラグ。次の4つAVICOMPRESSF_***を選択可で複数指定も可。（***は、DATARATE、INTERLEAVE、KEYFRAMES、VALID）
				LPVOID lpFormat;				データフォーマット構造体へのポインタ。オーディオストリームではWAVEFORMAT構造体へのポインタ。
				DWORD  cbFormat;				lpFormatで指定したデータフォーマット構造体のバイト数
				LPVOID lpParms;					ビデオ圧縮用のデータ
				DWORD  cbParms;					lpParamsで指定したデータのバイト数
				DWORD  dwInterleaveEvery;		インターリーブ値。AVICOMPRESSF_INTERLEAVEフラグを指定したときのみこの値は使われる。
			} AVICOMPRESSOPTIONS;

 */

 	/// 構造体の初期化
	ZeroMemory(&aviCompressOptions,sizeof(AVICOMPRESSOPTIONS));
	/// 構造体の設定	
	aviCompressOptions.fccType			= streamtypeVIDEO;
	aviCompressOptions.fccHandler		= aviStreamInfo.fccHandler;
	aviCompressOptions.dwFlags			= AVICOMPRESSF_KEYFRAMES|AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
	aviCompressOptions.dwKeyFrameEvery	= 1;
	//aviCompressOptions.dwBytesPerSecond	= 1000/8;
	//aviCompressOptions.dwQuality			= 100;

/**
 *	STEP 2
 *		ファイル名を指定してAVIファイルを開く
 */
	hResult = AVIFileOpen(&pAviFile, outputFileName, OF_CREATE|OF_WRITE, NULL);
	
	if (hResult != AVIERR_OK)
	{
		// Check it succeded.
		switch (hResult)
		{
			case AVIERR_BADFORMAT: 
				setLastErrMsg(_T("AVIFileOpen() Failed. The file couldn't be read, indicating a corrupt file or an unrecognized format."));
				break;

			case AVIERR_MEMORY:		
				setLastErrMsg(_T("AVIFileOpen() Failed. The file could not be opened because of insufficient memory.")); 
				break;

			case AVIERR_FILEREAD:
				setLastErrMsg(_T("AVIFileOpen() Failed. A disk error occurred while reading the file.")); 
				break;

			case AVIERR_FILEOPEN:		
				setLastErrMsg(_T("AVIFileOpen() Failed. A disk error occurred while opening the file."));
				break;
	
			case REGDB_E_CLASSNOTREG:		
				setLastErrMsg(_T("AVIFileOpen() Failed. According to the registry, the type of file specified in AVIFileOpen does not have a handler to process it"));
				break;
		}

		return hResult;
	}

/**
 *	STEP 3
 *		既存のファイルに新しいストリームを作成し、そのストリームへのインターフェイスを作成
 */
	hResult = AVIFileCreateStream(pAviFile, &pAviStream, &aviStreamInfo);
	if (hResult != AVIERR_OK)
	{
		setLastErrMsg(_T("Unable to Create Video Stream in the Movie File"));
		return hResult;
	}


/**
 *	STEP 4
 *		非圧縮ストリームと圧縮フィルタから圧縮されたストリームを作成し、圧縮されたストリームへのポインタのアドレスを取得する
 */
	hResult = AVIMakeCompressedStream(&pAviCompressedStream, pAviStream, &aviCompressOptions, NULL);
	
	if (hResult != AVIERR_OK)
	{

		/**
		 *	エラーが発生する可能性が比較的高い
		 *		理由の一つに、ユーザの環境にインストールされていないコーデックを使用する場合。
		 *		この場合は mmioFOURCC() をチェックする。
		 */		
		switch(hResult)
		{

			case AVIERR_NOCOMPRESSOR:
				setLastErrMsg(_T("AVIMakeCompressedStream() failed: A suitable compressor cannot be found."));
					break;

			case AVIERR_MEMORY:
				setLastErrMsg(_T("AVIMakeCompressedStream() failed: There is not enough memory to complete the operation."));
					break; 

			case AVIERR_UNSUPPORTED:
				setLastErrMsg(_T("AVIMakeCompressedStream() failed: Compression is not supported for this type of data. This error might be returned if you try to compress data that is not audio or video."));
				break;
		}

		return hResult;
	}

/**
 *	STEP 5
 *		特定の位置にストリームのフォーマットを設定
 */
	hResult = AVIStreamSetFormat(pAviCompressedStream, 0, (LPVOID)&bmpInfo, bmpInfo.bmiHeader.biSize +  bmpInfo.bmiHeader.biClrUsed * sizeof(RGBQUAD));
	if (hResult != AVIERR_OK)
	{
		/**
		 *	良くあるエラーの原因は、使用するビットマップがコーデックの仕様に満たない場合である。
		 *	例えば、
		 *		ユーザのビットマップが32bppなのにコーデックのサポートが16や24bppだった場合
		 *		コーデックの解像度サポートが2の乗数のみなのに、ユーザのビットマップの解像度が720*405などの場合
		 *	これらを避けるために、コーデックの選択とビットマップの仕様には注意すること
		 */
		setLastErrMsg(_T("AVI Compressed Stream format setting failed."));
		return hResult;
	}

	return S_OK;	/// 成功
}

/**
 *	説明
 *		メモリや関連ハンドルの解放
 */
void AviMaker::releaseMemory()
{
	/// ダミーセレクタを指定
	appendFuncSelector = 0;
	
	/// デバイスコンテキストの解放
	if(memDCHandle)
	{
		DeleteDC(memDCHandle);
		memDCHandle=NULL;
	}
	
	/// 圧縮ストリームの解放
	if(pAviCompressedStream)
	{
		AVIStreamRelease(pAviCompressedStream);
		pAviCompressedStream=NULL;
	}
	
	/// ストリームの解放
	if(pAviStream)
	{
		AVIStreamRelease(pAviStream);
		pAviStream=NULL;
	}
	
	/// ファイルポインタの解放
	if(pAviFile)
	{
		AVIFileRelease(pAviFile);
		pAviFile=NULL;
	}
	
	/// メモリブロックの解放
	if(lpDIBPixelBits)
	{
		HeapFree(heapHandle, HEAP_NO_SERIALIZE, lpDIBPixelBits);
		lpDIBPixelBits=NULL;
	}
	
	/// プライベートヒープの破棄
	if(heapHandle)
	{
		HeapDestroy(heapHandle);
		heapHandle=NULL;
	}
}

/**
 *	説明
 *		最終エラーの設定
 *	引数
 *		pLastErrMsg: エラー内容
 */
void AviMaker::setLastErrMsg(LPCTSTR lpszErrorMessage)
{
	_tcsncpy(lastErrMsg, lpszErrorMessage, __countof(lastErrMsg)-1);
}


/**
 *	----------------------------------------
 *	動画の作成のためのヘルプ関数
 *	オーバーロード
 *	----------------------------------------
 */
/// 初回のフレーム追加関数
HRESULT	AviMaker::appendFrameFirstTime(HBITMAP hBitmap)
{
	BITMAP Bitmap;

	/// ビットマップの情報を得る
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);

	/// 最初に動画の初期化を行う
	if (SUCCEEDED(initializeEngine(Bitmap.bmWidth, Bitmap.bmHeight, Bitmap.bmBitsPixel)))
	{
		/// 通常の追加機能を選択
		appendFuncSelector = 2;
		return appendFrameUsual(hBitmap);
	}

	releaseMemory();

	return E_FAIL;
}

/// 通常のフレーム追加関数
HRESULT AviMaker::appendFrameUsual(HBITMAP hBitmap)
{
	/// 返却値
	HRESULT hResult;

	BITMAPINFO	bmpInfo;
	bmpInfo.bmiHeader.biBitCount = 0;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	
	/**
	 *	int GetDIBits(HDC hdc,HBITMAP hbmp,UINT uStartScan,UINT cScanLines,LPVOID lpvBits,LPBITMAPINFO lpbi,UINT uUsage);
	 *		DDBの描画内容を既存のDIBにコピー
	 *	引数
	 *		hdc: デバイスコンテキストのハンドルを指定。
	 *		hbmp: デバイス依存ビットマップのハンドルを指定。
	 *		uStartScan: 取得する最初の走査行を指定。
	 *		cScanLines: 取得する走査行の数を指定。
	 *		lpvBits: バッファへのポインタを指定。このバッファにビットマップデータが格納される。
	 *				NULL を指定すると、ビットマップのサイズと形式が、lpbi パラメータが指す BITMAPINFO 構造体に格納される。
	 *		lpbi: 希望するデータの形式が入った、BITMAPINFO 構造体へのポインタを指定。
	 *		uUsage:	BITMAPINFO 構造体の bmiColors メンバの形式を指定。次の値のいずれかを指定する。
	 *					DIB_PAL_COLORS カラーテーブルは、現在の論理パレットへの 16 ビットのインデックスの配列。 
	 *					DIB_RGB_COLORS カラーテーブルは、RGB 値そのもの。 
	 *	戻り値 : lpvBits パラメータが NULL 以外のときに関数が成功すると、コピーされた走査行の数が返る。
	 *				Windows 95 では、lpvBits パラメータが NULL のときに関数が成功すると、ビットマップの走査行の数が返ります。
	 *				Windows NT では、lpvBits パラメータが NULL のときに関数が成功すると、0 以外の値が返ります。
	 *			関数が失敗すると 0。
	 */
	/**
	 *	lpvBitsをNULLにしてGetDIBitsを呼び出し、
	 *	デバイスドライバに BITMAPINFOHEADER 構造体のbiSizeImage フィールドを計算させる
	 */
	GetDIBits(memDCHandle, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
	bmpInfo.bmiHeader.biCompression = BI_RGB;	

	/// 目的のDIBを lpDIBPixelBits に格納する
	GetDIBits(memDCHandle, hBitmap, 0, bmpInfo.bmiHeader.biHeight, lpDIBPixelBits, &bmpInfo, DIB_RGB_COLORS);

	/// 作成したDIBをAVIに書き込む
	hResult = AVIStreamWrite(pAviCompressedStream, frameCounter++, 1, lpDIBPixelBits, bmpInfo.bmiHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);

	if (FAILED(hResult))
	{
		setLastErrMsg(_T("Unable to Write Video Stream to the output Movie File"));

		releaseMemory();

		return E_FAIL;
	}

    return S_OK;
}

/// フレーム追加関数のダミー（実際はなにもしない）
HRESULT AviMaker::appendDummy(HBITMAP hBitmap)
{
	return E_FAIL;
}

/// 初回のフレーム追加関数
HRESULT	AviMaker::appendFrameFirstTime(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	if (SUCCEEDED(initializeEngine(width, height, bitsPerPixel)))
	{
		/// 通常の追加機能を選択	
		appendFuncSelector=2;

		return appendFrameUsual(width, height, pBits, bitsPerPixel);
	}

	releaseMemory();

	return E_FAIL;
}

/// 通常のフレーム追加関数
HRESULT	AviMaker::appendFrameUsual(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	/// 返却値
	HRESULT hResult;

	DWORD size = width*height*bitsPerPixel/8;

	hResult = AVIStreamWrite(pAviCompressedStream, frameCounter++, 1, pBits, size, AVIIF_KEYFRAME, NULL, NULL);

	if (FAILED(hResult))
	{
		setLastErrMsg(_T("Unable to Write Video Stream to the output Movie File"));

		releaseMemory();

		return E_FAIL;
	}

    return S_OK;
}

/// フレーム追加関数のダミー（実際はなにもしない）
HRESULT	AviMaker::appendDummy(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	return E_FAIL;
}
