/**
 *  File: SafeOperation.h
 *
 *  Description: ���S�ȏ���
 *
 *  Created: 2008/03/10(Mon)
 *  Updated: 2008/03/10(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */
 
#ifndef __SafeOperation_h__
#define __SafeOperation_h__
 
/**
 *	���S�ȍ폜
 *	safeDeleteItem �폜�Ώۃ|�C���^
 */

#define SAFE_DELETE(safeDeleteItem){\
	if ( (safeDeleteItem) != NULL )	{\
		delete (safeDeleteItem);\
		(safeDeleteItem) = NULL;\
	}\
}

/**
 *	���S�Ȕz��폜
 *	safeDeleteItem �폜�Ώۃ|�C���^
 */
#define SAFE_DELETE_ARRAY(safeDeleteItem) {\
	if ( (safeDeleteItem) != NULL ) {\
		delete[] (safeDeleteItem);\
		(safeDeleteItem) = NULL;\
	}\
}

/**
 *	���S�ȉ��
 *	safeReleaseItem �폜�Ώۃ|�C���^
 */
#define SAFE_RELEASE(safeReleaseItem) {\
	if ( (safeReleaseItem) != NULL ) {\
		(safeReleaseItem)->Release();\
		(safeReleaseItem) = NULL;\
	}\
}
 
#endif	/// __SafeOperation_h__
