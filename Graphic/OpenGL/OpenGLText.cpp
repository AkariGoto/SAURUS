/**
 *  �t�@�C����
 *		OpenGLText.cpp
 *  ����
 *		OpenGL�ɂ��e�L�X�g�o��(Windows��p)
 *  ���t
 *		�쐬��: 2007/04/01(SAT)		�X�V��: 2007/04/15(SAT)
 */

#include <stdio.h>
#include <string.h>

#include "OpenGLText.h"

namespace Graphic
{
/**
 *	----------------------------------------------------------------------
 *		OpenGLText�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		OpenGLText�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
OpenGLText::OpenGLText()
{
	/// �t�H���g�쐬�t���O��off
	isFontBuilt = false;
}

/// �f�X�g���N�^
OpenGLText::~OpenGLText()
{
	killFont();
}

/// �t�H���g�̍쐬
void OpenGLText::buildFont(	HDC hDC,				/// �f�o�C�X�R���e�L�X�g�ւ̃n���h��
							char* fontName,			/// �t�H���g��
							GLfloat fontDepth,		/// z�i�[���j�����ւ̃t�H���g�T�C�Y
							int fontWeight,			/// �t�H���g�E�F�C�g
							DWORD fontItalic,		/// �C�^���b�N�̃t���O
							DWORD fontUnderline,	/// �A���_�[���C���̃t���O
							DWORD fontStrikeOut,	/// �ł��������̃t���O
							DWORD fontCharacterSet	/// �L�����N�^�[�Z�b�g�̎��ʎq
							)
{

	/// ���Ƀt�H���g���쐬����Ă��邩�ǂ���
	if (isFontBuilt)
		killFont();	/// �쐬����Ă���Ώ���

	/// ���[�U�̃t�H���g
	HFONT font;
	/// �f�t�H���g�̃t�H���g���g�����ǂ���
	bool  isDefaultFontUsed = false;

	/// �����̃t�H���g����NULL��������C�f�t�H���g���g��
	if (fontName == NULL)
	{
		/// �t�H���g�������'\0'�̂��߂̃��������m��
		fontName = new char[strlen( OPENGL_DEFAULT_FONT ) + 1];
		strcpy(fontName, OPENGL_DEFAULT_FONT);
		isDefaultFontUsed = true;
	}

	/// �f�B�X�v���C���X�g���m��
	fontBase = glGenLists(OPENGL_DEFAULT_BUFFER_SIZE);

	/// CreateFont�֐��iWin API�j�ɂ��t�H���g�쐬
	font = CreateFont(	-12,							/// �����Z���܂��͕����̍���
						2,								/// ���ϕ�����
						0,								/// ��������̕�����X���Ƃ̊p�x
						0,								/// �x�[�X���C����X���Ƃ̊p�x
						fontWeight,						/// �t�H���g�̑���
						fontItalic,						/// �C�^���b�N�̎w��
						fontUnderline,					/// �����t���w��
						fontStrikeOut,					/// �ł��������t���w��
						fontCharacterSet,				/// �L�����N�^�Z�b�g
						OUT_TT_PRECIS,					/// �o�͐��x
						CLIP_DEFAULT_PRECIS,			/// �N���b�s���O�̐��x
						ANTIALIASED_QUALITY,			/// �o�͕i��
						FF_DONTCARE | DEFAULT_PITCH,	/// �s�b�`�ƃt�@�~��
						(LPCTSTR)fontName				/// �t�H���g��
					);

	/// �쐬�����t�H���g���g�p
	SelectObject(hDC, font);

	/// 3 �����t�H���g�̃f�B�X�v���C���X�g���쐬����֐�
	wglUseFontOutlines(	hDC,							/// �f�o�C�X�R���e�L�X�g�ւ̃n���h��Select The Current DC
						0,								/// �ŏ��̕����R�[�h
						OPENGL_DEFAULT_BUFFER_SIZE-1,	/// �쐬����f�B�X�v���C���X�g�̐�
						fontBase,						/// �ŏ��̃f�B�X�v���C���X�g
						0.0f,							/// �^��3�����t�H���g�Ƃ̃Y�� 
						fontDepth,						/// z �̕��̕����ւ̌���
						WGL_FONT_POLYGONS,				/// WGL_FONT_LINES (���B���[�t���[��) �� WGL_FONT_POLYGONS (�\���b�h) �̎w�� (�萔) 
						gmf								/// �O���t (glyph) �̐��@���i�[����z�� 
						);							

	if (isDefaultFontUsed)
		delete [] fontName;

	/// �t�H���g�쐬����
	isFontBuilt = true;

}

/**
 *		������̏o��
 *			
 */
bool OpenGLText::print(const char* format, ...)
{
	/// �t�H���g���쐬����Ă��Ȃ�������
	if (!isFontBuilt)
		return false;

	/// �e�L�X�g�̒���
	float length=0;
	/// �e�L�X�g�̊i�[�z��
	char text[OPENGL_DEFAULT_BUFFER_SIZE];	
	/// �������X�g�ւ̃|�C���^
	va_list ap;

	/// �����̃e�L�X�g������������
	if (format == NULL)
		return false;	/// �������Ȃ�

	/// �ϐ��̂��߂̍\�����
	va_start(ap, format);
		/// ���ۂ̐��֕ϊ�
	    vsprintf(text, format, ap);
	va_end(ap);	/// ���ʂ��e�L�X�g�Ɋi�[

	unsigned int loop;
	/// �e�L�X�g�����𒲂ׂ邽�߂̃��[�v
	for (loop=0;loop<(strlen(text));loop++)
	{
		/// �e�����̕����Ƃɂ���Ē����𑝂₷
		length += gmf[ text[loop] ].gmfCellIncX;
	}
	
	/// �X�N���[����ł̃e�L�X�g�̒����ʒu
	glTranslatef(-length/2, 0.0f, 0.0f);

	/// �ȉ��Ŏ��ۂɃX�N���[���Ƀe�L�X�g��`��
	glPushAttrib(GL_LIST_BIT);		/// �f�B�X�v���C���X�g�̃r�b�g���v�b�V��
	glListBase(fontBase);	/// �x�[�X������0�ɃZ�b�g
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);		/// �f�B�X�v���C���X�g�̃e�L�X�g��`��
	glPopAttrib();	/// �f�B�X�v���C���X�g�̃r�b�g���|�b�v
	glTranslatef(-length/2, 0.0f, 0.0f);

	return true;
}

/**
 *	------------------------------------------------------------
 *		OpenGLText�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	�t�H���g�̔j��
 */
void OpenGLText::killFont(void)
{
	/// �f�B�X�v���C���X�g�̔j��
	glDeleteLists(fontBase, OPENGL_DEFAULT_BUFFER_SIZE);
}


}