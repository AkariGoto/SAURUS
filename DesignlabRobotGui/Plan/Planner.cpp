/**
 *  ファイル名
 *		Planner.cpp
 *  説明
 *		対象とするWalkingRobotオブジェクトの基本動作を生成するクラス
 *  日付
 *		作成日: 2007/03/06(TUE)		更新日: 2007/03/12(MON)
 */

//  20200819  安定余裕関連コメントアウト
//  20201020  動作停止後の再動作
 
 /**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "Planner.h"
#include "..\Math\MathLibrary.h"
#include "..\Math\Matrix\Matrix.h"

using namespace std;
using namespace Math;
using namespace Asura;
using namespace Const;

namespace Plan
{
/**
 *	----------------------------------------------------------------------
 *		Plannerクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		Plannerクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
Planner::Planner(AsuraX* asuraPointer_, TimeManager* timeManagerPointer_)
{
	/// ポインタのセット
	asuraPointer = asuraPointer_;
	timeManagerPointer = timeManagerPointer_;

	/// 軌道要素を生成
	newTrajectories();

	/// メンバ変数の初期化
	isRunning = false;
	isSuspended = false;

	//stabilityMargin = 0.0;  20200819
	elapsedTime = 0.0;
	planStartTime = 0.0;
}

/// デストラクタ
Planner::~Planner()
{
	/// ヌルポインタをセット
	asuraPointer = NULL;

	/// 軌道要素を消去
	deleteTrajectories();
}


/**
 *		制御する歩行ロボットオブジェクトへのポインタ
 */
/// ポインタを取得
void Planner::acquireAsura(AsuraX* ptr)
{
	/// 現在のポインタをリセット
	releaseAsura();

	asuraPointer = ptr;

	return;
}
/// ポインタを解放
void Planner::releaseAsura(void)
{
	asuraPointer = NULL;

	return;
}

/**
 *		使用する時間管理オブジェクトへのポインタ
 */

/// ポインタを取得
void Planner::acquireTimeManager(TimeManager* ptr)
{
	/// 現在のポインタをリセット
	releaseTimeManager();

	timeManagerPointer = ptr;

	return;
}

/// ポインタを解放
void Planner::releaseTimeManager(void)
{
	timeManagerPointer = NULL;

	return;
}

/**
 *		軌道パラメータの設定
 */
/// 遊脚軌道を設定する
void Planner::setLegSwingTrajectory(
		int legNo, 
		const Math::Vector& start,
		const Math::Vector& upPhase,
		const Math::Vector& returnPhase, 
		const Math::Vector& downPhase
	)
{
	/// 脚の引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	/// 軌道要素をセット
	legTrajectory[legNo-1].setLegTrajectory(start, upPhase, returnPhase, downPhase);

	return;
}

/// 遊脚時間を設定する
void Planner::setLegSwingTime(
		int legNo, 
		double start,
		double upPhase, 
		double returnPhase, 
		double downPhase
	)
{
	/// 脚の引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	/// 軌道要素をセット
	legTrajectory[legNo-1].setLegTrajectoryTime(start, upPhase, returnPhase, downPhase);

	return;
}

/**
 *		遊脚の軌道上の位置を取得する
 */
Vector Planner::getLegSwingPosition(int legNo, double splitTime)
{
	/// 脚の引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM ); 

	return legTrajectory[legNo-1].getPosition(splitTime);
}

/**
 *	----------------------------------------
 *	実際に動作を行う関数
 *	----------------------------------------
 */
/**
 *		遊脚する
 *		予めユーザが設定した軌道を遊脚する
 */
PlanStatus Planner::swingLeg(int legNo, double splitTime)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <=LEG_NUM );

/**
 *		ローカル変数の宣言
 */
	/// 運動学の結果
	Kinematics kine;
	/// エラーを起こした関節番号
	int errorJointNo = 0;

	/// 動作停止中だったら遊脚しない
	if ( isSuspended )
	{
		return (SUSPEND);
	}

	/// 遊脚時間内かどうかのチェック
	if ( ( legTrajectory[legNo-1].getStartTime() < splitTime ) && ( splitTime < legTrajectory[legNo-1].getGoalTime() ) )
	{
		/// 引数の時間での軌道上の位置を取得
		/// PTP制御により，足を目標位置にセット
		kine = asuraPointer->placeLegFootPosition( legNo, legTrajectory[legNo-1].getPosition(splitTime) );

		/// 逆運動学が解けたかどうかのチェック
		if ( kine != NO_KINE_ERROR )
		{
			printPlanErrorMessage();
			return INVALID;
		}

		/// 脚の状態を遊脚にセット
		asuraPointer->setLegPhase(legNo, SWING);
	}
	else
	{
		/// 引数の時間での軌道上の位置を取得
		/// PTP制御により，足を目標位置にセット
		kine = asuraPointer->placeLegFootPosition( legNo, legTrajectory[legNo-1].getPosition(splitTime) );

		/// 逆運動学が解けたかどうかのチェック
		if ( kine != NO_KINE_ERROR )
		{
			printPlanErrorMessage();
			return INVALID;
		}

		/// 脚の状態を支持脚にセット
		asuraPointer->setLegPhase(legNo, SUPPORT);

		/// 動作終了
		return END;
	}

	/// 動作実行中
	return RUN;
}

