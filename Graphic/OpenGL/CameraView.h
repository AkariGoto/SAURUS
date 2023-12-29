/**
 *  �t�@�C����
 *		CameraView.h
 *  ����
 *		�J�����r���[�𐶐�����i��Ƀ}�E�X����ɑΉ��j
 *  ���t
 *		�쐬��: 2007/06/04(MON)		�X�V��: 2007/06/13(WED)
 */

#ifndef __CameraView_h__
#define __CameraView_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <cmath>
#include "../../Math/MathLibrary.h"

namespace Graphic
{


/**
 *	----------------------------------------------------------------------
 *		CameraView�N���X
 *	----------------------------------------------------------------------
 */
class CameraView
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
/**
 *		�ύX���[�h
 *			PAN			: ���i
 *			SPIN		: ��]
 *			ZOOM		: �k��
 *			STOP		: ��~
 */
	enum Mode{PAN, SPIN, ZOOM, STOP};

private:
/**
 *		�J�������
 *			azimuth				: ���ʊp(�A�W�}�X)[deg]
 *			elevation			: �p(�G���x�C�V����)[deg]
 *			distance				: ����
 */
	double		azimuth;
	double		elevation;
	double		distance;

/**
 *		���_�̒��S�ʒu
 */
	double viewCenterPosition[3];

/**
 *		���_�̏������
 *			initAzimuth				: ���ʊp(�A�W�}�X)[deg]
 *			initElevation			: �p(�G���x�C�V����)[deg]
 *			initDistance			: ����
 */
	double		initAzimuth;
	double		initElevation;
	double		initDistance;

/**
 *		���_�̏������S�ʒu
 */
	double		initViewCenterPosition[Const::THREE_DIMENSION];

/**
 *		���_��2���������ʒu
 */
	int initX;
	int initY;

/**
 *		���[�h���
 */
	Mode	modeState;

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	CameraView();
	virtual ~CameraView();

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
	void beginViewControl(CameraView::Mode mode, int x, int y);

	/**
	 *	���_�ύX�I��
	 */
	void endViewControl(void);

	/**
	 *	����
	 *		���[�h�ɂ��View�̕ύX
	 */
	void doViewControl(int x, int y);

/**
 *		����̕ύX�̊e�푀��
 *			pan		: ���i����
 *			Rotate			: ��]����
 *			Scale			: �k�ڑ���
 */
	void	pan(int x, int y);
	void	spin(int x, int y);
	void	zoom(int x, int y);


/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
/**
 *	����
 *		��ʏ�̑���N�_�ƂȂ�_���Z�b�g
 */
	void setControlPoint(int x, int y);

/**
 *	����
 *		�ϐ��̒l���f�t�H���g�ɃZ�b�g
 */
	void setDefaultCondition(void);


/**
 *	����
 *		�ύX���[�h��ݒ�
 */
	void setMode(CameraView::Mode mode);

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	double getAzimuth(void) const {return azimuth;}
	double getElevation(void) const {return elevation;}
	double getDistance(void) const {return distance;}
	double getViewCenterPosition(int num) const
			{
				assert( 1<= num && num <= Const::THREE_DIMENSION);
				return viewCenterPosition[num-1];
			}

/**
 *	����
 *		�ύX���[�h���擾
 */
	CameraView::Mode getMode(void) const{ return modeState; }




private:
/**
 *	����
 *		���������̎擾
 */
	void getEyeDirection(double* x, double* y, double* z);

/**
 *	����
 *		����̏�����̎擾
 */
	void getUpDirection(double* x, double* y, double* z);

};	/// end of namespace CameraView

}	/// end of namespace Graphic	

#endif /// __CameraView_h__