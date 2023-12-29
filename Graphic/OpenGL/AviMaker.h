/**
 *  �t�@�C����
 *		AviMaker.h
 *  ����
 *		AVI���쐬����N���X
 *  ���t
 *		�쐬��: 2008/09/02(TUE)		�X�V��: 2008/09/03(WED)
 */

#ifndef __AviMaker_h__
#define __AviMaker_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <vfw.h>
#include <TChar.h>

/**
 *	----------------------------------------------------------------------
 *		�����J�ւ̒ʒm
 *	----------------------------------------------------------------------
 */
#pragma comment (lib, "vfw32.lib")

/**
 *	----------------------------------------------------------------------
 *		AviMaker�N���X
 *	----------------------------------------------------------------------
 */
class AviMaker
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
 
 /**
 *	----------------------------------------
 *	�r�b�g�}�b�v�̑���Ɋ֘A�������
 *	----------------------------------------
 */ 
	/// �`��̈�̃f�o�C�X�R���e�L�X�g
	HDC					memDCHandle;
	/// �q�[�v�ւ̃n���h��
	HANDLE				heapHandle;
	/**
	 *	DIB�i�r�b�g�}�b�v�j�̃r�b�g�z����R�s�[����o�b�t�@�ւ̃|�C���^
	 *	DIB�i�r�b�g�}�b�v�j�̃R�s�[�ɗ��p����
	 */
	LPVOID				lpDIBPixelBits;

/**
 *	----------------------------------------
 *	AVI�t�@�C���̑���Ɋ֘A�������
 *	----------------------------------------
 */ 
	/// AVI�t�@�C���̃n���h��	
	PAVIFILE			pAviFile;
	PAVISTREAM			pAviStream;
	PAVISTREAM			pAviCompressedStream;

	AVISTREAMINFO		aviStreamInfo;
	AVICOMPRESSOPTIONS	aviCompressOptions;

	/**
	 *	�J�n�T���v���ԍ�
	 *	���݂̃t���[���C���f�b�N�X�̃g���b�N��ێ�����
	 */
	LONG				frameCounter;
	
