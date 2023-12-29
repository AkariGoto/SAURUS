/**
 *  �t�@�C����
 *		ArrayList.h
 *  ����
 *		�z�񃊃X�g�N���X(�p�����Ȃ�����)
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)
 */

#ifndef	__ArrayList_h__
#define	__ArrayList_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <cassert>

template <typename Type>

class ArrayList
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
/**
 *		�f�t�H���g�L���p�V�e�B
 *		2�̗ݏ������
 */
	static const int DEFAULT_CAPACITY = 16;

private:

	Type*		typeArray;		/// �z��{��
	int			count;			/// �z��̐�
	int			capacity;			/// �L���p�V�e�B

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
	ArrayList()
	{
		capacity		= DEFAULT_CAPACITY;
		typeArray	= new Type[capacity];
		count			= 0;
	}
 
	explicit ArrayList(int capacity_)
	{
		assert( 0 < capacity_ );

		capacity		= capacity_;
		typeArray	= new Type[capacity];
		count			= 0;
	}

	~ArrayList()
	{
		delete [] typeArray;
	}

	/**
	 *		�N���[��
	 *		����
	 *			destination: �N���[����̃A���C���X�g
	 *
	 */
	void clone(ArrayList& destination) const
	{
		/// �����̔z��v�f������
		delete [] destination.typeArray;

		/// ���e�ʂ�ݒ�
		destination.capacity = capacity;
		
		/// �v�f���Ċm��
		destination.typeArray = new Type[capacity];
		
		/// �v�f�������R�s�[
		/// ������N���X���𐳂����R�s�[����ɂ�memcpy�ł͑ʖ�
		int i;
		for (i=0; i < count; i++){ destination.typeArray[i] = typeArray[i]; }
		
		destination.count = count;
	}

	/**
	 *		�v�f���𓾂�
	 */
	int getCount() const{ return count; }

	/**
	 *	�v�f���󂩂ǂ���
	 *	�߂�l: 
	 *		�󂾂�����true
	 */
	bool isEmpty() const{ return (count == 0); }
 
	/**
	 *	�v�f�̎擾
	 *	����:
	 *		index �擾����v�f�̃C���f�b�N�X
	 *	�߂�l:
	 *		�v�f
	 */
	Type& get(int index) const
	{
		assert( 0 <= index );
		assert( index < count );
		
		return typeArray[index];
	}

	/**
	 *	�v�f�̎擾
	 *	����:
	 *		index �擾����v�f�̃C���f�b�N�X
	 *	�߂�l:
	 *		�v�f
	 */
	Type& operator [](int index) const
	{
		assert( 0 <= index );
		assert( index < count );

		return typeArray[index];
	}

	/**
	 *	�L���p�V�e�B�̎擾
	 */
	int getCapacity() const{ return capacity; }
 
	/**
	 *	�l�̌���
	 *	�A���C���X�g�̑O����l���������C�����ł���΂��̃C���f�b�N�X��Ԃ�
	 *	����
	 *		searchValue ��������l
	 *	�߂�l
	 *		�l�̃C���f�b�N�X�D�l���������-1��Ԃ�
	 */
	int getFirstIndexOf(const Type& searchValue) const
	{
		int i;
		for (i=0; i<count; i++)
		{
			if (array[i] == searchValue){ return i; }
		}
	
		return -1;
	}

	/**
	 *	�l�̌���
	 *	�A���C���X�g�̑O����l���������C�����ł���΂��̃C���f�b�N�X��Ԃ�
	 *	����
	 *		searchValue ��������l
	 *	�߂�l
	 *		�l�̃C���f�b�N�X�D�l���������-1��Ԃ��D
	 */
    int getLastIndexOf(const Type& searchValue) const
	{
		int i;
		for ( i=(count - 1); i >= 0; i-- )
		{
			 if (typeArray[i] == searchValue){ return i; }
		}

		return -1;
	}

	/**
	 *	�A���C���X�g�̕ύX
	 *	�v�f�̒ǉ�
	 *	����
	 *		svalue �v�f
	 */	
	void add(const Type& value)
	{
		/// �v�f���S�Ė��܂��Ă����狖�e�ʕύX
		if ( (count+1) > capacity )
		{
			 resize(capacity*2);
		}

		typeArray[count] = value;
		count++;
	}
 
	/**
	 *	�v�f�̐ݒ�
	 *	����
	 *		index: �v�f��ݒ肷��C���f�b�N�X
	 *		value: �v�f
	 *	�߂�l
	 *		�v�f
	 */
	void set(int index, const Type& value) const
	{
		assert( 0 <= index );
		assert( index < count );

		typeArray[index] = value;
	}

	/**
	 *		�v�f�̍폜
	 *			����
	 *				index: �폜����v�f�̃C���f�b�N�X
	 *			�߂�l
	 *				�A���C���X�g����폜�����v�f
	 */
	Type remove(int index)
	{
		assert( 0 <= index );
		assert( index < count );
		assert( 0 < count );

		Type result = typeArray[index];
		count--;
         
		 for (int i=index; i<count; i++)
		 {
			 typeArray[i] = typeArray[i+1];
		 }
		 
		 return result;
	 }

	/**
	 *		�l�ɂ��v�f�̍폜
	 *		�A���C���X�g�̌�납��폜����l���������C�����v�f������΍폜
	 *			����
	 *				removeValue: �폜����v�f�̒l
	 *			�߂�l
	 *				�폜�����C���f�b�N�X�D
	 *				-1: �Y������v�f�����D
	 */
	int removeByValue(const Type& removeValue)
	 {
		 for (int i=(count-1); i>=0; i--)
		 {
			 if ( typeArray[i] == removeValue )
			 {
				 remove(i);
				 return i;
			 }
		 }

		 return -1;
	 }

	/**
	 *		�S�v�f���폜
	 */
	void clear(){ count = 0; }
 
	/**
	 *		�S�v�f���폜
	 *			����
	 *				capacity: �N���A��̃L���p�V�e�B
	 */
	void clear(int capacity_)
	{
		assert( 0 <= capacity_);

		// ���e�ʂ̍Đݒ�
		if (capacity_<= 0) { capacity_=1; }

		// �v�f�̍Đݒ�
		delete [] typeArray;
		capacity = capacity_;
		
		typeArray = new Type[capacity];
		count = 0;
	 }

	/**
	 *		�L���p�V�e�B�̐ݒ�
	 *		����
	 *			newCapacity �V�����L���p�V�e�B
	 */
	void setCapacity(int newCapacity)
	{
		assert( count <= newCapacity );
		
		resize(newCapacity);

		return;
	}

	/**
	 *		�g����
	 *		���݂̃T�C�Y�ɍ��킹�Ďg�p���������ŏ��ɂ���
	 */
	void trim()
	 {
		 if (count == 0){ resize(1);}
		 else { resize(count); }
	 }
 
	/**
	 *		�\�[�g
	 *		�N�C�b�N�\�[�g�ŃA���C���X�g���\�[�g
	 *		compare�̕Ԃ�l���ȉ��̂悤�ɂ���Ə����Ƀ\�[�g�����
	 *			�����������������傫���Ƃ���1�ȏ�<br>
	 *			�������Ƒ�Ɉ����������ꍇ��0<br>
	 *			����������������菬�����Ƃ���-1�ȉ�
	 *			����
	 *				compare ��r�֐�
	 */
	void sort( int(*compare)(const Type*, const Type*) )
	 {
		 qsort(typeArray, count, sizeof(Type), (int(*)(const void*, const void*))compare);
		 return;
	 }

	/**
	 *		�T�[�`
	 *		�o�C�i���T�[�`�ŃA���C���X�g������
	 *		�A���C���X�g�͏����Ƀ\�[�g����Ă���K�v������
	 *		�v�f��������Ȃ������ꍇ��NULL��Ԃ�
	 *		compare�͈ȉ��̂悤�ȕԂ�l��Ԃ�
	 *			�����������������傫���Ƃ���1�ȏ�
	 *			�������Ƒ�Ɉ����������ꍇ��0
	 *			����������������菬�����Ƃ���-1�ȉ�
	 *		����
	 *			key: ��������l
	 *			compare: ��r�֐�
	 *		�Ԃ�l
	 *			�������ʁA������Ȃ����NULL
	 */
	Type* search(Type key, int(*compare)(const Type*, const Type*) )
	{
		 return (Type*)bsearch(&key, typeArray, count, sizeof(Type),
								(int(*)(const void*, const void*))compare);
    }

private:

	/**
	 *		���T�C�Y
	 *		�z��̋��e�ʂ�ς���
	 */
	void resize(int newCapacity)
	{
		assert(0 < newCapacity);
		assert(count < newCapacity);

		Type* newArray = new Type[newCapacity];
		
		// ������N���X���𐳂����R�s�[����ɂ�memcpy�ł͑ʖ�
		int i;
		for (i=0; i<count; i++){ newArray[i] = typeArray[i]; }

		delete[] typeArray;
		typeArray = newArray;
		capacity = newCapacity;
	}
 
     // �R�s�[�R���X�g���N�^�̉B��
	ArrayList(const ArrayList& copy);
 
     // ����R�s�[�̉B��
	void operator =(const ArrayList& copy);
};

#endif	// __ArrayList_h__
