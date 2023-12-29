/**
 *  �t�@�C����
 *		SciRingBuffer.cpp
 *  ����
 *		�����O�o�b�t�@�ɂ�鏈��
 *  ���t
 *		�쐬��: 2007/12/10(MON)		�X�V��: 2007/12/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "SciRingBuffer.h"
#include "iostream"
//#include "ASURA2GUI/pch.h"
#include "..\pch.h"


using namespace std;

/**
 *	----------------------------------------------------------------------
 *		SciRingBuffer�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		SciRingBuffer�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
SciRingBuffer::SciRingBuffer()
{
	/// �e�ʂ̓f�t�H���g
	bufferBitSize = DEFAULT_BIT_SIZE;
	bufferSize = 1 << bufferBitSize;
	buffer = NULL;

	/// �C���f�b�N�X��0
	readIndex = 0;
	writeIndex = 0;

	/// �������m��
	newSciRingBuffer();
}

SciRingBuffer::SciRingBuffer(int bitSize)
{
	/// �C���f�b�N�X��0
	readIndex = 0;
	writeIndex = 0;
	buffer = NULL;

	initialize(bitSize);
}

/// �f�X�g���N�^
SciRingBuffer::~SciRingBuffer()
{
	deleteSciRingBuffer();
}

void SciRingBuffer::initialize(int bitSize)
{
	if (bitSize <= 0)
		cerr << "Error: [SciRingBuffer::SciRingBuffer] Argument is invalid\n" << endl;

	/// �������̈�����
	deleteSciRingBuffer();

	/// �e�ʂ�����
	bufferBitSize = bitSize;
	bufferSize = 1 << bufferBitSize;

	/// �x�N�g���I�u�W�F�N�g�̃������̈�m��
    newSciRingBuffer();
}

/**
 *	����
 *		�o�b�t�@�̃N���A
 */
void SciRingBuffer::clear(void)
{
	readIndex = 0;
	writeIndex = 0;
}

/**
 *	����
 *		�����O�o�b�t�@�̋L�^�T�C�Y���擾
 *	�ԋp�l
 *		�L�^���Ă���o�C�g�T�C�Y
 */
int SciRingBuffer::getReadableSize(void) const
{
	return ((writeIndex >= readIndex) ? (writeIndex - readIndex) : (bufferSize - (readIndex - writeIndex)));
}

/**
 *	����
 *		�����O�o�b�t�@�̋󂫂̃o�b�t�@�T�C�Y���擾
 *	�ԋp�l
 *		�󂫃o�b�t�@�̃o�C�g�T�C�Y
 */
int SciRingBuffer::getWritableSize(void) const
{
	return (bufferSize - getReadableSize() - 1);
}



/**
 *	----------------------------------------
 *	�������݊֐�
 *	----------------------------------------
 */
/**
 *	����
 *		�����O�o�b�t�@��1�o�C�g�i�[
 *	����
 *		buff: �������ޒl
 *	�ԋp�l
 *		-1: ���s
 *		0: ����
 */
int SciRingBuffer::put(unsigned char buff)
{
	/// �����O�o�b�t�@���󂩂ǂ���
	if (getWritableSize() <= 0)
	{
		/// �󂫗̈斳��
		return -1;
	}
	
	/// �o�b�t�@���i�[
	buffer[writeIndex] = buff;
	/// �������݈ʒu���X�V
	writeIndex = (writeIndex + 1) & (bufferSize - 1);

	return 0;
}

/**
 *	����
 *		�����O�o�b�t�@�֕����o�C�g�i�[����
 *	����
 *		dst: �������ރf�[�^�ւ̃|�C���^
 *		size: �������ރT�C�Y
 *	�ԋp�l
 *		���ۂɏ������񂾃T�C�Y
 */
int SciRingBuffer::write(unsigned char* dst, int size)
{
	int i;

	/// �������݃T�C�Y�̌���
	int writableSize = getWritableSize();
	int sizeToWrite = (size > writableSize) ? writableSize : size;

	/// �������ݏI���C���f�b�N�X�̌v�Z
	int endIndex = (sizeToWrite > (bufferSize - writeIndex)) ? bufferSize : writeIndex + sizeToWrite;

	unsigned char* p = &buffer[writeIndex];
	unsigned char* q = dst;
	
	for (i=writeIndex;i<endIndex;++i)
		*p++ = *q++;
	
	/// �Ō�̏������݃A�h���X�̌v�Z
	unsigned char* data_last = dst + sizeToWrite;
	
	/// �������߂Ă��Ȃ������c��̃o�b�t�@���i�[
	if (q < data_last)
	{
		p = &buffer[0];
		while (q < data_last)
		{
			/// �f�[�^���i�[���ă|�C���^���C���N�������g
			*p++ = *q++;
		}
	}

	writeIndex = static_cast<int>((p - &buffer[0]) & (bufferSize - 1));
	return sizeToWrite;
}

