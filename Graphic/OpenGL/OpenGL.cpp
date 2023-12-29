/**
 *  �t�@�C����
 *		OpenGL.cpp
 *  ����
 *		OpenGL�֌W�̕`���b�N���X(Windows��p)
 *  ���t
 *		�쐬��: 2007/04/01(SAT)		�X�V��: 2007/04/15(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "OpenGL.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGL�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		OpenGL�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
OpenGL::OpenGL()
{
	/// �n���h����������
	windowHandle = NULL;
 	deviceContextHandle = NULL;
 	renderingContextHandle = NULL;

	/// ���_����
	viewType = PERSPECTIVE;
}

/// �R���X�g���N�^
OpenGL::OpenGL(HWND hWnd)
{
	/// �n���h����������
	windowHandle = NULL;
 	deviceContextHandle = NULL;
 	renderingContextHandle = NULL;

	/// ���_����
	viewType = PERSPECTIVE;

	/// OpenGL������
	initializeGL(hWnd);

}

/// �f�X�g���N�^
OpenGL::~OpenGL()
{
	finalizeGL();
}

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
bool OpenGL::initializeGL(HWND hWnd)
{
	/// �����`�F�b�N
	if (!::IsWindow(hWnd))
	{
		cerr << "Error: [OpenGL::initializeOpenGL] Invalid Window Handle" << endl;
		return false;
	}

	/// �n���h���̑��
	windowHandle	= hWnd;
	deviceContextHandle	= ::GetDC(windowHandle);

	/// �s�N�Z���t�H�[�}�b�g�̐ݒ�
	if (!setWindowPixelFormat())
	{
		cerr << "Error: [OpenGL::initializeOpenGL] setWindowPixelFormat Error" << endl;
		return false;
	}

	/// GL�R���e�L�X�g�̐ݒ�
	if (!createGLContext())
	{
		cerr << "Error: [OpenGL::initializeOpenGL] createGLContext Error" << endl;
		return false;
	}

	/// OpenGL�̊e�@�\�̊J�n
	setOpenGLFunctions();

	/// �Ɩ��̐ݒ�
	setWorldLightings();

	/// �`�悷��V�[���̃T�C�Y���i�[
	RECT sceneSize;
	::GetClientRect(windowHandle, &sceneSize);
	sceneWidth = sceneSize.right;
	sceneHeight = sceneSize.bottom;	

	return true;
}

/**
 *	����
 *		�I������
 */
void OpenGL::finalizeGL(void)
{
	/// �����_�����O�R���e�L�X�g���J�����g����͂���
	if (wglGetCurrentContext())
		wglMakeCurrent(NULL, NULL);
	
	if (renderingContextHandle != NULL)
	{
		wglDeleteContext(renderingContextHandle);
		renderingContextHandle = NULL;
	}

	if (deviceContextHandle != NULL)
		::ReleaseDC(windowHandle, deviceContextHandle);

	return;
}

/**
 *	����
 *		OpenGL�̋@�\�ݒ�
 */
void OpenGL::setOpenGLFunctions(void)
{
	/**
	 *	�o�b�t�@������������J���[�����w��
	 *	glClear�Ŏg�p�����J���[
	 */
	glClearColor(0.7f, 0.8f, 0.9f, 1.0f);		/// �w�i�F�̌���

	/// �f�v�X�o�b�t�@�N���A�l�̐ݒ�
	glClearDepth( 1.0f );

	/// Z�i�f�v�X�j�o�b�t�@�̎g�p�J�n
	glEnable(GL_DEPTH_TEST);

	/**
	 *	�[�x�o�b�t�@�[��r�Ɏg�p�����l���w��
	 *	���ꂩ�A��O�ɂ�����̂ŏオ�����Ă���
	 */
	glDepthFunc(GL_LEQUAL);

	/**
	 *	�V�F�[�f�B���O�ݒ�
	 *	�v���~�e�B�u�̖ʂ��e���_�̐F����v�Z���ꂽ�O���f�[�V�����œh��
	 */
	glShadeModel(GL_SMOOTH);

	/**
	 *	���̍s�񉉎Z�^�[�Q�b�g��I������
	 *		�I�u�W�F�N�g�`�惂�[�h
	 */
	glMatrixMode(GL_MODELVIEW);

}

/**
 *	����
 *		GL�̐ݒ�̃��Z�b�g
 */
