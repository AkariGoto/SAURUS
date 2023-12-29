/**
 *  ファイル名
 *		ArrayList.h
 *  説明
 *		配列リストクラス(継承しないこと)
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

#ifndef	__ArrayList_h__
#define	__ArrayList_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <cassert>

template <typename Type>

class ArrayList
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
/**
 *		デフォルトキャパシティ
 *		2の累乗を示す
 */
	static const int DEFAULT_CAPACITY = 16;

private:

	Type*		typeArray;		/// 配列本体
	int			count;			/// 配列の数
	int			capacity;			/// キャパシティ

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
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
	 *		クローン
	 *		引数
	 *			destination: クローン先のアレイリスト
	 *
	 */
	void clone(ArrayList& destination) const
	{
		/// 引数の配列要素を消去
		delete [] destination.typeArray;

		/// 許容量を設定
		destination.capacity = capacity;
		
		/// 要素を再確保
		destination.typeArray = new Type[capacity];
		
		/// 要素数だけコピー
		/// 文字列クラス等を正しくコピーするにはmemcpyでは駄目
		int i;
		for (i=0; i < count; i++){ destination.typeArray[i] = typeArray[i]; }
		
		destination.count = count;
	}

	/**
	 *		要素数を得る
	 */
	int getCount() const{ return count; }

	/**
	 *	要素が空かどうか
	 *	戻り値: 
	 *		空だったらtrue
	 */
	bool isEmpty() const{ return (count == 0); }
 
	/**
	 *	要素の取得
	 *	引数:
	 *		index 取得する要素のインデックス
	 *	戻り値:
	 *		要素
	 */
	Type& get(int index) const
	{
		assert( 0 <= index );
		assert( index < count );
		
		return typeArray[index];
	}

	/**
	 *	要素の取得
	 *	引数:
	 *		index 取得する要素のインデックス
	 *	戻り値:
	 *		要素
	 */
	Type& operator [](int index) const
	{
		assert( 0 <= index );
		assert( index < count );

		return typeArray[index];
	}

	/**
	 *	キャパシティの取得
	 */
	int getCapacity() const{ return capacity; }
 
	/**
	 *	値の検索
	 *	アレイリストの前から値を検索し，発見できればそのインデックスを返す
	 *	引数
	 *		searchValue 検索する値
	 *	戻り値
	 *		値のインデックス．値が無ければ-1を返す
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
	 *	値の検索
	 *	アレイリストの前から値を検索し，発見できればそのインデックスを返す
	 *	引数
	 *		searchValue 検索する値
	 *	戻り値
	 *		値のインデックス．値が無ければ-1を返す．
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
	 *	アレイリストの変更
	 *	要素の追加
	 *	引数
	 *		svalue 要素
	 */	
	void add(const Type& value)
	{
		/// 要素が全て埋まっていたら許容量変更
		if ( (count+1) > capacity )
		{
			 resize(capacity*2);
		}

		typeArray[count] = value;
		count++;
	}
 
	/**
	 *	要素の設定
	 *	引数
	 *		index: 要素を設定するインデックス
	 *		value: 要素
	 *	戻り値
	 *		要素
	 */
	void set(int index, const Type& value) const
	{
		assert( 0 <= index );
		assert( index < count );

		typeArray[index] = value;
	}

	/**
	 *		要素の削除
	 *			引数
	 *				index: 削除する要素のインデックス
	 *			戻り値
	 *				アレイリストから削除した要素
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
	 *		値による要素の削除
	 *		アレイリストの後ろから削除する値を検索し，同じ要素があれば削除
	 *			引数
	 *				removeValue: 削除する要素の値
	 *			戻り値
	 *				削除したインデックス．
	 *				-1: 該当する要素無し．
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
	 *		全要素を削除
	 */
	void clear(){ count = 0; }
 
	/**
	 *		全要素を削除
	 *			引数
	 *				capacity: クリア後のキャパシティ
	 */
	void clear(int capacity_)
	{
		assert( 0 <= capacity_);

		// 許容量の再設定
		if (capacity_<= 0) { capacity_=1; }

		// 要素の再設定
		delete [] typeArray;
		capacity = capacity_;
		
		typeArray = new Type[capacity];
		count = 0;
	 }

	/**
	 *		キャパシティの設定
	 *		引数
	 *			newCapacity 新しいキャパシティ
	 */
	void setCapacity(int newCapacity)
	{
		assert( count <= newCapacity );
		
		resize(newCapacity);

		return;
	}

	/**
	 *		トリム
	 *		現在のサイズに合わせて使用メモリを最小にする
	 */
	void trim()
	 {
		 if (count == 0){ resize(1);}
		 else { resize(count); }
	 }
 
	/**
	 *		ソート
	 *		クイックソートでアレイリストをソート
	 *		compareの返り値を以下のようにすると昇順にソートされる
	 *			第一引数が第二引数より大きいときは1以上<br>
	 *			第一引数と大に引数が同じ場合は0<br>
	 *			第一引数が第二引数より小さいときは-1以下
	 *			引数
	 *				compare 比較関数
	 */
	void sort( int(*compare)(const Type*, const Type*) )
	 {
		 qsort(typeArray, count, sizeof(Type), (int(*)(const void*, const void*))compare);
		 return;
	 }

	/**
	 *		サーチ
	 *		バイナリサーチでアレイリストを検索
	 *		アレイリストは昇順にソートされている必要がある
	 *		要素が見つからなかった場合はNULLを返す
	 *		compareは以下のような返り値を返す
	 *			第一引数が第二引数より大きいときは1以上
	 *			第一引数と大に引数が同じ場合は0
	 *			第一引数が第二引数より小さいときは-1以下
	 *		引数
	 *			key: 検索する値
	 *			compare: 比較関数
	 *		返り値
	 *			検索結果、見つからなければNULL
	 */
	Type* search(Type key, int(*compare)(const Type*, const Type*) )
	{
		 return (Type*)bsearch(&key, typeArray, count, sizeof(Type),
								(int(*)(const void*, const void*))compare);
    }

private:

	/**
	 *		リサイズ
	 *		配列の許容量を変える
	 */
	void resize(int newCapacity)
	{
		assert(0 < newCapacity);
		assert(count < newCapacity);

		Type* newArray = new Type[newCapacity];
		
		// 文字列クラス等を正しくコピーするにはmemcpyでは駄目
		int i;
		for (i=0; i<count; i++){ newArray[i] = typeArray[i]; }

		delete[] typeArray;
		typeArray = newArray;
		capacity = newCapacity;
	}
 
     // コピーコンストラクタの隠蔽
	ArrayList(const ArrayList& copy);
 
     // 代入コピーの隠蔽
	void operator =(const ArrayList& copy);
};

#endif	// __ArrayList_h__