/**
 *		胴体推進
 */
PlanStatus Planner::moveBody(double splitTime)
{

	/// 動作停止中だったら遊脚しない
	if ( isSuspended )
	{
		return (SUSPEND);
	}

/**
 *		ローカル変数の宣言
 */
	/// 軌道上の目標脚先位置
	Vector targetPosition(THREE_DIMENSION);
	/// 運動学の結果
	Kinematics kine;
	/// エラーを起こした脚番号
	int errorLegNo = 0;
	/// エラーを起こした関節番号
	int errorJointNo = 0;

	/// 引数の時間での軌道上の位置を計算
	targetPosition = bodyTrajectory->getPosition(splitTime);

	/// PTP制御により重心を目標位置にセット
	kine = asuraPointer->placeBodyPosition(targetPosition);

	/// 逆運動が解けるかどうかのチェック
	if ( kine != NO_KINE_ERROR )
	{
		Planner::printPlanErrorMessage();

		return INVALID;
	}

	/// 胴体推進時間内にあるかどうかチェック
	if ( !( bodyTrajectory->getStartTime() < splitTime ) || !( splitTime < bodyTrajectory->getGoalTime() ) )
	{
		return END;
	}

	return RUN;


}

/**
 *		エラーメッセージの出力
 */
void Planner::printPlanErrorMessage(void)
{
	Kinematics kine = asuraPointer->getLastKineError();
	int legNo = asuraPointer->getLastErrorTrackLegNo();
	int jointNo = asuraPointer->getLegLastErrorJointNo(legNo);
	char error[256] = "No error";

	switch ( kine )
	{
		case KINE_ERROR_UNREACHABLE:
			sprintf_s(error, "Unreachable operation");
			break;
		
		case KINE_ERROR_SINGULAR:
			sprintf_s(error, "Singular posture");
			break;

		case KINE_ERROR_ELSE:
			sprintf_s(error, "Uncertain error");
			break;

		case KINE_ERROR_REACH_RADIUS_UNDER:
			sprintf_s(error, "Under minimum horizontal reach");
			break;

		case KINE_ERROR_REACH_RADIUS_OVER:
			sprintf_s(error, "Over maximum horizontal reach");
			break;

		case KINE_ERROR_REACH_HEIGHT_UNDER:
			sprintf_s(error, "Under minimum vertical reach");
			break;

		case KINE_ERROR_REACH_HEIGHT_OVER:
			sprintf_s(error, "Under maximum vertical reach");
			break;

		case KINE_ERROR_JOINT_UNDER_LIMIT:
			sprintf_s(error, "Under minimum joint angle");
			break;

		case KINE_ERROR_JOINT_OVER_LIMIT:
			sprintf_s(error, "Over maximum joint angle");
			break;

		case KINE_ERROR_JOINT_VELOCITY_LIMIT:
			sprintf_s(error, "Over maximum joint angular velocity");
			break;

		case KINE_ERROR_JOINT_TORQUE_LIMIT:
			sprintf_s(error, "Over maximum joint torque velocity");
			break;
	}

	cout << " Joint-" << jointNo << " of " << "Leg-" << legNo << ":" << error << endl;	
}

/**
 *	----------------------------------------
 *	運動を具体的に生成する関数群 
 *	----------------------------------------
 */
/// 歩行開始のための初期化
bool Planner::setup(void)
{
	elapsedTime = 0.0;  //20201020
	stopElapsedTime = elapsedTime;  //20201020

	/// 動作開始
	isRunning = true;

	return true;
}

