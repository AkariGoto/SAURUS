/**
 *  ファイル名
 *		WalkingRobot.cpp
 *  説明
 *		歩行ロボットの情報，運動学
 *		LegTrackクラスのコンポジション
 *  日付
 *		作成日: 2007/02/05(MON)		更新日: 2007/11/05(MON) SAURUS
 */

//  20200819  クローラ関連コメントアウト・TrackLegを介さずにLegを呼ぶ
//  20201015  初期高さ
//  20201018  第4関節

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "WalkingRobot.h"

using namespace std;
using namespace Math;
using namespace Const;

/**
 *	----------------------------------------------------------------------
 *		WalkingRobotクラス
 *	----------------------------------------------------------------------
 */
/**
 *				座標系の定義
 *
 *		 右手系					
 *									
 *						 y		
 *		x  1			6↑		
 *	   ←◎	|-----------|◎→	
 *	     ↓	|	  x		|   x			 
 *	     y	|	  ↑	|		
 *			|  ←◎		|
  *			|    y		|y		
 *	    x	|			|↑	
 *	   ←◎	|-----------|◎→	
 *	     ↓2|			|5   x	
 *	     y	|			|
 *			|    		|y		
 *	    x	|			|↑	
 *	   ←◎	|-----------|◎→	
 *	     ↓3			4   x	
 *	     y				
 */

namespace Asura
{
/**
 *	------------------------------------------------------------
 *		WalkingRobotクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
WalkingRobot::WalkingRobot()
{
	/// 脚オブジェクトの生成
	newTrackLegs();

	/// 初期化
	initializeWalkingRobot();

}
/// デストラクタ
WalkingRobot::~WalkingRobot()
{
	/// オブジェクト消去
	deleteTrackLegs();
}

/// 初期化関数
void WalkingRobot::initializeWalkingRobot(void)
{
	/// 胴体位置の初期化
	//initializeBodyPosition(0.0, 0.0, 500.0);
	//20201015
	initializeBodyPosition(0.0, 0.0, Plan::TRIPODGAIT_INITIAL_BODY_POSITION[2]);

	Vector* initialJointAngle;
	initialJointAngle = new Vector[LEG_NUM];

	double* initialFootJointAngle;  //20201018
	initialFootJointAngle = new double[LEG_NUM];  //20201018

	int i, j;
	for (i=0; i<LEG_NUM; i++)
		initialJointAngle[i].setSize(LEG_JOINT_NUM);

	initialJointAngle[0] = Vector(LEG_INITIAL_ANGLE1, LEG_JOINT_NUM);
	initialJointAngle[1] = Vector(LEG_INITIAL_ANGLE2, LEG_JOINT_NUM);
	initialJointAngle[2] = Vector(LEG_INITIAL_ANGLE3, LEG_JOINT_NUM);
	initialJointAngle[3] = Vector(LEG_INITIAL_ANGLE4, LEG_JOINT_NUM);
	initialJointAngle[4] = Vector(LEG_INITIAL_ANGLE5, LEG_JOINT_NUM);
	initialJointAngle[5] = Vector(LEG_INITIAL_ANGLE6, LEG_JOINT_NUM);

	//20201018
	initialFootJointAngle[0] = LEG_INITIAL_ANGLE1[3];
	initialFootJointAngle[1] = LEG_INITIAL_ANGLE2[3];
	initialFootJointAngle[2] = LEG_INITIAL_ANGLE3[3];
	initialFootJointAngle[3] = LEG_INITIAL_ANGLE4[3];
	initialFootJointAngle[4] = LEG_INITIAL_ANGLE5[3];
	initialFootJointAngle[5] = LEG_INITIAL_ANGLE6[3];
	

	/// 脚関節角の初期化
	for (j=0; j<LEG_NUM; j++)
		placeLegJointAngles( j+1, initialJointAngle[j], initialFootJointAngle[j] );

	delete [] initialJointAngle;
	delete[] initialFootJointAngle;
}

/**
 *	----------------------------------------
 *	ロボット本体に関するもの
 *	----------------------------------------
 */
/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */
/// 胴体の位置を刷新
void WalkingRobot::initializeBodyTransformation(const Math::Matrix& newBodyTransformation)
{
	bodyData.transformation = newBodyTransformation;

	int i;
	for (i=0; i<THREE_DIMENSION; i++)
		bodyData.position(i+1) = bodyData.transformation(i+1, 4);

	return;
}

/// 胴体の位置を刷新
void WalkingRobot::initializeBodyPosition(const Math::Vector& newBodyPosition)
{
	bodyData.position = newBodyPosition;

	int i;
	for (i=0; i<THREE_DIMENSION; i++)
		bodyData.transformation(i+1, 4) = bodyData.position(i+1);

	return;
}
void WalkingRobot::initializeBodyPosition(double x, double y, double z)
{
	bodyData.position(1) = x;
	bodyData.position(2) = y;
	bodyData.position(3) = z;

	int i;
	for (i=0; i<THREE_DIMENSION; i++)
		bodyData.transformation(i+1, 4) = bodyData.position(i+1);

	return;
}

/// 移動様式
void WalkingRobot::setLocomotionStyle(LocomotionStyle style)
{
	locomotionStyle = style;
}

/// 脚の運動相
void WalkingRobot::setLegPhase(int legNo, LegPhase phase)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );

	legPhases[legNo-1] = phase;

	return;
}

