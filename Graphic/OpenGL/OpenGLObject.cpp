/**
 *  �t�@�C����
 *		OpenGLObject.cpp
 *  ����
 *		OpenGL�ɂ�镨�̕`��N���X(Windows��p)
 *  ���t
 *		�쐬��: 2007/04/01(SAT)		�X�V��: 2007/04/17(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "OpenGLObject.h"

using namespace std;
using namespace Math;
using namespace Const;


namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGLObject�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		OpenGLObject�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
OpenGLObject::OpenGLObject()
{
}

/// �R���X�g���N�^
OpenGLObject::OpenGLObject(HWND hWnd)
{
	/**
	 *	OpenGLObject�̍쐬
	 */
	createGLObject(hWnd);
}

/// �f�X�g���N�^
OpenGLObject::~OpenGLObject()
{
	destroyGLObject();
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
bool OpenGLObject::createGLObject(HWND hWnd)
{
	/// �ŏ���OpenGL�̃f�t�H���g���������s��
	if (!OpenGL::initializeGL(hWnd))
		return false;

	/// �f�B�X�v���C���X�g�쐬
	newGLObjDispList();

	/**
	 *	�t���O�ނ̏�����
	 */
	isCoordinateAxisDrawn = true;
	isGridFloorDrawn = true;

	return true;
}

/**
 *	����
 *		�I������
 */
void OpenGLObject::destroyGLObject(void)
{
	/**
	 *	�t���O�ނ̏�����
	 */
	isCoordinateAxisDrawn = false;
	isGridFloorDrawn = false;

	/// �f�B�X�v���C���X�g�̔j��
	deleteObjDispList();

	/// �Ō�Ɋ��N���X�̏I������
	OpenGL::finalizeGL();

	return;
}

/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�`��Ɋ֌W�������
 *	----------------------------------------
 */
/**
 *	����
 *		OpenGL�̃I�u�W�F�N�g�`��
 */
void OpenGLObject::drawObjects(GLenum renderMode)
{
	switch (renderMode)
	{
		case GL_RENDER:
				glDisable(GL_CULL_FACE);	
				/// ���W�n�`��
					drawCoordinateAxis();
				glEnable(GL_CULL_FACE);	
			break;

		case GL_SELECT:
			break;

		default:
			break;
	}
}

/**
 *	����
 *		OpenGL�̃I�u�W�F�N�g�`��
 */
void OpenGLObject::drawScenes(void)
{
	drawObjects(GL_RENDER);
	return;
}

/**
 *	����
 *		OpenGL�C���[�W�̃����_�����O
 *		OpenGL�`��̃R�[���o�b�N�֐��ɂȂ�
 */
void OpenGLObject::renderScenes(void)
{
	/// �����_�����O�R���e�L�X�g���J�����g�ɂ���
	if (wglMakeCurrent(deviceContextHandle, renderingContextHandle))
	{
		/// �o�b�t�@���N���A�i�w�肵���o�b�t�@�����̐F�ŏ����j
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		/// ���_����
		setSceneView(sceneWidth, sceneHeight);

		/// ���̍��W�n��ۑ����Ă���
		glPushMatrix();
		{

			/// �V�[����`��
			drawScenes();

		}
		/// ���W�n��߂�
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
 *	----------------------------------------
 *	�s�b�L���O�Ɋւ������
 *	----------------------------------------
 */
/**
 *	����
 *		�}�E�X�ɂ�镨�̂̃s�b�N�A�b�v�֐�
 *		�}�E�X�C�x���g�i��͍̂��N���b�N�j�֐����ŌĂ�
 *	����
 *		x: �N���b�N�����_��x���W
 *		y: �N���b�N�����_��y���W
 */
void OpenGLObject::pickup(int x, int y)
{
	GLuint selectBuf[SELECT_BUFF_SIZE];		/// �Z���N�V�����o�b�t�@
	GLint hitCount = 0;					/// �q�b�g�J�E���g
	GLint viewPort[4] = {0,0,0,0};		/// �r���[�|�[�g		
	float currentAspect;				/// ���݂̃A�X�y�N�g��

	wglMakeCurrent(deviceContextHandle, renderingContextHandle);

/**
 *	�Z���N�V�����J�n
 */
	/// ���݂̃r���[�|�[�g����
	glGetIntegerv(GL_VIEWPORT, viewPort);

	/**
	 *	�o�b�t�@�̑I��
	 *
	 *	glSelectBuffer(GLsizei size, GLuint *buffer)
	 *	buffer �� GLuint �^�̔z��Asize �ɂ��̗v�f�����w�肷��B
	 *	�E���̔z��ɑI�������I�u�W�F�N�g�̖��O�A�Z���N�V�������ꂽ�ʒu�̉��s���̍ŏ��l�ƍő�l�Ȃǂ��i�[����邩��A
	 *	�@�v�f���̓I�u�W�F�N�g���ɂ��������āA���߂Ɏw�肵�Ă����B
	 *	�E�Z���N�V�������[�h�ɓ���O�iglRenderMode(GL_SELECT) �����s����O�j�Ɏ��s����K�v������B
	 */
	glSelectBuffer(SELECT_BUFF_SIZE, selectBuf);

	/// �Z���N�V�������[�h�ɓ���
	(void)glRenderMode(GL_SELECT);

	/**
	 *	�I�u�W�F�N�g�ɂ��閼�O�i�����l�j��o�^���Ă����l�[���X�^�b�N������������B
	 *	����̓Z���N�V�������[�h�ɓ�������iglRenderMode(GL_SELECT) �����s������j�ɍs��Ȃ���Ζ��������
	 */
	glInitNames();

	/**
	 *	�v���W�F�N�V�������[�h
	 *	�Z���N�V�����̏����͎��_���W�n�ōs��
	 */
	glMatrixMode(GL_PROJECTION);			
	
	/**
	 *	�Z���N�V�������[�h�̂Ƃ��̂݉��̍s��v�Z�𔽉f�����邽�߂�
	 *	Push�i���݂̓����ϊ��}�g���N�X��ۑ��j����
	 */
	glPushMatrix();
	/// �����ϊ��}�g���N�X������������
	glLoadIdentity();
		/**
		 *	�s�b�L���O�s��̏�Z
		 *		�\���̈悪�}�E�X�|�C���^�̎��͂����ɂȂ�悤�ɕϊ��s���ݒ肷��B
		 *		�}�E�X�̍��W�n�́A�X�N���[���̍��W�n�ɑ΂��ď㉺�����]���Ă���̂ł����␳����
		 */
		gluPickMatrix((GLdouble)x, (GLdouble)(viewPort[3] - y), 5.0, 5.0, viewPort);
		
		/**
		 *	�ʏ�̕`��Ɠ��������ϊ��}�g���N�X��ݒ肷��
		 *		�s�b�N�A�b�v���ƌ��݌����Ă���̂Ƃ̂����h������
		 */
		currentAspect = (float)viewPort[2]/(float)viewPort[3];
		gluPerspective(PERS_DEFAULT_FOVY, currentAspect, PERS_DEFAULT_NEAR, PERS_DEFAULT_FAR);
		
		/// �����Ń��f���r���[���[�h�ɂ���
		glMatrixMode(GL_MODELVIEW);
		
		/// �Z���N�V�������[�h�ŕ`�悷��
		drawObjects(GL_SELECT);
		/// �v���W�F�N�V�������[�h�ɖ߂�
		glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);			//���f���r���[���[�h�֖߂�

	hitCount = glRenderMode(GL_RENDER);

	/// �q�b�g�����I�u�W�F�N�g��I��
	selectHitObjects(hitCount, selectBuf);

	/// �q�b�g�������ʂɂ��A�������s��
	handlePickupResult();

	wglMakeCurrent( NULL, NULL );
}

/**
 *	����
 *		�Z���N�V�����o�b�t�@����f�v�X�o�b�t�@����ёւ����肵�Ȃ���q�b�g�����I�u�W�F�N�g�����ʂ���
 *	����
 *		hitCount: �q�b�g��
 *		selectBuffer:	glSelectBuffer() �̌��ʂ��i�[����Ă���Z���N�V�����o�b�t�@
 *						�I�������I�u�W�F�N�g�̖��O�A�Z���N�V�������ꂽ�ʒu�̉��s���̍ŏ��l�ƍő�l�Ȃǂ��i�[���ꂽ�o�b�t�@
 */
int OpenGLObject::selectHitObjects(GLuint hitCount, GLuint selectBuffer[])
{
	/**
	 *	���[�J���ϐ�
	 */
	GLuint i;				/// �J�E���^
	unsigned int nameIndex;	/// ���O��Ŏg�p����J�E���^

	GLuint nameCount;		/// �q�b�g�����I�u�W�F�N�g�̖��O�K�w�̐�
	GLuint* work_pointer;	/// ��Ɨp�|�C���^
	GLuint minZ;			/// z�i�[�x�j�̍ŏ��l
	GLuint maxZ;			/// z�i�[�x�j�̍ő�l
	
	double nearDepth;		/// �q�b�g�����I�u�W�F�N�g�̃f�v�X�͈́i��O�j
	double farDepth;		/// �q�b�g�����I�u�W�F�N�g�̃f�v�X�͈́i���j
	double minDepth;		/// �q�b�g�����|���S���̒��̍ŏ��f�v�X�l

	unsigned int nameArray[NAME_ARRAY_SIZE];	/// ���O�K�w�̔z��iNAME_ARRAY_SIZE�͍ő�K�w���j

	/**
	 *	�Z���N�V�����o�b�t�@�Ɋi�[�����f�[�^
	 *		1.�l�[���X�^�b�N�ɐς܂ꂽ���O�̌��i���O�K�w�̌��j
	 *		2.�I��͈͂����؂�v���~�e�B�u�̒��_�̃f�v�X�l�̍ŏ��l
	 *		3.�I��͈͂����؂�v���~�e�B�u�̒��_�̃f�v�X�l�̍ő�l
	 *		4.�l�[���X�^�b�N�i= �I�u�W�F�N�g�̖��O�j[n��] 
	 *	����炪�q�b�g�����I�u�W�F�N�g�������A���ԂɊm�ۂ���Ă���
	 *
	 *	�܂�Am�q�b�g�����Ȃ�Am * (1 + 2 + n)����unsigned int �̔z�񂪊m�ۂ���� 
	 *	 [�K�w�̐[��] [Z�l(�ŏ�)] [Z�l(�ő�)][�K�w���̔ԍ�]... [�K�w�̐[��]�E�E�E 
	 *
	 *	�Ⴆ�΁A
	 *		[2][1.5][1.8][2][1] �ƂȂ��Ă���A�u�s�b�N�A�b�v�v�����u�|���S���f�[�^�v�́A
	 *	�܂�
	 *		�K�w�̐[���F2 
	 *		���s���l(��O��)�F1.5 
	 *		���s���l(����)�F1.8 
	 *		�K�w���̔ԍ��F2-1 
     */

	/// �q�b�g���Ă��Ȃ�
	if (hitCount<=0) return -1;

	/// �ŏ��[�x�̏�����
	minDepth = 10.0;

	/// �Z���N�V�����o�b�t�@�̃|�C���^����Ɨp�|�C���^�֓n��
	work_pointer = (GLuint*)selectBuffer;

	/// �q�b�g�����m�F����
	for (i=0;i<hitCount;i++) 
	{
		/**
		 *	----------------------------------------
		 *	STEP1 �q�b�g�����I�u�W�F�N�g�X�̏���
		 *	----------------------------------------
		 */
		/// ���O�̌����i�[
		nameCount = *work_pointer;

		/// �|�C���^�̃C���N�������g�ŁA�f�v�X�l�̍ŏ��l
		work_pointer++;
		minZ		= *work_pointer;
		nearDepth	= (double)minZ / 0x7fffffff;

		/// �|�C���^�̃C���N�������g�ŁA�f�v�X�l�̍ő�l
		work_pointer++;
		maxZ		= *work_pointer;
		farDepth	= (double)minZ / 0x7fffffff;


		/**
		 *	�|�C���^�̃C���N�������g�ŁA���O��̎擾
		 *	�Ȃ��A���O�̌������擾����
		 */
		for (nameIndex=0;nameIndex<nameCount;nameIndex++)
		{
			work_pointer++;
			nameArray[nameIndex] = *work_pointer;			
		}

		if (NAME_ARRAY_SIZE < nameCount)
		{
			cerr << "Error: [OpenGLObject::selectHitObjects] Name array size error" << endl;
			continue;
		}

		/**
		 *	----------------------------------------
		 *	STEP2 �ŏ��f�v�X�i�ł���O�ɂ�����́j��T��
		 *	----------------------------------------
		 */

		/// �ŏ��l�̔�r
		if (nearDepth < minDepth) 
		{
			/// �ŏ��l�̍X�V
			minDepth = nearDepth;
			
			for (nameIndex=0;nameIndex<nameCount;nameIndex++)
			{
				nameNumberOfHitObj[nameIndex] = nameArray[nameIndex];			
			}
		}
	}

	return 1;
}

/**
 *	����
 *		�s�b�N�A�b�v�ɂ��I�񂾕��̂̏���
 */
void OpenGLObject::handlePickupResult(void)
{
	return;
}

/**
 *	------------------------------------------------------------
 *		�I�u�W�F�N�g�̕`��Ɋ֌W�������
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	3D���f�����O
 *	----------------------------------------
 */
/// ���W���`��
void OpenGLObject::drawCoordinateAxis(double length, double width, double scale)
{
	if ( !isCoordinateAxisDrawn )
		return;

	/// ���̔��a
	const double RADIUS = 50.0;

	glPushMatrix();
		/**
		 *	x���̕`��i�ԁj
		 */
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffRed );
		glLineWidth( (GLfloat)(width*scale) );
		glBegin(GL_LINES);
			glVertex3f( 0.0f,	0.0f,	0.0f );
			glVertex3f( (GLfloat)(length*scale),	0.0f,	0.0f );
		glEnd();

		glTranslatef( (GLfloat)(length*scale), 0.0f, 0.0f );
		drawSphere(RADIUS*scale);
	glPopMatrix();

	glPushMatrix();
		/**
		 *	y���̕`��i�΁j
		 */
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffGreen);
		glLineWidth( (GLfloat)(width*scale) );
		glBegin(GL_LINES);
			glVertex3f( 0.0f,	0.0f,	0.0f );
			glVertex3f( 0.0f,	(GLfloat)(length*scale),	0.0f );
		glEnd();

		glTranslatef( 0.0f, (GLfloat)(length*scale), 0.0f );
		drawSphere(RADIUS*scale);
	glPopMatrix();

	glPushMatrix();
		/**
		 *	z���̕`��i�j
		 */
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffBlue);
		glLineWidth( (GLfloat)(width*scale) );
		glBegin(GL_LINES);
			glVertex3f( 0.0f,	0.0f,	0.0f );
			glVertex3f( 0.0f,	0.0f,	(GLfloat)(length*scale) );
		glEnd();

		glTranslatef( 0.0f, 0.0f, (GLfloat)(length*scale) );
		drawSphere(RADIUS*scale);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffYellow);
	drawSphere(RADIUS*scale);
}

