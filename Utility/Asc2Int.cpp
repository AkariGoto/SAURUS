/*****************************************************************************
**
**	File:	Asc2Int.cpp(Ascii To Integer Main)
** 
**	����:	�����񁨐��l�ϊ����[�`��
**
**	Created: 2004/10/20(Wed)	Updated: 2004/10/20(Wed)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Asc2int.h"

/**
 *	------------------------------------------------------------
 *		Asc2Int�̊֐���`
 *	------------------------------------------------------------
 */
/**
 *	LONG AscHexToInt(PSTR ptrString)
 *	����
 *		�e�L�X�g�`����16�i�����o�C�i���ɕϊ�����
 *		������� xxxxh �܂��� xxxxH �ŕ\�L����
 *	����
 *		PSTR ptrString    �e�L�X�g�`����16�i����������w���|�C���^
 *	�Ԃ�l
 *		�ϊ����ꂽ�l  ����I��
 *		ASCINT_ERROR	�ُ�I��
 */
inline LONG ascHexToInt(PSTR ptrString)
{
	/**
	 *		���[�J���ϐ�
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
 *	����
 *		�e�L�X�g�`����2�i�����o�C�i���ɕϊ�����
 *		������� xxxxb �܂��� xxxxB �ŕ\�L����
 *	����
 *		PSTR ptrString    �e�L�X�g�`����2�i����������w���|�C���^
 *
 *	�ԋp�l
 *		�ϊ����ꂽ�l  ����I��
 *		ASCINT_ERROR	�ُ�I��
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
 *	����
 *		�e�L�X�g�`����2�i��, 10�i��, 16�i�����o�C�i���ɕϊ�����
 *		10�i���̕ϊ��́A�W���֐� atoi() �ōs��
 *
 *		2�i��������� xxxxb �܂��� xxxxB
 *		10�i��������� xxxx  �܂��� xxxx
 *		16�i��������� xxxxh �܂��� xxxxH �ŕ\�L����
 *	����
 *		PSTR ptrString: �e�L�X�g�`����2�i����������w���|�C���^
 *
 *	�ԋp�l
 *		�ϊ����ꂽ�l  ����I��
 *		ASCINT_ERROR	�ُ�I��
 *						10�i���̂Ƃ���atoi() �̎d�l�ɏ]��
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