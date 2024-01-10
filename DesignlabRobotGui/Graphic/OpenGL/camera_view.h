
#ifndef DESIGNLABROBOTGUI_GRAPHIC_OPENGL_CAMERA_VIEW_H_
#define DESIGNLABROBOTGUI_GRAPHIC_OPENGL_CAMERA_VIEW_H_

#include <cassert>

#include "Math/math_constant.h"


namespace designlab_robot_gui::graphic
{

class CameraView final
{
public:
    //! @enum Mode
    //! @brief 視点変更モード．
    enum class Mode
    {
        PAN,    //!< 並進．
        SPIN,   //!< 回転．
        ZOOM,   //!< 縮尺．
        STOP    //!< 停止．
    };

    //! @brief コンストラクタ．
    inline CameraView()
    {
        SetDefaultCondition();
    }

    ~CameraView() = default;  //!< デストラクタ．


    //! @brief 視点変更開始．
    //! @param[in] mode 変更モード．
    //! @param[in] x 起点のx座標．
    //! @param[in] y 起点のy座標．
    void BeginViewControl(Mode mode, int x, int y);

    //! @brief 視点変更を終了する．
    void EndViewControl();

    //! モードによるViewの変更を行う．
    //! @param[in] x 起点のx座標．
    //! @param[in] y 起点のy座標．
    void DoViewControl(int x, int y);


    //! @brief 視点の方位角を取得する．
    //! @return 視点の方位角．
    constexpr double GetAzimuth() const
    {
        return azimuth_;
    }

    //! @brief 視点の仰角を取得する．
    //! @return 視点の仰角．
    constexpr double GetElevation() const
    {
        return elevation_;
    }

    //! @brief 視点の距離を取得する．
    //! @return 視点の距離．
    constexpr double GetDistance() const
    {
        return distance_;
    }

    //! @brief 視点の中心位置を取得する．
    //! @param[in] num 取得する座標の番号．
    double GetViewCenterPos(const int num) const
    {
        // num は 1 から 3 までの値を取る．取らなければ assert で止まる．
        assert(1 <= num);
        assert(num <= math::THREE_DIMENSION);

        return view_center_pos_[num - 1];
    }

private:
    //! @brief 画面上の操作起点となる点をセットする．
    //! @param[in] x 起点のx座標．
    //! @param[in] y 起点のy座標．
    void SetControlPoint(int x, int y);

    //! @brief 変数の値をデフォルトにセット
    void SetDefaultCondition();


    //! @brief 視線方向を取得する．
    //! @param[out] x 視線方向のx成分．
    //! @param[out] y 視線方向のy成分．
    //! @param[out] z 視線方向のz成分．
    void GetEyeDirection(double* x, double* y, double* z);

    //! @brief 視野の上方向を取得する．
    //! @param[out] x 視野の上方向のx成分．
    //! @param[out] y 視野の上方向のy成分．
    //! @param[out] z 視野の上方向のz成分．
    void GetUpDirection(double* x, double* y, double* z);


    //! @brief 並進操作．
    void Pan(int x, int y);

    //! @brief 回転操作．
    void Spin(int x, int y);

    //! @brief 縮尺操作．
    void Zoom(int x, int y);


    double  azimuth_;    //!< 方位角(アジマス)[deg]
    double  elevation_;  //!< 仰角(エレベイション)[deg]
    double  distance_;   //!< 距離．

    double view_center_pos_[math::THREE_DIMENSION];  //!< 視点の中心位置

    double  init_azimuth_;    //!< 方位角(アジマス)[deg]
    double  init_elevation_;  //!< 仰角(エレベイション)[deg]
    double  init_distance_;   //!< 距離．

    double  init_view_center_pos_[math::THREE_DIMENSION];  //! 視点の初期中心位置．

    int init_x;  //!< 視点の2次元初期位置，起点のx座標．
    int init_y;  //!< 視点の2次元初期位置，起点のy座標．

    Mode mode_state_;  //!< 視点変更モード．
};

}  // namespace designlab_robot_gui::graphic


#endif  // DESIGNLABROBOTGUI_GRAPHIC_OPENGL_CAMERA_VIEW_H_