/**
 *	����
 *		�|���S�����f�����O�i�����j
 *		�Y��(1, 2, 3)�̏���CW�i���v���j�œ���
 *		�e���_�̍��W�����
 */
void OpenGLObject::drawPolygonSurface(	double x1, double y1, double z1,
														double x2, double y2, double z2,
														double x3, double y3, double z3)
{
	/// ���[�J���ϐ��̐錾
	double norm;
	double x, y, z;
	double vx1,vy1,vz1,vx2,vy2,vz2;

	/// �x�N�g���̍����v�Z
	vx1 = x1 - x2;
	vy1 = y1 - y2;
	vz1 = z1 - z2;

	vx2 = x3 - x2;
	vy2 = y3 - y2;
	vz2 = z3 - z2;

	/// �O�όv�Z
	outerProduct(vx1, vy1, vz1, vx2, vy2, vz2, &x, &y, &z);
	norm = sqrt(x*x + y*y + z*z);

	if (norm == 0)
	{
		cerr << "Error: [OpenGL::drawPolygonSurface] argument error\n" << endl;
		return;
	}

	x = x/norm;
	y = y/norm;
	z = z/norm;

	glBegin(GL_POLYGON);
		glNormal3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		glVertex3f( (GLfloat)x1, (GLfloat)y1, (GLfloat)z1 );
		glVertex3f( (GLfloat)x2, (GLfloat)y2, (GLfloat)z2 );
		glVertex3f( (GLfloat)x3, (GLfloat)y3, (GLfloat)z3 );
	glEnd();



	return;
}