/**
 *	----------------------------------------
 *	AVI�t�@�C���̐ݒ�Ɋ֘A�������
 *	----------------------------------------
 */
	/// �t���[�����[�g�iFPS�j
	DWORD				frameRate;
	/// �r�f�I�R�[�f�b�N�iFOURCC �R�[�h�p�j
	DWORD				fccHandler;
	/// �o�̓t�@�C����
	TCHAR				outputFileName[MAX_PATH];
	/// �ŏI�G���[���b�Z�[�W
	TCHAR				lastErrMsg[MAX_PATH];
	/**
	 *	�ǉ��֐��̃Z���N�^
	 *	0=Dummy	1=FirstTime	2=Usual
	 */
	int					appendFuncSelector;

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
	 *		�f�t�H���g�R���X�g���N�^
	 *	����
	 *		pFileName: �쐬����t�@�C����
	 *		codec_:	���k�Ɏg�p����r�f�I�R�[�f�b�N��FOURCC �R�[�h�i�C���X�g�[���ς݂̃R�[�f�b�N��I�����邱�Ɓj
	 *				�f�t�H���g��WMV3�B�Ⴆ�΁ADIVX�Ȃ� mmioFOURCC('D','I','V','X') �Ƃ���
	 *				0�Ȃ�Ζ����k
	 *		frameRate_: �t���[�����[�g
	 */
	explicit AviMaker(LPCTSTR pFileName = _T("Output.avi"), DWORD codec_ = mmioFOURCC('W','M','V','3'), DWORD frameRate_ = 1);

	/**
	 *	����
	 *		�f�X�g���N�^
	 *	����
	 *		����t�@�C������āA�S�t���[��������
	 */
	virtual ~AviMaker(void);

/**
 *	����
 *		������
 *	����
 *		pFileName: �쐬����t�@�C����
 *		codec_:	���k�Ɏg�p����r�f�I�R�[�f�b�N��FOURCC �R�[�h�i�C���X�g�[���ς݂̃R�[�f�b�N��I�����邱�Ɓj
 *				�f�t�H���g��WMV3�B�Ⴆ�΁ADIVX�Ȃ� mmioFOURCC('D','I','V','X') �Ƃ���
 *				0�Ȃ�Ζ����k
 *		frameRate_: �t���[�����[�g
 */
	void initialize(LPCTSTR pFileName = _T("Output.avi"), DWORD codec_ = mmioFOURCC('W','M','V','3'), DWORD frameRate_ = 1);
/**
 *	����
 *		����쐬�̂��߂̏I������
 */
	void fianalize(void);

/**
 *	����
 *		������
 *	����
 *		pFileName: �쐬����t�@�C����
 */
	void setOutputFileName(LPCTSTR pFileName);

/**
 *	----------------------------------------
 *	����̍쐬
 *	�I�[�o�[���[�h
 *	----------------------------------------
 */	
	/**
	 *	����
	 *		����̍Ō�Ɏw�肵��HBitmap��V�����t���[���Ƃ��đ}������
	 *	����
	 *		hBitmap: �}������HBITMAP�^�I�u�W�F�N�g
	 */
	HRESULT	appendNewFrame(HBITMAP hBitmap);

	/**
	 *	����
	 *		����̍Ō�Ɏw�肵��Bitmap�̃r�b�g��V�����t���[���Ƃ��đ}������
	 *	����
	 *		width: �}������r�b�g��̍���
	 *		height: �}������r�b�g��̕�
	 *		pBits: �w�肷��r�b�g��ւ̃|�C���^
	 *		bitsPerPixel: 1�s�N�Z��������̃r�b�g
	 */	
	HRESULT	appendNewFrame(int width, int height, LPVOID pBits,int bitsPerPixel);

/**
 *	����
 *		�ŏI�G���[�̎擾
 */
	LPCTSTR getLastErrorMessage(void) const {return lastErrMsg;}

private:
/**
 *	����
 *		����쐬�̂��߂̏�����
 *		�������A�X�g���[���A���k�̃I�v�V�����̐ݒ������
 *	����
 *		farameWidth: �t���[����
 *		frameHeight: �t���[������
 *		bitsPerPixel: �s�N�Z���P�ʂł̃r�b�g��
 *	�ԋp�l
 *		S_OK�ȊO: ���s
 *		S_OK: ����	 
 */
	HRESULT initializeEngine(int frameWidth, int frameHeight, int bitsPerPixel);

/**
 *	����
 *		��������֘A�n���h���̉��
 */
	void releaseMemory();

/**
 *	����
 *		�ŏI�G���[�̐ݒ�
 *	����
 *		pLastErrMsg: �G���[���e
 */
	void setLastErrMsg(LPCTSTR pLastErrMsg);

/**
 *	----------------------------------------
 *	����̍쐬�̂��߂̃w���v�֐�
 *	�I�[�o�[���[�h
 *	----------------------------------------
 */	
	HRESULT	appendFrameFirstTime(HBITMAP hBitmap);
	HRESULT	appendFrameUsual(HBITMAP hBitmap);
	HRESULT	appendDummy(HBITMAP hBitmap);
	HRESULT	(AviMaker::*pAppendFrame[3])(HBITMAP hBitmap);

	HRESULT	appendFrameFirstTime(int width, int height, LPVOID pBits,int bitsPerPixel);
	HRESULT	appendFrameUsual(int width, int height, LPVOID pBits,int bitsPerPixel);
	HRESULT	appendDummy(int nWidth, int nHeight, LPVOID pBits,int bitsPerPixel);
	HRESULT	(AviMaker::*pAppendFrameBits[3])(int nWidth, int nHeight, LPVOID pBits,int bitsPerPixel);
};

#endif