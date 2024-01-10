
#ifndef __AsuraGraphic_h__
#define __AsuraGraphic_h__

#include "OpenGL/OpenGLObject.h"
#include "OpenGL/OpenGLMovie.h"

#include "Data/asura_data.h"


namespace designlab_robot_gui::graphic
{

constexpr int LEG_DISP_LIST_NUM = asura::LEG_JOINT_NUM + 2;

class AsuraGraphic final : public OpenGLObject
{
    using AsuraData = data::AsuraData;
    using Vector = math::Vector;
    using Matrix = math::Matrix;

private:
    /**
     *	Asuraを描画するためのデータソース
     */
    AsuraData* asura_data_ptr_;

    /**
     *	ムービーレコーダ
     */
    OpenGLMovie movieRecorder;

    /**
     *	描画のためのOpenGLのディスプレイリスト
     */
    int bodyDisplayListID;
    int legDisplayListID[LEG_DISP_LIST_NUM];



    /**
     *	影描画用変数
     */
    GLfloat planeFactorOfGridFloor[4];


    /**
     *	フラグ
     */
     /// 描画用
    bool isAsuraDrawn;
    bool isSupportPolygonDrawn;
    bool isDisplayListReady;


    bool is_recorded_animation_;  //!< AVI の録画を行うか否かのフラグ．

    bool is_displayed_leg_coordinate_;  //!< 脚座標系を表示するか否かのフラグ．



public:
    /// デフォルトコンストラクタ
    AsuraGraphic(HWND hWnd = NULL, AsuraData* asuraData = NULL,
                 ViewType type = ViewType::PERSPECTIVE);

    /// デストラクタ
    ~AsuraGraphic();

    /// 初期化
    void initializeAsuraGraphic(HWND hWnd = NULL, AsuraData* asuraData = NULL);
    /// 終了処理
    void finalizeAsuraGraphic();

    /**
     *	説明
     *		ディスプレイリストの生成・消去
     */
    void newAsuraDisplayList();
    void deleteAsuraDisplayList();

    void drawObjects(GLenum renderMode) const override;

    void DrawScenes() const override;

    void DrawShadow();

    void RenderScenes() override;

    /**
     *	説明
     *		シーンの視点をViewTypeにより設定
     */
    void setSceneView(double width, double height);

    /**
     *	Asuraの描画
     *		drawAsura			: Asuraの全体描画
     *		drawBody			: Asuraの本体描画
     *		drawLeg				: Asuraの脚描画
    */
    void drawAsura() const;
    void drawBody() const;
    void drawLeg(int legNo) const;




    //! @brief 支持脚多角形(三角形or四角形)を表示する．
    //! @todo 未使用，削除予定．
    inline void ShowSupportPolygon()
    {
        isSupportPolygonDrawn = true;
    }

    //! @brief 支持脚多角形(三角形or四角形)を非表示にする．
    //! @todo 未使用，削除予定．
    inline void HideSupportPolygon()
    {
        isSupportPolygonDrawn = false;
    }

    //! @brief AVIの録画を開始する．
    inline void StartRecording()
    {
        is_recorded_animation_ = true;
    }

    //! @brief AVIの録画を終了する．
    inline void StopRecording()
    {
        is_recorded_animation_ = false;
    }

    //! @brief 脚座標を表示するか否かを設定する．
    //! @param display 表示する場合は true，非表示にする場合は false．
    void DisplayLegCoordinate(const bool display)
    {
        is_displayed_leg_coordinate_ = display;
    }

private:
    /**
     *		OpenGL座標変換のためのヘルプ関数
     *			OpenGL用の4x4座標変換
     */
    void transformOpenGLMatrix(const Matrix& matrix) const;

    /**
     *	説明
     *		影を投影する平面の係数を計算
     *	引数
     *		planeEq: 平面の方程式の係数配列
     *		p0: 平面上の頂点
     *		p1: 平面上の頂点
     *		p2: 平面上の頂点
     */
    void calcShadowPlaneEquation(GLfloat planeEq[], GLfloat p0[], GLfloat p1[], GLfloat p2[]);

    /**
     *	説明
     *		目的の影を投影する行列
     *	引数
     *		matrix: 投影した行列
     *		plnaeEq: 平面の方程式の係数配列（要素数4）
     *		lightPos: 光源の同時座標位置（要素数4）
     */
    void calcShadowMatrix(GLfloat matrix[], GLfloat planeEq[], GLfloat lightPos[]);


    //! @brief 支持脚多角形を描画する．
    void drawSupportPolygon();
};

}	/// end of namespace Graphic

#endif	/// __AsuraGraphic_h__