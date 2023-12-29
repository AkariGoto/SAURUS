/**
 *  �t�@�C����
 *		File.h
 *  ����
 *		�t�@�C���𑀍삷��N���X
 *		 fopen, fwrite, fread�Ȃ�CRT�̃��b�p�[�N���X
 *  ���t
 *		�쐬��: 2007/09/04(Tue)		�X�V��: 2007/09/04(Tue)
 */

#ifndef __File_h__
#define __File_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */

#include <windows.h>
#include <cstdio>
#include <ctime>

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		File�N���X
 *	----------------------------------------------------------------------
 */
class File
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
/**
 *	�G���[�R�[�h
 */
enum FileError
{
	NO_FILE_ERROR,				/// �G���[����
	ERR_FILE_NOT_FOUND,			/// �t�@�C����������Ȃ�
	ERR_FAIL_CREATE_FILE,		/// �t�@�C�����쐬�ł��Ȃ�
	ERR_FAIL_WRITE_FILE,		/// �����݃G���[
	ERR_FAIL_READ_FILE,			/// �Ǎ��݃G���[
	ERR_FAIL_CLOSE,				/// �t�@�C�������̂����s
	ERR_END_OF_FILE,			/// EOF�G���[
	ERR_NO_OPEN,				/// �t�@�C�����J���Ȃ�
	ERR_INVALID_OPEN_MODE,		/// �t�@�C�����[�h������
};

/**
 *		�t�@�C���I�[�v�����[�h
 */
enum FileMode
{
	READ_ONLY,		/// �Ǎ��ݐ�p
	WRITE_ONLY,		/// �����ݐ�p
	APPEND_ONLY,	/// �ǉ������ݐ�p
	READ_WRITE,		/// �Ǎ��݂Ə�����
	WRITE_READ,		/// �����݂ƓǍ���
	READ_APPEND,	/// �ǂݍ��݂ƒǉ�������
	INVALID_MODE,	/// �����ȏ���
};

protected:

	/// �f�t�H���g�o�b�t�@�T�C�Y
	static const int DEFAULT_BUFFER_SIZE = 1024;
	/// �t�H�[�}�b�g�f�t�H���g�o�b�t�@�T�C�Y
	static const int FORMAT_DEFAULT_BUFFER_SIZE = 1024;

/**
 *	�t�@�C���n���h��
 */
	FILE* pFile;

/**
 *	�t�@�C�����[�h
 */
	FileMode fileMode;

/**
 *	�t�H���_���i�^�C���X�^���v�j
 */
	TCHAR folderName[128];


/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
public:
	/// �f�t�H���g�R���X�g���N�^
	File();

	/// �R���X�g���N�^
	explicit File(const TCHAR* fileName, FileMode mode);

	/// �f�X�g���N�^
	virtual ~File();

/**
 *	����
 *		�w�肵���p�X�̃t�@�C�����J��
 *	�����F
 *		fileName: �t�@�C����
 *		mode: �t�@�C�����[�h
 *		isTimeStamped: �����t�H���_���쐬���邩
 */
	virtual int openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped = true);

/**
 *	����
 *		�t�@�C�������
 */
	int closeFile(void);

/**
 *	����
 *		�t�@�C�����J����Ă��邩�ǂ���
 */
	bool isOpen(void) const;

/**
 *	����
 *		�t�@�C���|�C���^�̈ʒu���擾
 */
	long getPosition(void) const;

/**
 *	����
 *		�t�@�C���|�C���^�̈ʒu��ݒ�
 */
	bool setPosition(long position);

private:
	void createTimeStampedFolder(void);

};	/// end of class File

}	/// end of namespace System

#endif  /// __File_h__