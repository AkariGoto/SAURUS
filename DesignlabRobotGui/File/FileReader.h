/**
 *  �t�@�C����
 *		FileReader.h
 *  ����
 *		�t�@�C������̓ǂݍ��݃N���X
 *  ���t
 *		�쐬��: 2007/09/05(WED)		�X�V��: 2007/09/05(WED)
 */

#ifndef __FileReader_h__
#define __FileReader_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "File.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		FileReader�N���X
 *	----------------------------------------------------------------------
 */
class FileReader : public File
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

private:
/**
 *	�Ǎ��݂ɗp����ϐ�
 */
	/// �o�b�t�@
	TCHAR* buffer;

	/// �o�b�t�@�T�C�Y
	int bufferSize;


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
	FileReader();

	/// �t�@�C���ɂ��R���X�g���N�^
	explicit FileReader(const TCHAR* fileName, FileMode mode);

	/// �f�X�g���N�^
	virtual ~FileReader();

/**
 *	����
 *		�w�肵���p�X�̃t�@�C�����J��
 *	����
 *		fileName: �t�@�C����
 *		mode: �t�@�C�����[�h
 *		isTimeStamped: �����t�H���_���쐬���邩
 */
	virtual int openFile(const TCHAR* fileName, FileMode mode, bool isTimeStamped = true);

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
	bool readByte(void* data, int size);

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
	int readLine(TCHAR* string, size_t size);

private:


};	/// end of class FileReader

}	/// end of namespace System

#endif	/// __FileReader_h__

