/**
 *  �t�@�C����
 *		OpenGLObject.h
 *  ����
 *		OpenGL�ɂ�镨�̕`��N���X(Windows��p)
 *  ���t
 *		�쐬��: 2007/04/01(SAT)		�X�V��: 2007/04/17(MON)
 */

#ifndef __OpenGLObject_h__
#define __OpenGLObject_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "OpenGL.h"

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGLObject�N���X
 *	----------------------------------------------------------------------
 */
class OpenGLObject : public OpenGL
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
protected:
/**
 *	----------------------------------------
 *	�s�b�L���O�Ɋւ������
 *	----------------------------------------
 */
	/// �ŏI�I�ɑI�������I�u�W�F�N�g�̖��O�ԍ����i�[����
	unsigned int nameNumberOfHitObj[NAME_ARRAY_SIZE];	

/**
 *	----------------------------------------
 *	�t���O
 *	----------------------------------------
 */
	/// ���W����`�悷�邩�ǂ���
	bool isCoordinateAxisDrawn;

	/// ���b�V���̏���`�悷�邩�ǂ���
	bool isGridFloorDrawn;

	/// ���`��̂��߂̃f�B�X�v���C���X�g
	int gridFloorDisplayListID;

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
	OpenGLObject();

	/// �f�t�H���g�R���X�g���N�^
	explicit OpenGLObject(HWND hWnd);

	/// �f�X�g���N�^
	virtual ~OpenGLObject();

/**
 *	----------------------------------------
 *	OpenGL�̃I�u�W�F�N�g�쐬�E����
 *	----------------------------------------
 */
	/**
	 *	����
	 *		OpenGL�̏��������s���AOpenGLObject�Ɋ֘A�Â���
	 *	����
	 *		hWnd: �`�悷��̈�̃E�B���h�E�n���h��
	 */
	bool createGLObject(HWND hWnd = NULL);

	/**
	 *	����
	 *		�I������
	 */
	void destroyGLObject(void);

