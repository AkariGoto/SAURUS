/**
 *  File: DataHandler.cpp
 *
 *  Description:	ロボットのデータハンドラ
 *
 *  Created: 2007/02/05(Mon)
 *  Updated: 2018/02/05(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

//  20200819  クローラ関連コメントアウト

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "DataHandler.h"

using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;

namespace Data
{
/**
 *	----------------------------------------------------------------------
 *		DataHandlerクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		DataHandlerクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
DataHandler::DataHandler(AsuraX* asuraPtr, AsuraData* asuraDataPtr, Planner* planPtr, PlanData* planDataPtr)
: asuraDataSourcePointer(NULL), 
  asuraDataTargetPointer(NULL),
  planDataSourcePointer(NULL),
  planDataTargetPointer(NULL)
{
	asuraDataSourcePointer = asuraPtr;
	asuraDataTargetPointer = asuraDataPtr;
	planDataSourcePointer = planPtr;
	planDataTargetPointer = planDataPtr;
}

DataHandler::~DataHandler()
{
}

/**
 *	データ元のポインタの取得・解放
 */
void DataHandler::acquireAsuraDataSource(AsuraX* asuraPointer)
{
	releaseAsuraDataSource();

	asuraDataSourcePointer = asuraPointer;
}

void DataHandler::releaseAsuraDataSource(void)
{
	asuraDataSourcePointer = NULL;
}

void DataHandler::acquirePlanDataSource(Planner* planPointer)
{
	releasePlanDataSource();

	planDataSourcePointer = planPointer;
}

void DataHandler::releasePlanDataSource(void)
{
	planDataSourcePointer = NULL;
}

/**
 *	データ先のポインタの取得・解放
 */
/// 取得
void DataHandler::acquireAsuraDataTarget(AsuraData* dataPointer)
{
	releaseAsuraDataTarget();

	asuraDataTargetPointer = dataPointer;
}

/// 解放
void DataHandler::releaseAsuraDataTarget(void)
{
	asuraDataTargetPointer = NULL;
}

/// 取得
void DataHandler::acquirePlanDataTarget(PlanData* dataPointer)
{
	releasePlanDataTarget();

	planDataTargetPointer = dataPointer;
}

/// 解放
void DataHandler::releasePlanDataTarget(void)
{
	planDataTargetPointer = NULL;
}

/**
 *	データの取り込みと転送
 */
/// 転送
void DataHandler::exportAsuraData(void)
{

}

/// 取り込み
void DataHandler::importAsuraData(void)
{
	/// インクリメント変数
	int i;
	int j;

	/// 胴体に関するデータを更新
	asuraData.setBodyTransformation( asuraDataSourcePointer->getBodyTransformation() );
	asuraData.setBodyPosition( asuraDataSourcePointer->getBodyPosition() );
	asuraData.setBodyVelocity( asuraDataSourcePointer->getBodyVelocity() );
	
	/**
	 *	脚に関するデータを更新
	 *	座標系は絶対座標系に変換
	 */
	for (i=0;i<LEG_NUM;i++)
	{
		asuraData.setLegBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBaseTransformation(i+1))
			);
		asuraData.setLegBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBasePosition(i+1))
			);

		for (j=0;j<LEG_JOINT_NUM;j++)
		{
			asuraData.setLegJointTransformation(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointTransformation(i+1, j+1))
				);
			asuraData.setLegJointPosition( i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointPosition(i+1, j+1))
				);
		}

		asuraData.setLegFootTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootTransformation(i+1))
			);
		asuraData.setLegFootPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootPosition(i+1))
			);

		asuraData.setLegJointAngle(i+1, asuraDataSourcePointer->getLegJointAngle(i+1));
		asuraData.setLegJointVelocity(i+1, asuraDataSourcePointer->getLegJointVelocity(i+1));
		asuraData.setLegJointTorque(i+1, asuraDataSourcePointer->getLegJointTorque(i+1));
		asuraData.setLegPhase(i+1, asuraDataSourcePointer->getLegPhase(i+1));
		asuraData.setLegActuatorPosition(i+1, asuraDataSourcePointer->getLegActuatorPosition(i+1));///-----add
		asuraData.setFootJointAngle(i+1, asuraDataSourcePointer->getFootJointAngle(i+1));///---------------add
	}

	/// クローラに関するデータを更新
	/*  20200819
	for (i=0;i<TRACK_NUM;i++)
	{
		asuraData.setTrackBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBaseTransformation(i+1))
			);
		asuraData.setTrackBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBasePosition(i+1))
			);

		for (j=0;j<TRACK_JOINT_NUM;j++)
		{
			asuraData.setTrackJointTransformation( +1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointTransformation(i+1, j+1))
				);
			asuraData.setTrackJointPosition(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointPosition(i+1, j+1))
				);
		}

		asuraData.setTrackEndTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndTransformation(i+1))
			);
		asuraData.setTrackEndPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndPosition(i+1))
			);

		asuraData.setTrackJointAngle(i+1, asuraDataSourcePointer->getTrackJointAngle(i+1));
		asuraData.setTrackJointVelocity(i+1, asuraDataSourcePointer->getTrackJointVelocity(i+1));
		asuraData.setTrackJointTorque(i+1, asuraDataSourcePointer->getTrackJointTorque(i+1));
		
		asuraData.setTrackSpeed(i+1, asuraDataSourcePointer->getTrackSpeed(i+1));
	}
	*/
	return;
}

