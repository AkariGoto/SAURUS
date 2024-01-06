
#ifndef __OpenGLText_h__
#define __OpenGLText_h__

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

// リンカへの通知

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")


namespace Graphic
{

//! デフォルトフォント
constexpr char OPENGL_DEFAULT_FONT[] = { "Arial" };
constexpr int OPENGL_DEFAULT_BUFFER_SIZE = 256;


class OpenGLText final
{
public:

    /// デフォルトコンストラクタ
    OpenGLText() = default;

    inline ~OpenGLText() { KillFont(); }


    //! @brief フォントの作成
    //! @param hDC デバイスコンテキストへのハンドル
    //! @param font_name フォント名
    //! @param font_depth z（深さ）方向へのフォントサイズ
    //! @param font_weight フォントウェイト
    //! @param fontItalic イタリックのフラグ
    //! @param fontUnderline アンダーラインのフラグ
    //! @param fontStrikeOut 打ち消し線付きのフラグ
    //! @param fontCharacterSet キャラクターセットの識別子
    void BuildFont(HDC hDC, char* font_name = NULL, GLfloat font_depth = 0.5f,
                   int font_weight = FW_BOLD, DWORD fontItalic = FALSE,
                   DWORD fontUnderline = FALSE, DWORD fontStrikeOut = FALSE,
                   DWORD fontCharacterSet = ANSI_CHARSET);

    //! @brief 文字列の出力．
    bool Print(const char* format, ...);

private:
    //! @brief フォントの破棄．
    void KillFont();


    //! Glyph Metrics float構造体
    GLYPHMETRICSFLOAT gmf[OPENGL_DEFAULT_BUFFER_SIZE];

    //! プレイリストのためのフォントベース
    GLuint font_base;

    //! フォントが作成できたかどうかのフラグ
    bool is_font_built{ false };

};

}	/// end of namespace Graphic

#endif	/// __OpenGLText_h__