/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�V�[���̕`��
 *	----------------------------------------
 */
	/**
	 *	����
	 *		OpenGL�̃I�u�W�F�N�g�`��
	 */
	virtual void drawObjects(GLenum renderMode);

	/**
	 *	����
	 *		OpenGL�̃C���[�W�`��
	 */
	virtual void drawScenes(void);

	/**
	 *	����
	 *		OpenGL�C���[�W�̃����_�����O
	 *		OpenGL�`��̃R�[���o�b�N�֐��ɂȂ�
	 */
	virtual void renderScenes(void);

/**
 *	----------------------------------------
 *	�s�b�L���O�Ɋւ������
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�f�o�C�X�i�}�E�X�Ȃǁj�ɂ�镨�̂̃s�b�N�A�b�v�֐�
	 *		�f�o�C�X�ɂ��C�x���g�i�}�E�X�������獶�N���b�N�Ȃǁj�֐����ŌĂ�
	 *	����
	 *		x: �N���b�N�����_��x���W
	 *		y: �N���b�N�����_��y���W
	 */
	virtual void pickup(int x, int y);

protected:
	/**
	 *	����
	 *		�Z���N�V�����o�b�t�@����f�v�X�o�b�t�@����ёւ����肵�Ȃ���q�b�g�����I�u�W�F�N�g�����ʂ���
	 *	����
	 *		hitCount: �q�b�g��
	 *		selectBuffer:	glSelectBuffer() �̌��ʂ��i�[����Ă���Z���N�V�����o�b�t�@
	 *						�I�������I�u�W�F�N�g�̖��O�A�Z���N�V�������ꂽ�ʒu�̉��s���̍ŏ��l�ƍő�l�Ȃǂ��i�[���ꂽ�o�b�t�@
	 */
	virtual int selectHitObjects(GLuint hitCount, GLuint selectBuffer[]);

	/**
	 *	����
	 *		�s�b�N�A�b�v�ɂ��I�񂾕��̂̏���
	 */
	virtual void handlePickupResult(void);

/**
 *	------------------------------------------------------------
 *		�I�u�W�F�N�g�̕`��Ɋ֌W�������
 *	------------------------------------------------------------
 */

public:
/**
 *	----------------------------------------
 *	���̂̕\��/��\��
 *	----------------------------------------
 */
	/// ����������
	void showGridFloor(void) {isGridFloorDrawn = true; return;}
	/// �����B��
	void hideGridFloor(void) {isGridFloorDrawn = false; return;}

	/// ���W����������
	void showCoordinateAxis(void) {isCoordinateAxisDrawn = true; return;}
	/// ���W�����B��
	void hideCoordinateAxis(void) {isCoordinateAxisDrawn = false; return;}

/**
 *	----------------------------------------
 *	3D���f�����O
 *	----------------------------------------
 */
	/// ���W���`��
	void drawCoordinateAxis(double length = 300.0, double width = 2.0, double scale = 1.0); 

	/**
	 *	����
	 *		�|���S�����f�����O�i�@�����v�Z�j
	 *		�Y��(1, 2, 3)�̏���CW�i���v���j�œ���
	 *		�e���_�̍��W�����
	 */
	void drawPolygonSurface(double x1, double y1, double z1,
							double x2, double y2, double z2,
							double x3, double y3, double z3);
	void drawPolygonSurface(const Math::Vector& point1, const Math::Vector& point2, const Math::Vector& point3);

	/// �~���`��
	void drawCylinder(double radius, double height);

	/// ���`��
	void drawSphere(double radius);

	/// Box�`��(����)
	void drawBox(double startX, double startY, double startZ, double endX, double endY, double endZ);
	void drawBox(const Math::Vector& startPoint, const Math::Vector& endPoint);

	/// �t���[���`��(���ʂ�4���̐���)
	void drawFrame(double startX, double startY, double startZ, double endX, double endY, double endZ);
	void drawFrame(const Math::Vector& startPoint, const Math::Vector& endPoint);

	/// ���`��(����)
	void drawLine(double startX, double startY, double startZ,
						 double endX, double endY, double endZ, 
						 double width = 1.0);
	void drawLine(const Math::Vector& startPoint, const Math::Vector& endPoint, double width = 1.0);

	/// �O�p�`�`��
	void drawTriangle(const Math::Vector& point1, const Math::Vector& point2, const Math::Vector& point3, double width = 1.0);

	/// �l�p�`�`��
	void drawQuadrangle(	const Math::Vector& point1, const Math::Vector& point2, 
									const Math::Vector& point3, const Math::Vector& point4,
									double width = 1.0);

/**
 *	----------------------------------------
 *	2D���f�����O
 *	----------------------------------------
 */
	/**
	 *	����
	 *		���ʔ`��(�Ίp���̒��_�ƍ���)
	 */
	/// �����ʁiXY�j
	void drawHorizontalPlane(double startX, double startY, double endX, double endY, double z);
	/// �O�z�ʁiYZ�j
	void drawFrontalPlane(double startY, double startZ, double endY, double endZ, double x);
	/// ���ʁiZX�j
	void drawSagittalPlane(double startZ, double startX, double endZ, double endX, double y);

	/**
	 *	����
	 *		���b�V����̏���`�悷��
	 *	����
	 *		startX, startY�F���̑Ίp���̎n�_
	 *		endX, endY�F���̑Ίp���̏I�_
	 *		z�F���̍���
	 *		dx, dy�F���b�V���̊Ԋu
	 */
	void drawGridFloor(void);

/**
 *	----------------------------------------
 *	�f�B�X�v���C���X�g
 *	----------------------------------------
 */
protected: 
	/**
	 *	����
	 *		�f�B�X�v���C���X�g�̐���
	 */
	void newGLObjDispList(void);
	/**
	 *	����
	 *		�f�B�X�v���C���X�g�̏���
	 */
	void deleteObjDispList(void);	
	
private:

};	/// end of class OpenGLObject

}	/// end of namespace Graphic

#endif	/// __OpenGLObject_h__