/// 歩行を開始する
bool Planner::startPlan(void)
{
	/// プランニング開始時間を設定
	//planStartTime = timeManagerPointer->getRealTime();  //20201020
	planStartTime = timeManagerPointer->getRealTime() - stopElapsedTime;  //elapsedTimeは起動時は0

	return true;
}
	
/// 歩行を停止する
bool Planner::stopPlan(void)
{
	//isRunning = false;  //20201020

	stopElapsedTime = elapsedTime;  //20201020

	return true;
}

bool Planner::standByForStop(void)
{
	return true;
}

/**
 *		安定余裕の計算
 *			引数: 重力方向成分
 */
/*  20200819
double Planner::calculateStabilityMargin(double gx, double gy, double gz)
{
	/// 足位置
	Vector foot[LEG_NUM];
	/// 重心位置
	Vector cog;
	/// 重力方向
	Vector gravity( positionVector(gx, gy, gz) );
	/// 脚の相
	LegPhase phase[LEG_NUM];
	/// 重心位置
	Vector body(THREE_DIMENSION);

	/// 足位置の取得
	int i;
	for (i=0; i<LEG_NUM; i++)
	{
		foot[i].setSize(THREE_DIMENSION);

		foot[i] = asuraPointer->transformationLocalToGlobal( 
									asuraPointer->getLegFootPosition(i+1) 
									);
	}

	/// 重心位置の取得
	body = asuraPointer->getBodyPosition();

	/// 脚の運動相の取得
	int j;
	for (j=0; j<LEG_NUM; j++ )
		phase[j] = asuraPointer->getLegPhase( j+1 );


	/// 脚相毎に安定余裕を計算
	if ( phase[0] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[1], foot[2], foot[3],  body, gravity );
	else if ( phase[1] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[0], foot[2], foot[3], body, gravity );
	else if ( phase[2] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[0], foot[1], foot[3], body, gravity );
	else if ( phase[3] == SWING )
		stabilityMargin = stabilityMarginForTriangle( foot[0], foot[1], foot[2], body, gravity );
	else
		stabilityMargin = stabilityMarginForQuadrangle( foot[0], foot[2], foot[3], foot[3], body, gravity );


	return stabilityMargin;
}
*/


/**
 *		Plannerクラスのprivateなメンバ関数
 *
 */
void Planner::newTrajectories(void)
{
	legTrajectory = new LegTrajectory[LEG_NUM];
	bodyTrajectory = new BodyTrajectory;

	return;
}

void Planner::deleteTrajectories(void)
{
	delete [] legTrajectory;
	delete bodyTrajectory;

	return;
}

/**
 *		安定余裕の計算のためのヘルプ関数
 */

/*  20200819
/// 支持脚三角形時の安定余裕
double Planner::stabilityMarginForTriangle(	const Vector& foot1, const Vector& foot2, const Vector& foot3, 
															const Vector& cog, const Vector& gravity)
{
	/// 安定余裕
	double stability;

	/// 支持脚三角形の法線ベクトル
	Vector triangleNormal(THREE_DIMENSION);
	/// 重心の支持脚三角形への投影点
	Vector cogProjection(THREE_DIMENSION);

	/// 支持脚三角形の法線ベクトルを計算
	triangleNormal = normalVectorOfPlane( foot1, foot2, foot3 );

	/// 重心の重力方向への支持脚三角形との交点を計算
	cogProjection = intersectPointLineAndPlane(cog, gravity, foot1, triangleNormal);
	
	/// 安定余裕を計算
	stability = minDistanceToTriangleEdge(cogProjection, foot1, foot2, foot3);

	return stability;
}

/// 支持脚四角形時の安定余裕
double Planner::stabilityMarginForQuadrangle(	const Vector& foot1, const Vector& foot2, 
																const Vector& foot3, const Vector& foot4, 
																const Vector& cog, const Vector& gravity)
{
	/// 安定余裕
	double stability;
	
	/// 支持脚三角形の法線ベクトル
	Vector quadrangleNormal(THREE_DIMENSION);
	/// 重心の支持脚三角形への投影点
	Vector cogProjection(THREE_DIMENSION);

	/// 支持脚三角形の法線ベクトルを計算
	quadrangleNormal = normalVectorOfPlane(foot1, foot2, foot3);
	/// 重心の重力方向への支持脚三角形との交点を計算
	cogProjection = intersectPointLineAndPlane(cog, gravity, foot1, quadrangleNormal);
	
	/// 安定余裕を計算
	stability = minDistanceToQuadrangleEdge(cogProjection, foot1, foot2, foot3, foot4);

	return stability;
}
*/

}	/// end of namespace Plan