/**
 *		�|���S�����f�����O�i�x�N�g���j
 *		�Y��(1, 2, 3)�̏���CW�i���v���j�œ���
 *		�e���_�̍��W�����
 */
void OpenGLObject::drawPolygonSurface(const Vector& point1, const Vector& point2, const Vector& point3)
{
	/// �����`�F�b�N
	assert( point1.getSize() == THREE_DIMENSION ); 
	assert( point2.getSize() == THREE_DIMENSION ); 
	assert( point3.getSize() == THREE_DIMENSION ); 
	
	drawPolygonSurface( point1(1), point1(2), point1(3), point2(1), point2(2), point2(3), point3(1), point3(2), point3(3) );
}

/// �~���`��
void OpenGLObject::drawCylinder(double radius, double height)
{
	/// quadric object �����������
    GLUquadricObj* quad = gluNewQuadric();
 
	/// �ʂ̓h��ׂ����w�肷��i����ł͂Ȃ��A�e�������~����`���j
    gluQuadricDrawStyle(quad, GLU_FILL);

    /// �X���[�X�V�F�[�f�B���O���s���悤�ݒ肷��
    gluQuadricNormals(quad, GLU_SMOOTH);

	glPushMatrix();
		glTranslated( (GLdouble)0,0, (GLdouble)(-0.5*height) );
		gluCylinder(quad, (GLdouble)radius, (GLdouble)radius, (GLdouble)height, (GLint)16, (GLint)1 );
		gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1 );
		glTranslated( (GLdouble)0,0, (GLdouble)height );
		gluDisk(quad, (GLdouble)0, (GLdouble)radius, (GLint)16, (GLint)1 );
	glPopMatrix();

	/// ��������quadric object ���폜
	gluDeleteQuadric(quad);

	return;
}

