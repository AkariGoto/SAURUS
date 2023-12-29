/**
 *  �t�@�C����
 *		OpenGL.h
 *  ����
 *		OpenGL�֌W�̕`���b�N���X(Windows��p)
 *  ���t
 *		�쐬��: 2007/04/01(SAT)		�X�V��: 2007/04/15(SAT)
 */

#ifndef __OpenGL_h__
#define __OpenGL_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <iostream>

#include <gl/GL.h>
#include <gl/GLU.h>
//#include <gl/GLAUX.h>

#include "..\..\Math\MathLibrary.h"
#include "OpenGLParameter.h"
#include "CameraView.h"
/**
 *	----------------------------------------------------------------------
 *		�����J�ւ̒ʒm
 *	----------------------------------------------------------------------
 */
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
//#pragma comment (lib, "glaux.lib")



namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGL�N���X
 *	----------------------------------------------------------------------
 */
class OpenGL
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
protected:
/**
 *	�n���h��
 *		windowHandle�F�E�B���h�E
 *		deviceContextHandle�F�f�o�C�X�R���e�L�X�g
 *		renderingContextHandle�F�����_�����O�R���e�L�X�g
 */
	HWND	windowHandle;
 	HDC		deviceContextHandle;
 	HGLRC	renderingContextHandle;

/**
 *	�`�悷��T�C�Y
 */
	int	sceneWidth;
	int sceneHeight;

/**
 *	���C�e�B���O
 */
	GLfloat ambientLight0[4];		/// ����
	GLfloat diffuseLight0[4];		/// �g�U��
	GLfloat specularLight0[4];		/// ���ʌ�
	GLfloat positionLight0[4];		/// �����ʒu
	GLfloat directionLight0[3];	/// �X�|�b�g���C�g����

/**
 *	���_�Ɋ֌W�������
 */

	/**
	 *	�J�������_�I�u�W�F�N�g
	 */
	CameraView cameraView;

	/// ���_�^�C�v
	ViewType viewType;

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
	OpenGL();

	/// �R���X�g���N�^
	explicit OpenGL(HWND hWnd);

	/// �f�X�g���N�^
	virtual ~OpenGL();

/**
 *	----------------------------------------
 *	OpenGL�̐ݒ�Ɋ֌W�������
 *	----------------------------------------
 */
	/**
	 *	����
	 *		������
	 *	����
	 *		hWnd: �`�悷��̈�̃E�B���h�E�n���h��
	 */
	bool initializeGL(HWND hWnd = NULL);

	/**
	 *	����
	 *		�I������
	 */
	void finalizeGL(void);

	/**
	 *	����
	 *		OpenGL�̋@�\�ݒ�
	 */
	void setOpenGLFunctions(void);

	/**
	 *	����
	 *		GL�̐ݒ�̃��Z�b�g
	 */
	void clearOpenGLSettings(void);

/**
 *	----------------------------------------
 *	�`��Ɋ֌W�������
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�Ɩ����ʂ̐ݒ�
	 *		OpenGL�ł�GL_LIGHT0����GL_LIGHT7�܂ł̂W�̌�����ݒ�ł���
	 */
	void setWorldLightings(void);

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
	 *	����
	 *		�`��̈�̃��T�C�Y����
	 */
	void resizeScenes(int width, int height);

/**
 *	----------------------------------------
 *	���_�̐ݒ�
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�V�[���̎��_��ViewType�ɂ��ݒ�
	 *	����
	 *		width: �V�[���̉���
	 *		height: �V�[���̍���
	 */
	virtual void setSceneView(double width, double height);

	/**
	 *	����
	 *		�V�[���̎��_�������ɂ��ݒ�
	 *	����
	 *		distance, azimuth, elevation�F���_�ւ̋����C���ʊp�C�p
	 *		centerX, centerY, centerZ�F���E�̒��S�ʒu�̎Q�Ɠ_���W
	 *		width: �V�[���̉���
	 *		height: �V�[���̍���
	 */
	void setViewPoint(	double distance, double azimuth, double elevation,
						double centerX = 0.0, double centerY = 0.0, double centerZ = 0.0,
						double width = 1.0, double height = 1.0);

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
	bool setWindowHandle(HWND hWnd);
	void setViewType(ViewType type);

	/**
	 *	����
	 *		�}�e���A���J���[�̐ݒ�
	 */
	void setMaterialColor(double red, double green, double blue, double alpha);
	void setMaterialColor(const GLfloat* materialAmbDiffColor);
	void setMaterialColor(COLOR color);

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	HDC getDeviceContextHandle(void) const{return deviceContextHandle;}
	HWND getWindowHandle(void) const{return windowHandle;}
	ViewType getViewType(void) const{return viewType;}

	/**
	 *	����
	 *		�}�e���A���J���[�̎擾
     */
	const GLfloat* selectMaterialColor(COLOR color);

/**
 *	----------------------------------------
 *	�J�����I�u�W�F�N�g
 *	----------------------------------------
 */
	/**
	 *	�J�����I�u�W�F�N�g�̎擾
	 */
	CameraView& getCameraView(void){return cameraView;}

	/**
	 *	�J�������_�ύX�J�n
	 */
	void beginCameraViewControl(CameraView::Mode mode, int x, int y);

	/**
	 *	�J�������_�ύX�I��
	 */
	void endCameraViewControl(void);

	/**
	 *	�J�������_�ύX
	 */
	void doCameraViewControl(int x, int y);

/**
 *	----------------------------------------
 *	OpenGL�̏������̂��߂̕⏕�֐�
 *	----------------------------------------
 */
private:

	/**
	 *	����
	 *		�s�N�Z���t�H�[�}�b�g�̐ݒ�
	 */
	bool setWindowPixelFormat(void);
	/**
	 *	����
	 *		�R���e�L�X�g�̐ݒ�
	 */
	bool createGLContext(void);

};

}	/// end of namespace Graphic

#endif	/// __OpenGL_h__
