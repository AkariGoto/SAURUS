/**
 *  ファイル名
 *		AsuraData.cpp
 *  説明
 *		TITAN Xのデータ（キネマティクス，クローラなど）
 *  日付
 *		作成日: 2007/02/12(Mon)		更新日: 2007/02/19(Mon)
 */

//20200819  クローラ関連コメントアウト
//20200820  ファイル名をTitanData.cppからAsuraData.cppに変更
//20201013  第4関節
//20201014  第4関節

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "AsuraData.h"
#include "..\Utility\EngConstant.h"


using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;

namespace Data
{
/**
 *	----------------------------------------------------------------------
 *		AsuraDataクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		AsuraDataクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
AsuraData::AsuraData()
{
	newAsuraData();
}

/// コピーコンストラクタ
AsuraData::AsuraData(const AsuraData& asuraData)
{
	newAsuraData();
	copy(asuraData);
}

/// デストラクタ
AsuraData::~AsuraData()
{
	deleteAsuraData();
}

/**
 *	演算子
 */
/// 代入演算子
AsuraData& AsuraData::operator=(const AsuraData& asuraData)
{
	/// 自己代入のチェック
	if ( &asuraData != this )
	{
		copy(asuraData);
	}

	return *this;
}

/// 等価演算子
bool AsuraData::operator==(const AsuraData& asuraData)
{
	/// インクリメント変数
	int i, j;

	/**
	 *	胴体
	 */
	if ( bodyTransformation	!= asuraData.bodyTransformation ){return false;}
	if ( bodyPosition			!= asuraData.bodyPosition ){return false;}
	if ( bodyVelocity			!= asuraData.bodyVelocity ){return false;}
	if ( walkingDirection		!= asuraData.walkingDirection ){return false;}
	if ( locomotionStyle		!= asuraData.locomotionStyle ) {return false;}

	/**
	 *	脚
	 */
	for (i=0; i<LEG_NUM; i++)
	{
		if ( legBaseTransformation[i]	!= asuraData.legBaseTransformation[i] ){return false;}
		if ( legBasePosition[i]			!= asuraData.legBasePosition[i] ){return false;}

		for (j=0; j<=LEG_JOINT_NUM; j++)
		{
			if ( legJointTransformation[i][j]	!= asuraData.legJointTransformation[i][j] ){return false;}
			if ( legJointPosition[i][j]			!= asuraData.legJointPosition[i][j] ){return false;}
		}

		if ( legFootTransformation[i] != asuraData.legFootTransformation[i] ){return false;}
		if ( legFootPosition[i] != asuraData.legFootPosition[i] ){return false;}

		if ( legJointAngle[i]		!= asuraData.legJointAngle[i] ){return false;}
		if ( legJointVelocity[i]	!= asuraData.legJointVelocity[i] ){return false;}
		if ( legJointTorque[i]	!= asuraData.legJointTorque[i] ){return false;}

		if ( legPhase[i] != asuraData.legPhase[i] ){return false;}
	}


	/**
	 *	クローラ
	 */

	/*  20200819
	/// クローラごとの同次変換行列
	for (i=0; i<TRACK_NUM; i++)
	{
		if ( trackBaseTransformation[i]	!= asuraData.trackBaseTransformation[i] ){return false;}
		if ( trackBasePosition[i]			!= asuraData.trackBasePosition[i] ){return false;}

		for (j=0; j<TRACK_JOINT_NUM; j++)
		{
			if ( trackJointTransformation[i][j]	!= asuraData.trackJointTransformation[i][j] ){return false;}
			if ( trackJointPosition[i][j]			!= asuraData.trackJointPosition[i][j] ){return false;}
		}

		if ( trackEndTransformation[i]	!= asuraData.trackEndTransformation[i] ){return false;}
		if ( trackEndPosition[i]				!= asuraData.trackEndPosition[i] ){return false;}

		if ( trackJointAngle[i]		!= asuraData.trackJointAngle[i] ){return false;}
		if ( trackJointVelocity[i]	!= asuraData.trackJointVelocity[i] ){return false;}
		if ( trackJointTorque[i]		!= asuraData.trackJointTorque[i] ){return false;}

		if ( trackSpeed[i] != asuraData.trackSpeed[i] ){return false;}
	}
	*/

	return true;
}

/// 非等価演算子
bool AsuraData::operator!=(const AsuraData& asuraData)
{
	if ( *this == asuraData )
		return false;
	else
		return true;
}

/**
 *	------------------------------------------------------------
 *		AsuraDataクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/**
 *	オブジェクトのメモリ領域を確保
 */
void AsuraData::newAsuraData(void)
{
/// インクリメント変数
	int i, j;

/// メンバ変数の初期化
/**
 *	胴体
 */
	/// 同次変換行列のサイズ決定
	bodyTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
	bodyTransformation.loadIdentity();

	/// 胴体位置
	bodyPosition.setSize(THREE_DIMENSION);

	/// 胴体速度
	bodyVelocity.setSize(THREE_DIMENSION);

	/// 歩行方向
	walkingDirection.setSize(THREE_DIMENSION);

	/// 移動様式
	locomotionStyle = LEGGED;

/**
 *	脚
 */
	/// オブジェクトのポインタのメモリ領域確保
	legJointTransformation	= new Matrix*[LEG_NUM];
	legJointPosition		= new Vector*[LEG_NUM];
	
	
	/// オブジェクトのメモリ領域確保
	legBaseTransformation	= new Matrix[LEG_NUM];
	legBasePosition			= new Vector[LEG_NUM];

	legFootTransformation	= new Matrix[LEG_NUM];
	legFootPosition			= new Vector[LEG_NUM];

	legJointAngle		= new Vector[LEG_NUM];
	legJointVelocity	= new Vector[LEG_NUM];
	legJointTorque		= new Vector[LEG_NUM];
	legActuatorPosition		= new Vector[LEG_NUM];

	/// 脚の運動相
	legPhase = new LegPhase[LEG_NUM];

	FootJointAngle = new double[LEG_NUM];  //20201013

	 for (i=0; i<LEG_NUM; i++)
	 {
		/// オブジェクトのメモリ領域確保
		legJointTransformation[i]	= new Matrix[LEG_JOINT_NUM];
		legJointPosition[i]			= new Vector[LEG_JOINT_NUM];

		/// 行列のサイズ決定		
		 for (j=0; j<LEG_JOINT_NUM; j++)
		 {
			legJointTransformation[i][j].setSize(DH_DIMENSION, DH_DIMENSION);
			legJointTransformation[i][j].loadIdentity();

			legJointPosition[i][j].setSize(THREE_DIMENSION);
		 }

		 legBaseTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		 legBaseTransformation[i].loadIdentity();

		 legBasePosition[i].setSize(THREE_DIMENSION);

		 legFootTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		 legFootTransformation[i].loadIdentity();

		 legFootPosition[i].setSize(THREE_DIMENSION);

		 legJointAngle[i].setSize(LEG_JOINT_NUM);
		 legJointVelocity[i].setSize(LEG_JOINT_NUM);
		 legJointTorque[i].setSize(LEG_JOINT_NUM);
		 legActuatorPosition[i].setSize(LEG_ACT_NUM);///<----Add

		 legPhase[i] = SUPPORT;

		 //20201014
		 FootJointAngle[i] = 0;
	 }
	 //20201013
	 //FootJointAngle.setSize(LEG_NUM);///<-------------------Add
/**
 *	クローラ
 */
/*20200819
	/// クローラごとの同次変換行列と関節位置
	/// オブジェクトのポインタのメモリ領域確保
	trackJointTransformation	= new Matrix*[TRACK_NUM];
	trackJointPosition				= new Vector*[TRACK_NUM];

	/// オブジェクトのメモリ領域確保
	trackBaseTransformation	= new Matrix[TRACK_NUM];
	trackBasePosition				= new Vector[TRACK_NUM];

	trackEndTransformation		= new Matrix[TRACK_NUM];
	trackEndPosition				= new Vector[TRACK_NUM];

	trackJointAngle		= new Vector[TRACK_NUM];
	trackJointVelocity	= new Vector[TRACK_NUM];
	trackJointTorque		= new Vector[TRACK_NUM];

	trackSpeed = new double[TRACK_NUM];
	
	for (i=0; i<TRACK_NUM; i++)
	{
	 	/// オブジェクトのメモリ領域確保
		trackJointTransformation[i]	= new Matrix[TRACK_JOINT_NUM];
		trackJointPosition[i]				= new Vector[TRACK_JOINT_NUM];

		/// 行列のサイズ決定		
		for (j=0; j<TRACK_JOINT_NUM; j++)
		{
			trackJointTransformation[i][j].setSize(DH_DIMENSION, DH_DIMENSION);
			trackJointTransformation[i][j].loadIdentity();
			
			trackJointPosition[i][j].setSize(THREE_DIMENSION);
		}

		trackBaseTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		trackBaseTransformation[i].loadIdentity();

		trackBasePosition[i].setSize(THREE_DIMENSION);

		trackEndTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
		trackEndTransformation[i].loadIdentity();

		trackEndPosition[i].setSize(THREE_DIMENSION);

		trackJointAngle[i].setSize(TRACK_JOINT_NUM);
		trackJointVelocity[i].setSize(TRACK_JOINT_NUM);
		trackJointTorque[i].setSize(TRACK_JOINT_NUM);

		trackSpeed[i] = 0.0;
	}
	*/

	return;
}

/**
 *	オブジェクトのメモリ領域を解放
 */
void AsuraData::deleteAsuraData(void)
{
	/// インクリメント
	int i, j;

/**
 *	脚
 */
	/// 脚根元の同次変換行列
	delete [] legBaseTransformation;

	/// 脚関節の同次変換行列
	for (i=0; i<LEG_NUM; i++)
		delete [] legJointTransformation[i];

	delete [] legJointTransformation;

	/// 足先の同時変換行列
	delete [] legFootTransformation;

	/// 脚根元の位置ベクトル
	delete [] legBasePosition;

	/// 脚関節の位置ベクトル
	for (j=0; j<LEG_NUM; j++)
		delete [] legJointPosition[j];

	delete [] legJointPosition;

	/// 足位置
	delete [] legFootPosition;

	/// 脚関節の角度，角速度，トルクベクトル
	delete [] legJointAngle;
	delete [] legJointVelocity;
	delete [] legJointTorque;
	//delete [] FootJointAngle;//----Add

	/// 脚の運動相
	delete [] legPhase;

/**
 *	クローラ
 */
	/*  20200819
	/// クローラ根元の同次変換行列
	delete [] trackBaseTransformation;

	/// クローラの同次変換行列
	for (i=0; i<TRACK_NUM; i++)
		delete[]  trackJointTransformation[i];

	delete [] trackJointTransformation;

	/// クローラ先端の同次変換行列
	delete [] trackEndTransformation;

	/// クローラ根元位置
	delete [] trackBasePosition;

	/// クローラ関節の位置ベクトル
	for (j=0; j<TRACK_NUM; j++)
		delete [] trackJointPosition[j];

	delete [] trackJointPosition;

	/// クローラ先端位置
	delete [] trackEndPosition;

	/// クローラ関節の角度，角速度，トルクベクトル
	delete [] trackJointAngle;
	delete [] trackJointVelocity;
	delete [] trackJointTorque;

	/// クローラ速度
	delete [] trackSpeed;
	*/

	return;
}
/**
 *	コピーコンストラクタと代入演算子のヘルプ関数
 */
void AsuraData::copy(const AsuraData& asuraData)
{
	/// インクリメント変数
	int i, j;

	/**
	 *		胴体
	 */
	bodyTransformation	= asuraData.bodyTransformation;
	bodyPosition			= asuraData.bodyPosition;
	bodyVelocity			= asuraData.bodyVelocity;
	walkingDirection		= asuraData.walkingDirection;
	locomotionStyle		= asuraData.locomotionStyle;

	/**
	 *		脚
	 */
	for (i=0; i<LEG_NUM; i++)
	{
		legBaseTransformation[i]	= asuraData.legBaseTransformation[i];
		legBasePosition[i]				= asuraData.legBasePosition[i];

		for (j=0; j<LEG_JOINT_NUM; j++)
		{
			legJointTransformation[i][j]	 = asuraData.legJointTransformation[i][j];
			legJointPosition[i][j]			 = asuraData.legJointPosition[i][j];
		}
		for (j=0; j<LEG_ACT_NUM; j++)
		{
			legActuatorPosition[i]		 = asuraData.legActuatorPosition[i];//---Add
		}
		legFootTransformation[i] = asuraData.legFootTransformation[i];
		legFootPosition[i] = asuraData.legFootPosition[i];

		legJointAngle[i]		= asuraData.legJointAngle[i];
		legJointVelocity[i]		= asuraData.legJointVelocity[i];
		legJointTorque[i]		= asuraData.legJointTorque[i];
		FootJointAngle[i]		= asuraData.FootJointAngle[i];//---Add

		legPhase[i] = asuraData.legPhase[i];
	}

	/**
	 *		クローラ
	 */
	/*20200819
	for (i=0; i<TRACK_NUM; i++)
	{
		trackBaseTransformation[i]	= asuraData.trackBaseTransformation[i];
		trackBasePosition[i]				= asuraData.trackBasePosition[i];

		for (j=0; j<TRACK_JOINT_NUM; j++)
		{
			trackJointTransformation[i][j]	= asuraData.trackJointTransformation[i][j];
			trackJointPosition[i][j]				= asuraData.trackJointPosition[i][j];
		}

		trackEndTransformation[i]		= asuraData.trackEndTransformation[i];
		trackEndPosition[i]				= asuraData.trackEndPosition[i];

		trackJointAngle[i]		= asuraData.trackJointAngle[i];
		trackJointVelocity[i]		= asuraData.trackJointVelocity[i];
		trackJointTorque[i]		= asuraData.trackJointTorque[i];

		trackSpeed[i] = asuraData.trackSpeed[i];
	}
	*/

	return;
}

}	/// end of namespace Asura