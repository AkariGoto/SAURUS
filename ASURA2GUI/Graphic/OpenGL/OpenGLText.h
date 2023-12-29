/**
 *  �t�@�C����
 *		OpenGLText.h
 *  ����
 *		OpenGL�ɂ��e�L�X�g�o��(Windows��p)
 *  ���t
 *		�쐬��: 2007/04/01(SAT)		�X�V��: 2007/04/15(SAT)
 */

#ifndef __OpenGLText_h__
#define __OpenGLText_h__


/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>

/// �����J�ւ̒ʒm
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
//#pragma comment (lib, "glaux.lib")

/**
 *	----------------------------------------------------------------------
 *		�萔�A�}�N���錾
 *	----------------------------------------------------------------------
 */
/// �f�t�H���g�t�H���g
const char OPENGL_DEFAULT_FONT[]={ "Arial" };
const int OPENGL_DEFAULT_BUFFER_SIZE = 256;

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGLText�N���X
 *	----------------------------------------------------------------------
 */
class OpenGLText
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
protected:
	/**
	 *	Glyph Metrics float�\����
	 */
	GLYPHMETRICSFLOAT	gmf[256];
	/// �v���C���X�g�̂��߂̃t�H���g�x�[�X
	GLuint				fontBase;
	/// �t�H���g���쐬�ł������ǂ����̃t���O
	bool				isFontBuilt;

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
	explicit OpenGLText();
	/// �f�X�g���N�^
	virtual ~OpenGLText();
	
/**
 *	�t�H���g�̍쐬
 */
	void buildFont(	HDC hDC,									/// �f�o�C�X�R���e�L�X�g�ւ̃n���h��
					char* fontName = NULL,						/// �t�H���g��
					GLfloat fontDepth = 0.5f,					/// z�i�[���j�����ւ̃t�H���g�T�C�Y
					int fontWeight = FW_BOLD,					/// �t�H���g�E�F�C�g
					DWORD fontItalic = FALSE,					/// �C�^���b�N�̃t���O
					DWORD fontUnderline = FALSE,				/// �A���_�[���C���̃t���O
					DWORD fontStrikeOut = FALSE,				/// �ł��������t���̃t���O
					DWORD fontCharacterSet = ANSI_CHARSET		/// �L�����N�^�[�Z�b�g�̎��ʎq
					);

/**
 *		������̏o��
 */
	bool print(const char* format, ...);

protected:
/**
 *	�t�H���g�̔j��
 */
	void killFont(void);

};	/// end of class OpenGLText

}	/// end of namespace Graphic

#endif	/// __OpenGLText_h__