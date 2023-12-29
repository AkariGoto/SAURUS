/**
 *  �t�@�C����
 *		RingBuffer.h
 *  ����
 *		�����O�o�b�t�@�ɂ�鏈��
 *  ���t
 *		�쐬��: 2007/12/10(MON)		�X�V��: 2007/12/10(MON)
 */

#ifndef __RingBuffer_h__
#define __RingBuffer_h__

/**
 *	----------------------------------------------------------------------
 *		RingBuffer�N���X
 *	----------------------------------------------------------------------
 */
class RingBuffer
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:

	static const int MAX_BUFFER_SIZE = 4096;
	static const int DEFAULT_BUFFER_SIZE = 1024;
	
protected:
private:
	/// �o�b�t�@�e��
	int capacity;

	/// �v�f�|�C���^
	unsigned char* buffer;

	/// �f�[�^�擪
	int head;
	/// �f�[�^��
	int count;


/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
 	/// �f�t�H���g�R���X�g���N�^
 	RingBuffer();
	/// �R���X�g���N�^
	explicit RingBuffer(int size);
	/// �f�X�g���N�^
	virtual ~RingBuffer();

/**
 *		�o�b�t�@�̗e�ʂ�����
 */
	void setSize(int size);

/**
 *	----------------------------------------
 *	�ǂݏ����֐�
 *	----------------------------------------
 */
	/**
	 *	����
	 *		1�o�C�g�f�[�^��������
	 *	����
	 *		source:�������ރf�[�^
	 *	�߂�l
	 *		source�F����
	 *		-1�F���s�i�I�[�o�[�t���[�j
	 */
	int write(unsigned char source);
	/**
	 *	����
	 *		�����o�C�g�f�[�^��������
	 *	����
	 *		source:�������ރf�[�^�̃A�h���X
	 *	�߂�l
	 *		1�F����
	 *		-1�F���s�i�I�[�o�[�t���[�j
	 */
	int write(unsigned char* source, int size);

	/**
	 *	����
	 *		1�o�C�g�f�[�^�ǂݍ���
	 *	����
	 *		target: �ǂݍ��ݕۑ��p�̃o�b�t�@
	 *	�߂�l
	 *		�����F1
	 *		���s�F-1�i�f�[�^���j
	 */
	int read(unsigned char* target);
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
	int read(unsigned char* target, int size);

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/// ���݂̏������݈ʒu�𓾂�
	int getWriteIndex(void) const {return head + count;}
	/// ���݂̓ǂݍ��݈ʒu�𓾂�
	int getReadIndex(void) const {return head;}

	/// ���ݏ������߂�T�C�Y�𓾂�
	int getWritableSize(void);
	/// ���ݓǂݍ��߂�T�C�Y�𓾂�
	int getReadableSize(void);

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */

protected:
private:

	/// �R�s�[�R���X�g���N�^�̋֎~
	RingBuffer(const RingBuffer& copy);

	/// ����R�s�[�̋֎~
	RingBuffer& operator =(const RingBuffer& copy);

/**
 *	�I�u�W�F�N�g����
 */
	/// �������̊m��
	void newRingBuffer(void);

	/// �������̔j��
	void deleteRingBuffer(void);

};

#endif /// __RingBuffer_h__