void OpenGL::clearOpenGLSettings(void)
{
	/// �����_�����O�R���e�L�X�g�̏�����
	if (renderingContextHandle != NULL)
	{
		wglDeleteContext(renderingContextHandle);
		renderingContextHandle = NULL;
	}

	/// �f�o�C�X�R���e�L�X�g�n���h���̏�����
	if (deviceContextHandle != NULL)
		::ReleaseDC(windowHandle, deviceContextHandle);

	/// ���_���p�[�X�y�N�e�B�u��
	viewType = PERSPECTIVE;

}

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
void OpenGL::setWorldLightings(void)
{
/**
 *	�Ɩ����ʂ̌���
 */
	/// ����
	ambientLight0[0] = 1.0f;
	ambientLight0[1] = 1.0f;
	ambientLight0[2] = 1.0f;
	ambientLight0[3] = 0.5f;
	
	/// �g�U��
	diffuseLight0[0] = 1.0f;		
	diffuseLight0[1] = 1.0f;
	diffuseLight0[2] = 1.0f;
	diffuseLight0[3] = 1.0f;
	
	/// ���ʌ�
	specularLight0[0] = 1.0f;		
	specularLight0[1] = 1.0f;
	specularLight0[2] = 1.0f;
	specularLight0[3] = 1.0f;
	
	/// �����ʒu
	positionLight0[0] = 0.5f;		
	positionLight0[1] = -0.5f;
	positionLight0[2] = 1.0f;
	positionLight0[3] = 0.0f;
	
	/// �X�|�b�g���C�g����
	directionLight0[0] = 0.0f;	
	directionLight0[1] = 0.0f;
	directionLight0[2] = -1.0f;

	/// �����ł�GL_LIGHT0�̈�̌����̂ݐݒ肷��
	glLightfv(GL_LIGHT0,	GL_AMBIENT,			ambientLight0);
	glLightfv(GL_LIGHT0,	GL_DIFFUSE,			diffuseLight0);
	glLightfv(GL_LIGHT0,	GL_SPECULAR,		specularLight0);
	glLightfv(GL_LIGHT0,	GL_POSITION,		positionLight0);
//	glLightf(GL_LIGHT0,		GL_SPOT_CUTOFF,		45.0f);
//	glLightfv(GL_LIGHT0,	GL_SPOT_DIRECTION,	directionLight0);
//	glLightf(GL_LIGHT0,		GL_SPOT_EXPONENT,	20.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);	
	glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
}

/**
 *	����
 *		OpenGL�C���[�W�`��
 */
void OpenGL::drawScenes(void)
{
	/// �s�񃂁[�h�؂�ւ�
	glMatrixMode(GL_MODELVIEW);
	/// �s�񏉊���
	glLoadIdentity();

	glDisable(GL_CULL_FACE);	

	glEnable(GL_CULL_FACE);

	return;
}

/**
 *	����
 *		OpenGL�C���[�W�̃����_�����O
 *		OpenGL�`��̃R�[���o�b�N�֐��ɂȂ�
 */
void OpenGL::renderScenes(void)
{
	/// �����_�����O�R���e�L�X�g���J�����g�ɂ���
	if (wglMakeCurrent(deviceContextHandle, renderingContextHandle))
	{

		/// �o�b�t�@���N���A�i�w�肵���o�b�t�@�����̐F�ŏ����j
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		/// �s�񏉊���
		glLoadIdentity();

		/// ���_����
		setSceneView(sceneWidth, sceneHeight);
	
		/// �V�[����`��
		glPushMatrix();
			drawScenes();
		glPopMatrix();
		
		/// �R�}���h�̃t���b�V��
		glFlush();

	}

	/**
	 *		�o�b�t�@�̐ؑ�
	 *			�o�b�N�o�b�t�@���t�����g�o�b�t�@�ɐ؂�ւ��C�V�����摜��������
	 */
	SwapBuffers(wglGetCurrentDC());

	wglMakeCurrent(deviceContextHandle, NULL);

	return;
}

/**
 *	����
 *		�`��̈�̃��T�C�Y����
 */
