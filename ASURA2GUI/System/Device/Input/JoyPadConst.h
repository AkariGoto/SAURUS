/**
 *  File: JoyPadConst.h
 *
 *  Description: �W���C�p�b�h�̒萔�̒�`
 *
 *  Created: 2008/03/10(Mon)
 *  Updated: 2008/03/10(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __JoyPadConst_h__
#define __JoyPadConst_h__

/// �ő�{�^����
const int DIJOY_MAX_BUTTON_NUM = 32;

/// ���̃X�P�[��
const int DIJOY_AXIS_SCALE = 1.0E6;

/// ���̍ő�l
const int DIJOY_MAX_AXIS_VALUE = 1000;
	
/// ���̍ŏ��l
const int DIJOY_MIN_AXIS_VALUE = 0;
	
/// ���̃[���_�̒l
const int DIJOY_CENTER_AXIS_VALUE = ( DIJOY_MAX_AXIS_VALUE + DIJOY_MIN_AXIS_VALUE ) / 2;

/// �f�b�h�]�[��
const int DIJOY_DEAD_ZONE = 50;

/// �O�a�]�[��
const int DIJOY_SATURATION_ZONE = 50;

#endif	/// __JoyPadConst_h__
