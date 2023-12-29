/*****************************************************************************
**
**	File:	Asc2Int.cpp(Ascii To Integer Main)
** 
**	説明:	文字列→数値変換ルーチン
**
**	Created: 2004/10/20(Wed)	Updated: 2004/10/20(Wed)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Asc2int.h"

/**
 *	------------------------------------------------------------
 *		Asc2Intの関数定義
 *	------------------------------------------------------------
 */
/**
 *	LONG AscHexToInt(PSTR ptrString)
 *	説明
 *		テキスト形式の16進数をバイナリに変換する
 *		文字列は xxxxh または xxxxH で表記する
 *	引数
 *		PSTR ptrString    テキスト形式の16進数文字列を指すポインタ
 *	返り値
 *		変換された値  正常終了
 *		ASCINT_ERROR	異常終了
 */
inline LONG ascHexToInt(PSTR ptrString)
{
	/**
	 *		ローカル変数
	 */
	LONG	dataTranslated = 0L;
	BYTE	charPick;
	int 	error = ASCINT_ON;

	do
	{
		charPick = (BYTE)*ptrString;

		if ( (charPick >= '0') && (charPick <= '9') )
			charPick &= 0x0f;
		else
		{
			if ( isalpha(charPick) )
			{
				if ( (charPick=='h') || (charPick=='H') )
					error = ASCINT_OFF;
				else
				{
					charPick = ( (charPick & 0x0f) - 1 ) + 10;
					if (charPick > 0x0f)
						error = ASCINT_ERROR;
				}
			}
		}	/// end not if ( (charPick >= '0') && (charPick <= '9') )

		if ( (error == ASCINT_ON) && (*ptrString++ != '\0') )
		{
			dataTranslated = dataTranslated * 16L + (LONG)charPick;
		}

	}while (error == ASCINT_ON);

	if ( error == ASCINT_ERROR )
	{
		dataTranslated = ASCINT_ERROR;
	}

	return dataTranslated;
}

/**
 *	LONG ascBinToInt(PSTR ptrString)
 *	説明
 *		テキスト形式の2進数をバイナリに変換する
 *		文字列は xxxxb または xxxxB で表記する
 *	引数
 *		PSTR ptrString    テキスト形式の2進数文字列を指すポインタ
 *
 *	返却値
 *		変換された値  正常終了
 *		ASCINT_ERROR	異常終了
 */
inline LONG ascBinToInt(PSTR ptrString)
{
	LONG	dataTranslated = 0L;
	BYTE	charPick;
	int		error = ASCINT_ON;

	do
	{
		charPick = (BYTE)*ptrString;
		
		if ( (charPick == '0') || (charPick == '1') )
		{
			charPick &= 0x0f;
			dataTranslated = dataTranslated * 2L + (LONG)charPick;
		}
		else
		{
			if ( (charPick == 'b') || (charPick == 'B') )
			{
				error = ASCINT_OFF;
			}
			else
			{
				error = ASCINT_ERROR;
			}
		}

	}while ( (error == ASCINT_ON) && (*ptrString++ != '\0') );

	if (error == ASCINT_ERROR)
	{
		dataTranslated = ASCINT_ERROR;
	}

	return dataTranslated;
}

/**
 *	LONG AscToInt(PSTR ptrString)
 *	説明
 *		テキスト形式の2進数, 10進数, 16進数をバイナリに変換する
 *		10進数の変換は、標準関数 atoi() で行う
 *
 *		2進数文字列は xxxxb または xxxxB
 *		10進数文字列は xxxx  または xxxx
 *		16進数文字列は xxxxh または xxxxH で表記する
 *	引数
 *		PSTR ptrString: テキスト形式の2進数文字列を指すポインタ
 *
 *	返却値
 *		変換された値  正常終了
 *		ASCINT_ERROR	異常終了
 *						10進数のときはatoi() の仕様に従う
 */
inline LONG ascToInt(PSTR ptrString)
{
	int 	pick;
	char	szBuff[ASCINT_MAX_DATA_SIZE];

	if (strlen(ptrString) < ASCINT_MAX_DATA_SIZE)
	{
		strcpy(szBuff, ptrString);
		strrev(szBuff);

		pick = (int)*szBuff;
		
		switch(pick)
		{
			case 'B':
			
			case 'b':
				return ascBinToInt(ptrString);
			
			case 'H':
			
			case 'h':
				return ascHexToInt(ptrString);
		}

		return (LONG)atoi(ptrString);
	}

	return ASCINT_ERROR;
}