/// ���`��
void OpenGLObject::drawSphere(double radius)
{
	/// quadric object �����������
	GLUquadricObj* quad = gluNewQuadric();

	/// �ʂ̓h��ׂ����w�肷��(����ł͂Ȃ��A�e�������~����`��)
	gluQuadricDrawStyle(quad, GLU_FILL);

	/// �X���[�X�V�F�[�f�B���O���s���悤�ݒ肷��
	gluQuadricNormals(quad, GLU_SMOOTH);

	/// ����
	gluSphere(quad, (GLdouble)radius, (GLint)16, (GLint)16 );
	
	/// ��������quadric object ���폜
	gluDeleteQuadric(quad);
	
	return;
}

/**
 *		Box�`��(����)
 *			��������
 */
void OpenGLObject::drawBox(double startX, double startY, double startZ, double endX, double endY, double endZ)
{
	/// ��1�ʕ`��(x-y���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)endZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)endZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)endZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)endZ );
	glEnd();
	
	/// ��2�ʕ`��(y-z���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( (GLfloat)endX, (GLfloat)endY,		(GLfloat)endZ );
		glVertex3f( (GLfloat)endX, (GLfloat)startY,	(GLfloat)endZ );
		glVertex3f( (GLfloat)endX, (GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)endX, (GLfloat)endY,		(GLfloat)startZ );
	glEnd();
	
	/// ��3�ʕ`��(x-y���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)startZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)startZ );
	glEnd();
	
	/// ��4�ʕ`��(y-z���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f( (GLfloat)startX, (GLfloat)endY,		(GLfloat)startZ );
		glVertex3f( (GLfloat)startX, (GLfloat)startY,		(GLfloat)startZ );
		glVertex3f( (GLfloat)startX, (GLfloat)startY,		(GLfloat)endZ );
		glVertex3f( (GLfloat)startX, (GLfloat)endY,		(GLfloat)endZ );
	glEnd();
	
	/// ��5�ʕ`��(x-z���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)endZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)endZ );
	glEnd();
	
	/// ��6�ʕ`��(x-z���ʂɕ��s�Ȗ�)
	//////////////////////////////////
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)endZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)endZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)startZ );
	glEnd();

	return;
}

