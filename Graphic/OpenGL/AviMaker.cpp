/**
 *  �t�@�C����
 *		AviMaker.cpp
 *  ����
 *		AVI���쐬����N���X
 *  ���t
 *		�쐬��: 2008/09/02(TUE)		�X�V��: 2008/09/03(WED)
 */
/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "AviMaker.h"

/**
 *	----------------------------------------------------------------------
 *		�}�N���A�^�̒�`
 *	----------------------------------------------------------------------
 */
#ifndef __countof
#define __countof(x)	((sizeof(x)/sizeof(x[0])))
#endif

/**
 *	----------------------------------------------------------------------
 *		AviMaker�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		AviMaker�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
AviMaker:: AviMaker(LPCTSTR pFileName /* =_T("Output.avi") */, 
			DWORD codec_ /* = mmioFOURCC('M','P','G','4') */,
			DWORD frameRate_ /* = 1 */)
{
	initialize(pFileName, codec_, frameRate_);

}

AviMaker::~AviMaker(void)
{
	fianalize();
}

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
void AviMaker::initialize(LPCTSTR pFileName, DWORD codec_, DWORD frameRate_)
{
/**
 *	�v���b�g�t�H�[�� SDK �֐�
 *		AVIFile ���C�u������������
 *			�EAVIFile ���C�u�����͏��������ꂽ�񐔂̃J�E���g���Ǘ����邪�A������ꂽ�񐔂̃J�E���g�͊Ǘ����܂Ȃ��B
 *			�EAVIFile ���C�u������������ăJ�E���g�� 1 ���炷�ɂ́AAVIFileExit �֐����g���B
 *			�EAVIFileInit �֐��́A���� AVIFile �֐����g���O�ɌĂяo�Ȃ��Ă͂Ȃ�Ȃ��B
 *		�v���O�����̏��������Ɉ�x�����Ăяo��
 */
	AVIFileInit();

/**
 *	�����o�ϐ��̏�����
 */ 
	heapHandle = NULL;
	memDCHandle = NULL;
	lpDIBPixelBits = NULL;
	frameCounter = NULL;

	pAviFile = NULL;
	pAviStream = NULL;
	pAviCompressedStream = NULL;

	fccHandler = codec_;
	frameRate = frameRate_;

/**
 *	������̏�����
 */
	_tcscpy(outputFileName, pFileName);
	_tcscpy(lastErrMsg, _T("Method Succeeded"));
	lastErrMsg[__countof(lastErrMsg)-1] = _T('\0');

/**
 *	�ȉ��̏�����VC8�ȍ~�ŕK�v�B�i�֐��|�C���^�p�j
 *	�R���p�C���Ɍx�����󂯂���A�폜���Ă��ǂ�
 */ 
	pAppendFrame[0] = &AviMaker::appendDummy;
	pAppendFrame[1] = &AviMaker::appendFrameFirstTime;
	pAppendFrame[2] = &AviMaker::appendFrameUsual;

	pAppendFrameBits[0] = &AviMaker::appendDummy;
	pAppendFrameBits[1] = &AviMaker::appendFrameFirstTime;
	pAppendFrameBits[2] = &AviMaker::appendFrameUsual;

	appendFuncSelector=1;		//0=Dummy	1=FirstTime	2=Usual

	return;
}

/**
 *	����
 *		����쐬�̂��߂̏I������
 */
void AviMaker::fianalize(void)
{
	releaseMemory();

/**
 *	�v���b�g�t�H�[�� SDK �֐�
 *		AVIFile ���C�u�����̏I������
 */	
	AVIFileExit();
}

/**
 *	����
 *		������
 *	����
 *		pFileName: �쐬����t�@�C����
 */
void AviMaker::setOutputFileName(LPCTSTR pFileName)
{
	_tcscpy(outputFileName, pFileName);
}

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
HRESULT AviMaker::appendNewFrame(HBITMAP hBitmap)
{
	return (this->*pAppendFrame[appendFuncSelector])((HBITMAP)hBitmap);
}

