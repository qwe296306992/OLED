
#ifndef __OLED_H
#define __OLED_H			  	 

#include "stm32l1xx.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	

#define Delay 0


//-----------------OLED IIC�˿ڶ���----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)//ѡGPIOC2ΪSCL IIC�ӿڵ�ʱ���ź�
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//ѡGPIOC6ΪSDA IIC�ӿڵ������ź�
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)

//�޸�GPIO�˿��Լ�RCC����
#define RCC_OLED_GPIO   RCC_AHBPeriph_GPIOB
#define RCC_Fun_Cmd     RCC_AHBPeriphClockCmd

//�޸�OLED��I2C����,�˿ڣ����ģʽ
#define OLED_SDA_Pin    GPIO_Pin_7
#define OLED_SCL_Pin    GPIO_Pin_6
#define OLED_GPIO_Port  GPIOB
#define OLED_GPIO_Mode  GPIO_Mode_Out_OD

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//����ʱ��ṹ��

/*ͨ��ʱ��ṹ��DataTime ����ֵ
 �ýṹ���Ա�� 
Mounth�·ݣ�1-12 
Day�죺1-31 
ClkСʱ��1-24 
Min���ӣ�0-59
Signal�ź�ǿ�ȣ�0-4��0����
Battery������0-3��0����Ϊ0,3���磻
Temperature�¶ȣ�-2147483648 ~ 2147483647;�����ɸ����Զ���ʾ
humidityʪ�Ȱٷֱȣ�0~100��
DrinkValue���պ�ˮ����0~65536ML
WaterAmountӦ��ˮ����0~65536ML
u8 charge_state;//���״̬��0(�޳��)��1(�����)
*/


//���¶����������
typedef uint8_t  u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

typedef struct
{
	u16 Year;
	u8 Mounth;
	u8 Day;
	u8 Clk;
	u8 Min;
	u8 Seconds;
	u8 Signal;
	u8 Battery;
	int Temperature;
	u8 humidity;
	u16 DrinkValue;
	u16 WaterAmount;
	u16 test_message;
	u8 charge_state;//���״̬��0(�޳��)��1(�����)
}DataTime;

void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Sent_Ack(u8 Ack);
void Write_IIC_Byte(unsigned char IIC_Byte);
unsigned char Recieve_IIC_Byte(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);  

void OLED_Clear1(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_Scrolling(void);
void OLED_Showsmallbmp(u8 x);
void delay200ms(void);
void OLED_DisplayMode(u8 x);
void OLED_showDataTime(DataTime* Rtime, u8 x, u8 y);
void OLED_ShowSignal(DataTime* Rtime, u8 x, u8 y);
void OLED_ShowBattery(DataTime* Rtime, u8 x,u8 y);
void OLED_ShowTable(u8 x,u16 TotalValue,u16 DrinkWaterValue);
void OLED_FillBlank(u8 x1, u8 x2, u8 y1, u8 y2);
void OLED_Black(void);

/****************************�ⲿ���ó���ʼ***************************************************************/
void TimeStruct_Init(void);//��ʼ��ȫ�ֱ���Overall_Time
void OLED_Init(void);//��ʼ��OLED��Ļ
void OLED_Clear(void);//����
void OLED_Display_On(void);//����Ļ
void OLED_Display_Off(void);//����Ļ	   							   		    
void OLED_ShowWelcome(void);//��ʾ��������************************************************************�¼ӣ�
void OLED_ShowCharging(void);//��ʾ���ڳ�����*******************************************************�¼ӣ�
void OLED_ShowCharge_Completed(void);//��ʾ�����ɽ���***********************************************�¼ӣ�
void OLED_RemindPlaceCup(void);//��ʾ���ѷ���ˮ������	
void OLED_Reminddrinking(void);//��ʾ���Ѻ�ˮ����
void OLED_ShowWeight(u32 Jian);//��ʾ�������棬�������ΪҪ���ĳ�ʼ��

void OLED_DrinkingWaterThistime(u16 Value);//��ʾ���κ�ˮ��,�������Ϊ���κ�ˮ������λml**************�¼ӣ�
void OLED_Temperature_humidity(void);//��ʾ��ǰ�¶Ⱥ�ʪ��
void OLED_SuggestDrinkingwater(u16 Value);//��ʾ�����ˮ������
void OLED_Time(void);//��ʾʱ��������,�����������ź���ʾ
void OLED_Drinkwater_value(void);//��ʾ�����ˮ��������ǰ�����Time�ṹ��

/****************************�ⲿ���ó������****************************************************************/

void OLED_ShowBattery_Voltage(void);
/*������������Ϣ��ʾ��OLED��Ļ��numΪ��Ҫ��ʾ�ĵ������֣�u32��ʽ��size��ѡ8��16��x��y������ʾ�ڼ��еڼ���*/
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);

#endif  
	 