/**
 *		Box�`��(����)
 *			�x�N�g������
 */
void OpenGLObject::drawBox(const Vector& startPoint, const Vector& endPoint)
{
	/// �����`�F�b�N
	assert( startPoint.getSize() == THREE_DIMENSION );
	assert( endPoint.getSize() == THREE_DIMENSION );

	drawBox( startPoint(1), startPoint(2), startPoint(3), endPoint(1), endPoint(2), endPoint(3) );

	return;
}

/**
 *		�t���[���`��i���ʂ�4���̐����j
 *			��������
 */
void OpenGLObject::drawFrame(double startX, double startY, double startZ, double endX, double endY, double endZ)
{
	/// �F���O���[�ɐݒ�
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiffGray);
	/// ��������
	glLineWidth(4.0f);

	/// ���ʕ`��
	glBegin(GL_LINE_LOOP);
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)startZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)startZ );
	glEnd();

	/// �ȉ��Ő����`��
	glBegin(GL_LINES);
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)endZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)startZ );
	glEnd();

	glBegin(GL_LINES);
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)endZ );
	glEnd();

	glBegin(GL_LINES);
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)startZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)endZ );
	glEnd();

	glBegin(GL_LINES);
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)endZ );
	glEnd();

	return;
}

/**
 *		�t���[���`��i���ʂ�4���̐����j
 *			�x�N�g������
 */
