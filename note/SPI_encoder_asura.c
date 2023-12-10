
#include <check_pin_asura.h>
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include <stdlib.h>
#include <string.h>
#include <SPI_encoder_asura.h>



//ENC_count EncoderStructure = {0};//!
#define SPI_WAIT_CYCLE 10
///wait�̃T�C�N��

/**
 *	/////////////////////////////////////////////////
 *		�v���g�^�C�v�錾
 *	//////////////////////////////////////////////////
 */
//void spi_wait(unsigned int wait_num);
uint16_t spi_get_uint16(SPI_CS ch);
void spi_init16(void);
/**
 *	//////////////////////////////////////////////////
 *		�֐��̒�`
 *	//////////////////////////////////////////////////
 */

//GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,LED4_GPIO_PORT};
/***********
 *	������
 ***********/
void spi_init16(void){

//*** spi_pin_confin ***
	GPIO_InitTypeDef  		GPIO_InitStrct;

	//*** spi_CS0 ***
	GPIO_InitStrct.GPIO_Pin 	= SPI_CS0_PIN_MISO;
	GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(SPI_CS0_GPIOPort, &GPIO_InitStrct);

	GPIO_InitStrct.GPIO_Pin 	= SPI_CS0_PIN_CLK |SPI_CS0_PIN_NCS;
	GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(SPI_CS0_GPIOPort, &GPIO_InitStrct);

	//NCS&CLK _are_high
	GPIO_SetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_NCS);
	GPIO_SetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_CLK);
	//GPIO_ResetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_MISO);

	//*** spi_CS1 ***
	GPIO_InitStrct.GPIO_Pin 	= SPI_CS1_PIN_MISO;
	GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(SPI_CS1_GPIOPort, &GPIO_InitStrct);

	GPIO_InitStrct.GPIO_Pin 	= SPI_CS1_PIN_CLK |SPI_CS1_PIN_NCS;
	GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(SPI_CS1_GPIOPort, &GPIO_InitStrct);

	//NCS&CLK _are_high
	GPIO_SetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_NCS);
	GPIO_SetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_CLK);

}

/**********************************************************
 *	uint16_t spi_get_uint16(SPI_CS ch)�G
 *	�}�O�l�b�g�G���R�[�_�̃f�[�^ �Ǐo���֐�
 *
 *	NCS:�f�[�^���M�ҋ@�v��
 *	CLK��LOW�ɂȂ����ꍇ��MISO(�f�[�^�M����)�̃��x����ǂݎ��
 *	1bit�ǂݎ������CLK��HIGH/LOW��؂�ւ����̌���MISO�̃��x����ǂݎ��
 *	16�񏈗��œǂݎ��I��
 *
 ***********************************************************/
