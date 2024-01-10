
#include "pch.h"

#include "Graphic/OpenGL/camera_view.h"

#include "Math/Matrix/matrix_library.h"


namespace  designlab_robot_gui::graphic
{

void  CameraView::BeginViewControl(const Mode mode, const  int x, const  int y)
{
    mode_state_ = mode;

    init_x = x;
    init_y = y;

    init_azimuth_ = azimuth_;
    init_elevation_ = elevation_;
    init_distance_ = distance_;

    for (int i = 0; i < math::THREE_DIMENSION; i++)
    {
        init_view_center_pos_[i] = view_center_pos_[i];
    }
}

void  CameraView::EndViewControl()
{
    mode_state_ = Mode::STOP;
}

void  CameraView::DoViewControl(const int x, const int y)
{
    switch (mode_state_)
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
    const double dx = static_cast<double>(x) - init_x;
    const double dy = static_cast<double>(y) - init_y;

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
        view_center_pos_[i] = (vx * dx * right[i] + vy * dy * up_view[i]) +
            init_view_center_pos_[i];
    }
}

void CameraView::Spin(const int x, const  int y)
{
    double dx = static_cast<double>(x) - init_x;
    double dy = static_cast<double>(y) - init_y;

    azimuth_ = (-0.5) * dx + init_azimuth_;
    elevation_ = (0.5) * dy + init_elevation_;
}

void CameraView::Zoom(const int x, const int y)
{
    // const double dx = static_cast<double>(x) - init_x;
    const double dy = static_cast<double>(y) - init_y;

    const double dl = dy;

    distance_ = 100.0 * dl + init_distance_;

    return;
}

void CameraView::SetControlPoint(const int x, const int y)
{
    init_x = x;
    init_y = y;

    init_azimuth_ = azimuth_;
    init_elevation_ = elevation_;
    init_distance_ = distance_;

    for (int i = 0; i < math::THREE_DIMENSION; i++)
    {
        init_view_center_pos_[i] = view_center_pos_[i];
    }
}

void CameraView::SetDefaultCondition()
{
    init_x = 0;
    init_y = 0;

    azimuth_ = 90;
    elevation_ = 15;
    distance_ = 2500;

    for (int i = 0; i < math::THREE_DIMENSION; i++)
    {
        view_center_pos_[i] = 0.0;
    }

    SetControlPoint(0, 0);

    mode_state_ = Mode::STOP;
}

void CameraView::GetEyeDirection(double* x, double* y, double* z)
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(z != nullptr);

    const double xo = cos(elevation_ * math::PI / 180) *
        cos(azimuth_ * math::PI / 180);

    const double yo = cos(elevation_ * math::PI / 180) *
        sin(azimuth_ * math::PI / 180);

    const double zo = sin(elevation_ * math::PI / 180);

    *x = xo;
    *y = yo;
    *z = zo;
}

void CameraView::GetUpDirection(double* x, double* y, double* z)
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(z != nullptr);

    const double xo = distance_ * cos((elevation_ + 90) * math::PI / 180) *
        cos(azimuth_ * math::PI / 180);

    const double yo = distance_ * cos((elevation_ + 90) * math::PI / 180) *
        sin(azimuth_ * math::PI / 180);

    const double zo = distance_ * sin((elevation_ + 90) * math::PI / 180);

    *x = xo;
    *y = yo;
    *z = zo;
}

}  // namespace  designlab_robot_gui::graphic