void OpenGLObject::drawFrame(const Vector& startPoint, const Vector& endPoint)
{
	/// �����`�F�b�N
	assert( startPoint.getSize() == THREE_DIMENSION );
	assert( endPoint.getSize() == THREE_DIMENSION );

	drawFrame( startPoint(1), startPoint(2), startPoint(3), endPoint(1), endPoint(2), endPoint(3) );

	return;
}

/**
 *		���`��
 */

/// ���`��i�������́j
void OpenGLObject::drawLine(double startX, double startY, double startZ,
										double endX, double endY, double endZ, 
										double width)
{
	/// ���̑�������
	glLineWidth( (GLfloat)width );

	/// ���̕`��
	glBegin(GL_LINES);
		glVertex3f( (GLfloat)startX, (GLfloat)startY, (GLfloat)startZ );
		glVertex3f( (GLfloat)endX, (GLfloat)endY, (GLfloat)endZ );  
	glEnd();

	return;
}

/// ���`��i�x�N�g�����́j
void OpenGLObject::drawLine(const Vector& startPoint, const Vector& endPoint, double width)
{
	/// �����`�F�b�N
	assert( startPoint.getSize() == THREE_DIMENSION );
	assert( endPoint.getSize() == THREE_DIMENSION );

	drawLine( startPoint(1), startPoint(2), startPoint(3), endPoint(1), endPoint(2), endPoint(3), width );
}

/// �O�p�`�`��
void OpenGLObject::drawTriangle(	const Vector& point1, const Vector& point2, 
												const Vector& point3, double width )
{
	/// �����`�F�b�N
	assert( point1.getSize() == THREE_DIMENSION );
	assert( point2.getSize() == THREE_DIMENSION );
	assert( point3.getSize() == THREE_DIMENSION );

	/// ���ۂɕ`��
	drawLine( point1, point2, width );
	drawLine( point2, point3, width );
	drawLine( point3, point1, width );

}

/// �l�p�`�`��
void OpenGLObject::drawQuadrangle(	const Vector& point1, const Vector& point2, 
													const Vector& point3, const Vector& point4,
													double width )
{
	/// �����`�F�b�N
	assert( point1.getSize() == THREE_DIMENSION );
	assert( point2.getSize() == THREE_DIMENSION );
	assert( point3.getSize() == THREE_DIMENSION );
	assert( point4.getSize() == THREE_DIMENSION );

	drawLine( point1, point2, width);
	drawLine( point2, point3, width);
	drawLine( point3, point1, width);
	drawLine( point1, point2, width);
}

/**
 *	----------------------------------------
 *	2D���f�����O
 *	----------------------------------------
 */
/**
 *		���ʔ`��(�Ίp���̒��_�ƍ���)
 */
/// �����ʁiXY
void OpenGLObject::drawHorizontalPlane(double startX, double startY, double endX, double endY, double z)
{
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)startX,	(GLfloat)startY,	(GLfloat)z );
		glVertex3f( (GLfloat)startX,	(GLfloat)endY,		(GLfloat)z );
		glVertex3f( (GLfloat)endX,		(GLfloat)endY,		(GLfloat)z );
		glVertex3f( (GLfloat)endX,		(GLfloat)startY,	(GLfloat)z );
	glEnd();
}