void OpenGL::resizeScenes(int width, int height)
{
	/// �s�񃂁[�h�̐؂�ւ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/**
	 *	gluPerspective(fovy, aspect, zNear, zFar)
	 *		fovy: ����̐���������p�i�p�x�j�B�傫���قǍL�p�����Y�A�������قǖ]�������Y�ɂȂ�B
	 *			  180�x�ȏ��-���g���邪�A45�`50�x�����肪�W�������Y�i�l�̖ڂɋ߂��j�B 
	 *		aspect: �`��͈͂̏c����BViewport��width/Height���Z�b�g����΁A���̂̏c���䂪�������`�悳���B 
	 *		zNear: ���_����ǂꂾ�����ꂽ�ʒu����\�����邩�B�K��0���傫���A����ZFar���������Ȓl������B 
	 *		zFar: ���_����ǂꂾ�����ꂽ�ʒu�܂ŕ\�����邩�B�K��0���傫���A����ZNear�����傫�Ȓl������B 
	 */
	gluPerspective(PERS_DEFAULT_FOVY, (GLdouble)width/(GLdouble)height, PERS_DEFAULT_NEAR, PERS_DEFAULT_FAR);
	
	/**
	 *	void glViewport(GLint x , GLint y , GLsizei width , GLsizei height);
	 *	�쐬�摜���A�ǂ��ɕ`�悷�邩��ݒ肷��
	 *	�f�t�H���g�ł́AX ���W�AY ���W���� 0�A�E�B���h�E�̕��ƍ����ŕ`�悳���
	 *		x, y: �`��͈͂̌��_�ʒu���w��B�P�ʂ̓s�N�Z���B�����ł͍������i0, 0�j�A�������+y�Őݒ肷��B 
	 *		width, height: �`��͈͂̕��A������ݒ�B�P�ʂ̓s�N�Z���B 
	 *
	 */
	glViewport(0, 0, (GLint)width, (GLint)height);

	/// �V�[���̃T�C�Y���i�[
	sceneWidth = width;
	sceneHeight = height;

	/// �s�񃂁[�h�����f���r���[�ɖ߂�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

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
void OpenGL::setSceneView(double width, double height)
{
	/// �r���[�^�C�v�ɂ�莋�_��ύX
	switch (viewType)
	{
		case PERSPECTIVE:
			setViewPoint(	cameraView.getDistance(),
							cameraView.getAzimuth(), 
							cameraView.getElevation(),
							cameraView.getViewCenterPosition(1),
							cameraView.getViewCenterPosition(2),
							cameraView.getViewCenterPosition(3),
							width, height
						);
			break;

		case TOP:
			setViewPoint(300, -90.0, 90.0, 0, 0, 0, width, height);
			break;
        
		case SIDE:
			setViewPoint(300, -90.0, 0.0, 0, 0, 0, width, height);
			break;
    
		case FRONT:
			setViewPoint(300, 0.0, 0.0, 0, 0, 0, width, height);
			break;

		default:
			break;
    }

	return;
}

/**
 *	����
 *		�V�[���̎��_�������ɂ��ݒ�
 *	����
 *		distance, azimuth, elevation�F���_�ւ̋����C���ʊp�C�p
 *		centerX, centerY, centerZ�F���E�̒��S�ʒu�̎Q�Ɠ_���W
 *		width: �V�[���̉���
 *		height: �V�[���̍���
 */
void OpenGL::setViewPoint(	double distance,	double azimuth, double elevation,
							double centerX,		double centerY,	double centerZ,
							double width,	double height
							)

{
	/// �s�񃂁[�h�؂�ւ�
	glMatrixMode(GL_PROJECTION);
	/// �s�񏉊���
	glLoadIdentity();

	glViewport(0, 0, (GLint)width, (GLint)height);
	gluPerspective(	(GLdouble)45.0,						/// ����p�x[deg]
					(GLdouble)(width/height),			/// �A�X�y�N�g��(�E�B���h�E�̕�/����)
					(GLdouble)(distance*0.1),			/// ���_����ł��߂��_�܂ł̋���
					(GLdouble)(distance*3.0)			/// ���_����ł������_�܂ł̋���
					);

	/// �s�񃂁[�h�؂�ւ�
	glMatrixMode(GL_MODELVIEW);
	/// �s�񏉊���
	glLoadIdentity();

/**
 *	��������gluLookAt �̂��߂̈����v�Z
 */
	double theta = azimuth;
	double phi = elevation;

	/// ���_�̈ʒu
	double eyeX;
	double eyeY;
	double eyeZ;
	
	/// ���E�̒��S�ʒu�̎Q�Ɠ_
	double refCenterX;
	double refCenterY;
	double refCenterZ;
	
	/// ���E�̏�����̃x�N�g��
	double upDirectionX;
	double upDirectionY;
	double upDirectionZ;

	eyeX = distance * cos(phi * PI / 180) * cos(theta * PI / 180);
	eyeY = distance * cos(phi * PI / 180) * sin(theta * PI / 180);
	eyeZ = distance * sin(phi * PI / 180);
	
	refCenterX = centerX;
	refCenterY = centerY;
	refCenterZ = centerZ;
	
	upDirectionX = distance * cos((phi + 90) * PI / 180) * cos(theta * PI / 180);
	upDirectionY = distance * cos((phi + 90) * PI / 180) * sin(theta * PI / 180);
	upDirectionZ = distance * sin((phi + 90) * PI / 180);

	eyeX += refCenterX;
	eyeY += refCenterY;
	eyeZ += refCenterZ;

	/// ���_�Ǝ����̌���������
	gluLookAt(	(GLdouble)eyeX,				(GLdouble)eyeY,				(GLdouble)eyeZ, 
				(GLdouble)refCenterX,		(GLdouble)refCenterY,		(GLdouble)refCenterZ,
				(GLdouble)upDirectionX,		(GLdouble)upDirectionY,		(GLdouble)upDirectionZ 
			);

	return;
}

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
bool OpenGL::setWindowHandle(HWND hWnd)
{
	/// �����`�F�b�N
	if ( !::IsWindow(hWnd) )
	{
		cerr << "Error: [OpenGL::setWindowHandle] Invalid Window Handle" << endl;
		return false;
	}

	return true;
}

