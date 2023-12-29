/**
 *  File: SafeOperation.h
 *
 *  Description: 安全な処理
 *
 *  Created: 2008/03/10(Mon)
 *  Updated: 2008/03/10(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */
 
#ifndef __SafeOperation_h__
#define __SafeOperation_h__
 
/**
 *	安全な削除
 *	safeDeleteItem 削除対象ポインタ
 */

#define SAFE_DELETE(safeDeleteItem){\
	if ( (safeDeleteItem) != NULL )	{\
		delete (safeDeleteItem);\
		(safeDeleteItem) = NULL;\
	}\
}

/**
 *	安全な配列削除
 *	safeDeleteItem 削除対象ポインタ
 */
#define SAFE_DELETE_ARRAY(safeDeleteItem) {\
	if ( (safeDeleteItem) != NULL ) {\
		delete[] (safeDeleteItem);\
		(safeDeleteItem) = NULL;\
	}\
}

/**
 *	安全な解放
 *	safeReleaseItem 削除対象ポインタ
 */
#define SAFE_RELEASE(safeReleaseItem) {\
	if ( (safeReleaseItem) != NULL ) {\
		(safeReleaseItem)->Release();\
		(safeReleaseItem) = NULL;\
	}\
}
 
#endif	/// __SafeOperation_h__
