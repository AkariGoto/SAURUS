/**
 *  ファイル名
 *		OpenGLText.h
 *  説明
 *		OpenGLによるテキスト出力(Windows専用)
 *  日付
 *		作成日: 2007/04/01(SAT)		更新日: 2007/04/15(SAT)
 */

#ifndef __OpenGLText_h__
#define __OpenGLText_h__


/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>

/// リンカへの通知
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
//#pragma comment (lib, "glaux.lib")

/**
 *	----------------------------------------------------------------------
 *		定数、マクロ宣言
 *	----------------------------------------------------------------------
 */
/// デフォルトフォント
const char OPENGL_DEFAULT_FONT[]={ "Arial" };
const int OPENGL_DEFAULT_BUFFER_SIZE = 256;

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGLTextクラス
 *	----------------------------------------------------------------------
 */
class OpenGLText
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
protected:
	/**
	 *	Glyph Metrics float構造体
	 */
	GLYPHMETRICSFLOAT	gmf[256];
	/// プレイリストのためのフォントベース
	GLuint				fontBase;
	/// フォントが作成できたかどうかのフラグ
	bool				isFontBuilt;

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
	/// デフォルトコンストラクタ
	explicit OpenGLText();
	/// デストラクタ
	virtual ~OpenGLText();
	
/**
 *	フォントの作成
 */
	void buildFont(	HDC hDC,									/// デバイスコンテキストへのハンドル
					char* fontName = NULL,						/// フォント名
					GLfloat fontDepth = 0.5f,					/// z（深さ）方向へのフォントサイズ
					int fontWeight = FW_BOLD,					/// フォントウェイト
					DWORD fontItalic = FALSE,					/// イタリックのフラグ
					DWORD fontUnderline = FALSE,				/// アンダーラインのフラグ
					DWORD fontStrikeOut = FALSE,				/// 打ち消し線付きのフラグ
					DWORD fontCharacterSet = ANSI_CHARSET		/// キャラクターセットの識別子
					);

/**
 *		文字列の出力
 */
	bool print(const char* format, ...);

protected:
/**
 *	フォントの破棄
 */
	void killFont(void);

};	/// end of class OpenGLText

}	/// end of namespace Graphic

#endif	/// __OpenGLText_h__