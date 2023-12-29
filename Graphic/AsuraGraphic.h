/**
 *  �t�@�C����
 *		AsuraGraphic.h
 *  ����
 *		OpenGL��p����TITAN X��`�悷��N���X
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2008/07/07(MON)
 */

//  20200819  �N���[���֘A�R�����g�A�E�g
//  20201019  �r��̃��[���h���W�Ƌr���W�̐؂�ւ�

#ifndef __AsuraGraphic_h__
#define __AsuraGraphic_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "OpenGL\OpenGLObject.h"
#include "OpenGL\OpenGLText.h"
#include "OpenGL\OpenGLMovie.h"

#include "..\Data\AsuraData.h"


namespace Graphic
{

/**
 *	----------------------------------------------------------------------
 *		�萔�錾
 *	----------------------------------------------------------------------
 */
const int LEG_DISP_LIST_NUM = Asura::LEG_JOINT_NUM+2;

/*  20200819
const int TRACK_DISP_LIST_NUM = Asura::TRACK_JOINT_NUM+2;
*/

/**
 *	----------------------------------------------------------------------
 *		AsuraGraphic�N���X
 *	----------------------------------------------------------------------
 */
class AsuraGraphic : public OpenGLObject
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:
	/**
	 *	Asura��`�悷�邽�߂̃f�[�^�\�[�X
	 */
	Data::AsuraData* asuraDataSrcPtr;
	
	/**
	 *	���[�r�[���R�[�_
	 */
	OpenGLMovie movieRecorder;

	/**
	 *	�`��̂��߂�OpenGL�̃f�B�X�v���C���X�g
	 */
	int bodyDisplayListID;
	int legDisplayListID[LEG_DISP_LIST_NUM];

	/*  20200819
	int trackDisplayListID[TRACK_DISP_LIST_NUM];
	*/

	/**
	 *	�e�`��p�ϐ�
	 */
	GLfloat planeFactorOfGridFloor[4];


	/**
	 *	�t���O
	 */
	/// �`��p
	bool isAsuraDrawn;
	bool isSupportPolygonDrawn;
	bool isDisplayListReady;

	/// AVI�ۑ��p
	bool isAnimationRecorded;

//20201019
	bool legCoordinate;  //�r���W��\�����邩�ۂ��̃t���O


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
	/// �f�t�H���g�R���X�g���N�^
	explicit AsuraGraphic(HWND hWnd = NULL, Data::AsuraData* asuraData = NULL, ViewType type = PERSPECTIVE);

	/// �f�X�g���N�^
	virtual ~AsuraGraphic();

/**
 *	----------------------------------------
 *	OpenGL�̐ݒ�̏������E�I������
 *	----------------------------------------
 */
	/// ������
	void initializeAsuraGraphic(HWND hWnd = NULL, Data::AsuraData* asuraData = NULL);
	/// �I������
	void finalizeAsuraGraphic(void);

/**
 *	����
 *		�f�B�X�v���C���X�g�̐����E����
 */
	void newAsuraDisplayList(void);
	void deleteAsuraDisplayList(void);

/**
 *	----------------------------------------
 *	�`��Ɋ֌W�������
 *	----------------------------------------
 */
	/**
	 *	����
	 *		OpenGL�̃I�u�W�F�N�g�`��
	 */
	virtual void drawObjects(GLenum renderMode);

/**
 *	����
 *		OpenGL�C���[�W�`��
 */
	virtual void drawScenes(void);

/**
 *	����
 *		�I�u�W�F�N�g�̉e�`��
 */
	virtual void drawShadow(void);

	/**
	 *	����
	 *		OpenGL�C���[�W�̃����_�����O
	 *		OpenGL�`��̃R�[���o�b�N�֐��ɂȂ�
	 */
	virtual void renderScenes(void);

/**
 *	����
 *		�V�[���̎��_��ViewType�ɂ��ݒ�
 */
	virtual void setSceneView(double width, double height);

/**
 *	Asura�̕`��
 *		drawAsura			: Asura�̑S�̕`��
 *		drawBody			: Asura�̖{�̕`��
 *		drawLeg				: Asura�̋r�`��
*/
	void drawAsura(void);
	void drawBody(void);
	void drawLeg(int legNo);
	void drawTrack(int trackNo);

/**
 *	�x���r���p�`(�O�p�`or�l�p�`)�̕`��
 */
	void drawSupportPolygon(void);

/**
 *	�x���r���p�`(�O�p�`or�l�p�`)��������/�B��
 */
	/// ������
	void showSupportPolygon(void) { isSupportPolygonDrawn = true; return; }
	/// �B��
	void hideSupportPolygon(void) { isSupportPolygonDrawn = false; return; }

/**
 *	AVI��ۑ����邩�ǂ���
 */
	/// �^�悷��
	void startRecording(void) {isAnimationRecorded = true; return;}
	/// �^���~����
	void stopRecording(void) {isAnimationRecorded = false; return;}

	//20201019  �r���W�n�̃t���O�̊֐�
	void legCoordinateOn(void) { legCoordinate = true; }  //�r���W�n��\������
	void legCoordinateOff(void) { legCoordinate = false; }  //�r���W�n��\�����Ȃ�

private:
	/**
	 *		OpenGL���W�ϊ��̂��߂̃w���v�֐�
	 *			OpenGL�p��4x4���W�ϊ�
	 */
	void transformOpenGLMatrix(const Math::Matrix& matrix);

	/**
	 *	����
	 *		�e�𓊉e���镽�ʂ̌W�����v�Z
	 *	����
	 *		planeEq: ���ʂ̕������̌W���z��
	 *		p0: ���ʏ�̒��_
	 *		p1: ���ʏ�̒��_
	 *		p2: ���ʏ�̒��_
	 */
	void calcShadowPlaneEquation(GLfloat planeEq[], GLfloat p0[], GLfloat p1[], GLfloat p2[]);

	/**
	 *	����
	 *		�ړI�̉e�𓊉e����s��
	 *	����
	 *		matrix: ���e�����s��
	 *		plnaeEq: ���ʂ̕������̌W���z��i�v�f��4�j
	 *		lightPos: �����̓������W�ʒu�i�v�f��4�j
	 */
	 void calcShadowMatrix(GLfloat matrix[], GLfloat planeEq[], GLfloat lightPos[]);

};

}	/// end of namespace Graphic

#endif	/// __AsuraGraphic_h__