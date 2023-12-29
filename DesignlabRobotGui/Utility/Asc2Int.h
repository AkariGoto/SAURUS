/**
 *  ファイル名
 *		Asc2Int.h(Ascii To Integer Header)
 *  説明
 *		文字列→数値変換ルーチン
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/19(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		定数、マクロ宣言
 *	----------------------------------------------------------------------
 */
const int		ASCINT_MAX_DATA_SIZE	=	256;		/// Max String Size
const LONG		ASCINT_ERROR			=	-1L;
const int		ASCINT_ON				=	1;
const int		ASCINT_OFF				=	0;

#ifdef __cplusplus
extern "C" {
#endif	/// __cplusplus

/**
 *	説明
 *		テキスト形式の16進数をバイナリに変換
 */
extern "C" LONG ascHexToInt(PSTR pszData);

/**
 *	説明
 *		テキスト形式の2進数をバイナリに変換
 */
extern "C" LONG ascBinToInt(PSTR pszData);

/**
 *	説明
 *		テキスト形式の2進数, 10進数, 16進数をバイナリに変換
 */
extern "C" LONG ascToInt(PSTR pszData);

#ifdef __cplusplus
}
#endif	// __cplusplus