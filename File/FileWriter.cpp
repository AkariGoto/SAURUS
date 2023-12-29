/**
 *  �t�@�C����
 *		FileWriter.h
 *  ����
 *		�t�@�C���ւ̏����݃N���X
 *  ���t
 *		�쐬��: 2007/09/05(WED)		�X�V��: 2007/09/05(WED)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "FileWriter.h"
#include <tchar.h>
#include <stdarg.h>

namespace System
{
/**
 *	------------------------------------------------------------
 *		FileWriter�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
FileWriter::FileWriter()
{
	/// �o�b�t�@�̐ݒ�
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];
}

FileWriter::FileWriter(const TCHAR *fileName, FileMode mode)
{
	/// �o�b�t�@�̐ݒ�
	bufferSize = DEFAULT_BUFFER_SIZE;
	buffer = new TCHAR[bufferSize];

	FileWriter::openFile(fileName, mode);
}

/// �f�X�g���N�^
FileWriter::~FileWriter()
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
int FileWriter::openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped)
{
	/// �t�@�C�����[�h�̃`�F�b�N
	/// �Ǎ��ݐ�p�ŊJ������G���[
	if ( mode == READ_ONLY )
		 return ERR_INVALID_OPEN_MODE;

	return File::openFile(fileName, mode, isTimeStamped);

}

/**
 *	����
 *		�o�C�g�f�[�^�����o��
 *	����
 *		data: �����o���f�[�^�ւ̃|�C���^
 *		size: �f�[�^�T�C�Y
 *	�ԋp�l
 *		false: ���s
 *		true: ����
 */
bool FileWriter::writeByte(const void* data, unsigned long size)
{
	/// �t�@�C�����J���Ă��邩�ǂ���
	if ( pFile == NULL || data == NULL )
		return false;

	/// ���ۂɏ�������
	int result = (int)fwrite( data, 1, size, pFile );

	/// �����ݗ\��T�C�Y�Ǝ��ۂ̏�����
	return result == size;
}

/**
 * ����
 *		1�s�e�L�X�g�����o��
 *	����
 *		data: �����o���f�[�^�ւ̃|�C���^
 *		size: �f�[�^�T�C�Y
 *	�ԋp�l
 *		false: ���s
 *		true: ����
 */
bool FileWriter::writeLine(const TCHAR* string)
{
	/// �t�@�C�����J���Ă��邩
	if ( pFile == NULL || string == NULL )
		return false;
	
	_fputts( string, pFile );
	_fputts( TEXT("\n"), pFile );

	return true;
}

/**
 *	����
 *		�����t�����񏑂��o��
 *		fprintf�̃��b�p�[
 *	����
 *			format: �����w�蕶������i�[����|�C���^
 */
void FileWriter::writeFormattedString(const TCHAR* format, ...)
{
	/// �t�@�C�����J���Ă��邩
	if (pFile == NULL)
		return;

	va_list ap;
	
	va_start(ap, format);
              _vftprintf( pFile, format, ap );
	va_end(ap);

}

/**
 *	����
 *		�e�L�X�g�f�[�^�����o��
 *		���s�R�[�h\\n��\r\n�ɕϊ�����
 *	����
 *		data: �����o���f�[�^�ւ̃|�C���^
 *		size: �f�[�^�T�C�Y
 *	�ԋp�l
 *		false: ���s
 *		true: ����
 */
bool FileWriter::writeText(const TCHAR* text, unsigned long size)
{
	if (text != NULL)
		return false;

	/// ���S�̂��߃o�b�t�@�g��
	int maxSize = size*2;

	/// �ȉ��Ńo�b�t�@�T�C�Y�������̃T�C�Y�܂ő��₷
	if (maxSize > bufferSize)
	{
		delete [] buffer;
		while (1)
		{
			bufferSize *= 2;
			if (maxSize <= bufferSize)
				break;
		}
	}

	/// Windows�̉��s�t�B���^�����O
	int writeSize = 0;
	TCHAR preTChar = TEXT('');
	int i;		/// �J�E���^
	
	/// �ȉ��Ŏ��ۂɉ��s�R�[�h�̃t�B���^�����O���s��
	for (i=0;i<(int)size;i++)
	{
		if ((text[i] == TEXT('\n')) && preTChar != TEXT('\r'))
		{
			buffer[writeSize] = TEXT('\r');
			writeSize++;
		}

		buffer[writeSize] = text[i];
		preTChar = text[i];
		writeSize++;
	}

	/// �o�C�g�f�[�^���o��
	return writeByte(buffer, writeSize);
}

/**
 *	����
 *		�����w��e�L�X�g�f�[�^�����o��
 *		���s�R�[�h\\n��\r\n�ɕϊ�����
 *	����
 *		format: �����w�蕶������i�[����|�C���^
 *	�ԋp�l
 *		false: ���s
 *		true: ����
 */
bool FileWriter::writeFormattedText(const TCHAR* format, ...)
{
	/// �����`�F�b�N
	if ( format == NULL )
		return false;

	/// ����
	int formatBufferSize = FORMAT_DEFAULT_BUFFER_SIZE;
	TCHAR* formatBuffer;
	int writeSize;
	va_list ap;

	va_start(ap, format);
	while(1)
	{
		formatBuffer = new TCHAR[formatBufferSize];
		writeSize =  _vsntprintf(formatBuffer, formatBufferSize, format, ap);

		/// �֐����s����
		if ( writeSize != -1 )
			break;

		formatBufferSize *= 2;
		delete [] formatBuffer;
	}
	va_end(ap);

	/// NULL�I�[�łȂ��\��������i���Ȃ����j
	bool ret = writeText(formatBuffer, writeSize);

	/// �o�b�t�@�̌㏈��
	delete [] formatBuffer;

	/// writeText()�̐��ۂ��߂�l
	return ret;
}

}	/// end of namespace System