/// �O�z�ʁiYZ�j
void OpenGLObject::drawFrontalPlane(double startY, double startZ, double endY, double endZ, double x)
{
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)x,	(GLfloat)startY,	(GLfloat)startZ );
		glVertex3f( (GLfloat)x,	(GLfloat)startY,	(GLfloat)endZ );
		glVertex3f( (GLfloat)x,	(GLfloat)endY,		(GLfloat)endZ );
		glVertex3f( (GLfloat)x,	(GLfloat)endY,		(GLfloat)startZ );
	glEnd();
}

/// ���ʁiZX�j
void OpenGLObject::drawSagittalPlane(double startZ, double startX, double endZ, double endX, double y)
{
	glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f( (GLfloat)startX,	(GLfloat)y,	(GLfloat)startZ );
		glVertex3f( (GLfloat)startX,	(GLfloat)y,	(GLfloat)endZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)y,	(GLfloat)endZ );
		glVertex3f( (GLfloat)endX,		(GLfloat)y,	(GLfloat)startZ );
	glEnd();
}

/**
 *		���b�V����̏���`�悷��
 *			startX, startY�F���̑Ίp���̎n�_
 *			endX, endY�F���̑Ίp���̏I�_
 *			z�F���̍���
 *			dx, dy�F���b�V���̊Ԋu
 */
void OpenGLObject::drawGridFloor(void)
{
	if ( !isGridFloorDrawn )
		return;

	glCallList( gridFloorDisplayListID );

}

/**
 *	------------------------------------------------------------
 *		OpenGLObject�N���X��protected�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�f�B�X�v���C���X�g�̐���
 */
void OpenGLObject::newGLObjDispList(void)
{
	/// �f�B�X�v���C���X�gID�̍쐬
	gridFloorDisplayListID = glGenLists(1);

	/// ���[�J���ϐ��̐錾
	Vector start(3);
	Vector end(3);

	const GLfloat glidColor[4] = {0.1f, 0.1f, 0.3f, 1.0f};
	const GLfloat planeColor[4] = {0.2f, 0.4, 0.6f, 1.0f};

	/// �f�B�X�v���C���X�g�̍쐬
	glNewList(gridFloorDisplayListID, GL_COMPILE);

		setMaterialColor( planeColor );
		/**
		 *		���ʂ̕`��
		 */
		glBegin(GL_POLYGON);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f( (GLfloat)GRID_START_X,	(GLfloat)GRID_START_Y,	(GLfloat)GRID_HEIGHT );
			glVertex3f( (GLfloat)GRID_START_X,	(GLfloat)GRID_END_Y,		(GLfloat)GRID_HEIGHT );
			glVertex3f( (GLfloat)GRID_END_X,		(GLfloat)GRID_END_Y,		(GLfloat)GRID_HEIGHT );
			glVertex3f( (GLfloat)GRID_END_X,		(GLfloat)GRID_START_Y,	(GLfloat)GRID_HEIGHT );
		glEnd();

		/**
		 *		�O���b�h�̕`��
		 */
		/// �F�̑I��
		setMaterialColor( glidColor );

		/// �����Œ�
		start(3) = GRID_HEIGHT+1.0;	end(3) = GRID_HEIGHT+1.0;

		// x�����̕`��
		start(2) = GRID_START_Y;		end(2) = GRID_END_Y;

		double x;
		for ( x=GRID_START_X; x<=GRID_END_X; x+=GRID_DX )
		{
			start(1) = x;	end(1) = x;
			drawLine( start, end, 1.5);
		}
	
		/// y�����̕`��
		start(1) = GRID_START_X;		end(1) = GRID_END_X;

		double y;
		for ( y=GRID_START_Y; y<=GRID_END_Y; y+=GRID_DY )
		{
			start(2) = y;	end(2) = y;
			drawLine( start, end, 1.5);
		}

	/// �f�B�X�v���C���X�g�̕ۑ�
	glEndList();

	return;
}

/**
 *	����
 *		�f�B�X�v���C���X�g�̏���
 */
void OpenGLObject::deleteObjDispList(void)
{
	/// �f�B�X�v���C���X�g�̏���
	glDeleteLists(gridFloorDisplayListID, 1);
}

}	/// end of namespace Graphic