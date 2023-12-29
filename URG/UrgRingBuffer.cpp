/**
 *  �t�@�C����
 *		UrgRingBuffer.cpp
 *  ����
 *		�����O�o�b�t�@�ɂ�鏈��
 *  ���t
 *		�쐬��: 2014/06/20(FRI)		�X�V��:
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "UrgRingBuffer.h"
#include "iostream"

using namespace std;

/**
 *	----------------------------------------------------------------------
 *		UrgRingBuffer�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		UrgRingBuffer�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
UrgRingBuffer::UrgRingBuffer()
{
	/// �C���f�b�N�X��0
	readIndex = 0;
	writeIndex = 0;
	buffer = NULL;

	bufferTime=5000;		//[ms]	200[ms]�̔{��
	initialize(684 * bufferTime / 200);
}


/// �f�X�g���N�^
UrgRingBuffer::~UrgRingBuffer()
{
	deleteUrgRingBuffer();
}

void UrgRingBuffer::initialize(int longSize)
{
	if (longSize <= 0)
		cerr << "Error: [UrgRingBuffer::UrgRingBuffer] Argument is invalid\n" << endl;

	/// �������̈�����
	deleteUrgRingBuffer();

	/// �e�ʂ�����
	bufferlongSize = longSize;

	/// �x�N�g���I�u�W�F�N�g�̃������̈�m��
    newUrgRingBuffer();
}

/**
 *	����
 *		�o�b�t�@�̃N���A
 */
void UrgRingBuffer::clear(void)
{
	readIndex = 0;
	writeIndex = 0;
}

/**
 *	����
 *		�����O�o�b�t�@�̋L�^�T�C�Y���擾
 *	�ԋp�l
 *		�L�^���Ă���long�̃T�C�Y
 */
int UrgRingBuffer::getReadableSize(void) const
{
	return ((writeIndex >= readIndex) ? (writeIndex - readIndex) : (bufferlongSize - (readIndex - writeIndex)));
}

/**
 *	����
 *		�����O�o�b�t�@�̋󂫂�long�T�C�Y���擾
 *	�ԋp�l
 *		�󂫃o�b�t�@��long�T�C�Y
 */
int UrgRingBuffer::getWritableSize(void) const
{
	return (bufferlongSize - getReadableSize() - 1);
}



/**
 *	----------------------------------------
 *	�������݊֐�
 *	----------------------------------------
 */

/**
 *	����
 *		�����O�o�b�t�@��long���i�[����
 *	����
 *		dst: �������ރf�[�^�ւ̃|�C���^
 *		size: �������ރT�C�Y
 *	�ԋp�l
 *		���ۂɏ������񂾃T�C�Y
 */
int UrgRingBuffer::write(long* dst, int size)
{
	int i;

	/// �������݃T�C�Y�̌���
	int writableSize = getWritableSize();
	int sizeToWrite = (size > writableSize) ? writableSize : size;

	/// �������ݏI���C���f�b�N�X�̌v�Z
	int endIndex = (sizeToWrite > (bufferlongSize - writeIndex)) ? bufferlongSize : writeIndex + sizeToWrite;

	long* p = &buffer[writeIndex];
	long* q = dst;
	
	for (i=writeIndex;i<endIndex;++i)
		*p++ = *q++;
	
	/// �Ō�̏������݃A�h���X�̌v�Z
	long* data_last = dst + sizeToWrite;
	
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

	readIndex = writeIndex;
	writeIndex  = ((writeIndex + sizeToWrite) >= bufferlongSize ) ? (writeIndex + sizeToWrite) - bufferlongSize : writeIndex + sizeToWrite;


	return sizeToWrite;
}

/**
 *	����
 *		�����O�o�b�t�@����long��ǂݍ���
 *	����
 *		dst: �ǂݍ��ݐ�ւ̃|�C���^
 *		size: �ǂݍ��ރT�C�Y
 *	�ԋp�l
 *		�ǂݍ��񂾃T�C�Y 
 */
int UrgRingBuffer::read(long* dst, int size)
{
	int i;

	/// �ǂݍ��݃T�C�Y�̌���
	int readableSize = getReadableSize();
	int sizeToRead = (size > readableSize) ? readableSize : size;

	/// �ǂݍ��݃C���f�b�N�X�̌v�Z
	int endIndex = (sizeToRead > (bufferlongSize - readIndex)) ? bufferlongSize : readIndex + sizeToRead;

	long* p = dst;
	long* q = &buffer[readIndex];

	for (i=readIndex;i<endIndex;++i)
		*p++ = *q++;

	long* data_last = dst + sizeToRead;
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
 *	����
 *		�o�b�t�@�̉ߋ��̋L�^�f�[�^��ǂݍ���
 *	����
 *		dst: �R�s�[��ւ̃|�C���^
 *		size: �R�s�[����T�C�Y
 *		old_time: �k�鎞�� [ms]
 *	�ԋp�l
 *		�ǂݍ��񂾃T�C�Y  
 */
int UrgRingBuffer::old_read(long* dst, int size, int old_time)
{
	/// �k�鎞�Ԃ�URG�X���b�h�̎��s�����̔{���łȂ��ꍇ	�܂��͒��߂Ă���o�b�t�@�ȑO�̒l���擾���悤�Ƃ����ꍇ
	if (old_time % 200 != 0 || old_time >= 200 * bufferTime / 200)	//URG�X���b�h�̎��s������200[ms]	���߂Ă���o�b�t�@��10��[2000ms]
	{
		cerr << "Error: [UrgRingBuffer::old_read] old_time is not correct\n" << endl;
		return 0;
    }

	int i;

	old_readIndex = ((readIndex - old_time/200 * 684) > 0) ? readIndex - old_time/200 * 684 : bufferlongSize + (readIndex - old_time/200 * 684);

	/// �ǂݍ��݃T�C�Y�̌���

	int sizeToRead = size;


	/// �ǂݍ��݃C���f�b�N�X�̌v�Z
	int endIndex = (sizeToRead > (bufferlongSize - old_readIndex)) ? bufferlongSize : old_readIndex + sizeToRead;

	long* p = dst;
	long* q = &buffer[old_readIndex];

	for (i=old_readIndex;i<endIndex;++i)
		*p++ = *q++;

	long* data_last = dst + sizeToRead;
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
 *		UrgRingBuffer�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�����O�o�b�t�@�̃������̈�m��
 */
void UrgRingBuffer::newUrgRingBuffer(void)
{
	/// �e�ʂ��O�ȉ��̏ꍇ
	if (bufferlongSize <= 0)
	{
		cerr << "Error: [UrgRingBuffer::newUrgRingBuffer] Memory is not allocated\n" << endl;
        abort();
    }

	/// �������[�̈���m�ۂ���
	buffer = new long[bufferlongSize];

	readIndex  = 0;
	writeIndex  = 0;
}

/**
 *	����
 *		�����O�o�b�t�@�̃������̈���
 */
void UrgRingBuffer::deleteUrgRingBuffer(void)
{
	///�������[�̈���J������
    if (buffer != NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}
}