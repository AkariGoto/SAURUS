
#ifndef __OpenGLObject_h__
#define __OpenGLObject_h__

#include "Graphic/OpenGL/abstract_open_gl_base.h"

#include "Math/Matrix/matrix.h"
#include "Math/Matrix/matrix_library.h"


namespace designlab_robot_gui::graphic
{

class OpenGLObject : public AbstractOpenGLBase
{
    using Vector = math::Vector;
    using Matrix = math::Matrix;

public:
    /// デフォルトコンストラクタ
    OpenGLObject() = default;

    /// デフォルトコンストラクタ
    inline explicit OpenGLObject(HWND hWnd)
    {
        createGLObject(hWnd);
    }

    /// デストラクタ
    inline virtual ~OpenGLObject()
    {
        destroyGLObject();
    }

    /**
     *	説明
     *		OpenGLのオブジェクト描画
     */
    virtual void drawObjects(GLenum renderMode) const;

    /**
     *	説明
     *		OpenGLイメージのレンダリング
     *		OpenGL描画のコールバック関数になる
     */
    virtual void RenderScenes();



public:




    /// フレーム描画(平面と4隅の垂線)
    void drawFrame(double startX, double startY, double startZ, double endX, double endY, double endZ);
    void drawFrame(const Vector& startPoint, const Vector& endPoint);



    /// 三角形描画
    void drawTriangle(const Vector& point1, const Vector& point2, const Vector& point3, double width = 1.0);

    /// 四角形描画
    void drawQuadrangle(const Vector& point1, const Vector& point2,
                    const Vector& point3, const Vector& point4,
                    double width = 1.0);

    /**
     *	----------------------------------------
     *	2Dモデリング
     *	----------------------------------------
     */
     /**
      *	説明
      *		平面板描画(対角線の頂点と高さ)
      */
      /// 水平面（XY）
    void drawHorizontalPlane(double startX, double startY, double endX, double endY, double z);
    /// 前額面（YZ）
    void drawFrontalPlane(double startY, double startZ, double endY, double endZ, double x);
    /// 矢状面（ZX）
    void drawSagittalPlane(double startZ, double startX, double endZ, double endX, double y);

    /**
     *	説明
     *		メッシュ状の床を描画する
     *	引数
     *		startX, startY：床の対角線の始点
     *		endX, endY：床の対角線の終点
     *		z：床の高さ
     *		dx, dy：メッシュの間隔
     */
    void drawGridFloor(void);


protected:
    /**
 *	説明
 *		OpenGLのイメージ描画
 */
    virtual void DrawScenes() const override;

    //! @brief OpenGLの初期化を行う．OpenGLObjectに関連づける．
    //! @param[in] window_handle 描画する領域のウィンドウハンドル．
    bool createGLObject(HWND window_handle);

    /**
     *	説明
     *		終了処理
     */
    void destroyGLObject();

    //! @brief 座標軸を描画する．
    //! @param[in] length 座標軸の長さ．
    //! @param[in] width 座標軸の太さ．
    //! @param[in] scale 座標軸のスケール．
    void DrawCoordinateAxis(double length = 300.0, double width = 2.0,
                            double scale = 1.0) const;

    //! @brief 円柱を描画する．
    //! @param[in] radius 円柱の半径．
    //! @param[in] height 円柱の高さ．
    void DrawCylinder(double radius, double height) const;

    //! @brief 円柱の描画を行う．
    //! @param[in] radius 円柱の半径．
    void DrawSphere(double radius) const;

    //! @brief 線分の描画を行う．
    //! @param[in] start_x 始点のx座標．
    //! @param[in] start_y 始点のy座標．
    //! @param[in] start_z 始点のz座標．
    //! @param[in] end_x 終点のx座標．
    //! @param[in] end_y 終点のy座標．
    //! @param[in] end_z 終点のz座標．
    //! @param[in] width 線分の太さ．デフォルトでは1．
    void DrawLine(double start_x, double start_y, double start_z,
                  double end_x, double end_y, double end_z,
                  double width = 1.0) const;

    //! @brief 線分の描画を行う．
    //! @param[in] start_point 始点の座標．
    //! @param[in] end_point 終点の座標．
    //! @param[in] width 線分の太さ．デフォルトでは1．
    void DrawLine(const Vector& start_point, const Vector& end_point,
                  double width = 1.0) const;

    //! @brief 2点を指定して，直方体を描画する．
    //! @param[in] start_x 始点のx座標．
    //! @param[in] start_y 始点のy座標．
    //! @param[in] start_z 始点のz座標．
    //! @param[in] end_x 終点のx座標．
    //! @param[in] end_y 終点のy座標．
    //! @param[in] end_z 終点のz座標．
    void DrawBox(double start_x, double start_y, double start_z,
                 double end_x, double end_y, double end_z) const;

    //! @brief 2点を指定して，直方体を描画する．
    //! @param[in] start_point 始点の座標．
    //! @param[in] end_point 終点の座標．
    void DrawBox(const Vector& start_point, const Vector& end_point);

    /**
     *	説明
     *		ディスプレイリストの生成
     */
    void newGLObjDispList();
    /**
     *	説明
     *		ディスプレイリストの消去
     */
    void deleteObjDispList();


    bool is_drawn_coordinate_axis_;  //!< 座標軸を描画するかどうか．

    bool is_drawn_grid_floor_;  //!< メッシュの床を描画するかどうか．

    int grid_floor_display_list_id_;  //!< 床描画のためのディスプレイリスト．
};

}  // namespace designlab_robot_gui::graphic


#endif	/// __OpenGLObject_h__
