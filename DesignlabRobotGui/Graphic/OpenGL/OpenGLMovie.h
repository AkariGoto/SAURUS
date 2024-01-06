/**
 *  ファイル名
 *		OpenGLMovie.h
 *  説明
 *		OpenGLのアニメーションをAVIに変換
 *  日付
 *		作成日: 2008/09/02(TUE)		更新日: 2008/09/03(WED)
 */

#ifndef __OpenGLMovie_h__
#define __OpenGLMovie_h__

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
#include <windows.h>

#include <gl/GL.h> 
#include "AviMaker.h"

namespace Graphic
{

/**
 *	----------------------------------------------------------------------
 *		OpenGLMovieクラス
 *	----------------------------------------------------------------------
 *
 *	OpenGLのレンダリングサーフェスからAVI動画を作成する
 *	ただし、一度動画オブジェクトが作成されたら動画フレームの幅と高さは変更不可
 *	本クラスのオブジェクトは単一プロセス内で同時に複数生成可能
 */

 /**
  *	使い方（How to use）
  *		1. オブジェクトを作成する
  *			例	OpenGLMovie m_movieRecorder("Output.Avi", VIEWPORTWIDTH, VIEWPORTHEIGHT);
  *		2. オブジェクト作成後、SwapBuffers()呼び出し前にOpenGLMovie::RecordFrame()メソッドを各フレームの描写毎に呼び出せば、AVIを作成可
  *
  *			SAMPLE CODE
  *				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  *
  *				// Render as usual
  *				glPushMatrix();
  *					drawScene();
  *				glPopMatrix();
  *
  *				// Capture the Rendering into OpenGLMovie's movie file
  *				m_movieRecorder.recordFrame();
  *
  *				SwapBuffers(wglGetCurrentDC());
  *
  *			OpenGLMovie::recordFrame() は内部で glReadPixels() メソッドを使用し、フレームバッファの内容を読み込み出力ファイルに
  *			そのフレームを加えている。
  *			記録したくないフレームがある場合は、ただm_movieRecorder.recordFrame() メソッドを呼ばなければよい。
  */

class OpenGLMovie
{
    /**
     *	------------------------------------------------------------
     *		メンバ変数
     *	------------------------------------------------------------
     */
private:
    /**
     *	ムービーファイルのオブジェクト
     */
    AviMaker	movieFile;

    /**
     *	----------------------------------------
     *	作成するムービーファイル情報
     *	----------------------------------------
     */
    int			movieWidth;			/// ムービーフレームの幅
    int			movieHeight;		/// ムービーフレームの高さ
    int			movieBitsPerPixel;	/// 1ピクセル当たりのビット数
    LPVOID		pFramePixelBits;	/// 書き込むピクセル列データへのポインタ

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
      *		コンストラクタ
      *	引数
      *		pFileName: 作成するファイル名
      *		codec_:	圧縮に使用するビデオコーデックのFOURCC コード（インストール済みのコーデックを選択すること）
      *				デフォルトはWMV3。例えば、DIVXなら mmioFOURCC('D','I','V','X') とする
      *				0ならば無圧縮
      *		frameRate_: フレームレート
      */
    explicit OpenGLMovie(LPCTSTR pOutputMovieFileName = _T("Output.avi"),
                DWORD codec = mmioFOURCC('W', 'M', 'V', '3'),				/// ビデオコーデック
                DWORD frameRate = 10,									/// フレームレート (FPS)
                int frameWidth = GetSystemMetrics(SM_CXSCREEN),			/// ムービーフレーム幅
                int frameHeight = GetSystemMetrics(SM_CYSCREEN),		/// ムービーフレーム高さ
                int bitsPerPixel = 24)									/// 単位ピクセル当たりのビット数
        : movieFile(pOutputMovieFileName, codec, frameRate)
    {
        movieWidth = 0;
        movieHeight = 0;
        movieBitsPerPixel = 0;
        pFramePixelBits = NULL;

        initialize(frameWidth, frameHeight, bitsPerPixel);
    }

    /**
     *	説明
     *		デストラクタ
     *	引数
     *		動画ファイルを閉じて、全フレームを消去
     */
    virtual ~OpenGLMovie(void)
    {
        if (pFramePixelBits)
        {
            free(pFramePixelBits);
            pFramePixelBits = NULL;
        }
    }

    /**
     *	----------------------------------------
     *	初期化
     *	----------------------------------------
     */
    void initialize(int frameWidth = GetSystemMetrics(SM_CXSCREEN),			/// ムービーフレーム幅
            int frameHeight = GetSystemMetrics(SM_CYSCREEN),		/// ムービーフレーム高さ
            int bitsPerPixel = 24)									/// 単位ピクセル当たりのビット数	
    {
        movieWidth = frameWidth;
        movieHeight = frameHeight;
        movieBitsPerPixel = bitsPerPixel;

        if (pFramePixelBits)
        {
            free(pFramePixelBits);
            pFramePixelBits = NULL;
        }

        /// ピクセルデータのビット列を保持するために必要なメモリを確保
        pFramePixelBits = malloc(movieWidth * movieHeight * movieBitsPerPixel / 8);
    }

    /**
     *	説明
     *		動画の記録
     *		・OpenGLのフレームバッファの内容を読み込み、読み込み済みフレームデータを
     *		　新しいフレームとして、動画ファイルに挿入する。
     *		・各レンダリングフレーム毎のSwapBuffers()メソッド前にて呼び出す必要がある
     */
    inline HRESULT recordFrame()
    {
        glFlush();
        glFinish();
        glReadPixels(0, 0, movieWidth, movieHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pFramePixelBits);

        return movieFile.appendNewFrame(movieWidth, movieHeight, pFramePixelBits, movieBitsPerPixel);
    }

    inline const AviMaker* getMovieFile() const { return &movieFile; }
};	/// end of class OpenGLMovie

}	/// end of namespace Graphics

#endif	/// OpenGLMovie