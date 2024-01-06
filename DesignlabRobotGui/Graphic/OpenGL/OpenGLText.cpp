
#include <stdio.h>
#include <string.h>

#include "OpenGLText.h"

namespace Graphic
{

void OpenGLText::BuildFont(HDC hDC, char* fontName, GLfloat fontDepth,
    int fontWeight, DWORD fontItalic,
    DWORD fontUnderline, DWORD fontStrikeOut,
    DWORD fontCharacterSet)
{
    // 既にフォントが作成されているかどうか
    if (is_font_built)
    {
        // 作成されていれば消去
        KillFont();
    }

    /// ユーザのフォント
    HFONT font;
    /// デフォルトのフォントを使うかどうか
    bool  isDefaultFontUsed = false;

    /// 引数のフォント名がNULLだったら，デフォルトを使う
    if (fontName == NULL)
    {
        /// フォント文字列と'\0'のためのメモリを確保
        fontName = new char[strlen(OPENGL_DEFAULT_FONT) + 1];
        strcpy(fontName, OPENGL_DEFAULT_FONT);
        isDefaultFontUsed = true;
    }

    /// ディスプレイリストを確保
    font_base = glGenLists(OPENGL_DEFAULT_BUFFER_SIZE);

    /// CreateFont関数（Win API）によりフォント作成
    font = CreateFont(-12,							/// 文字セルまたは文字の高さ
        2,								/// 平均文字幅
        0,								/// 文字送りの方向とX軸との角度
        0,								/// ベースラインとX軸との角度
        fontWeight,						/// フォントの太さ
        fontItalic,						/// イタリック体指定
        fontUnderline,					/// 下線付き指定
        fontStrikeOut,					/// 打ち消し線付き指定
        fontCharacterSet,				/// キャラクタセット
        OUT_TT_PRECIS,					/// 出力精度
        CLIP_DEFAULT_PRECIS,			/// クリッピングの精度
        ANTIALIASED_QUALITY,			/// 出力品質
        FF_DONTCARE | DEFAULT_PITCH,	/// ピッチとファミリ
        (LPCTSTR)fontName				/// フォント名
    );

    /// 作成したフォントを使用
    SelectObject(hDC, font);

    /// 3 次元フォントのディスプレイリストを作成する関数
    wglUseFontOutlines(hDC,							/// デバイスコンテキストへのハンドルSelect The Current DC
        0,								/// 最初の文字コード
        OPENGL_DEFAULT_BUFFER_SIZE - 1,	/// 作成するディスプレイリストの数
        font_base,						/// 最初のディスプレイリスト
        0.0f,							/// 真の3次元フォントとのズレ 
        fontDepth,						/// z の負の方向への厚さ
        WGL_FONT_POLYGONS,				/// WGL_FONT_LINES (ワィヤーフレーム) か WGL_FONT_POLYGONS (ソリッド) の指定 (定数) 
        gmf								/// グリフ (glyph) の寸法を格納する配列 
    );

    if (isDefaultFontUsed)
    {
        delete[] fontName;
    }

    // フォント作成完了
    is_font_built = true;
}

bool OpenGLText::Print(const char* format, ...)
{
    /// フォントが作成されていなかったら
    if (!is_font_built)
    {
        return false;
    }


    /// テキストの長さ
    float length = 0;
    /// テキストの格納配列
    char text[OPENGL_DEFAULT_BUFFER_SIZE];
    /// 引数リストへのポインタ
    va_list ap;

    // 引数のテキストが無かったら
    if (format == NULL)
    {
        return false;  // 何もしない．
    }


    /// 変数のための構文解析
    va_start(ap, format);
    /// 実際の数へ変換
    vsprintf(text, format, ap);
    va_end(ap);	/// 結果をテキストに格納

    unsigned int loop;
    /// テキスト長さを調べるためのループ
    for (loop = 0; loop < (strlen(text)); loop++)
    {
        /// 各文字の幅ごとによって長さを増やす
        length += gmf[text[loop]].gmfCellIncX;
    }

    /// スクリーン上でのテキストの中央位置
    glTranslatef(-length / 2, 0.0f, 0.0f);

    /// 以下で実際にスクリーンにテキストを描画
    glPushAttrib(GL_LIST_BIT);		/// ディスプレイリストのビットをプッシュ
    glListBase(font_base);	/// ベース文字を0にセット
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);		/// ディスプレイリストのテキストを描画
    glPopAttrib();	/// ディスプレイリストのビットをポップ
    glTranslatef(-length / 2, 0.0f, 0.0f);

    return true;
}

void OpenGLText::KillFont()
{
    // ディスプレイリストの破棄．
    glDeleteLists(font_base, OPENGL_DEFAULT_BUFFER_SIZE);
}


}