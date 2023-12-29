/**
 *  �t�@�C����
 *		UdpRingBuffer.h
 *  ����
 *		�����O�o�b�t�@�ɂ�鏈��
 *  ���t
 *		�쐬��: 2020/08/19
 */

#ifndef __UdpRingBuffer_h__
#define __UdpRingBuffer_h__

/**
 *	----------------------------------------------------------------------
 *		UdpRingBuffer�N���X
 *	----------------------------------------------------------------------
 */
class UdpRingBuffer
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
	static const int DEFAULT_BIT_SIZE = 10;
	
protected:
private:
	/// �o�b�t�@�r�b�g�T�C�Y
	int bufferBitSize;

	/// �o�b�t�@�e��
	int bufferSize;

	/// �v�f�|�C���^
	unsigned char* buffer;

	/// �L���[�擪
	int readIndex;
	/// �f�[�^�������݈ʒu
	int writeIndex;


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
 	UdpRingBuffer();
	/// �R���X�g���N�^
	explicit UdpRingBuffer(int bitSize);
	/// �f�X�g���N�^
	virtual ~UdpRingBuffer();

/**
 *	����
 *		������
 *	����
 *		bitSize: �o�b�t�@�̃r�b�g�T�C�Y
 */
	void initialize(int bitSize);

/**
 *	����
 *		�o�b�t�@�̃N���A
 */
	void clear(void);
	
/**
 *	����
 *		�����O�o�b�t�@�̋L�^�T�C�Y���擾
 *	�ԋp�l
 *		�L�^���Ă���o�C�g�T�C�Y
 */
	int getReadableSize(void) const;
	
/**
 *	����
 *		�����O�o�b�t�@�̋󂫂̃o�b�t�@�T�C�Y���擾
 *	�ԋp�l
 *		�󂫃o�b�t�@�̃o�C�g�T�C�Y
 */
	int getWritableSize(void) const;

/**
 *	----------------------------------------
 *	�ǂݏ����֐�
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
	int put(unsigned char buff);

	/**
	 *	����
	 *		�����O�o�b�t�@�֕����o�C�g�i�[����
	 *	����
	 *		dst: �������ރf�[�^�ւ̃|�C���^
	 *		size: �������ރT�C�Y
	 *	�ԋp�l
	 *		���ۂɏ������񂾃T�C�Y
	 */
	int write(unsigned char* dst, int size);

	/**
	 *	����
	 *		�����O�o�b�t�@����1�o�C�g�擾
	 *	����
	 *		buff: �ǂݍ��ݐ�ւ̃|�C���^
	 *	�ԋp�l
	 *		-1: �f�[�^����
	 *		0: �ǂݍ��ݐ��� 
	 */
	int get(unsigned char* buff);

	/**
	 *	����
	 *		�����O�o�b�t�@���畡���o�C�g�ǂݍ���
	 *	����
	 *		dst: �ǂݍ��ݐ�ւ̃|�C���^
	 *		size: �ǂݍ��ރT�C�Y
	 *	�ԋp�l
	 *		�ǂݍ��񂾃T�C�Y 
	 */
	int read(unsigned char* dst, int size);

	/**
	*	����
	*		�o�b�t�@�̋L�^�f�[�^���R�s�[
	*	����
	*		dst: �R�s�[��ւ̃|�C���^
	*		size: �R�s�[����T�C�Y
	*/
	int copy(unsigned char* dst, int size);
	
protected:
private:

	/// �R�s�[�R���X�g���N�^�̋֎~
	UdpRingBuffer(const UdpRingBuffer& copy);

	/// ����R�s�[�̋֎~
	UdpRingBuffer& operator =(const UdpRingBuffer& copy);

/**
 *	�I�u�W�F�N�g����
 */
	/// �������̊m��
	void newUdpRingBuffer(void);

	/// �������̔j��
	void deleteUdpRingBuffer(void);

};

#endif /// __UdpRingBuffer_h__