/**
 *	����
 *		����̍Ō�Ɏw�肵��Bitmap�̃r�b�g��V�����t���[���Ƃ��đ}������
 *	����
 *		width: �}������r�b�g��̍���
 *		height: �}������r�b�g��̕�
 *		pBits: �w�肷��r�b�g��ւ̃|�C���^
 *		bitsPerPixel: 1�s�N�Z��������̃r�b�g
 */
HRESULT	AviMaker::appendNewFrame(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	return (this->*pAppendFrameBits[appendFuncSelector])(width, height, pBits, bitsPerPixel);
}
/**
 *	------------------------------------------------------------
 *		AviMaker �N���X�� private �ȃ����o�֐�
 *	------------------------------------------------------------
 */
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
HRESULT AviMaker::initializeEngine(int frameWidth, int frameHeight, int bitsPerPixel)
{
	/// �ԋp�l
	HRESULT hResult;

	/// AVI���k�I�v�V�����\����
	LPAVICOMPRESSOPTIONS pOptions[1] = {&aviCompressOptions};

	/**
	 *	��ʍ\�����擾
	 */
	int	maxWidth = GetSystemMetrics(SM_CXSCREEN); 
	int maxHeight = GetSystemMetrics(SM_CYSCREEN);

/**
 *	STEP 0
 *		�������̈�̊m��
 */
	/**
	 *	�������f�o�C�X�R���e�L�X�g�i�������̘_���I�ȃf�o�C�X�j���쐬
	 */
	memDCHandle = CreateCompatibleDC(NULL);
	/// ���s����
	if (memDCHandle == NULL)	
	{
		setLastErrMsg(_T("Unable to Create Compatible DC"));
		return E_FAIL;
	}
	
	/// ���ƍ����͈̔͐ݒ�
	if (frameWidth > maxWidth)	maxWidth= frameWidth;
	if (frameHeight > maxHeight)	maxHeight = frameHeight;

	/**
	 *	�p�t�H�[�}���X�̂��߃v���C�x�[�g�q�[�v�̍쐬
	 *		�\�߂�����x�̉��z�������̈��\�񂵂Ă����A�v���O��������v�����ꎟ��A
	 *		���̗\�񂳂ꂽ�̈悩�烁������؂�o���Ă��Č����ǂ��������u���b�N���m�ۂ���B
	 */
	heapHandle = HeapCreate(HEAP_NO_SERIALIZE, maxWidth * maxHeight * 4, 0);
	/// ���s����
	if(heapHandle==NULL)
	{
		setLastErrMsg(_T("Unable to Create Heap"));
		return E_FAIL;
	}
	
	/**
	 *	�������u���b�N��HeapCreate �֐��ō쐬�����q�[�v���犄�蓖�Ă�
	 *	����ɂ��ADIB�i�f�o�C�X�Ɨ��r�b�g�}�b�v�j�p�s�N�Z������쐬
	 *	1�s�N�Z��������4�o�C�g�B
	 */ 
	lpDIBPixelBits = HeapAlloc(heapHandle, HEAP_ZERO_MEMORY|HEAP_NO_SERIALIZE, maxWidth * maxHeight * 4);
	/// ���s����
	if (lpDIBPixelBits == NULL)	
	{	
		setLastErrMsg(_T("Unable to Allocate Memory on Heap"));
		return E_FAIL;
	}

/**
 *	STEP 1
 *		AVI�̐ݒ�Ɏg�p����e��ϐ��A�\���̂�����
 */
/**
 *	AVIStreamSetFormat �֐��Ŏg�p���� BITMAPINFO �\���̂̐ݒ�
 *		typedef struct tagBITMAPINFO {
 *							BITMAPINFOHEADER bmiHeader;				�F�t�H�[�}�b�g�ƃT�C�Y�Ɋւ������ێ����Ă���ABITMAPINFOHEADER �\����
 *							RGBQUAD          bmiColors[1];			���̂����̂ǂꂩ��ێ��B
 *																	RGBQUAD �̔z��B�z��̗v�f�͐F�e�[�u���i���F�p���b�g�j���\������B 
 *																	���ݎg�p����Ă���_���p���b�g�ւ̃C���f�b�N�X���w�肷��A16�r�b�g���������̐����̔z��B
 *						} BITMAPINFO, *PBITMAPINFO;
 *
 *		typedef struct tagBITMAPINFOHEADER{
 *							DWORD  biSize;							�\���̂ɕK�v�ȃo�C�g��( �K�� sizeof(BITMAPINFOHEADER) �Ŏw��B)
 *							LONG   biWidth;							�r�b�g�}�b�v�̕����s�N�Z���P�ʂŎw��			
 *							LONG   biHeight;						�r�b�g�}�b�v�̍������s�N�Z���P�ʂŎw��
 *							WORD   biPlanes;						�^�[�Q�b�g �f�o�C�X�̖ʂ̐����w��B���̒l�� 1
 *							WORD   biBitCount;						1 �s�N�Z��������̃r�b�g�� (bits-per-pixel) ���w��B�����0�A1�A4�A8�A16�A24�A32�̂ǂꂩ���w��B
 *							DWORD  biCompression;					���k���ꂽ "bottom-up" �r�b�g�}�b�v�̈��k�`�����w��BBI_***���w��B�i***�́ARGB�ARLE8�ARLE4�ABITFIELDS�AJPEG�APNG�j
 *							DWORD  biSizeImage;						�摜�T�C�Y���o�C�g�P�ʂŎw��
 *							LONG   biXPelsPerMeter;					�r�b�g�}�b�v�̃^�[�Q�b�g �f�o�C�X�̐��������̉𑜓x���Apixels-per-meter �Ŏw��
 *							LONG   biYPelsPerMeter;					�r�b�g�}�b�v�̃^�[�Q�b�g �f�o�C�X�̐��������̉𑜓x���Apixels-per-meter �Ŏw��
 *							DWORD  biClrUsed;						���ۂɃr�b�g�}�b�v�ɂ���Ďg�p�����F�e�[�u�����̐F�C���f�b�N�X�̐����w��
 *							DWORD  biClrImportant;					�r�b�g�}�b�v��\������̂ɕK�v�ȐF�C���f�b�N�X�̐����w��
 *						} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
 *						
 */
	/// �\���̂̏�����
	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
	/// �\���̂̐ݒ�
	bmpInfo.bmiHeader.biPlanes		= 1;
	bmpInfo.bmiHeader.biWidth		= frameWidth;
	bmpInfo.bmiHeader.biHeight		= frameHeight;
	bmpInfo.bmiHeader.biCompression	= BI_RGB;
	bmpInfo.bmiHeader.biBitCount	= bitsPerPixel;
	bmpInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biSizeImage	= ((bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biBitCount+31)/32*4)*bmpInfo.bmiHeader.biHeight;//bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biHeight*bmpInfo.bmiHeader.biBitCount/32*4;


/**
 *	AVI�t�@�C���t�H�[�}�b�g�����肷�� AVISTREAMINFO �\���̂�ݒ肷��
 *
 *	typedef struct {
 *				DWORD fccType;					�J���X�g���[���^�C�v�������B�K��l�� streamtype***�B�i***��AUDIO�AMIDI�ATEXT�AVIDEO�̂ǂꂩ�j
 *				DWORD fccHandler;				�r�f�I�X�g���[����ۑ����鎞�Ɉ��k����n���h����4�����R�[�h
 *				DWORD dwFlags;					�X�g���[���t���O
 *				DWORD dwCaps;					�@�\�t���O
 *				WORD  wPriority;				�X�g���[���̗D�揇��
 *				WORD  wLanguage;				�X�g���[���̌���
 *				DWORD dwScale;					�X�g���[���̎��ԒP��
 *				DWORD dwRate;					�X�g���[���̃��[�g
 *				DWORD dwStart;					AVI�t�@�C���̍ŏ��̃t���[���̃T���v���ԍ��BdwScale��dwRate�ŒP�ʂ���`�����B�ʏ��0
 *				DWORD dwLength;					�X�g���[���̒����BdwScale��dwRate�ŒP�ʂ���`�����B
 *				DWORD dwInitialFrames;			�I�[�e�B�I �X�L���[�B�C���^�[���[�u�t�@�C�����̃r�f�I�X�g���[���O���փI�[�f�B�I�X�g���[�����ǂ̒��x���炷�����w�肷��B�ʏ��0.75�b�B
 *				DWORD dwSuggestedBufferSize;	�X�g���[����ǂݍ��ނƂ��ɓK�����o�b�t�@�T�C�Y�B���̒l��0�̂Ƃ�������B
 *				DWORD dwQuality;				�r�f�I�X�g���[���̕i���B0�`10000�͈̔͂�-1�̂Ƃ��̓f�t�H���g�̕i���B
 *				DWORD dwSampleSize;				1�̃f�[�^�T���v���̃o�C�g�T�C�Y�B0�̏ꍇ�A�T���v���̃T�C�Y�͉ρB
 *				RECT  rcFrame;					�\�����s����`�B�r�f�I�X�g���[���ł̂ݎg����B
 *	 			DWORD dwEditCount;				�t�@�C����ҏW�i�ǉ��܂��͍폜�j������
 *				DWORD dwFormatChangeCount;		�t�H�[�}�b�g�̕ύX��
 *				char  szName[64];				�X�g���[�����������NULL�ŏI��镶����
 *			} AVISTREAMINFO;
 */
	/// �\���̂̏�����
	ZeroMemory(&aviStreamInfo,sizeof(AVISTREAMINFO));
	/// �\���̂̐ݒ�
	aviStreamInfo.fccType				= streamtypeVIDEO;
	aviStreamInfo.fccHandler			= fccHandler;
	aviStreamInfo.dwScale				= 1;
	aviStreamInfo.dwRate				= frameRate;
	aviStreamInfo.dwQuality				= (DWORD)-1;				
	aviStreamInfo.dwSuggestedBufferSize = maxWidth*maxHeight*4;
    SetRect(&aviStreamInfo.rcFrame, 0, 0, frameWidth, frameHeight);
	_tcscpy(aviStreamInfo.szName, _T("Video Stream"));

/**
 *	���k�I�v�V�������i�[����AVICOMPRESSOPTIONS�\���̂̐ݒ�
 *	typedef struct { 
				DWORD  fccType;					�J���X�g���[���^�C�v�������B�K��l�� streamtype***�B�i***��AUDIO�AMIDI�ATEXT�AVIDEO�̂ǂꂩ�j
				DWORD  fccHandler;				�r�f�I�X�g���[����ۑ����鎞�Ɉ��k����n���h����4�����R�[�h
				DWORD  dwKeyFrameEvery;			�L�[�t���[���̎����BAVICOMPRESSF_KEYFRAMES�t���O���w�肵���Ƃ��݂̂��̒l�͎g����B����ȊO�ł͂��ׂẴt���[�����L�[�t���[���B
				DWORD  dwQuality;				�r�f�I�X�g���[���̕i��
				DWORD  dwBytesPerSecond;		�f�[�^�䗦�BAVICOICMF_CHOOSE_PREVIEWMPRESSF_DATARATE�t���O���w�肵���Ƃ��݂̂��̒l�͎g����B
				DWORD  dwFlags;					���k�t���O�B����4��AVICOMPRESSF_***��I���ŕ����w����B�i***�́ADATARATE�AINTERLEAVE�AKEYFRAMES�AVALID�j
				LPVOID lpFormat;				�f�[�^�t�H�[�}�b�g�\���̂ւ̃|�C���^�B�I�[�f�B�I�X�g���[���ł�WAVEFORMAT�\���̂ւ̃|�C���^�B
				DWORD  cbFormat;				lpFormat�Ŏw�肵���f�[�^�t�H�[�}�b�g�\���̂̃o�C�g��
				LPVOID lpParms;					�r�f�I���k�p�̃f�[�^
				DWORD  cbParms;					lpParams�Ŏw�肵���f�[�^�̃o�C�g��
				DWORD  dwInterleaveEvery;		�C���^�[���[�u�l�BAVICOMPRESSF_INTERLEAVE�t���O���w�肵���Ƃ��݂̂��̒l�͎g����B
			} AVICOMPRESSOPTIONS;

 */

 	/// �\���̂̏�����
	ZeroMemory(&aviCompressOptions,sizeof(AVICOMPRESSOPTIONS));
	/// �\���̂̐ݒ�	
	aviCompressOptions.fccType			= streamtypeVIDEO;
	aviCompressOptions.fccHandler		= aviStreamInfo.fccHandler;
	aviCompressOptions.dwFlags			= AVICOMPRESSF_KEYFRAMES|AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
	aviCompressOptions.dwKeyFrameEvery	= 1;
	//aviCompressOptions.dwBytesPerSecond	= 1000/8;
	//aviCompressOptions.dwQuality			= 100;

/**
 *	STEP 2
 *		�t�@�C�������w�肵��AVI�t�@�C�����J��
 */
	hResult = AVIFileOpen(&pAviFile, outputFileName, OF_CREATE|OF_WRITE, NULL);
	
	if (hResult != AVIERR_OK)
	{
		// Check it succeded.
		switch (hResult)
		{
			case AVIERR_BADFORMAT: 
				setLastErrMsg(_T("AVIFileOpen() Failed. The file couldn't be read, indicating a corrupt file or an unrecognized format."));
				break;

			case AVIERR_MEMORY:		
				setLastErrMsg(_T("AVIFileOpen() Failed. The file could not be opened because of insufficient memory.")); 
				break;

			case AVIERR_FILEREAD:
				setLastErrMsg(_T("AVIFileOpen() Failed. A disk error occurred while reading the file.")); 
				break;

			case AVIERR_FILEOPEN:		
				setLastErrMsg(_T("AVIFileOpen() Failed. A disk error occurred while opening the file."));
				break;
	
			case REGDB_E_CLASSNOTREG:		
				setLastErrMsg(_T("AVIFileOpen() Failed. According to the registry, the type of file specified in AVIFileOpen does not have a handler to process it"));
				break;
		}

		return hResult;
	}

/**
 *	STEP 3
 *		�����̃t�@�C���ɐV�����X�g���[�����쐬���A���̃X�g���[���ւ̃C���^�[�t�F�C�X���쐬
 */
	hResult = AVIFileCreateStream(pAviFile, &pAviStream, &aviStreamInfo);
	if (hResult != AVIERR_OK)
	{
		setLastErrMsg(_T("Unable to Create Video Stream in the Movie File"));
		return hResult;
	}


/**
 *	STEP 4
 *		�񈳏k�X�g���[���ƈ��k�t�B���^���爳�k���ꂽ�X�g���[�����쐬���A���k���ꂽ�X�g���[���ւ̃|�C���^�̃A�h���X���擾����
 */
	hResult = AVIMakeCompressedStream(&pAviCompressedStream, pAviStream, &aviCompressOptions, NULL);
	
	if (hResult != AVIERR_OK)
	{

		/**
		 *	�G���[����������\������r�I����
		 *		���R�̈�ɁA���[�U�̊��ɃC���X�g�[������Ă��Ȃ��R�[�f�b�N���g�p����ꍇ�B
		 *		���̏ꍇ�� mmioFOURCC() ���`�F�b�N����B
		 */		
		switch(hResult)
		{

			case AVIERR_NOCOMPRESSOR:
				setLastErrMsg(_T("AVIMakeCompressedStream() failed: A suitable compressor cannot be found."));
					break;

			case AVIERR_MEMORY:
				setLastErrMsg(_T("AVIMakeCompressedStream() failed: There is not enough memory to complete the operation."));
					break; 

			case AVIERR_UNSUPPORTED:
				setLastErrMsg(_T("AVIMakeCompressedStream() failed: Compression is not supported for this type of data. This error might be returned if you try to compress data that is not audio or video."));
				break;
		}

		return hResult;
	}

/**
 *	STEP 5
 *		����̈ʒu�ɃX�g���[���̃t�H�[�}�b�g��ݒ�
 */
	hResult = AVIStreamSetFormat(pAviCompressedStream, 0, (LPVOID)&bmpInfo, bmpInfo.bmiHeader.biSize +  bmpInfo.bmiHeader.biClrUsed * sizeof(RGBQUAD));
	if (hResult != AVIERR_OK)
	{
		/**
		 *	�ǂ�����G���[�̌����́A�g�p����r�b�g�}�b�v���R�[�f�b�N�̎d�l�ɖ����Ȃ��ꍇ�ł���B
		 *	�Ⴆ�΁A
		 *		���[�U�̃r�b�g�}�b�v��32bpp�Ȃ̂ɃR�[�f�b�N�̃T�|�[�g��16��24bpp�������ꍇ
		 *		�R�[�f�b�N�̉𑜓x�T�|�[�g��2�̏搔�݂̂Ȃ̂ɁA���[�U�̃r�b�g�}�b�v�̉𑜓x��720*405�Ȃǂ̏ꍇ
		 *	����������邽�߂ɁA�R�[�f�b�N�̑I���ƃr�b�g�}�b�v�̎d�l�ɂ͒��ӂ��邱��
		 */
		setLastErrMsg(_T("AVI Compressed Stream format setting failed."));
		return hResult;
	}

	return S_OK;	/// ����
}

