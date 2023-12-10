/**
 *  �t�@�C����
 *		RingBuffer.h
 *  ����
 *		�����O�o�b�t�@�Ɋւ���錾��
 *  ���t
 *		�쐬��: 2008/06/25		�X�V��: 2008/06/25
 */

#ifndef __RingBuffer_h__
#define __RingBuffer_h__

/**
 *	//////////////////////////////////////////////////
 *		�^�錾�A�}�N���錾
 *	//////////////////////////////////////////////////
 */
typedef struct
{
	unsigned int	read;			/// �ǂݍ��݈ʒu�i�n�_�j
	unsigned int	write;			/// �������݈ʒu�i�I�_�j
	unsigned int	buffSize;		/// �o�b�t�@�T�C�Y
	unsigned char	bitSize;		/// �o�b�t�@�̃r�b�g�T�C�Y
	unsigned char	isInitialized;	/// ����������
	unsigned char	*buffer;		/// �o�b�t�@�{��

} RingBuffer;
 
/**
 *	//////////////////////////////////////////////////
 *			�֐��̃v���g�^�C�v�錾
 *	//////////////////////////////////////////////////
 */

/**
 *	�����O�o�b�t�@�̏�����
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		bitSize: �o�b�t�@�̃r�b�g�T�C�Y
 */ 
extern int ring_initialize(RingBuffer* ring, int bitSize);

/**
 *	�����O�o�b�t�@�̃N���A
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 */
extern void ring_clear(RingBuffer* ring);

/**
 *	�����O�o�b�t�@�̋󂫂̃o�b�t�@�T�C�Y���擾
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *	�ԋp�l
 *		-1: �󂫂Ȃ�
 *		���̑�: �󂫃o�b�t�@�T�C�Y
 */
extern int ring_getEmptySize(RingBuffer* ring);

/**
 *	�����O�o�b�t�@�̋L���T�C�Y���擾
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *	�ԋp�l
 *		�L���T�C�Y
 */
extern int ring_getSaveSize(RingBuffer* ring);

/**
 *	�����O�o�b�t�@��1�o�C�g�i�[
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		buffer: �������ޒl
 *	�ԋp�l
 *		-1: ���s
 *		0: ����
 */
extern int ring_putBuffer(RingBuffer* ring, unsigned char buffer);

/**
 *	�����O�o�b�t�@�֕����o�C�g�i�[����
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		data: �������ރf�[�^�ւ̃|�C���^
 *		size: �������ރT�C�Y
 *	�ԋp�l
 *		���ۂɏ������񂾃T�C�Y
 */
extern int ring_writeBuffer(RingBuffer* ring, const unsigned char* data, int size);

/**
 *	�����O�o�b�t�@����1�o�C�g�擾
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		buffer: �ǂݍ��ݐ�ւ̃|�C���^
 *	�ԋp�l
 *		-1: �f�[�^����
 *		0: �ǂݍ��ݐ���
 */
extern int ring_getBuffer(RingBuffer* ring, unsigned char* buffer);

/**
 *	�����O�o�b�t�@���畡���o�C�g�ǂݍ���
 *	����
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		data: �ǂݍ��ݐ�ւ̃|�C���^
 *		size: �ǂݍ��ރT�C�Y
 *	�ԋp�l
 *		�ǂݍ��񂾃T�C�Y
 */
extern int ring_readBuffer(RingBuffer* ring, unsigned char* data, int size);

/**
 *	�����O�o�b�t�@�̒��g���R�s�[����i�o�b�t�@�̒��g�͕ύX���Ȃ��j
 *	����
 *		dst: �R�s�[��ւ̃|�C���^
 *		ring: �����O�o�b�t�@�\���̂ւ̃|�C���^
 *		size: �R�s�[����T�C�Y
 */
extern void ring_copyBuffer(unsigned char* dst, RingBuffer* ring, int size);
 
#endif	/// __RingBuffer_h__

