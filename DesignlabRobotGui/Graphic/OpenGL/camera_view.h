
#ifndef __CameraView_h__
#define __CameraView_h__

#include <cassert>

#include "Math/math_constant.h"


namespace designlab_robot_gui::graphic
{

class CameraView final
{
public:
    //! @enum Mode
    //! @brief 視点変更モード
    enum class Mode
    {
        PAN,    //!< 並進
        SPIN,   //!< 回転
        ZOOM,   //!< 縮尺
        STOP    //!< 停止
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


    double getAzimuth() const { return azimuth; }
    double getElevation() const { return elevation; }
    double getDistance() const { return distance; }
    double getViewCenterPosition(int num) const
    {
        assert(1 <= num && num <= designlab_robot_gui::math::THREE_DIMENSION);
        return viewCenterPosition[num - 1];
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


    double  azimuth;    //!< 方位角(アジマス)[deg]
    double  elevation;  //!< 仰角(エレベイション)[deg]
    double  distance;   //!< 距離

    double viewCenterPosition[3];   //!< 視点の中心位置

    double  initAzimuth;    //!< 方位角(アジマス)[deg]
    double  initElevation;  //!< 仰角(エレベイション)[deg]
    double  initDistance;   //!< 距離

    //! 視点の初期中心位置
    double  initViewCenterPosition[designlab_robot_gui::math::THREE_DIMENSION];

    int initX;  //!< 視点の2次元初期位置，起点のx座標．
    int initY;  //!< 視点の2次元初期位置，起点のy座標．

    Mode modeState;  //!< 視点変更モード
};

}  // namespace designlab_robot_gui::graphic


#endif /// __CameraView_h__