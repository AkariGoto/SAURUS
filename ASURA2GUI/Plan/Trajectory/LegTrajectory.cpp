/**
 *  File: LegTrajectory.cpp
 *
 *  Description: 脚軌道情報を扱うクラスの定義部
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#include "LegTrajectory.h"


using namespace std;
using namespace Math;
using namespace Const;

namespace Plan
{
/**
 *		コンストラクタとデストラクタ
 */
/// デフォルトコンストラクタ（実引数の指定が不要なコンストラクタ、仮引数が宣言されていないコンストラクタはデフォルトコンストラクタとなる）

LegTrajectory::LegTrajectory()//LegTrajectoryクラスのメンバ関数LegTrajectory()
{
	initializeLegTrajectory();
}

/// コピーコンストラクタ
LegTrajectory::LegTrajectory(const LegTrajectory& trajectory)
{
	initializeLegTrajectory();
	copy(trajectory);
}

/// デストラクタ
LegTrajectory::~LegTrajectory()
{
}

/**
 *		演算子
 */
/// 代入演算子
LegTrajectory& LegTrajectory::operator=(const LegTrajectory& trajectory)
{
	/// 自己代入を防ぐ
	if ( &trajectory != this )
	{
		copy(trajectory);
	}

	return *this;
}

/// 等価演算子
bool LegTrajectory::operator==(const LegTrajectory& trajectory) const
{
	if ( returnStroke != trajectory.returnStroke ){return false;}
	if ( upSwing != trajectory.upSwing ){return false;}
	if ( downSwing != trajectory.downSwing ){return false;}

	if ( returnTime != trajectory.returnTime ){return false;}
	if ( upTime != trajectory.upTime ){return false;}
	if ( downTime != trajectory.downTime ){return false;}

	return true;
}

/// 非等価演算子
bool LegTrajectory::operator!=(const LegTrajectory& trajectory) const
{
	if ( *this == trajectory )
		return false;
	else 
		return true;
}


/**
 *		初期化
 */
void LegTrajectory::initializeLegTrajectory(void)
{
/**
 *		メンバ変数の初期化
 */
	/// 遊脚ストローク
	returnStroke.setSize(THREE_DIMENSION);
	/// 遊脚振り上げ高さ
	upSwing.setSize(THREE_DIMENSION);
	/// 遊脚振り下げ高さ
	downSwing.setSize(THREE_DIMENSION);

	Swing.setSize(THREE_DIMENSION);

	/// 復帰時間
	returnTime = 0.0;
	/// 振り上げ時間
	upTime = 0.0;
	/// 振り下げ時間
	downTime = 0.0;
}

/**
 *		軌道パラメータの設定
 */
void LegTrajectory::setLegTrajectory
	(
		const Math::Vector& start,
		const Math::Vector& upPhase, 
		const Math::Vector& returnPhase, 
		const Math::Vector& downPhase

		//const Math::Vector& swingPhase
	)
{
	/// メンバ変数の更新
	startPosition = start;

	upSwing = upPhase;
	returnStroke = returnPhase;
	downSwing = downPhase;

	/// ゴール位置を更新する
	goalPosition = startPosition + upSwing + returnStroke + downSwing ;

	return;
}

void LegTrajectory::setLegTrajectoryTime(double start, double upPhase, double returnPhase, double downPhase)
{
	/// メンバ変数の更新
	startTime = start;

	upTime = upPhase;
	returnTime = returnPhase;
	downTime = downPhase;

	/// ゴール時間を更新する
	goalTime = startTime + upTime + returnTime + downTime;

	return;
}

/**
 *		軌道上の位置を得る
 *		splitTime: 軌道中の時間（startTime < time < goalTime）
 */
