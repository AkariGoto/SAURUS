/**
 *  �t�@�C����
 *		Asc2Int.h(Ascii To Integer Header)
 *  ����
 *		�����񁨐��l�ϊ����[�`��
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		�萔�A�}�N���錾
 *	----------------------------------------------------------------------
 */
const int		ASCINT_MAX_DATA_SIZE	=	256;		/// Max String Size
const LONG		ASCINT_ERROR			=	-1L;
const int		ASCINT_ON				=	1;
const int		ASCINT_OFF				=	0;

#ifdef __cplusplus
extern "C" {
#endif	/// __cplusplus

/**
 *	����
 *		�e�L�X�g�`����16�i�����o�C�i���ɕϊ�
 */
extern "C" LONG ascHexToInt(PSTR pszData);

/**
 *	����
 *		�e�L�X�g�`����2�i�����o�C�i���ɕϊ�
 */
extern "C" LONG ascBinToInt(PSTR pszData);

/**
 *	����
 *		�e�L�X�g�`����2�i��, 10�i��, 16�i�����o�C�i���ɕϊ�
 */
extern "C" LONG ascToInt(PSTR pszData);

#ifdef __cplusplus
}
#endif	// __cplusplus