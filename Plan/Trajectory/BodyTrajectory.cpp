/**
 *  File: BodyTrajectory.cpp
 *
 *  Description: 胴体軌道情報を扱うクラスの宣言部
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#include "BodyTrajectory.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Plan
{
/**
 *		コンストラクタとデストラクタ
 */
/// デフォルトコンストラクタ
BodyTrajectory::BodyTrajectory()
{
	initializeBodyTrajectory();
}

/// コピーコンストラクタ
BodyTrajectory::BodyTrajectory(const BodyTrajectory& trajectory)
{
	initializeBodyTrajectory();
	copy(trajectory);
}

/// デストラクタ
BodyTrajectory::~BodyTrajectory()
{
}

/**
 *		演算子
 */
/// 代入演算子
BodyTrajectory& BodyTrajectory::operator=(const BodyTrajectory& trajectory)
{
	/// 自己代入防止
	if ( &trajectory != this )
	{
		copy(trajectory);
	}

	return *this;
}

/// 等価演算子
bool BodyTrajectory::operator==(const BodyTrajectory& trajectory) const
{
	if ( moveStride != trajectory.moveStride ){return false;}
	if ( moveTime != trajectory.moveTime ){return false;}

	return true;
}

/// 非等価演算子
bool BodyTrajectory::operator!=(const BodyTrajectory& trajectory) const
{
	if ( *this == trajectory )
		return false;
	else
		return true;
}

/**
 *		初期化
 */
void BodyTrajectory::initializeBodyTrajectory(void)
{
	moveStride.setSize(THREE_DIMENSION);
	moveTime = 0.0;
}

/**
 *		軌道パラメータの設定
 *		goalPointおよびgoalTimeも自動更新される
 */
void BodyTrajectory::setBodyTrajectory(const Math::Vector& stride)
{
	moveStride = stride;
	goalPosition = startPosition + stride;
	return;
}

void BodyTrajectory::setBodyTrajectoryTime(double time)
{
	moveTime = time;
	goalTime = startTime + time;
	return;
}

/**
 *		軌道を得る
 */
Vector BodyTrajectory::getPosition(double splitTime)
{
	/// 取り出す重心軌道
	Vector trajectory(THREE_DIMENSION);

	return trajectory;
}

/**
 *		BodyTrajectoryクラスのprivateなメンバ関数
 */
void BodyTrajectory::copy(const BodyTrajectory &trajectory)
{
	moveStride = trajectory.moveStride;
	moveTime = trajectory.moveTime;

	return;
}

}	/// end of namespace Plan