/// 転送
void DataHandler::exportPlanData(void)
{
}

/// 取り込み
void DataHandler::importPlanData(void)
{
	//planData.setStabilityMargin(planDataSourcePointer->getStabilityMargin());  20200819
	planData.setElapsedTime(planDataSourcePointer->getElapsedTime());

	return;	
}

/**
 *		データ渡し
 *			直接データ元からデータ先への代入
 */
void DataHandler::handleAsuraData(void)
{
	/// インクリメント変数
	int i;
	int j;

	/// 胴体に関するデータを更新
	asuraDataTargetPointer->setBodyTransformation(asuraDataSourcePointer->getBodyTransformation());
	asuraDataTargetPointer->setBodyPosition(asuraDataSourcePointer->getBodyPosition());
	asuraDataTargetPointer->setBodyVelocity(asuraDataSourcePointer->getBodyVelocity());

	/**
	 *		脚に関するデータを更新
	 *		座標系は絶対座標系に変換
	 */
	for (i=0;i<LEG_NUM;i++)
	{
		asuraDataTargetPointer->setLegBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBaseTransformation(i+1))
			);
		asuraDataTargetPointer->setLegBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBasePosition(i+1))
			);

		for (j=0;j<LEG_JOINT_NUM;j++)
		{
			asuraDataTargetPointer->setLegJointTransformation(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointTransformation(i+1, j+1))
				);
			asuraDataTargetPointer->setLegJointPosition(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointPosition(i+1, j+1))
				);
		}

		asuraDataTargetPointer->setLegFootTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootTransformation(i+1))
			);
		asuraDataTargetPointer->setLegFootPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootPosition(i+1))
			);

		asuraDataTargetPointer->setLegJointAngle(i+1, asuraDataSourcePointer->getLegJointAngle(i+1));
		asuraDataTargetPointer->setLegJointVelocity(i+1, asuraDataSourcePointer->getLegJointVelocity(i+1));
		asuraDataTargetPointer->setLegJointTorque(i+1, asuraDataSourcePointer->getLegJointTorque(i+1));
		asuraDataTargetPointer->setLegPhase(i+1, asuraDataSourcePointer->getLegPhase(i+1));
		asuraDataTargetPointer->setFootJointAngle(i+1, asuraDataSourcePointer->getFootJointAngle(i+1));//--add
		asuraDataTargetPointer->setLegActuatorPosition(i+1, asuraDataSourcePointer->getLegActuatorPosition(i+1));//--add
	}

	/// クローラに関するデータを更新
	/*  20200819
	for (i=0; i<TRACK_NUM; i++)
	{
		asuraDataTargetPointer->setTrackBaseTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBaseTransformation(i+1))
			);
		asuraDataTargetPointer->setTrackBasePosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackBasePosition(i+1))
			);

		for (j=0;j<TRACK_JOINT_NUM;j++)
		{
			asuraDataTargetPointer->setTrackJointTransformation(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointTransformation(i+1, j+1))
				);
			asuraDataTargetPointer->setTrackJointPosition(i+1, j+1, 
				asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackJointPosition(i+1, j+1))
				);
		}

		asuraDataTargetPointer->setTrackEndTransformation(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndTransformation(i+1))
			);
		asuraDataTargetPointer->setTrackEndPosition(i+1, 
			asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getTrackEndPosition(i+1))
			);

		asuraDataTargetPointer->setTrackJointAngle(i+1, asuraDataSourcePointer->getTrackJointAngle(i+1));
		asuraDataTargetPointer->setTrackJointVelocity(i+1, asuraDataSourcePointer->getTrackJointVelocity(i+1));
		asuraDataTargetPointer->setTrackJointTorque(i+1, asuraDataSourcePointer->getTrackJointTorque(i+1));

		asuraDataTargetPointer->setTrackSpeed(i+1, asuraDataSourcePointer->getTrackSpeed(i+1));
	}
	*/
	return;
}

