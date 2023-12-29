/**
 *  File: FileReader.cpp
 *
 *  Description: �t�@�C���ւ���̓Ǎ��݃N���X
 *
 *  Created: 2007/09/04(Tue)
 *  Updated: 2007/09/04(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "FileReader.h"
#include <tchar.h>
#include <stdarg.h>

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		AsuraData�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		FileReader�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
FileReader::FileReader()
{
/**
 *	�Ǎ��݂̏���
 */
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];
}

/// �R���X�g���N�^
FileReader::FileReader(const TCHAR* fileName, FileMode mode)
{
/**
 *	�Ǎ��݂̏���
 */
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];

	FileReader::openFile(fileName, mode);
}

/// �f�X�g���N�^
FileReader::~FileReader()
{
	if ( buffer != NULL )
		delete [] buffer;
}

/**
 *	����
 *		�w�肵���p�X�̃t�@�C�����J��
 *	����
 *		fileName: �t�@�C����
 *		mode: �t�@�C�����[�h
 *		isTimeStamped: �����t�H���_���쐬���邩
 */
int FileReader::openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped)
{
	/// �t�@�C�����[�h�̃`�F�b�N
	/// �����ݐ�p�ŊJ������G���[
	if ( mode == WRITE_ONLY )
		 return ERR_INVALID_OPEN_MODE;

	return File::openFile(fileName, mode, isTimeStamped);
}

/**
 *	����
 *		�o�C�g�f�[�^�Ǎ���
 *	����
 *		data: �ǂݏo���f�[�^�ւ̃|�C���^
 *		size: �f�[�^�T�C�Y
 *	�ԋp�l
 *		true: ����
 *		false: ���s
 */
bool FileReader::readByte(void* data, int size)
{
	/// �t�@�C�����J����Ă��邩�ǂ���
	if ( pFile == NULL )
		return false;

	/// �Ǎ��݃o�C�g��
	int result = (int)fread( data, 1, size, pFile );

	return result == size;
}

/**
 *	����
 *		�e�L�X�g�f�[�^��s�Ǎ���
 *	����
 *		string: �ǂݏo���f�[�^�ւ̃|�C���^
 *		size: �ǂݏo������T�C�Y
 *	�ԋp�l
 *		-1: ���s
 *		���̑�: �ǂݏo�����T�C�Y
 */
int FileReader::readLine(TCHAR* string, size_t size)
{
	/// �t�@�C�����J����Ă��邩�ǂ���
	if (pFile == NULL)
		return -1;

	if (_fgetts(string, size, pFile) == NULL)
		return 0;
	
	return _tcslen(string);
}

}	/// end of namespace System

