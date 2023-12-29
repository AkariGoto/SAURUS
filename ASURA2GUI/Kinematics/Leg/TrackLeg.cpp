/**
 *  ファイル名
 *		TrackLeg.cpp
 *  説明
 *		クローラ可変型脚機構の情報，運動学
 *  日付
 *		作成日: 2007/02/05(MON)		更新日: 2007/02/19(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "TrackLeg.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Asura
{
/**
 *	----------------------------------------------------------------------
 *		TrackLegクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		TrackLegクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
TrackLeg::TrackLeg(int no, double x, double y, double z, double theta) : Leg(no, x, y, z, theta)
{
	/// 関節角限界
	trackLimit.angleMin[0] = TRACK_ANGLE1_MIN;
	trackLimit.angleMin[1] = TRACK_ANGLE2_MIN;
	trackLimit.angleMin[2] = TRACK_ANGLE3_MIN;		//追加

	trackLimit.angleMax[0] = TRACK_ANGLE1_MAX;
	trackLimit.angleMax[1] = TRACK_ANGLE2_MAX;
	trackLimit.angleMax[2] = TRACK_ANGLE3_MAX;		//追加

	/// 最大クローラ速度
	trackLimit.trackSpeedMax = TRACK_SPEED_MAX;

	trackLastErrorJointNo = 0;

	driveSystem = LEG;
}

TrackLeg::~TrackLeg()
{
}

/**
 *	----------------------------------------
 *	運動学
 *	----------------------------------------
 */
/// クローラユニットの姿勢を変える
Kinematics TrackLeg::changeTrackPosture(const Vector& angle)
{
	/// 引数チェック
	if ( angle.getSize() != 3 )
	{
		cerr << "Error: [TrackLeg::changeTrackPosture] Argument is invalid\n" << endl;
        
		return KINE_ERROR_ELSE;
	}
	for (int i=1; i<=TRACK_JOINT_NUM; i++)
	trackData.jointAngle(i)=angle(i);
	/// まず関節が可動範囲内にあるかどうか調べる
	Kinematics kine = checkTrackJointMotionRange();

	/// 関節が可動範囲外
	if ( kine != NO_KINE_ERROR )	
	{
		cerr << "Error: [TrackLeg::changeTrackPosture] Invalid joint angle" << endl;
	
		return kine;
	}

	/// クローラ根元までの座標変換
	trackData.baseTransformation = trackBaseHomogeneousTransformation();
	/// クローラ根元位置ベクトルを代入
	for (int i=1; i<=THREE_DIMENSION; i++)
		trackData.basePosition(i) = trackData.baseTransformation(i, 4);

	/// クローラ根元から第1関節までの座標変換
	trackData.jointTransformation[0] = trackData.baseTransformation*trackJointHomogeneousTransformation(1);
	/// クローラ第1関節位置ベクトルを代入
	for (int i=1; i<=THREE_DIMENSION; i++)
		trackData.jointPosition[0](i) = trackData.jointTransformation[0](i, 4);

	/// 第1関節から第2関節までの座標変換
	trackData.jointTransformation[1] = trackData.jointTransformation[0]*trackJointHomogeneousTransformation(2);
	/// クローラ第2関節位置ベクトルを代入
	for (int j=1; j<=THREE_DIMENSION; j++)
		trackData.jointPosition[1](j) = trackData.jointTransformation[1](j, 4);

	/// 第2関節からクローラ外側先端までの座標変換
	trackData.endTransformation = trackData.jointTransformation[1]*trackEndHomogeneousTransformation();
	/// クローラ外側先端位置ベクトル代入
	for (int k=1; k<=THREE_DIMENSION; k++)
		trackData.endPosition(k) = trackData.endTransformation(k, 4);

	return kine;
}

/**
 *	----------------------------------------
 *	状態計算関数
 *	----------------------------------------
 */
/**
 *	説明
 *		関節速度: [3]
 *		過去の角度との差分で角度計算
 */
void TrackLeg::calculateTrackJointVelocity(const Math::Vector& trackVelocity)
{
	return;
}

/**
 *	説明
 *		関節トルク: [3]
 *		クローラ先端に加わった荷重から計算
 */
void TrackLeg::calculateTrackJointTorque(const Vector& trackReaction)
{
	return;
}

/**
 *	説明
 *		クローラ速度を変更する
 */
void TrackLeg::changeTrackSpeed(double trackSpeed)
{
	trackData.trackSpeed = trackSpeed;
}

/**
 *	説明
 *		脚の機構制限をチェックする
 */
	/// 関節の可動範囲を調査
Kinematics TrackLeg::checkTrackJointMotionRange(void)
{
	for (int i=1; i<=TRACK_JOINT_NUM; i++)
	{
		if ( trackData.jointAngle(i) < trackLimit.angleMin[i-1] )
		{
			/// 可動範囲の下限以下
			trackLastErrorJointNo = i;
			return KINE_ERROR_JOINT_UNDER_LIMIT;
		}
		else if ( trackData.jointAngle(i) > trackLimit.angleMax[i-1] )
		{
			/// 可動範囲の上限以上
			trackLastErrorJointNo = i;
			return KINE_ERROR_JOINT_OVER_LIMIT;
		}

	}

	/// エラーなし
	trackLastErrorJointNo = 0;

	return NO_KINE_ERROR;
}

/**
 *	----------------------------------------------------------------------
 *		TrackLegの内部クラス　TrackDataクラス
 *	----------------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
TrackLeg::TrackData::TrackData()
{
	newTrackData();
}

/// デストラクタ
TrackLeg::TrackData::~TrackData()
{
	deleteTrackData();
}

/**
 *	------------------------------------------------------------
 *		TrackDataクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/// オブジェクトのメモリ領域を確保する
void TrackLeg::TrackData::newTrackData(void)
{
/**
 *	クローラ根元の行列・ベクトルのサイズ決定
 */
	baseTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	basePosition.setSize(THREE_DIMENSION);

	/// メンバのメモリ領域確保
	jointTransformation		=	new Matrix[TRACK_JOINT_NUM];
	jointPosition				=	new Vector[TRACK_JOINT_NUM];

/**
 *	クローラ関節の行列・ベクトルのサイズ決定
 */
	/// 同時変換行列
	for ( int i=0; i<TRACK_JOINT_NUM; i++)
	{
		jointTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		jointTransformation[i].loadIdentity();
	}

	/// 各関節位置
	for (int j=0; j<TRACK_JOINT_NUM; j++)
		jointPosition[j].setSize(THREE_DIMENSION);

/**
 *	クローラ先端の行列・ベクトルのサイズ決定
 */
	endTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	endPosition.setSize(THREE_DIMENSION);

	/// 関節角度
	jointAngle.setSize(TRACK_JOINT_NUM);
	/// 関節速度
	jointVelocity.setSize(TRACK_JOINT_NUM);
	/// 関節トルク
	jointTorque.setSize(TRACK_JOINT_NUM);

	return;
}

/// オブジェクトのメモリ領域を解放する
void TrackLeg::TrackData::deleteTrackData(void)
{
	delete [] jointTransformation;
	delete [] jointPosition;

	return;
}


} /// end of namespace Asura