Math::Vector LegTrajectory::getPosition(double splitTime)
{
	
	/// 取り出す脚先軌道
	Vector trajectory(THREE_DIMENSION);

	/*	合成サイクロイド軌道
	*		離脱,接地時の速度が連続. 遊脚時間の場合分けが2パターンで済む.
	*		[引用] 榊原義宏, 菅和俊, 細田祐司, 服部誠, 藤江正克, "4脚歩行機構のための低衝撃足先軌",日本ロボット学会誌 8巻6号 ,pp22-31,1990
	*/
		//*  20221219
		///遊脚の中間時間
		double MiddleTime	= ( startTime + goalTime )/2.00;//+dutyFactor
		///遊脚時間
		double SwingTime	=  goalTime - startTime ;//Ts

		/// 遊脚開始前
		if ( splitTime <= startTime )
		{			
			trajectory = startPosition;
		}
		/// up相
		else if ( ( startTime < splitTime ) && ( splitTime <= MiddleTime ) )//( splitTime < goalTime ) )//
		{
			trajectory = startPosition 
						+ ( (splitTime - startTime)/SwingTime - sin(4 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/4 ) * upSwing * 2 
						+ ( (splitTime - startTime)/SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )* returnStroke;
		}
		/// down相	*downSwingの中身は負
		else if ( ( MiddleTime < splitTime) && ( splitTime < goalTime ) )
		{
			trajectory = startPosition +upSwing
						+ (  ( splitTime - MiddleTime)/SwingTime - sin(4 * Const::PI * (splitTime - MiddleTime)/SwingTime )/ Const::PI/4 ) *downSwing *2
						+ ( (splitTime - startTime)/SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )*returnStroke;
		}
		/// 遊脚終了
		else if (goalTime <= splitTime)
		{
			trajectory = goalPosition;
		} 
		//*/
		

		

       /********変形サイクロイド軌道*******/

/*****MiddleTimeにおける加速度の変化を小さくする****/

    /*    if(splitTime <= startTime)
		{
			trajectory = startPosition ;
		}

		else if( ( startTime < splitTime ) && ( splitTime <= startTime + SwingTime/4 ) ) //up=>ok
		{
			trajectory = startPosition 
				//+ (upSwing / (4 + Const::PI)) * (4 * Const::PI*( (splitTime -startTime)/ SwingTime) - sin(4*Const::PI*( (splitTime - startTime )/ SwingTime)) )
				+ ( (splitTime - startTime)/SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )* returnStroke;
		}

		else if( ( startTime + SwingTime/4 < splitTime ) && ( splitTime <= startTime + MiddleTime ) ) //up=>ok
		{
			trajectory = startPosition  
				//+ (4 * upSwing /(4 + Const::PI)) * ( sin(2 * Const::PI* ((splitTime - startTime)/ SwingTime) - Const::PI/2) + Const::PI/4 )
				+ ( (splitTime - startTime)/SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )* returnStroke;
		}

		else if( ( startTime + MiddleTime < splitTime ) && ( splitTime <= startTime + 3*SwingTime/4 ) )//down
		{
			trajectory = startPosition
				//+ (4 * upSwing / (4 + Const::PI)) * ( sin( 2 * Const::PI * (splitTime - startTime) / SwingTime - Const::PI/2 ) + Const::PI/4 ) 
				+ ( (splitTime - startTime)/SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )* returnStroke;
		}

		else if( ( startTime + 3*SwingTime/4 < splitTime ) && ( splitTime < goalTime ) )//down
		{
			trajectory = startPosition
				//+ ( 4 * Const::PI * upSwing / (4 + Const::PI) ) * ( (1 - ((splitTime - startTime) / SwingTime) ) - (1/4/Const::PI) * sin(4*Const::PI*( 1 - ((splitTime - startTime) /SwingTime) ) ) )
				+ ( (splitTime - startTime) / SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )*returnStroke;
		}

		else if( goalTime <= splitTime)
		{
			trajectory = goalPosition ;
		}
		*/

	/*  20221219
	//  20200822  脚先離脱点よりも脚先着地点が高い場合or低い場合
	if (upSwing[2] != -downSwing[2])
	{
		///遊脚の頂点中点に行った時の時間
		double MiddleTime = startTime + (goalTime - startTime) * 3 / 4;//( startTime + goalTime );

		///遊脚時間
		double SwingTime = goalTime - startTime;//Ts

		double first, second;

		//printf("MiddleTime%lf \n SwingTime%lf",MiddleTime,SwingTime);

		if (upSwing[2] > -downSwing[2])
		{
			first = SwingTime * 3 / 5;  //up用
			second = SwingTime * 2 / 5; //up用
		}
		else
		{
			first = SwingTime * 2 / 5; //down
			second = SwingTime * 3 / 5; //down
		}
		

	   // my trajectory
		if (splitTime <= startTime)
		{
			trajectory = startPosition;
			//cout << trajectory[0]<<","<<trajectory[1]<<","<<trajectory[2]<<endl;
		}

		else if ((startTime < splitTime) && (splitTime <= startTime + first / 2)) //up
		{
			trajectory = startPosition
				+ (upSwing / (4 + Const::PI)) * (4 * Const::PI * ((splitTime - startTime) / first / 2) - sin(4 * Const::PI * ((splitTime - startTime) / first / 2))); //Z
			 // printf("1111\n");
			 // cout << trajectory[0]<<","<<trajectory[1]<<","<<trajectory[2]<<endl;
		}

		else if ((startTime + first / 2 < splitTime) && (splitTime <= startTime + first)) //up
		{
			trajectory = startPosition
				+ (4 * upSwing / (4 + Const::PI)) * (sin(2 * Const::PI * ((splitTime - startTime) / first / 2) - Const::PI / 2) + Const::PI / 4) //Z
				+ returnStroke * (2 * ((splitTime - startTime) / first / 2) - (1 / (2 * Const::PI)) * sin(4 * Const::PI * (((splitTime - startTime) / first / 2) - 0.25)) - 0.5); //X
		 //printf("2222\n");
		 //cout << trajectory[0]<<","<<trajectory[1]<<","<<trajectory[2]<<endl;


		}
		else if ((startTime + first < splitTime) && (splitTime <= startTime + first + second / 2)) //down
		{
			trajectory = startPosition + upSwing + downSwing
				- (4 * downSwing / (4 + Const::PI)) * (sin(2 * Const::PI * ((splitTime - startTime - first + second)) / second / 2 - Const::PI / 2) + Const::PI / 4) //Z
				+ returnStroke * (2 * ((splitTime - startTime - first + second) / second / 2) - (1 / (2 * Const::PI)) * sin(4 * Const::PI * (((splitTime - startTime - first + second) / second / 2) - 0.25)) - 0.5);//X
			//printf("3333\n");
			//cout << trajectory[0]<<","<<trajectory[1]<<","<<trajectory[2]<<endl;
		}

		else if ((startTime + first + second / 2 < splitTime) && (splitTime <= goalTime)) //down
		{
			trajectory = startPosition + upSwing + downSwing
				- (4 * Const::PI * downSwing / (4 + Const::PI)) * ((1 - ((splitTime - startTime - first + second) / second / 2)) - (1 / 4 / Const::PI) * sin(4 * Const::PI * (1 - ((splitTime - startTime - first + second) / second / 2)))) //Z
				+ returnStroke; //X
			//printf("4444\n");
			//cout << trajectory[0]<<","<<trajectory[1]<<","<<trajectory[2]<<endl;
		}

		else if (goalTime < splitTime)
		{
			trajectory = goalPosition;
			// cout << trajectory[0]<<","<<trajectory[1]<<","<<trajectory[2]<<endl;
		}

		if (trajectory[1] < -700)
		{

			cout << trajectory[0] << "," << trajectory[1] << "," << trajectory[2] << endl;
		}

		if (trajectory[1] < -500)
		{
			cout << trajectory[0] << "," << trajectory[1] << "," << trajectory[2] << endl;
		}
	}
	//  20221219  */
		/*
		//遊脚の頂点中点に行った時の時間
		double MiddleTime = startTime + (goalTime - startTime) * 2 / 3;//( startTime + goalTime );
		///遊脚時間
		double SwingTime = goalTime - startTime;//Ts

		// my trajectory(これで3パターン, PlanParameter.h の振り上げと振り下げ高さの値をいじって再現)
		if (splitTime <= startTime)
		{
			trajectory = startPosition;
		}
		else if ((startTime < splitTime) && (splitTime <= MiddleTime / 2)) //up
		{
			trajectory = startPosition
				+ (upSwing / (4 + Const::PI)) * (4 * Const::PI * ((splitTime - startTime) / SwingTime) - sin(4 * Const::PI * ((splitTime - startTime) / SwingTime))); //Z

		}
		else if ((MiddleTime / 2 < splitTime) && (splitTime <= MiddleTime)) //up
		{
			trajectory = startPosition
				+ (4 * upSwing / (4 + Const::PI)) * (sin(2 * Const::PI * ((splitTime - startTime) / SwingTime) - Const::PI / 2) + Const::PI / 4) //Z
				+ returnStroke * (2 * ((splitTime - startTime) / SwingTime) - (1 / (2 * Const::PI)) * sin(4 * Const::PI * (((splitTime - startTime) / SwingTime) - 0.25)) - 0.5); //X
		}
		else if ((MiddleTime < splitTime) && (splitTime <= MiddleTime * 5 / 8)) //down
		{
			trajectory = startPosition + upSwing + downSwing
				- (4 * downSwing / (4 + Const::PI)) * (sin(2 * Const::PI * ((splitTime - startTime)) / SwingTime - Const::PI / 2) + Const::PI / 4) //Z
				+ returnStroke * (2 * ((splitTime - startTime) / SwingTime) - (1 / (2 * Const::PI)) * sin(4 * Const::PI * (((splitTime - startTime) / SwingTime) - 0.25)) - 0.5);//X

		}
		else if ((MiddleTime * 5 / 8 < splitTime) && (splitTime <= goalTime)) //down
		{
			trajectory = startPosition + upSwing + downSwing
				- (4 * Const::PI * downSwing / (4 + Const::PI)) * ((1 - ((splitTime - startTime) / SwingTime)) - (1 / 4 / Const::PI) * sin(4 * Const::PI * (1 - ((splitTime - startTime) / SwingTime)))) //Z
				+ returnStroke; //X

		}
		else if (goalTime < splitTime)
		{
			trajectory = goalPosition;
			//startPosition + upSwing + returnStroke +downSwing
			//- ( 4 * Const::PI * downSwing / (4 + Const::PI) ) * ( (1 - ((splitTime - startTime) / SwingTime) ) - (1/4/Const::PI) * sin(4*Const::PI*( 1 - ((splitTime - startTime) /SwingTime) ) ) );

		}

		*/



	   //高低差なし
		/*
		if (splitTime <= startTime)
		{
			trajectory = startPosition;
		}
		else if ((startTime < splitTime) && (splitTime <= startTime + SwingTime / 4)) //up
		{
			trajectory = startPosition
				+ (upSwing / (4 + Const::PI)) * (4 * Const::PI * ((splitTime - startTime) / SwingTime) - sin(4 * Const::PI * ((splitTime - startTime) / SwingTime))); //Z

		}
		else if ((startTime + SwingTime / 4 < splitTime) && (splitTime <= MiddleTime)) //up
		{
			trajectory = startPosition
				+ (4 * upSwing / (4 + Const::PI)) * (sin(2 * Const::PI * ((splitTime - startTime) / SwingTime) - Const::PI / 2) + Const::PI / 4) //Z
				+ returnStroke * (2 * ((splitTime - startTime) / SwingTime) - (1 / (2 * Const::PI)) * sin(4 * Const::PI * (((splitTime - startTime) / SwingTime) - 0.25)) - 0.5); //X
		}
		else if ((MiddleTime < splitTime) && (splitTime <= startTime + 3 * SwingTime / 4)) //down
		{
			trajectory = startPosition + upSwing + downSwing
				- (4 * downSwing / (4 + Const::PI)) * (sin(2 * Const::PI * ((splitTime - startTime)) / SwingTime - Const::PI / 2) + Const::PI / 4) //Z
				+ returnStroke * (2 * ((splitTime - startTime) / SwingTime) - (1 / (2 * Const::PI)) * sin(4 * Const::PI * (((splitTime - startTime) / SwingTime) - 0.25)) - 0.5);//X
		}
		else if ((startTime + 3 * SwingTime / 4 < splitTime) && (splitTime < startTime + goalTime)) //down
		{
			trajectory = startPosition + upSwing
				- (4 * Const::PI * downSwing / (4 + Const::PI)) * ((1 - ((splitTime - 3 * SwingTime / 4) / SwingTime)) - (1 / 4 / Const::PI) * sin(4 * Const::PI * (1 - ((splitTime - startTime) / SwingTime)))) //Z
				+ returnStroke; //X

		}
		else if (goalTime <= splitTime)
		{
			trajectory = goalPosition;
		}
		*/
	

	/*  20221219
		else
		{
			// my trajectory==>>最終版（平面）
			//遊脚時間
			double SwingTime = goalTime - startTime;//Ts

			if (splitTime <= startTime)
			{
				trajectory = startPosition;
			}
			else if ((startTime < splitTime) && (splitTime <= startTime + SwingTime / 4)) //up
			{
				trajectory = startPosition
					+ (upSwing / (4 + Const::PI)) * (4 * Const::PI * ((splitTime - startTime) / SwingTime) - sin(4 * Const::PI * ((splitTime - startTime) / SwingTime))); //Z

			}
			else if ((startTime + SwingTime / 4 < splitTime) && (splitTime <= startTime + 3 * SwingTime / 4)) //up
			{
				trajectory = startPosition
					+ (4 * upSwing / (4 + Const::PI)) * (sin(2 * Const::PI * ((splitTime - startTime) / SwingTime) - Const::PI / 2) + Const::PI / 4)  //Z
					+ returnStroke * (2 * ((splitTime - startTime) / SwingTime) - (1 / (2 * Const::PI)) * sin(4 * Const::PI * (((splitTime - startTime) / SwingTime) - 0.25)) - 0.5); //X
			}
			else if ((startTime + 3 * SwingTime / 4 < splitTime) && (splitTime < startTime + goalTime)) //down
			{
				trajectory = startPosition
					+ (4 * Const::PI * upSwing / (4 + Const::PI)) * ((1 - ((splitTime - startTime) / SwingTime)) - (1 / 4 / Const::PI) * sin(4 * Const::PI * (1 - ((splitTime - startTime) / SwingTime)))) //Z
					+ returnStroke; //X
			}
			else if (goalTime <= splitTime)
			{
				trajectory = goalPosition;
			}

		}
	//  20221219  */   
	   


	/*   if(splitTime <= startTime)
	   {
			trajectory = startPosition ;
	   }
	   else if( ( startTime < splitTime ) && ( splitTime <= startTime + SwingTime/4 )) //up
	   {
		   trajectory = startPosition  
			  + (upSwing / (4 + Const::PI)) * (4 * Const::PI*( (splitTime -startTime)/ SwingTime) - sin(4*Const::PI*( (splitTime - startTime )/ SwingTime)) );

	   }
	   else if(( startTime + SwingTime/4 < splitTime ) && ( splitTime <= startTime + 3*SwingTime/4 ) ) //up
	   {
		   trajectory = startPosition  
			  + (4 * upSwing /(4 + Const::PI)) * ( sin(2 * Const::PI* ((splitTime - startTime)/ SwingTime) - Const::PI/2) + Const::PI/4 );
			  + 2 * returnStroke * (  ((splitTime -startTime)/SwingTime) - (1/4/Const::PI)*sin(4 * Const::PI*( ((splitTime - startTime)/SwingTime ) - 0.25) ) - 0.25 );
	   }
	  
	   else if(( startTime + 3*SwingTime/4 < splitTime ) && ( splitTime < goalTime )) //down
	   {
		    trajectory = startPosition 
				+ ( 4 * Const::PI * upSwing / (4 + Const::PI) ) * ( (1 - ((splitTime - startTime) / SwingTime) ) - (1/4/Const::PI) * sin(4*Const::PI*( 1 - ((splitTime - startTime) /SwingTime) ) ) )
				 + returnStroke ;
	   }

	   else if( goalTime <= splitTime)
	   {
		    trajectory = goalPosition ;
	   }
	   */
	  
	return trajectory;

}


/**
 *		LegTrajectoryクラスのprivateなメンバ関数
 */
void LegTrajectory::copy(const LegTrajectory& trajectory)
{
	/// 遊脚ストローク
	returnStroke = trajectory.returnStroke;
	/// 遊脚振り上げ高さ
	upSwing = trajectory.upSwing;
	/// 遊脚振り下げ高さ
	downSwing = trajectory.downSwing;

	/// 復帰時間
	returnTime = trajectory.returnTime;
	/// 振り上げ時間
	upTime = trajectory.upTime;
	/// 振り下げ時間
	downTime = trajectory.downTime;
}

}	/// end of namespace Plan