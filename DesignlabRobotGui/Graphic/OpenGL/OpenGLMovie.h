/**
 *  �t�@�C����
 *		OpenGLMovie.h
 *  ����
 *		OpenGL�̃A�j���[�V������AVI�ɕϊ�
 *  ���t
 *		�쐬��: 2008/09/02(TUE)		�X�V��: 2008/09/03(WED)
 */

#ifndef __OpenGLMovie_h__
#define __OpenGLMovie_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>

#include <gl/GL.h> 
#include "AviMaker.h"

namespace Graphic
{
    
/**
 *	----------------------------------------------------------------------
 *		OpenGLMovie�N���X
 *	----------------------------------------------------------------------
 *
 *	OpenGL�̃����_�����O�T�[�t�F�X����AVI������쐬����
 *	�������A��x����I�u�W�F�N�g���쐬���ꂽ�瓮��t���[���̕��ƍ����͕ύX�s��
 *	�{�N���X�̃I�u�W�F�N�g�͒P��v���Z�X���œ����ɕ��������\
 */

/**
 *	�g�����iHow to use�j
 *		1. �I�u�W�F�N�g���쐬����
 *			��	OpenGLMovie m_movieRecorder("Output.Avi", VIEWPORTWIDTH, VIEWPORTHEIGHT);
 *		2. �I�u�W�F�N�g�쐬��ASwapBuffers()�Ăяo���O��OpenGLMovie::RecordFrame()���\�b�h���e�t���[���̕`�ʖ��ɌĂяo���΁AAVI���쐬�� 
 *
 *			SAMPLE CODE
 *				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 *   
 *				// Render as usual
 *				glPushMatrix();
 *					drawScene();
 *				glPopMatrix();
 *
 *				// Capture the Rendering into OpenGLMovie's movie file
 *				m_movieRecorder.recordFrame();
 *
 *				SwapBuffers(wglGetCurrentDC());
 *
 *			OpenGLMovie::recordFrame() �͓����� glReadPixels() ���\�b�h���g�p���A�t���[���o�b�t�@�̓��e��ǂݍ��ݏo�̓t�@�C����
 *			���̃t���[���������Ă���B
 *			�L�^�������Ȃ��t���[��������ꍇ�́A����m_movieRecorder.recordFrame() ���\�b�h���Ă΂Ȃ���΂悢�B
 */

class OpenGLMovie
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:
/**
 *	���[�r�[�t�@�C���̃I�u�W�F�N�g
 */
	AviMaker	movieFile;

/**
 *	----------------------------------------
 *	�쐬���郀�[�r�[�t�@�C�����
 *	----------------------------------------
 */	
	int			movieWidth;			/// ���[�r�[�t���[���̕�
	int			movieHeight;		/// ���[�r�[�t���[���̍���
	int			movieBitsPerPixel;	/// 1�s�N�Z��������̃r�b�g��
	LPVOID		pFramePixelBits;	/// �������ރs�N�Z����f�[�^�ւ̃|�C���^

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
	/**
	 *	����
	 *		�R���X�g���N�^
	 *	����
	 *		pFileName: �쐬����t�@�C����
	 *		codec_:	���k�Ɏg�p����r�f�I�R�[�f�b�N��FOURCC �R�[�h�i�C���X�g�[���ς݂̃R�[�f�b�N��I�����邱�Ɓj
	 *				�f�t�H���g��WMV3�B�Ⴆ�΁ADIVX�Ȃ� mmioFOURCC('D','I','V','X') �Ƃ���
	 *				0�Ȃ�Ζ����k
	 *		frameRate_: �t���[�����[�g
	 */
	explicit OpenGLMovie(	LPCTSTR pOutputMovieFileName = _T("Output.avi"), 
							DWORD codec = mmioFOURCC('W','M','V','3'),				/// �r�f�I�R�[�f�b�N
							DWORD frameRate = 10,									/// �t���[�����[�g (FPS)
							int frameWidth = GetSystemMetrics(SM_CXSCREEN),			/// ���[�r�[�t���[����
							int frameHeight = GetSystemMetrics(SM_CYSCREEN),		/// ���[�r�[�t���[������
							int bitsPerPixel = 24)									/// �P�ʃs�N�Z��������̃r�b�g��
								: movieFile(pOutputMovieFileName, codec, frameRate)
	{
		movieWidth = 0;
		movieHeight = 0;
		movieBitsPerPixel = 0;
		pFramePixelBits = NULL;
		
		initialize(frameWidth, frameHeight, bitsPerPixel);
	}

	/**
	 *	����
	 *		�f�X�g���N�^
	 *	����
	 *		����t�@�C������āA�S�t���[��������
	 */	
	virtual ~OpenGLMovie(void)
	{
		if(pFramePixelBits)
		{
			free(pFramePixelBits);
			pFramePixelBits = NULL;
		}
	}

/**
 *	----------------------------------------
 *	������
 *	----------------------------------------
 */
	void initialize(int frameWidth = GetSystemMetrics(SM_CXSCREEN),			/// ���[�r�[�t���[����
					int frameHeight = GetSystemMetrics(SM_CYSCREEN),		/// ���[�r�[�t���[������
					int bitsPerPixel = 24)									/// �P�ʃs�N�Z��������̃r�b�g��	
	{
		movieWidth = frameWidth;
		movieHeight = frameHeight;
		movieBitsPerPixel = bitsPerPixel;

		if (pFramePixelBits)
		{
			free(pFramePixelBits);
			pFramePixelBits = NULL;
		}

		/// �s�N�Z���f�[�^�̃r�b�g���ێ����邽�߂ɕK�v�ȃ��������m��
		pFramePixelBits = malloc(movieWidth * movieHeight * movieBitsPerPixel/8);
	}

	/**
	 *	����
	 *		����̋L�^
	 *		�EOpenGL�̃t���[���o�b�t�@�̓��e��ǂݍ��݁A�ǂݍ��ݍς݃t���[���f�[�^��
	 *		�@�V�����t���[���Ƃ��āA����t�@�C���ɑ}������B
	 *		�E�e�����_�����O�t���[������SwapBuffers()���\�b�h�O�ɂČĂяo���K�v������
	 */
	inline HRESULT recordFrame()
	{
		glFlush();
		glFinish();
		glReadPixels(0, 0, movieWidth, movieHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pFramePixelBits);

		return movieFile.appendNewFrame(movieWidth, movieHeight, pFramePixelBits, movieBitsPerPixel);
	}

	inline const AviMaker* getMovieFile() const{return &movieFile;}
};	/// end of class OpenGLMovie

}	/// end of namespace Graphics

#endif	/// OpenGLMovie