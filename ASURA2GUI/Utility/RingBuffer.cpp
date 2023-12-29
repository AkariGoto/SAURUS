/**
 *  �t�@�C����
 *		RingBuffer.cpp
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
#include "RingBuffer.h"
#include "iostream"


using namespace std;

/**
 *	----------------------------------------------------------------------
 *		RingBuffer�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		RingBuffer�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
RingBuffer::RingBuffer()
{
	/// �e�ʂ̓f�t�H���g
	capacity = DEFAULT_BUFFER_SIZE;

	/// �C���f�b�N�X��0
	head = 0;
	count = 0;

	/// �������m��
	newRingBuffer();
}

RingBuffer::RingBuffer(int size)
{
	if ( size < 0 )
	{
		cerr << "Error: [RingBuffer::RingBuffer] Argument is invalid\n" << endl;
	}

	/// �e�ʂ�����
	capacity = size;

	/// �C���f�b�N�X��0
	head = 0;
	count = 0;

	/// �������m��
	newRingBuffer();
}

/// �f�X�g���N�^
RingBuffer::~RingBuffer()
{
	deleteRingBuffer();
}

void RingBuffer::setSize(int size)
{
	/// �������̈�����
	deleteRingBuffer();

	capacity = size;
	/// �x�N�g���I�u�W�F�N�g�̃������̈�m��
    newRingBuffer();
}

/**
 *	----------------------------------------
 *	�������݊֐�
 *	----------------------------------------
 */
/**
 *	����
 *		1�o�C�g�f�[�^��������
 *	����
 *		source:�������ރf�[�^
 *	�߂�l
 *		�����F1
 *		���s�F-1�i�I�[�o�[�t���[�j
 */
int RingBuffer::write(unsigned char source)
{
	int ret;

	/// �c�ʃ`�F�b�N
	if ( count < capacity )
	{
		/// �o�b�t�@�ɒl���i�[
	    buffer[( head + count )  % capacity] = source;
		/// �f�[�^�ʂ��X�V
		count++;
		ret = 1;		/// ����I��
	}
	else
	{	
		ret = -1;	/// �f�[�^����t�Ȃ̂ŃI�[�o�[�t���[
	}

	return ret;
}

/**
 *	����
 *		�����o�C�g�f�[�^��������
 *	����
 *		source:�������ރf�[�^�̃A�h���X
 *	�߂�l
 *		1�F����
 *		-1�F���s�i�I�[�o�[�t���[�j
 */
int RingBuffer::write(unsigned char* source, int size)
{
	/// �߂�l
	int ret = -1;

	int i;
	for (i=0; i<size; i++)
	{
		/// 1�f�[�^���i�[
		ret = write(source[i]);
		
		/// �I�[�o�[�t���[
		if (ret == -1)
		{
			cerr << "Error: [RingBuffer::write] Buffer Overflow\n" << endl;
			break;
		}

	}

	if ( i == size -1) ret = 1;	/// ����I��

	return ret;
}

/**
 *	----------------------------------------
 *	�ǂݍ��݊֐�
 *	----------------------------------------
 */
/**
 *	����
 *		1�o�C�g�f�[�^�ǂݍ���
 *	����
 *		target: �ǂݍ��ݕۑ��p�̃o�b�t�@
 *	�߂�l
 *		�����F1
 *		���s�F-1�i�f�[�^���j
 */
int RingBuffer::read(unsigned char* target)
{
	char ret;

	/// �f�[�^���c���Ă���ꍇ
	if ( count > 0 )
	{
		/// �ǂݏo���l���i�[
		*target = buffer[head];
		
		/// �f�[�^�擪�ʒu�̍X�V
		head = ( head + 1 ) % capacity;

		/// �f�[�^�c�ʂ̍X�V
		count--;

		/// ����I��
		ret = 1;
	}
	else	/// �f�[�^�c�ʖ�
	{
		ret = -1;
	}
    
	return ret;
}

/**
 *	����
 *		�����o�C�g�f�[�^�ǂݍ���
 *	����
 *		target: �ǂݍ��ݕۑ��p�̃o�b�t�@
 *		size: target�̃o�C�g�T�C�Y
 *	�߂�l
 *		�����F1
 *		���s�F-1�i�f�[�^���j
 */
int RingBuffer::read(unsigned char* target, int size)
{
	/// �߂�l
	char ret;

	int i;
	for (i=0; i<size; i++)
	{
		/// 1�f�[�^���i�[
		ret = read(&target[i]);
		
		/// �f�[�^����
		if (ret == -1)
		{
			cerr << "Error: [RingBuffer::write] No Buffer \n" << endl;
			break;
		}

	}

	if ( i == size -1) ret = 1;	/// ����I��

	return ret;
}

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
/// ���ݏ������߂�T�C�Y�𓾂�
int RingBuffer::getWritableSize(void)
{
	return ( capacity - count );
}

/// ���ݓǂݍ��߂�T�C�Y�𓾂�
int RingBuffer::getReadableSize(void)
{
	return count;
}


/**
 *	------------------------------------------------------------
 *		RingBuffer�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�����O�o�b�t�@�̃������̈�m��
 */
void RingBuffer::newRingBuffer(void)
{
	/// �e�ʂ��O�ȉ��̏ꍇ
	if (capacity <= 0)
	{
		cerr << "Error: [RingBuffer::newRingBuffer] Memory is not allocated\n" << endl;
        abort();
    }

	/// �������[�̈���m�ۂ���
	buffer = new unsigned char[capacity];
}

/**
 *	����
 *		�����O�o�b�t�@�̃������̈���
 */
void RingBuffer::deleteRingBuffer(void)
{
	///�������[�̈���J������
    delete [] buffer;

	buffer = NULL;
}