void OpenGL::setViewType(ViewType type)
{
	viewType = type;

	return;
}

/**
 *	����
 *		�}�e���A���J���[�̐ݒ�
 */
void OpenGL::setMaterialColor(double red, double green, double blue, double alpha)
{
	GLfloat materialAmbDiff[] = {(GLfloat)red, (GLfloat)green, (GLfloat)blue, (GLfloat)alpha};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff);

	GLfloat materialSpecular[] = {(GLfloat)(red*0.01), (GLfloat)(green*0.01), (GLfloat)(blue*0.01), (GLfloat)alpha};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);

	return;
}

void OpenGL::setMaterialColor(const GLfloat* materialAmbDiffColor)
{
	setMaterialColor(materialAmbDiffColor[0], materialAmbDiffColor[1], materialAmbDiffColor[2], materialAmbDiffColor[3]);
	return;
}

void OpenGL::setMaterialColor(COLOR color)
{
	const GLfloat* materialAmbDiffColor = selectMaterialColor(color);
	setMaterialColor(materialAmbDiffColor[0], materialAmbDiffColor[1], materialAmbDiffColor[2], materialAmbDiffColor[3]);
	return;
}

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
/**
 *	����
 *		�}�e���A���J���[�̎擾
 */
const GLfloat* OpenGL::selectMaterialColor(COLOR color)
{
	switch (color)
	{
		case RED:		return materialAmbDiffRed;
		case GREEN:		return materialAmbDiffGreen;
		case BLUE:		return materialAmbDiffBlue;
		case YELLOW:	return materialAmbDiffYellow;
		case GRAY:		return materialAmbDiffGray;
		case BLACK:		return materialAmbDiffBlack;
		case WHITE:		return materialAmbDiffWhite;
		case SKY:		return materialAmbDiffSky;
		case ORANGE:	return materialAmbDiffOrange;
		case PURPLE:	return materialAmbDiffPurple;
		default:		return NULL;
	}
	return NULL;
}

/**
 *	�J�������_�ύX�J�n
 */
void OpenGL::beginCameraViewControl(CameraView::Mode mode, int x, int y)
{
	cameraView.beginViewControl(mode, x, y);
	return;
}

/**
 *	�J�������_�ύX�I��
 */
void OpenGL::endCameraViewControl(void)
{
	cameraView.endViewControl();
	return;
}

/**
 *	�J�������_�ύX
 */
void OpenGL::doCameraViewControl(int x, int y)
{
	cameraView.doViewControl(x, y);
	return;
}

/**
 *	------------------------------------------------------------
 *		OpenGL�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	OpenGL�̏������̂��߂̕⏕�֐�
 *	----------------------------------------
 */