/**
 *	����
 *		��������֘A�n���h���̉��
 */
void AviMaker::releaseMemory()
{
	/// �_�~�[�Z���N�^���w��
	appendFuncSelector = 0;
	
	/// �f�o�C�X�R���e�L�X�g�̉��
	if(memDCHandle)
	{
		DeleteDC(memDCHandle);
		memDCHandle=NULL;
	}
	
	/// ���k�X�g���[���̉��
	if(pAviCompressedStream)
	{
		AVIStreamRelease(pAviCompressedStream);
		pAviCompressedStream=NULL;
	}
	
	/// �X�g���[���̉��
	if(pAviStream)
	{
		AVIStreamRelease(pAviStream);
		pAviStream=NULL;
	}
	
	/// �t�@�C���|�C���^�̉��
	if(pAviFile)
	{
		AVIFileRelease(pAviFile);
		pAviFile=NULL;
	}
	
	/// �������u���b�N�̉��
	if(lpDIBPixelBits)
	{
		HeapFree(heapHandle, HEAP_NO_SERIALIZE, lpDIBPixelBits);
		lpDIBPixelBits=NULL;
	}
	
	/// �v���C�x�[�g�q�[�v�̔j��
	if(heapHandle)
	{
		HeapDestroy(heapHandle);
		heapHandle=NULL;
	}
}

