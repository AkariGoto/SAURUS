/**
 *  �t�@�C����
 *		FileWriter.h
 *  ����
 *		�t�@�C���ւ̏����݃N���X
 *  ���t
 *		�쐬��: 2007/09/05(WED)		�X�V��: 2007/09/05(WED)
 */

#ifndef __FileWriter_h__
#define __FileWriter_h__

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
 *		FileWriter�N���X
 *	----------------------------------------------------------------------
 */
class FileWriter : public File
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

private:
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
	FileWriter();

	/// �t�@�C���ɂ��R���X�g���N�^
	explicit FileWriter(const TCHAR* fileName, FileMode mode);

	/// �f�X�g���N�^
	virtual ~FileWriter();

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
 *		�o�C�g�f�[�^�����o��
 *	����
 *		data: �����o���f�[�^�ւ̃|�C���^
 *		size: �f�[�^�T�C�Y
 *	�ԋp�l
 *		false: ���s
 *		true: ����
 */
	bool writeByte(const void* data, unsigned long size);

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
	bool writeLine(const TCHAR* string);

/**
 *	����
 *		�����t�����񏑂��o��
 *		fprintf�̃��b�p�[
 *	����
 *			format: �����w�蕶������i�[����|�C���^
 */
	void writeFormattedString(const TCHAR* format, ...);

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
	bool writeText(const TCHAR* text, unsigned long size);

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
	bool writeFormattedText(const TCHAR* format, ...);



private:


};	/// end of class FileWriter

}	/// end of namespace System

#endif	/// __FileWriter_h__