/// �s�N�Z���t�H�[�}�b�g�̐ݒ�
bool OpenGL::setWindowPixelFormat(void)
{
	PIXELFORMATDESCRIPTOR pfd;
	
	pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);		/// ���̍\���̂̃T�C�Y
	pfd.nVersion		= 1;									/// Version number : must be 1
	pfd.dwFlags			=	PFD_DRAW_TO_WINDOW |				/// �E�B���h�E�ւ̕`����T�|�[�g
							PFD_SUPPORT_OPENGL |				/// OpenGL�̃T�|�[�g
							PFD_DOUBLEBUFFER |					/// �_�u���o�b�t�@����
							PFD_STEREO_DONTCARE;				/// monoscopic �� stereoscopic���T�|�[�g
	pfd.iPixelType		= PFD_TYPE_RGBA;						/// RGBA �^�C�v
	pfd.cColorBits		= 32;									/// ���p�ł���J���[�̐������߂�
	pfd.cRedBits		= 8;									/// RGBA�J���[�o�b�t�@�̐ԃr�b�g��
	pfd.cRedShift		= 16;									/// RGBA�J���[�o�b�t�@�̐ԃr�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cGreenBits		= 8;									/// RGBA�J���[�o�b�t�@�̗΃r�b�g��
	pfd.cGreenShift		= 8;									/// RGBA�J���[�o�b�t�@�̗΃r�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cBlueBits		= 8;									/// RGBA�J���[�o�b�t�@�̐r�b�g��
	pfd.cBlueShift		= 0;									/// RGBA�J���[�o�b�t�@�̐r�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cAlphaBits		= 0;									/// RGBA�J���[�o�b�t�@�̃A���t�@�r�b�g��
	pfd.cAlphaShift		= 0;									/// RGBA�J���[�o�b�t�@�̃A���t�@�r�b�g�̂��߂̃V�t�g�J�E���g
	pfd.cAccumBits		= 64;									/// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̃r�b�g��
	pfd.cAccumRedBits	= 16;									/// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̐ԃr�b�g��
	pfd.cAccumGreenBits = 16;									/// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̗΃r�b�g��
	pfd.cAccumBlueBits	= 16;									/// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̐r�b�g��
	pfd.cAccumAlphaBits = 0;									/// �A�L���[�����[�V�����o�b�t�@�̃s�N�Z������̃A���t�@�r�b�g��
	pfd.cDepthBits		= 32;									/// �f�v�X�o�b�t�@�̃s�N�Z������̃r�b�g��
	pfd.cStencilBits	= 8;									/// �X�e���V���o�b�t�@�̃s�N�Z������̃r�b�g��
	pfd.cAuxBuffers		= 0;									/// �⏕�o�b�t�@�̐�. Win32��ł̓T�|�[�g�O
	pfd.iLayerType		= PFD_MAIN_PLANE;						/// ���C���^�C�v�̋L�q. ���݂͎g�p����ĂȂ�
	pfd.bReserved		= 0;									/// �I�[�o�[���C�ƃA���_�[���C��plane�̐�. (0�łȂ���΂Ȃ�Ȃ�)
	pfd.dwLayerMask		= 0;									/// ���C���}�X�N�̋L�q. ���݂ł͎g�p����Ă��Ȃ�
	pfd.dwVisibleMask	= 0;									/// �r�W�u���}�X�N�̐ݒ�
	pfd.dwDamageMask	= 0;									/// �_���[�W�}�X�N�̐ݒ�. ���݂ł͎g�p����Ă��Ȃ�

	// �v�����ꂽ�s�N�Z���t�H�[�}�b�g�ɍł��߂��s�N�Z���t�H�[�}�b�g��Ԃ�
	int pixelFormat = ChoosePixelFormat(deviceContextHandle, &pfd);
	
	if (pixelFormat == 0)		// Choose default
	{
		pixelFormat = 1;

		// �w�肵���s�N�Z���t�H�[�}�b�g�Ɋւ�����𓾂�
		if ( DescribePixelFormat(deviceContextHandle, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0 )
			return false;
	}

	// �w�肵���f�o�C�X�R���e�L�X�g�̃s�N�Z���t�H�[�}�b�g��ݒ肷��
	if ( !SetPixelFormat(deviceContextHandle, pixelFormat, &pfd) )
		return false;
	
	return true;
}

/// �R���e�L�X�g�̐ݒ�
bool OpenGL::createGLContext(void)
{
	/// OpenGL�̃����_�����O�R���e�L�X�g���쐬
	renderingContextHandle = wglCreateContext(deviceContextHandle);
	if ( !renderingContextHandle )	// �쐬�s��
		return false;
	
	/// ���݂̃����_�����O�R���e�L�X�g�ɃZ�b�g
	if ( !wglMakeCurrent(deviceContextHandle, renderingContextHandle) )
		return false;

	return true;
}


}