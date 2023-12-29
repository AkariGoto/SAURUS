/**
 *  �t�@�C����
 *		File.cpp
 *  ����
 *		�t�@�C���𑀍삷��N���X
 *		 fopen, fwrite, fread�Ȃ�CRT�̃��b�p�[�N���X
 *  ���t
 *		�쐬��: 2007/09/04(Tue)		�X�V��: 2007/09/04(Tue)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */

#include "File.h"
#include <tchar.h>

namespace System
{

/**
 *	------------------------------------------------------------
 *		File�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */
/**
 *	�R���X�g���N�^�ƃf�X�g���N�^
 */
/// �R���X�g���N�^
File::File()
{
	pFile = NULL;
}

/// �R���X�g���N�^
File::File(const TCHAR* fileName, FileMode mode)
{
	openFile(fileName, mode);
}

/// �f�X�g���N�^
File::~File()
{
	closeFile();
}

/**
 *	����
 *		�w�肵���p�X�̃t�@�C�����J��
 *	�����F
 *		fileName: �t�@�C����
 *		mode: �t�@�C�����[�h
 *		isTimeStamped: �����t�H���_���쐬���邩
 */
int File::openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped)
{

	/// �쐬����p�X
	TCHAR path[256];
	errno_t err;


	/// �t�@�C�����[�h�̃`�F�b�N
	if ( mode != READ_ONLY &&
		 mode != WRITE_ONLY &&
		 mode != APPEND_ONLY &&
		 mode != READ_WRITE &&
		 mode != READ_APPEND )
		 return ERR_INVALID_OPEN_MODE;

	/// �t�H���_�t���̃p�X���쐬
	if (isTimeStamped)
	{
		createTimeStampedFolder();
		wsprintf(path, TEXT(".\\%s\\%s"), folderName, fileName);
	}
	else	/// ����ȊO��������t�@�C���������̂܂܃p�X��
	{
		lstrcpy(path, fileName);
	}


	/// Win32API�̂��΂�Ńo�C�i���`���̂݁i�e�L�X�g�����Ȃ��j
	switch (mode)
	{
		case READ_ONLY:	/// �o�C�i�����[�h�ł̓Ǎ��ݐ�p
			err = _tfopen_s(&pFile, path, TEXT("rb"));
			break;

		case WRITE_ONLY:	/// �o�C�i�����[�h�ł̏������ݐ�p
			err = _tfopen_s(&pFile, path, TEXT("wb"));
			break;

		case APPEND_ONLY:	/// �o�C�i�����[�h�ł̒ǉ�������
			err = _tfopen_s(&pFile, path, TEXT("ab"));
			break;

		case READ_WRITE:	/// �o�C�i�����[�h�ł̓ǂݍ��݂Ə�������
			err = _tfopen_s(&pFile, path, TEXT("r+b"));
			break;

		case WRITE_READ:	/// �o�C�i�����[�h�ł̏������݂Ɠǂݍ���
			err = _tfopen_s(&pFile, path, TEXT("w+b"));
			break;

		case READ_APPEND:	/// �o�C�i�����[�h�ł̓ǂݍ��݂ƒǉ�������
			err = _tfopen_s(&pFile, path, TEXT("a+b"));
			break;
		
		default:
			break;
	}

	/// ::CreateFile()�̐�������
	if (err == 0)
	{
		fileMode = mode;
		return NO_FILE_ERROR;
	}
	else
	{
		/// ���s������
		::MessageBox(0, TEXT("[File::openFile] Failed"), TEXT(""), 0);
	}

	/// �t�@�C�����Ȃ�������
	if (fileMode == READ_ONLY)
		return ERR_FILE_NOT_FOUND;

	return ERR_FAIL_CREATE_FILE;
}

/**
 *	����
 *		�t�@�C�������
 */
int File::closeFile(void)
{
	/// �t�@�C�����J����Ă��Ȃ�������
	if (!pFile)
		return ERR_NO_OPEN;

	/// �t�@�C�������
	if (fclose( pFile ) == 0)
	{
		pFile = 0;
		return NO_FILE_ERROR;
	}

	return ERR_FAIL_CLOSE;
}

/**
 *	����
 *		�t�@�C�����J����Ă��邩�ǂ���
 */
bool File::isOpen(void) const
{
	return pFile != NULL;
}

/**
 *	����
 *		�t�@�C���|�C���^�̈ʒu��ݒ�
 */
long File::getPosition(void) const
{
	if (pFile == NULL)
		return 0;

	long pos = ftell(pFile);

	/// ftell()�̎��s����
	if (pos < 0) pos = -1;

	return pos;
}

/**
 *	����
 *		�t�@�C���|�C���^�̈ʒu��ݒ�
 */
bool File::setPosition(long position)
{
	if (pFile == NULL)
		return false;

	int ret = fseek(pFile, position, SEEK_SET);

	if (ret != 0)
		return false;

	return true;
}

/**
 *	------------------------------------------------------------
 *		File�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
void File::createTimeStampedFolder(void)
{
	/// SYSTEMTIME �\����
	SYSTEMTIME sysTime;
	
	/// ���[�J�����Ԃ̎擾
	::GetLocalTime(&sysTime);

	wsprintf(	folderName,
				TEXT("%.4d%.2d%.2d-%.2d%.2d%.2d"),
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond
			);

	/// �^�C���X�^���v���Ńt�H���_�쐬
	_tmkdir(folderName);

	return;
}

}	/// end of namespace System