uint16_t spi_get_uint16(SPI_CS ch){//
	//check_pin_low();
	//*** ���[�J���ϐ�
	uint16_t read_data = 255;

	uint8_t PIN_LEVEL_1;
	__IO uint32_t Port;
	uint32_t NCS;
	uint32_t CLK;
	uint32_t MISO;

	//*** �`�b�v�Z���N�gON ***
	switch(ch){
		case SPI_CS0:{
			//*** NCS��LOW ******
			GPIO_ResetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_NCS);

			//delay_usec(10);//CLK��LOW�ɗ��������܂ő҂�

			//*** CLK��LOW�� ******
			GPIO_ResetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_CLK);

			delay_usec(1);//CLK��LOW�ɗ��������܂ő҂�

			//*** CLK��HIGH�ɂ��� ******
			GPIO_SetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_CLK);//PIN_CLK_is_high//PIN_CLK=1;

			delay_usec(1);//CLK��HIGH�ɗ��������܂ő҂�

			//*** Get_16bit ******
			for (int i=0; i<16; i++){
				//check_pin_low();
				//*** CLK��Low ***
				//GPIO_ResetBits (Port, CLK);
				GPIO_ResetBits (SPI_CS0_GPIOPort,SPI_CS0_PIN_CLK);
				delay_usec(1);//1

				PIN_LEVEL_1 = GPIO_ReadInputDataBit(SPI_CS0_GPIOPort, SPI_CS0_PIN_MISO);	//MISO�̃��x����ǂݎ��PIN_LEVEL_1�ɑ��
				read_data<<=1;	//���V�t�g

				//*** MISO��High�Ȃ�read_data��1���}�X�N ******
				if(PIN_LEVEL_1==SET){
					read_data |= 0x0001; //0b 0000 0000 0000 0001
				}
				//*** MISO��High�ȊO�Ȃ�read_data��0���}�X�N ******
				else{
					read_data &= ~0x0001;//0b 1111 1111 1111 1110
				}

				//*** PIN_CLK_is_high �f�[�^�X�V ******
				//GPIO_SetBits (Port, CLK);
				GPIO_SetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_CLK);
				delay_usec(1);//CLK��HIGH�ɗ��������܂ő҂�1
				//check_pin_high();
			}

			//*** �N���b�N��High�ɌŒ� ******
			GPIO_SetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_CLK);//PIN_CLK_is_high//PIN_CLK = 1;

			delay_usec(10);//MISO��High�ɂȂ�܂őҋ@
			//delay_usec(5);
			//*** �`�b�v�Z���N�gOFF *********
			GPIO_SetBits (SPI_CS0_GPIOPort, SPI_CS0_PIN_NCS);//PIN_NCS_is_HIGH
			//return	read_data;
		}
		break;

		case SPI_CS1:{
						//*** NCS��LOW ******
						GPIO_ResetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_NCS);

						delay_usec(1);//CLK��LOW�ɗ��������܂ő҂�

						//*** CLK��LOW�� ******
						GPIO_ResetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_CLK);

						delay_usec(1);//CLK��LOW�ɗ��������܂ő҂�

						//*** CLK��HIGH�ɂ��� ******
						GPIO_SetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_CLK);//PIN_CLK_is_high//PIN_CLK=1;

						delay_usec(1);//CLK��HIGH�ɗ��������܂ő҂�

						//*** Get_16bit ******
						for (int i=0; i<16; i++){
							//check_pin_low();
							//*** CLK��Low ***
							//GPIO_ResetBits (Port, CLK);
							GPIO_ResetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_CLK);
							delay_usec(1);

							PIN_LEVEL_1 = GPIO_ReadInputDataBit(SPI_CS1_GPIOPort, SPI_CS1_PIN_MISO);	//MISO�̃��x����ǂݎ��PIN_LEVEL_1�ɑ��
							read_data<<=1;	//���V�t�g

							//*** MISO��High�Ȃ�read_data��1���}�X�N ******
							if(PIN_LEVEL_1==SET){
								read_data |= 0x0001; //0b 0000 0000 0000 0001
							}
							//*** MISO��High�ȊO�Ȃ�read_data��0���}�X�N ******
							else{
								read_data &= ~0x0001;//0b 1111 1111 1111 1110
							}

							//*** PIN_CLK_is_high �f�[�^�X�V ******
							//GPIO_SetBits (Port, CLK);
							GPIO_SetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_CLK);
							delay_usec(1);//CLK��HIGH�ɗ��������܂ő҂�
							//check_pin_high();
						}

						//*** �N���b�N��High�ɌŒ� ******
						GPIO_SetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_CLK);//PIN_CLK_is_high//PIN_CLK = 1;

						delay_usec(11);//MISO��High�ɂȂ�܂őҋ@

						//*** �`�b�v�Z���N�gOFF *********
						GPIO_SetBits (SPI_CS1_GPIOPort, SPI_CS1_PIN_NCS);//PIN_NCS_is_HIGH
						//return	read_data;
						//read_data = 123;
		}
		break;
		default:
		break;
	}
	//check_pin_high();
	return	read_data;//1234;//
}
