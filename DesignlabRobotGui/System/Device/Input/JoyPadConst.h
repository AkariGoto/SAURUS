/**
 *  File: JoyPadConst.h
 *
 *  Description: ジョイパッドの定数の定義
 *
 *  Created: 2008/03/10(Mon)
 *  Updated: 2008/03/10(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __JoyPadConst_h__
#define __JoyPadConst_h__

/// 最大ボタン数
const int DIJOY_MAX_BUTTON_NUM = 32;

/// 軸のスケール
const int DIJOY_AXIS_SCALE = 1.0E6;

/// 軸の最大値
const int DIJOY_MAX_AXIS_VALUE = 1000;
	
/// 軸の最小値
const int DIJOY_MIN_AXIS_VALUE = 0;
	
/// 軸のゼロ点の値
const int DIJOY_CENTER_AXIS_VALUE = ( DIJOY_MAX_AXIS_VALUE + DIJOY_MIN_AXIS_VALUE ) / 2;

/// デッドゾーン
const int DIJOY_DEAD_ZONE = 50;

/// 飽和ゾーン
const int DIJOY_SATURATION_ZONE = 50;

#endif	/// __JoyPadConst_h__
