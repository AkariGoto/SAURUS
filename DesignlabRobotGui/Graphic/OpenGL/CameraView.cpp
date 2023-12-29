/**
 *  �t�@�C����
 *		CameraView.cpp
 *  ����
 *		�J�����r���[�𐶐�����i��Ƀ}�E�X����ɑΉ��j
 *  ���t
 *		�쐬��: 2007/06/04(MON)		�X�V��: 2007/06/13(WED)
 */

//�����J�����ʒu�ݒ�  20200822

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "CameraView.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		CameraView�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		CameraView�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
CameraView::CameraView()
{
	setDefaultCondition();
}

/// �f�X�g���N�^
CameraView::~CameraView()
{
}

/**
 *	----------------------------------------
 *	��ȑ���
 *	----------------------------------------
 */
/**
 *	����
 *		���_�ύX�J�n
 *	����
 *		mode: �ύX���[�h
 *		x: �N�_��x���W
 *		y: �N�_��y���W
 */
void  CameraView::beginViewControl(CameraView::Mode mode, int x, int y)
{
	modeState = mode;

	initX = x;
	initY = y;

	initAzimuth		= azimuth;
	initElevation	= elevation;
	initDistance	= distance;

	for (int i=0; i<THREE_DIMENSION; i++)
		initViewCenterPosition[i] = viewCenterPosition[i];
			
	return;
}

/**
 *	���_�ύX�I��
 */
void  CameraView::endViewControl(void)
{
	modeState = CameraView::STOP;
}

/**
 *	����
 *		���[�h�ɂ��View�̕ύX
 */
void  CameraView::doViewControl(int x, int y)
{
	switch ( modeState )
	{
		case CameraView::PAN:
			pan(x, y);
			break;
		
		case CameraView::SPIN:
			spin(x, y);
			break;

		case CameraView::ZOOM:
			zoom(x, y);
			break;

		case CameraView::STOP:
			break;
			
		default:
			return;
			break;
	}
}

/**
 *	����
 *		����̕ύX�̊e�푀��
 */
/**
 *	----------------------------------------
 *	����̕ύX�̊e�푀��
 *	----------------------------------------
 */
/**
 *	���i����
 */
void CameraView::pan(int x, int y)
{
	double dx = (double)(x - initX);
	double dy = (double)(y - initY);

	double upview[3];
	double view[3];
	double right[3];

	getUpDirection(&upview[0], &upview[1], &upview[2]);

	getEyeDirection(&view[0], &view[1], &view[2]);

	outerProduct(view[0], view[1], view[2],
				 upview[0], upview[1], upview[2],
				 &right[0], &right[1], &right[2]
				 );

	double vx = 5.0E-4;
	double vy = 5.0E-4;

	for (int i=0; i<THREE_DIMENSION; i++)
	{
		viewCenterPosition[i] = ( vx*dx*right[i] + vy*dy*upview[i] ) + initViewCenterPosition[i];
	}

	return;
}

/**
 *	��]����
 */
void CameraView::spin(int x, int y)
{
	double dx = (double)(x - initX);
	double dy = (double)(y - initY);

	azimuth		= (-0.5)*(double)dx + initAzimuth;
	elevation	= (0.5)*(double)dy + initElevation;

	return;
}

/**
 *	�k�ڑ���
 */
void CameraView::zoom(int x, int y)
{
	double dx = (double)(x - initX);
	double dy = (double)(y - initY);

	double dl = dy;

	distance = (100.0)*dl + initDistance;

	return;
}

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
/**
 *	����
 *		��ʏ�̑���N�_�ƂȂ�_���Z�b�g
 */
void CameraView::setControlPoint(int x, int y)
{
	initX = x;
	initY = y;

	initAzimuth		= azimuth;
	initElevation	= elevation;
	initDistance	= distance;

	for (int i=0; i<THREE_DIMENSION; i++)
		initViewCenterPosition[i] = viewCenterPosition[i];
			
	return;
}

/**
 *	����
 *		�ϐ��̒l���f�t�H���g�ɃZ�b�g
 */
void CameraView::setDefaultCondition(void)
{
	initX = 0;
	initY = 0;

	/*
	azimuth		= 45;
	elevation	= 45;
	distance	= 2500;
	*/
	//20200822
	azimuth = 90;
	elevation = 15;
	distance = 2500;

	for (int i=0; i<THREE_DIMENSION; i++)
		viewCenterPosition[i] = 0.0;

	setControlPoint(0, 0);

	modeState = CameraView::STOP;

	return;
}

/**
 *	����
 *		�ύX���[�h��ݒ�
 */
void CameraView::setMode(CameraView::Mode mode)
{
	modeState = mode;
}



/**
 *	����
 *		���������̎擾
 */
void CameraView::getEyeDirection(double* x, double* y, double* z)
{
	double xo = cos(elevation*PI/180)*cos(azimuth*PI/180);
	double yo = cos(elevation*PI/180)*sin(azimuth*PI/180);
	double zo = sin(elevation*PI/180);

	*x = xo;
	*y = yo;
	*z = zo; 

	return;
}

/**
 *	����
 *		����̏�����̎擾
 */
void CameraView::getUpDirection(double* x, double* y, double* z)
{
	double xo = distance*cos( (elevation+90)*PI/180 )*cos(azimuth*PI/180);
	double yo = distance*cos( (elevation+90)*PI/180 )*sin(azimuth*PI/180);
	double zo = distance*sin( (elevation+90)*PI/180 );
 
	*x = xo;
	*y = yo;
	*z = zo;
	
	return;
}

}	/// end of namespace Graphic