/**
 *	����
 *		�ŏI�G���[�̐ݒ�
 *	����
 *		pLastErrMsg: �G���[���e
 */
void AviMaker::setLastErrMsg(LPCTSTR lpszErrorMessage)
{
	_tcsncpy(lastErrMsg, lpszErrorMessage, __countof(lastErrMsg)-1);
}


/**
 *	----------------------------------------
 *	����̍쐬�̂��߂̃w���v�֐�
 *	�I�[�o�[���[�h
 *	----------------------------------------
 */
/// ����̃t���[���ǉ��֐�
HRESULT	AviMaker::appendFrameFirstTime(HBITMAP hBitmap)
{
	BITMAP Bitmap;

	/// �r�b�g�}�b�v�̏��𓾂�
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);

	/// �ŏ��ɓ���̏��������s��
	if (SUCCEEDED(initializeEngine(Bitmap.bmWidth, Bitmap.bmHeight, Bitmap.bmBitsPixel)))
	{
		/// �ʏ�̒ǉ��@�\��I��
		appendFuncSelector = 2;
		return appendFrameUsual(hBitmap);
	}

	releaseMemory();

	return E_FAIL;
}

/// �ʏ�̃t���[���ǉ��֐�
HRESULT AviMaker::appendFrameUsual(HBITMAP hBitmap)
{
	/// �ԋp�l
	HRESULT hResult;

	BITMAPINFO	bmpInfo;
	bmpInfo.bmiHeader.biBitCount = 0;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	
	/**
	 *	int GetDIBits(HDC hdc,HBITMAP hbmp,UINT uStartScan,UINT cScanLines,LPVOID lpvBits,LPBITMAPINFO lpbi,UINT uUsage);
	 *		DDB�̕`����e��������DIB�ɃR�s�[
	 *	����
	 *		hdc: �f�o�C�X�R���e�L�X�g�̃n���h�����w��B
	 *		hbmp: �f�o�C�X�ˑ��r�b�g�}�b�v�̃n���h�����w��B
	 *		uStartScan: �擾����ŏ��̑����s���w��B
	 *		cScanLines: �擾���鑖���s�̐����w��B
	 *		lpvBits: �o�b�t�@�ւ̃|�C���^���w��B���̃o�b�t�@�Ƀr�b�g�}�b�v�f�[�^���i�[�����B
	 *				NULL ���w�肷��ƁA�r�b�g�}�b�v�̃T�C�Y�ƌ`�����Alpbi �p�����[�^���w�� BITMAPINFO �\���̂Ɋi�[�����B
	 *		lpbi: ��]����f�[�^�̌`�����������ABITMAPINFO �\���̂ւ̃|�C���^���w��B
	 *		uUsage:	BITMAPINFO �\���̂� bmiColors �����o�̌`�����w��B���̒l�̂����ꂩ���w�肷��B
	 *					DIB_PAL_COLORS �J���[�e�[�u���́A���݂̘_���p���b�g�ւ� 16 �r�b�g�̃C���f�b�N�X�̔z��B 
	 *					DIB_RGB_COLORS �J���[�e�[�u���́ARGB �l���̂��́B 
	 *	�߂�l : lpvBits �p�����[�^�� NULL �ȊO�̂Ƃ��Ɋ֐�����������ƁA�R�s�[���ꂽ�����s�̐����Ԃ�B
	 *				Windows 95 �ł́AlpvBits �p�����[�^�� NULL �̂Ƃ��Ɋ֐�����������ƁA�r�b�g�}�b�v�̑����s�̐����Ԃ�܂��B
	 *				Windows NT �ł́AlpvBits �p�����[�^�� NULL �̂Ƃ��Ɋ֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B
	 *			�֐������s����� 0�B
	 */
	/**
	 *	lpvBits��NULL�ɂ���GetDIBits���Ăяo���A
	 *	�f�o�C�X�h���C�o�� BITMAPINFOHEADER �\���̂�biSizeImage �t�B�[���h���v�Z������
	 */
	GetDIBits(memDCHandle, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
	bmpInfo.bmiHeader.biCompression = BI_RGB;	

	/// �ړI��DIB�� lpDIBPixelBits �Ɋi�[����
	GetDIBits(memDCHandle, hBitmap, 0, bmpInfo.bmiHeader.biHeight, lpDIBPixelBits, &bmpInfo, DIB_RGB_COLORS);

	/// �쐬����DIB��AVI�ɏ�������
	hResult = AVIStreamWrite(pAviCompressedStream, frameCounter++, 1, lpDIBPixelBits, bmpInfo.bmiHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);

	if (FAILED(hResult))
	{
		setLastErrMsg(_T("Unable to Write Video Stream to the output Movie File"));

		releaseMemory();

		return E_FAIL;
	}

    return S_OK;
}

/// �t���[���ǉ��֐��̃_�~�[�i���ۂ͂Ȃɂ����Ȃ��j
HRESULT AviMaker::appendDummy(HBITMAP hBitmap)
{
	return E_FAIL;
}

/// ����̃t���[���ǉ��֐�
HRESULT	AviMaker::appendFrameFirstTime(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	if (SUCCEEDED(initializeEngine(width, height, bitsPerPixel)))
	{
		/// �ʏ�̒ǉ��@�\��I��	
		appendFuncSelector=2;

		return appendFrameUsual(width, height, pBits, bitsPerPixel);
	}

	releaseMemory();

	return E_FAIL;
}

/// �ʏ�̃t���[���ǉ��֐�
HRESULT	AviMaker::appendFrameUsual(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	/// �ԋp�l
	HRESULT hResult;

	DWORD size = width*height*bitsPerPixel/8;

	hResult = AVIStreamWrite(pAviCompressedStream, frameCounter++, 1, pBits, size, AVIIF_KEYFRAME, NULL, NULL);

	if (FAILED(hResult))
	{
		setLastErrMsg(_T("Unable to Write Video Stream to the output Movie File"));

		releaseMemory();

		return E_FAIL;
	}

    return S_OK;
}

/// �t���[���ǉ��֐��̃_�~�[�i���ۂ͂Ȃɂ����Ȃ��j
HRESULT	AviMaker::appendDummy(int width, int height, LPVOID pBits,int bitsPerPixel)
{
	return E_FAIL;
}
