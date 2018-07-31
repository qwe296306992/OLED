#ifndef _SHT20_H
#define _SHT20_H

#include "oled.h"

//SDA��SCL����
#define sht_scl_pin            OLED_SCL_Pin
#define sht_sda_pin            OLED_SDA_Pin

//SHT����
#define   READ    0x81
#define   WRITE   0x80  

#define Ask_sinal 1
#define NAsk_sinal 0

#define  HOLD_AT_START  		0xe3  	//�����¶Ȳ���
#define  HOLD_AH_START  		0xe5  	//����ʪ�Ȳ���
#define  NO_HOLD_AT_START  	0xf3 		//�Ǳ����¶Ȳ���
#define  NO_HOLD_AH_START  	0xf5 		//�Ǳ���ʪ�Ȳ���
#define  WriteREG      			0xe6    //д�û��Ĵ���
#define  ReadREG       			0xe7   	//���û��Ĵ���
#define  REST           		0xfe    //��λ



//SHT��������
void SHT_iic_Init(void);//��λSHT20
void SHT_get_temp(void);//��ȡת�����SHT�¶�ֵ������ֵ���µ�Overall_Timeȫ�ֱ�����
void SHT_get_hum(void);//��ȡת����SHT��ʪ��ֵ,����ֵ���µ�Overall_Timeȫ�ֱ�����
void Set_REG(void);//��������Ϊ8λʪ�ȣ�12λ�¶�

/*������ֵ��������*/
int SHT_get_temp_Revalue(void);//�����¶�ֵ
u8 SHT_get_hum_Revalue(void);//����ʪ��ֵ



#endif