/**
 *	����
 *		�����O�o�b�t�@����1�o�C�g�擾
 *	����
 *		buff: �ǂݍ��ݐ�ւ̃|�C���^
 *	�ԋp�l
 *		-1: �f�[�^����
 *		0: �ǂݍ��ݐ��� 
 */
int SciRingBuffer::get(unsigned char* buff)
{
	/// �f�[�^�����邩�ǂ���
	if (readIndex == writeIndex)
		return -1;

	*buff = buffer[readIndex];
	/// �ǂݍ��݈ʒu���X�V
	readIndex = (readIndex + 1) & (bufferSize - 1);
	
	return 0;
}

/**
 *	����
 *		�����O�o�b�t�@���畡���o�C�g�ǂݍ���
 *	����
 *		dst: �ǂݍ��ݐ�ւ̃|�C���^
 *		size: �ǂݍ��ރT�C�Y
 *	�ԋp�l
 *		�ǂݍ��񂾃T�C�Y 
 */
int SciRingBuffer::read(unsigned char* dst, int size)
{
	int i;

	/// �ǂݍ��݃T�C�Y�̌���
	int readableSize = getReadableSize();
	int sizeToRead = (size > readableSize) ? readableSize : size;

	/// �ǂݍ��݃C���f�b�N�X�̌v�Z
	int endIndex = (sizeToRead > (bufferSize - readIndex)) ? bufferSize : readIndex + sizeToRead;

	unsigned char* p = dst;
	unsigned char* q = &buffer[readIndex];

	for (i=readIndex;i<endIndex;++i)
		*p++ = *q++;

	unsigned char* data_last = dst + sizeToRead;
	if (p < data_last)
	{
		q = &buffer[0];
		while (p < data_last)
		{
			*p++ = *q++;
		}
	}
	
	readIndex = static_cast<int>((q - &buffer[0]) & (bufferSize - 1));
	
	return sizeToRead;
}

/**
 *	����
 *		�o�b�t�@�̋L�^�f�[�^���R�s�[
 *	����
 *		dst: �R�s�[��ւ̃|�C���^
 *		size: �R�s�[����T�C�Y
 *	�ԋp�l
 *		�R�s�[�����T�C�Y  
 */
int SciRingBuffer::copy(unsigned char* dst, int size)
{
	int i;

	/// �ǂݍ��݃T�C�Y�̌���
	int readableSize = getReadableSize();
	int sizeToRead = (size > readableSize) ? readableSize : size;

	/// �ǂݍ��݃C���f�b�N�X�̌v�Z
	int endIndex = (sizeToRead > (bufferSize - readIndex)) ? bufferSize : readIndex + sizeToRead;

	unsigned char* p = dst;
	unsigned char* q = &buffer[readIndex];

	for (i=readIndex;i<endIndex;++i)
		*p++ = *q++;

	unsigned char* data_last = dst + sizeToRead;
	if (p < data_last)
	{
		q = &buffer[0];
		while (p < data_last)
		{
			*p++ = *q++;
		}
	}
	
	return sizeToRead;
}

/**
 *	------------------------------------------------------------
 *		SciRingBuffer�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�����O�o�b�t�@�̃������̈�m��
 */
void SciRingBuffer::newSciRingBuffer(void)
{
	/// �e�ʂ��O�ȉ��̏ꍇ
	if (bufferSize <= 0)
	{
		cerr << "Error: [SciRingBuffer::newSciRingBuffer] Memory is not allocated\n" << endl;
        abort();
    }

	/// �������[�̈���m�ۂ���
	buffer = new unsigned char[bufferSize];
}

/**
 *	����
 *		�����O�o�b�t�@�̃������̈���
 */
void SciRingBuffer::deleteSciRingBuffer(void)
{
	///�������[�̈���J������
    if (buffer != NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}
}