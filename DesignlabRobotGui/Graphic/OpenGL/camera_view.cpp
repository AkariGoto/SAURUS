
#include "pch.h"

#include "Graphic/OpenGL/camera_view.h"

#include "Math/Matrix/matrix_library.h"


namespace  designlab_robot_gui::graphic
{

void  CameraView::BeginViewControl(CameraView::Mode mode, int x, int y)
{
    modeState = mode;

    initX = x;
    initY = y;

    initAzimuth = azimuth;
    initElevation = elevation;
    initDistance = distance;

    for (int i = 0; i < math::THREE_DIMENSION; i++)
    {
        initViewCenterPosition[i] = viewCenterPosition[i];
    }
}

void  CameraView::EndViewControl()
{
    modeState = Mode::STOP;
}

void  CameraView::DoViewControl(const int x, const int y)
{
    switch (modeState)
    {
        case Mode::PAN:
        {
            Pan(x, y);
            break;
        }
        case Mode::SPIN:
        {
            Spin(x, y);
            break;
        }
        case Mode::ZOOM:
        {
            Zoom(x, y);
            break;
        }
        case Mode::STOP:
            [[fallthrough]];
        default:
        {
            break;
        }
    }
}

void CameraView::Pan(const int x, const  int y)
{
    const double dx = static_cast<double>(x) - initX;
    const double dy = static_cast<double>(y) - initY;

    double up_view[3] = {};
    double view[3] = {};
    double right[3] = {};

    GetUpDirection(&up_view[0], &up_view[1], &up_view[2]);

    GetEyeDirection(&view[0], &view[1], &view[2]);

    math::outerProduct(view[0], view[1], view[2],
                       up_view[0], up_view[1], up_view[2],
                       &right[0], &right[1], &right[2]);

    const double vx = 5.0E-4;
    const double vy = 5.0E-4;

    for (int i = 0; i < math::THREE_DIMENSION; i++)
    {
        viewCenterPosition[i] = (vx * dx * right[i] + vy * dy * up_view[i]) +
            initViewCenterPosition[i];
    }
}

void CameraView::Spin(const int x, const  int y)
{
    double dx = static_cast<double>(x) - initX;
    double dy = static_cast<double>(y) - initY;

    azimuth = (-0.5) * dx + initAzimuth;
    elevation = (0.5) * dy + initElevation;
}

void CameraView::Zoom(const int x, const int y)
{
    // const double dx = static_cast<double>(x) - initX;
    const double dy = static_cast<double>(y) - initY;

    const double dl = dy;

    distance = 100.0 * dl + initDistance;

    return;
}

void CameraView::SetControlPoint(const int x, const int y)
{
    initX = x;
    initY = y;

    initAzimuth = azimuth;
    initElevation = elevation;
    initDistance = distance;

    for (int i = 0; i < math::THREE_DIMENSION; i++)
    {
        initViewCenterPosition[i] = viewCenterPosition[i];
    }
}

void CameraView::SetDefaultCondition()
{
    initX = 0;
    initY = 0;

    azimuth = 90;
    elevation = 15;
    distance = 2500;

    for (int i = 0; i < math::THREE_DIMENSION; i++)
    {
        viewCenterPosition[i] = 0.0;
    }

    SetControlPoint(0, 0);

    modeState = Mode::STOP;

    return;
}

void CameraView::GetEyeDirection(double* x, double* y, double* z)
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(z != nullptr);

    const double xo = cos(elevation * math::PI / 180) *
        cos(azimuth * math::PI / 180);

    const double yo = cos(elevation * math::PI / 180) *
        sin(azimuth * math::PI / 180);

    const double zo = sin(elevation * math::PI / 180);

    *x = xo;
    *y = yo;
    *z = zo;
}

void CameraView::GetUpDirection(double* x, double* y, double* z)
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(z != nullptr);

    const double xo = distance * cos((elevation + 90) * math::PI / 180) *
        cos(azimuth * math::PI / 180);

    const double yo = distance * cos((elevation + 90) * math::PI / 180) *
        sin(azimuth * math::PI / 180);

    const double zo = distance * sin((elevation + 90) * math::PI / 180);

    *x = xo;
    *y = yo;
    *z = zo;
}

}  // namespace  designlab_robot_gui::graphic
