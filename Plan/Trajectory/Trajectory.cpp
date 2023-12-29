/**
 *  File: Trajectory.cpp
 *
 *  Description: 軌道情報を扱うクラスの定義部
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#include "Trajectory.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Plan
{
/**
 *		コンストラクタとデストラクタ
 */
/// デフォルトコンストラクタ
Trajectory::Trajectory()
{
	initializeTrajectory();
}

/// コピーコンストラクタ
Trajectory::Trajectory(const Trajectory& trajectory)
{
	initializeTrajectory();
	copy(trajectory);
}

/// デストラクタ
Trajectory::~Trajectory()
{
}

/**
 *		演算子
 */
/// 代入演算子
Trajectory& Trajectory::operator=(const Trajectory& trajectory)
{
	/// 自己代入を防ぐ
	if ( &trajectory != this )
	{
		copy(trajectory);
	}

	return *this;
}

/// 等価演算子
bool Trajectory::operator ==(const Trajectory& trajectory) const
{
	if ( startPosition != trajectory.startPosition ){return false;}
	if ( goalPosition != trajectory.goalPosition ){return false;}
	if ( startTime != trajectory.startTime ){return false;}
	if ( startTime != trajectory.goalTime ) {return false;}

	return true;
}


/// 非等価演算子
bool Trajectory::operator!=(const Trajectory& trajectory) const
{
	if ( *this == trajectory )
		return false;
	else
		return true;
}

/**
 *		初期化
 */
void Trajectory::initializeTrajectory(void)
{
	/// スタート地点
	startPosition.setSize(THREE_DIMENSION);
	/// ゴール地点
	goalPosition.setSize(THREE_DIMENSION);

	/// スタート時間
	startTime = 0.0;
	/// ゴール時間
	goalTime = 0.0;
}

/**
 *		軌道パラメータの設定
 */
void Trajectory::setDistance(const Vector& start, const Vector& goal)
{
	startPosition = start;
	goalPosition = goal;
}

void Trajectory::setTime(double start, double goal)
{
	startTime = start;
	goalTime = goal;
}

/**
 *		軌道パラメータの移行（スタートをずらす）
 */
void Trajectory::shiftStartPosition(const Math::Vector& start)
{
	Vector shift(THREE_DIMENSION);
	shift = goalPosition - startPosition;

	startPosition = start;
	goalPosition = start + shift;
}

void Trajectory::shiftStartTime(double start)
{
	double shift;
	shift = goalTime - startTime;

	startTime = start;
	goalTime = start + shift;

}

/**
 *		軌道を得る
 */
Vector Trajectory::getPosition(double splitTime)
{
	/// 取り出す脚先軌道
	Vector trajectory(THREE_DIMENSION);

	if ( splitTime <= startTime )
	{
		trajectory = startPosition;
	}
	else if ( startTime < splitTime && splitTime <= goalTime )
	{
		trajectory = (goalPosition - startPosition)*( (splitTime - startTime) / (goalTime - startTime) );
	}
	else if ( goalTime < splitTime )
	{
		trajectory = goalPosition;
	}
	
	return trajectory;
}

/**
 *		Trajectoryクラスのprivateなメンバ関数
 */
void Trajectory::copy(const Trajectory& trajectory)
{
	startPosition = trajectory.startPosition;
	goalPosition = trajectory.goalPosition;

	startTime = trajectory.startTime;
	goalTime = trajectory.goalTime;
}


}	/// end of namespace Plan