/**
 *  File: LegTrajectory.cpp
 *
 *  Description: �r�O�����������N���X�̒�`��
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
 *		�R���X�g���N�^�ƃf�X�g���N�^
 */
/// �f�t�H���g�R���X�g���N�^�i�������̎w�肪�s�v�ȃR���X�g���N�^�A���������錾����Ă��Ȃ��R���X�g���N�^�̓f�t�H���g�R���X�g���N�^�ƂȂ�j

LegTrajectory::LegTrajectory()//LegTrajectory�N���X�̃����o�֐�LegTrajectory()
{
	initializeLegTrajectory();
}

/// �R�s�[�R���X�g���N�^
LegTrajectory::LegTrajectory(const LegTrajectory& trajectory)
{
	initializeLegTrajectory();
	copy(trajectory);
}

/// �f�X�g���N�^
LegTrajectory::~LegTrajectory()
{
}

/**
 *		���Z�q
 */
/// ������Z�q
LegTrajectory& LegTrajectory::operator=(const LegTrajectory& trajectory)
{
	/// ���ȑ����h��
	if ( &trajectory != this )
	{
		copy(trajectory);
	}

	return *this;
}

/// �������Z�q
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

/// �񓙉����Z�q
bool LegTrajectory::operator!=(const LegTrajectory& trajectory) const
{
	if ( *this == trajectory )
		return false;
	else 
		return true;
}


/**
 *		������
 */
void LegTrajectory::initializeLegTrajectory(void)
{
/**
 *		�����o�ϐ��̏�����
 */
	/// �V�r�X�g���[�N
	returnStroke.setSize(THREE_DIMENSION);
	/// �V�r�U��グ����
	upSwing.setSize(THREE_DIMENSION);
	/// �V�r�U�艺������
	downSwing.setSize(THREE_DIMENSION);

	Swing.setSize(THREE_DIMENSION);

	/// ���A����
	returnTime = 0.0;
	/// �U��グ����
	upTime = 0.0;
	/// �U�艺������
	downTime = 0.0;
}

/**
 *		�O���p�����[�^�̐ݒ�
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
	/// �����o�ϐ��̍X�V
	startPosition = start;

	upSwing = upPhase;
	returnStroke = returnPhase;
	downSwing = downPhase;

	/// �S�[���ʒu���X�V����
	goalPosition = startPosition + upSwing + returnStroke + downSwing ;

	return;
}

void LegTrajectory::setLegTrajectoryTime(double start, double upPhase, double returnPhase, double downPhase)
{
	/// �����o�ϐ��̍X�V
	startTime = start;

	upTime = upPhase;
	returnTime = returnPhase;
	downTime = downPhase;

	/// �S�[�����Ԃ��X�V����
	goalTime = startTime + upTime + returnTime + downTime;

	return;
}

/**
 *		�O����̈ʒu�𓾂�
 *		splitTime: �O�����̎��ԁistartTime < time < goalTime�j
 */
Math::Vector LegTrajectory::getPosition(double splitTime)
{
	
	/// ���o���r��O��
	Vector trajectory(THREE_DIMENSION);

	/*	�����T�C�N���C�h�O��
	*		���E,�ڒn���̑��x���A��. �V�r���Ԃ̏ꍇ������2�p�^�[���ōς�.
	*		[���p] �匴�`�G, ���a�r, �דc�S�i, ������, ���]����, "4�r���s�@�\�̂��߂̒�Ռ�����O",���{���{�b�g�w� 8��6�� ,pp22-31,1990
	*/
		//*  20221219
		///�V�r�̒��Ԏ���
		double MiddleTime	= ( startTime + goalTime )/2.00;//+dutyFactor
		///�V�r����
		double SwingTime	=  goalTime - startTime ;//Ts

		/// �V�r�J�n�O
		if ( splitTime <= startTime )
		{			
			trajectory = startPosition;
		}
		/// up��
		else if ( ( startTime < splitTime ) && ( splitTime <= MiddleTime ) )//( splitTime < goalTime ) )//
		{
			trajectory = startPosition 
						+ ( (splitTime - startTime)/SwingTime - sin(4 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/4 ) * upSwing * 2 
						+ ( (splitTime - startTime)/SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )* returnStroke;
		}
		/// down��	*downSwing�̒��g�͕�
		else if ( ( MiddleTime < splitTime) && ( splitTime < goalTime ) )
		{
			trajectory = startPosition +upSwing
						+ (  ( splitTime - MiddleTime)/SwingTime - sin(4 * Const::PI * (splitTime - MiddleTime)/SwingTime )/ Const::PI/4 ) *downSwing *2
						+ ( (splitTime - startTime)/SwingTime - sin(2 * Const::PI * (splitTime - startTime)/SwingTime )/ Const::PI/2  )*returnStroke;
		}
		/// �V�r�I��
		else if (goalTime <= splitTime)
		{
			trajectory = goalPosition;
		} 
		//*/
		

		

       /********�ό`�T�C�N���C�h�O��*******/

/*****MiddleTime�ɂ���������x�̕ω�������������****/

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
	//  20200822  �r�旣�E�_�����r�撅�n�_�������ꍇor�Ⴂ�ꍇ
	if (upSwing[2] != -downSwing[2])
	{
		///�V�r�̒��_���_�ɍs�������̎���
		double MiddleTime = startTime + (goalTime - startTime) * 3 / 4;//( startTime + goalTime );

		///�V�r����
		double SwingTime = goalTime - startTime;//Ts

		double first, second;

		//printf("MiddleTime%lf \n SwingTime%lf",MiddleTime,SwingTime);

		if (upSwing[2] > -downSwing[2])
		{
			first = SwingTime * 3 / 5;  //up�p
			second = SwingTime * 2 / 5; //up�p
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
		//�V�r�̒��_���_�ɍs�������̎���
		double MiddleTime = startTime + (goalTime - startTime) * 2 / 3;//( startTime + goalTime );
		///�V�r����
		double SwingTime = goalTime - startTime;//Ts

		// my trajectory(�����3�p�^�[��, PlanParameter.h �̐U��グ�ƐU�艺�������̒l���������čČ�)
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



	   //���፷�Ȃ�
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
			// my trajectory==>>�ŏI�Łi���ʁj
			//�V�r����
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
 *		LegTrajectory�N���X��private�ȃ����o�֐�
 */
void LegTrajectory::copy(const LegTrajectory& trajectory)
{
	/// �V�r�X�g���[�N
	returnStroke = trajectory.returnStroke;
	/// �V�r�U��グ����
	upSwing = trajectory.upSwing;
	/// �V�r�U�艺������
	downSwing = trajectory.downSwing;

	/// ���A����
	returnTime = trajectory.returnTime;
	/// �U��グ����
	upTime = trajectory.upTime;
	/// �U�艺������
	downTime = trajectory.downTime;
}

}	/// end of namespace Plan