/**
 *	----------------------------------------
 *	運動学
 *	----------------------------------------
 */
/// 胴体の位置を決定する
Kinematics WalkingRobot::placeBodyPosition(Math::Vector& nextBodyPosition)
{
	/// 運動学結果
	Kinematics kine;
	/// 一時保存のための胴体同次変換行列
	Matrix lastBodyTransform(DH_DIMENSION, DH_DIMENSION);
	Vector presentGlobalFootPosition[LEG_NUM];
	Vector nextLocalFootPosition[LEG_NUM];

	/// カウンタ
	int i, j;

	/// 一時保存
	lastBodyTransform = bodyData.transformation;

	/// 胴体位置を更新する前の脚のグローバル座標を取得
	for (i=0; i<LEG_NUM; i++)
	{
		/// ベクトルサイズ決定
		presentGlobalFootPosition[i].setSize(THREE_DIMENSION);
		/// 現在の脚位置を胴体座標系で保存
		presentGlobalFootPosition[i] = transformationLocalToGlobal( trackLegs[i]->getLegFootPosition() );
	}

	/// 胴体位置を更新
	for (i=1; i<=THREE_DIMENSION; i++)
		bodyData.transformation(i, 4) = nextBodyPosition(i);

	/// 更新した胴体座標系での新しいローカル脚位置を計算
	for (j=0; j<LEG_NUM; j++)
	{
		/// ベクトルサイズ決定
		nextLocalFootPosition[j].setSize(THREE_DIMENSION);
		/// 胴体座標系での次の脚位置を計算
		nextLocalFootPosition[j] = transformationGlobalToLocal( presentGlobalFootPosition[j] );
	}

	/// 支持脚のみで胴体位置を変更
	for (j=0; j<LEG_NUM; j++)
	{
		if (legPhases[j] == SUPPORT )
		{
			/// 新しい足先目標値に置く
			kine = trackLegs[j]->placeLegFootPosition( nextLocalFootPosition[j] );
			
			/// 脚先が新しい指令値に到達できなかったら
			if ( kine != NO_KINE_ERROR )
			{
				/// 胴体の状態を最初に戻す
				bodyData.transformation = lastBodyTransform;

				/// エラーコードを記録
				lastKineError = kine;
				lastErrorTrackLegNo = i+1;
				
				return kine;
			}
		}
	}

	/// 全て成功したので胴体位置更新
	bodyData.position = nextBodyPosition;

	return NO_KINE_ERROR;
}

/// 胴体の位置を決定する
Kinematics WalkingRobot::placeBodyPosture(Math::Matrix& nextBodyPosture)
{
	/// 運動学結果
	Kinematics kine;

	return NO_KINE_ERROR;
}

/// 胴体の位置姿勢を同時に決定する
Kinematics WalkingRobot::placeBodyFrame(Math::Matrix& nextBodyFrame)
{
	/// 運動学結果
	Kinematics kine;

	return NO_KINE_ERROR;
}

/**
 *	----------------------------------------
 *	脚に関するもの
 *	TrackLegクラスを介した脚クラス関数の呼び出し
 *	----------------------------------------
 */
/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
/// 脚根元の同次変換行列
const Matrix& WalkingRobot::getLegBaseTransformation(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegBaseTransformation();
}
/// 関節の同次変換行列
const Matrix& WalkingRobot::getLegJointTransformation(int legNo, int jointNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );
	return trackLegs[legNo-1]->getLegJointTransformation( jointNo );
}
/// 足裏の同次変換行列
const Matrix& WalkingRobot::getLegFootTransformation(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegFootTransformation();
}
/// 脚根元の位置ベクトル
const Vector& WalkingRobot::getLegBasePosition(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegBasePosition();
}
/// 関節位置の位置ベクトル
const Vector& WalkingRobot::getLegJointPosition(int legNo, int jointNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );
	return trackLegs[legNo-1]->getLegJointPosition( jointNo );
}
/// 足裏位置の位置ベクトル
const Vector& WalkingRobot::getLegFootPosition(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegFootPosition();
}
/// 関節角度ベクトル
const Vector& WalkingRobot::getLegJointAngle(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegJointAngle();
}
/// 足首関節角度

