/**
 *  �t�@�C����
 *		UrgRingBuffer.h
 *  ����
 *		�����O�o�b�t�@�ɂ�鏈��
 *  ���t
 *		�쐬��: 2014/06/20(FRI)		�X�V��:
 */

#ifndef __UrgRingBuffer_h__
#define __UrgRingBuffer_h__

/**
 *	----------------------------------------------------------------------
 *		UrgRingBuffer�N���X
 *	----------------------------------------------------------------------
 */
class UrgRingBuffer
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
	int bufferlongSize;

	/// �o�b�t�@�e��
	//int bufferSize;

	/// �v�f�|�C���^
	long* buffer;

	/// �L���[�擪
	int readIndex;
	/// �f�[�^�������݈ʒu
	int writeIndex;

	/// �ߋ��̃f�[�^�ǂݍ��݈ʒu
	int old_readIndex;

	/// �ߋ��̑����f�[�^�̕ۑ�����[ms]
	int bufferTime;


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
 	UrgRingBuffer();
	/// �f�X�g���N�^
	virtual ~UrgRingBuffer();

/**
 *	����
 *		������
 *	����
 *		bitSize: �o�b�t�@��long�T�C�Y
 */
	void initialize(int longSize);

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
 *		�����O�o�b�t�@�̋󂫂�long�T�C�Y���擾
 *	�ԋp�l
 *		�󂫃o�b�t�@��long�T�C�Y
 */
	int getWritableSize(void) const;



/**
 *	----------------------------------------
 *	�ǂݏ����֐�
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
	int write(long* dst, int size);

	/**
	 *	����
	 *		�����O�o�b�t�@����long��ǂݍ���
	 *	����
	 *		dst: �ǂݍ��ݐ�ւ̃|�C���^
	 *		size: �ǂݍ��ރT�C�Y
	 *	�ԋp�l
	 *		�ǂݍ��񂾃T�C�Y 
	 */
	int read(long* dst, int size);

	/**
	*	����
	*		�o�b�t�@�̋L�^�f�[�^���R�s�[
	*	����
	*		dst: �R�s�[��ւ̃|�C���^
	*		size: �R�s�[����T�C�Y
	*		old_time: �k�鎞��
	*/
	int UrgRingBuffer::old_read(long* dst, int size, int old_time);
	
protected:
private:

	/// �R�s�[�R���X�g���N�^�̋֎~
	UrgRingBuffer(const UrgRingBuffer& copy);

	/// ����R�s�[�̋֎~
	UrgRingBuffer& operator =(const UrgRingBuffer& copy);

/**
 *	�I�u�W�F�N�g����
 */
	/// �������̊m��
	void newUrgRingBuffer(void);

	/// �������̔j��
	void deleteUrgRingBuffer(void);

};

#endif /// __UrgRingBuffer_h__