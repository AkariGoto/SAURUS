/**
 *  �t�@�C����
 *		RingBuffer.c
 *  ����
 *		�����O�o�b�t�@�Ɋւ���錾��
 *  ���t
 *		�쐬��: 2008/06/25		�X�V��: 2008/06/25
 */
/**
 *	//////////////////////////////////////////////////
 *		�w�b�_�t�@�C���C���N���[�h
 *	//////////////////////////////////////////////////
 */

/// C�W�����C�u����
#include <stdlib.h>

/// ���������C�u����
#include "RingBuffer.h"

/**
 *	//////////////////////////////////////////////////
 *		�֐��̒�`
 *	//////////////////////////////////////////////////
 */

/**
 *	�����O�o�b�t�@�̏�����
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		bitSize: �o�b�t�@�̃r�b�g�T�C�Y
 */ 
int ring_initialize(RingBuffer* ring, int bitSize)
{
	// RX�����O�o�b�t�@
	ring->bitSize	= bitSize;
	ring->buffSize	= 1 << bitSize;
	ring->read		= 0;	
	ring->write		= 0;
	
	if ( !ring->isInitialized )
	{
		ring->buffer = (unsigned char*)malloc(sizeof(unsigned char) * ring->buffSize);

		if (ring->buffer != 0)
			ring->isInitialized = 1;
		else
			return -1;	/// ���s
	}
	
	return 1;
}

/**
 *	�����O�o�b�t�@�̃N���A
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 */
void ring_clear(RingBuffer* ring)
{
	ring->read = 0;
	ring->write = 0;
}
 
/**
 *	�����O�o�b�t�@�̋󂫂̃o�b�t�@�T�C�Y���擾
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *	�ԋp�l
 *		-1: �󂫂Ȃ�
 *		���̑�: �󂫃o�b�t�@�T�C�Y
 */
int ring_getEmptySize(RingBuffer* ring)
{
	int saveSize = ring->write - ring->read;
	
	return (ring->buffSize - (saveSize + ((saveSize < 0) ? ring->buffSize : 0))) -1;
}

/**
 *	�����O�o�b�t�@�̋L���T�C�Y���擾
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *	�ԋp�l
 *		�L���T�C�Y
 */
int ring_getSaveSize(RingBuffer* ring)
{
	return ring->buffSize - ring_getEmptySize(ring) - 1;
}

/**
 *	�����O�o�b�t�@��1�o�C�g�i�[
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		buffer: �������ޒl
 *	�ԋp�l
 *		-1: ���s
 *		0: ����
 */
int ring_putBuffer(RingBuffer* ring, unsigned char buffer)
{
	/// �����O�o�b�t�@���󂩂ǂ���
	if (ring_getEmptySize(ring) <= 0)
	{
		/// �󂫗̈斳��
		return -1;
	}
	
	/// �o�b�t�@���i�[
	ring->buffer[ring->write] = buffer;
	/// �������݈ʒu���X�V
	ring->write = (ring->write + 1) & (ring->buffSize - 1);

	return 0;
}

/**
 *	�����O�o�b�t�@�֕����o�C�g�i�[����
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		data: �������ރf�[�^�ւ̃|�C���^
 *		size: �������ރT�C�Y
 *	�ԋp�l
 *		���ۂɏ������񂾃T�C�Y
 */
int ring_writeBuffer(RingBuffer* ring, const unsigned char* data, int size)
{
	int i;

	/// 1�����i�[���w��񐔍s��
	for (i=0;i<size;++i)
	{
		if (ring_putBuffer(ring, data[i]) < 0)
		{
			break;
		}
	}
	
	return i;
}

/**
 *	�����O�o�b�t�@����1�o�C�g�擾
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		buffer: �ǂݍ��ݐ�ւ̃|�C���^
 *	�ԋp�l
 *		-1: �f�[�^����
 *		0: �ǂݍ��ݐ���
 */
int ring_getBuffer(RingBuffer* ring, unsigned char* buffer)
{
	/// �f�[�^�����邩�ǂ���
	if (ring->read == ring->write)
	{
		return -1;
	}

	*buffer = ring->buffer[ring->read];
	ring->read = (ring->read + 1) & (ring->buffSize - 1);
	
	return 0;
}

/**
 *	�����O�o�b�t�@���畡���o�C�g�ǂݍ���
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		data: �ǂݍ��ݐ�ւ̃|�C���^
 *		size: �ǂݍ��ރT�C�Y
 *	�ԋp�l
 *		�ǂݍ��񂾃T�C�Y
 */
int ring_readBuffer(RingBuffer* ring, unsigned char* data, int size)
{

	int i;
	
	for (i=0;i<size;++i)
	{
		if (ring_getBuffer(ring, (unsigned char*)&data[i]) < 0)
		{
			break;
		}
	}
	
	return i;
}

/**
 *	�����O�o�b�t�@�̋L���T�C�Y���擾
 *	����
 *		dst: �R�s�[��ւ̃|�C���^
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		size: �R�s�[����T�C�Y
 */
void ring_copyBuffer(unsigned char* dst, RingBuffer* ring, int size)
{
	unsigned char *p = dst;
	int index;
	int i;

	index = ring->read;
	for (i=0;i<size;++i)
	{
		*p++ = ring->buffer[index++];
		index &= (ring->buffSize -1);
	}
	
}