const  double WalkingRobot::getFootJointAngle(int legNo) const//------Add
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getFootJointAngle();
	//trackLegs[legNo-1]->getFootJointAngle();
}
// 関節速度ベクトル
const Vector& WalkingRobot::getLegJointVelocity(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegJointVelocity();
}
/// 関節トルクベクトル
const Vector& WalkingRobot::getLegJointTorque(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegJointTorque();
}
/// 脚の根元の位置・姿勢
const Vector& WalkingRobot::getBasePose(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getBasePose();
}
/// 脚番号の取得
const int WalkingRobot::getLegNo(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegNo();
}
/// 関節のエラー番号
const int WalkingRobot::getLegLastErrorJointNo(int legNo) const
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegLastErrorJointNo();
}
/// ボールねじ座標（ワイヤ変位）
const Vector& WalkingRobot:: getLegActuatorPosition(int legNo) const//<----add
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	return trackLegs[legNo-1]->getLegActuatorPosition();
}
/// 脚番号の設定
void WalkingRobot::setLegNo(int legNo, int no)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	assert( 1 <= no && no <= LEG_NUM );
	trackLegs[legNo-1]->setLegNo(no);
	return;
}
/// 順運動学
Kinematics WalkingRobot::solveLegDirectKinematics(int legNo)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->solveDirectKinematics();
	return kine;
}
/// 逆運動学
Kinematics WalkingRobot::solveLegInverseKinematics(int legNo)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->solveInverseKinematics();
	//trackLegs[legNo-1]->calculationActatorPosition();//<----add
	return kine;
}
/// 姿勢指標の変更
void WalkingRobot::setLegPoseIndicator(int legNo, int hip, int knee)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->setLegPoseIndicator(hip, knee);
	return;
}
// 脚の根元設定
void WalkingRobot::setLegBasePose(int legNo, double x, double y, double z, double theta)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->setLegBasePose(x, y, z, theta);
	return;
}


/**
 *	----------------------------------------
 *	脚の運動学
 *	----------------------------------------
 */
/**
 *	説明
 *		足の位置をセット（足裏は常に重力方向と仮定）
 *		順運動学と逆運動学が一致しているかを確認する
 *		途中の関節位置を計算する
 */
Kinematics WalkingRobot::placeLegFootPosition(int legNo, const Math::Vector& nextFootPosition)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->placeLegFootPosition( nextFootPosition );

	if ( kine != NO_KINE_ERROR )
	{
		/// エラーコードを記録
		lastKineError = kine;
		lastErrorTrackLegNo = legNo;
	}

	return kine;
}
/**
 *	説明
 *		関節角をセット
 *		セットした関節角で順運動学を解く
 */
Kinematics WalkingRobot::placeLegJointAngles(int legNo, const Math::Vector& nextJointAngle, const double& nextFootJointAngle)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	Kinematics kine = trackLegs[legNo-1]->placeLegJointAngles( nextJointAngle, nextFootJointAngle );

	if ( kine != NO_KINE_ERROR )
	{
		/// エラーコードを記録
		lastKineError = kine;
		lastErrorTrackLegNo = legNo;
	}

	return kine;
}

/**
 *	----------------------------------------
 *	ヤコビアンを使っての状態計算
 *	----------------------------------------
 */
/**
 *	説明
 *		関節速度: [3]
 *		脚先速度から計算
 */
void WalkingRobot::calculateLegJointVelocity(int legNo, const Math::Vector& footVelocity)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->calculateLegJointVelocity(footVelocity);
	return;
}
/**
 *	説明
 *		関節トルク: [3]
 *		脚先端に加わった荷重から計算
 */
void WalkingRobot::calculateLegJointTorque(int legNo, const Math::Vector& footReaction)
{
	/// 引数チェック
	assert( 1 <= legNo && legNo <= LEG_NUM );
	trackLegs[legNo-1]->calculateLegJointTorque(footReaction);
	return;
}

/**
 *	----------------------------------------
 *	クローラに関するもの
 *	----------------------------------------
 */
/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
/// クローラ根元の同次変換行列