void DataHandler::handlePlanData(void)
{
	if (planDataTargetPointer == NULL)
		return;

	//planDataTargetPointer->setStabilityMargin(planDataSourcePointer->getStabilityMargin());  20200819
	planDataTargetPointer->setElapsedTime(planDataSourcePointer->getElapsedTime());

	return;
}

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
	/**
	 *		脚
	 */
/// 脚根元ごとの同次変換行列
const Matrix& DataHandler::getLegBaseTransformation(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegBaseTransformation(legNo);
}

/// 脚関節ごとの同次変換行列
const Math::Matrix& DataHandler::getLegJointTransformation(int legNo, int jointNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);
	assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

	return asuraData.getLegJointTransformation(legNo, jointNo);
}

/// 足位置ごとの同次変換行列
const Matrix& DataHandler::getLegFootTransformation(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegFootTransformation(legNo);
}

/// 脚の根元位置
const Vector& DataHandler::getLegBasePosition(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegBasePosition(legNo);
}

/// 脚の関節位置
const Vector& DataHandler::getLegJointPosition(int legNo, int jointNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);
	assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

	return asuraData.getLegJointPosition(legNo, jointNo);
}

/// 足位置
const Vector& DataHandler::getLegFootPosition(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegFootPosition(legNo);
}

/// 脚の関節角度
const Vector& DataHandler::getLegJointAngle(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegJointAngle(legNo);
}

/// 脚の関節速度
const Vector& DataHandler::getLegJointVelocity(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegJointVelocity(legNo);
}

/// 脚の関節トルク
const Vector& DataHandler::getLegJointTorque(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegJointTorque(legNo);
}

/// 脚の運動相
const LegPhase DataHandler::getLegPhase(int legNo) const
{
	/// 引数チェック
	assert(1 <= legNo && legNo <= LEG_NUM);

	return asuraData.getLegPhase(legNo);
}

/**
 *		クローラ
 */
/*  20200819

/// クローラ根元ごとの同次変換行列
const Matrix& DataHandler::getTrackBaseTransformation(int trackNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackBaseTransformation(trackNo);
}

/// クローラごとの同次変換行列
const Matrix& DataHandler::getTrackJointTransformation(int trackNo, int jointNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);
	assert(1 <= jointNo && jointNo <= TRACK_JOINT_NUM);

	return asuraData.getTrackJointTransformation(trackNo, jointNo);
}

/// クローラ先端ごとの同次変換行列
const Matrix& DataHandler::getTrackEndTransformation(int trackNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackEndTransformation(trackNo);
}

/// クローラの根元位置
const Vector& DataHandler::getTrackBasePosition(int trackNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackBasePosition(trackNo);
}

/// クローラの関節位置
const Vector& DataHandler::getTrackJointPosition(int trackNo, int jointNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);
	assert(1 <= jointNo && jointNo <= TRACK_JOINT_NUM);

	return asuraData.getTrackJointPosition(trackNo, jointNo);
}

/// クローラの先端位置
const Vector& DataHandler::getTrackEndPosition(int trackNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackEndPosition(trackNo);
}

/// クローラの関節角度
const Vector& DataHandler::getTrackJointAngle(int trackNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackJointAngle(trackNo);
}

/// クローラの関節速度
const Math::Vector& DataHandler::getTrackJointVelocity(int trackNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackJointVelocity(trackNo);
}

/// クローラの関節トルク
const Math::Vector& DataHandler::getTrackJointTorque(int trackNo) const
{
	/// 引数チェック
	assert(1 <= trackNo && trackNo <= TRACK_NUM);

	return asuraData.getTrackJointTorque(trackNo);
}

*/

/**
 *		DataHandlerクラスのprivateなメンバ関数
 */


}