/*  20200819
const Matrix& WalkingRobot::getTrackBaseTransformation(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackBaseTransformation();
}
/// 関節の同次変換行列
const Matrix& WalkingRobot::getTrackJointTransformation(int trackNo, int jointNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );
	return trackLegs[trackNo-1]->getTrackJointTransformation( jointNo );
}
/// クローラ先端の同次変換行列
const Matrix& WalkingRobot::getTrackEndTransformation(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackEndTransformation();
}
/// クローラ根元の位置ベクトル
const Vector& WalkingRobot::getTrackBasePosition(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackBasePosition();
}
/// クローラ関節の位置ベクトル
const Vector& WalkingRobot::getTrackJointPosition(int trackNo, int jointNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointPosition( jointNo );
}
/// クローラ先端の位置ベクトル
const Vector& WalkingRobot::getTrackEndPosition(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackEndPosition();
}
/// 関節角度ベクトル
const Vector& WalkingRobot::getTrackJointAngle(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointAngle();
}
/// 関節速度ベクトル
const Vector& WalkingRobot::getTrackJointVelocity(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointVelocity();
}
/// 関節トルクベクトル
const Vector& WalkingRobot::getTrackJointTorque(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackJointTorque();
}
/// クローラスピード
const double WalkingRobot::getTrackSpeed(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackSpeed();
}
/// 脚駆動方式
const DriveSystem WalkingRobot::getDriveSystem(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getDriveSystem();
}
/// クローラ形態時に最後にエラーとなった関節番号
const int WalkingRobot::getTrackLastErrorJointNo(int trackNo) const
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	return trackLegs[trackNo-1]->getTrackLastErrorJointNo();
}
*/

/**
 *	----------------------------------------
 *	クローラの運動学
 *	----------------------------------------
 */
/**
 *	説明
 *		クローラユニットの姿勢を変える
 */

/*  20200819
Kinematics WalkingRobot::changeTrackPosture(int trackNo, const Math::Vector& angle)
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	Kinematics kine = trackLegs[trackNo-1]->changeTrackPosture(angle);
	return kine;
}
*/

/**
 *	説明
 *		関節速度: [3]
 *		クローラ先端の速度から計算
 */
/*  20200819
void WalkingRobot::calculateTrackJointVelocity(int trackNo, const Math::Vector& trackVelocity)
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	trackLegs[trackNo-1]->calculateTrackJointVelocity( trackVelocity );
	return;
}
*/

/**
 *	説明
 *		関節トルク: [3]
 *		クローラ先端に加わった荷重から計算
 */
 /*  20200819
void WalkingRobot::calculateTrackJointTorque(int trackNo, const Math::Vector& trackReaction)
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	trackLegs[trackNo-1]->calculateTrackJointTorque( trackReaction );
	return;
}
*/

/**
 *	説明
 *		クローラ速度を変更する
 */
 /*  20200819
void WalkingRobot::changeTrackSpeed(int trackNo, double trackSpeed)
{
	/// 引数チェック
	assert( 1 <= trackNo && trackNo <= TRACK_NUM );
	trackLegs[trackNo-1]->changeTrackSpeed(trackSpeed);
	return;
}
*/

/**
 *	説明
 *		駆動方式の設定
 *		戻り値：設定した駆動方式
 */
/*  20200819
DriveSystem WalkingRobot::setDriveSystem(int trackLegNo, DriveSystem ds)
{
	/// 引数チェック
	assert( 1 <= trackLegNo && trackLegNo <= TRACK_NUM );
	return trackLegs[trackLegNo-1]->setDriveSystem( ds );
}
*/

/**
 *	------------------------------------------------------------
 *		WalkingRobotクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/// オブジェクトのメモリ領域を確保する
void WalkingRobot::newTrackLegs(void)
{
	/// 脚の運動相の配列を確保
	legPhases = new LegPhase[LEG_NUM];

	/// クラスポインタの配列確保
	//trackLegs = new TrackLeg*[LEG_NUM];
	trackLegs = new Leg * [LEG_NUM];

	/// クラスのインスタンスを生成
	for (int i=0; i<LEG_NUM; i++)
	{
		//trackLegs[i] = new TrackLeg( (i+1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i] );
		trackLegs[i] = new Leg((i + 1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i]);
	}

	return;
}

/// オブジェクトのメモリ領域を解放する
void WalkingRobot::deleteTrackLegs(void)
{
	delete [] legPhases;

	/// インスタンスの破棄
	for (int i=0; i<LEG_NUM; i++)
		delete trackLegs[i];

	/// クラスポインタの破棄
	delete [] trackLegs;

	return;
}

/**
 *	----------------------------------------------------------------------
 *		WalkingRobotの内部クラス　BodyDataクラス
 *	----------------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
WalkingRobot::BodyData::BodyData()
{
	initBodyData();
}

/// デストラクタ
WalkingRobot::BodyData::~BodyData()
{
}

/**
 *	------------------------------------------------------------
 *		BodyDataクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/// オブジェクトのメモリ領域を確保する
void WalkingRobot::BodyData::initBodyData(void)
{
	/// 行列のサイズ決定
	transformation.setSize(DH_DIMENSION, DH_DIMENSION);
	transformation.loadIdentity();

	/// 位置ベクトルのサイズ決定
	position.setSize(THREE_DIMENSION);

	/// 速度ベクトルのサイズ決定
	velocity.setSize(THREE_DIMENSION);

	return;
}

} /// end of namespace Asura