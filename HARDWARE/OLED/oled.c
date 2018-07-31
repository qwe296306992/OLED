#include "oled.h"
#include "oledfont.h"  	
#include "bsp_adc.h" 
#include "SHT20.h"
#include "weigh.h"
 
extern DataTime Overall_Time;//ʹ��Overall_Time��Ϊȫ�ֱ�������������OLED��ʾ����




/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
static void i2c_Delay(void)
{
	uint8_t i;
// 
//	/*��
//	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
//    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
//  
//		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
//		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
//	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
//	*/
	for (i = 0; i < Delay; i++);
}


void IIC_Start(void)
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	i2c_Delay();
	OLED_SDIN_Clr();
//	i2c_Delay();
	OLED_SCLK_Clr();
	i2c_Delay();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	i2c_Delay();
	OLED_SDIN_Set();
//	i2c_Delay();
	
}

//CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�,����ֵΪ1ʱ���ܵ�Ӧ��
//����0��ʾ��ȷӦ��1��ʾ��������Ӧ
u8 IIC_Wait_Ack(void)
{
	u8 re;
	
	OLED_SDIN_Set();/* CPU�ͷ�SDA���� */
	i2c_Delay();
	OLED_SCLK_Set() ;/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	
	 if((OLED_GPIO_Port->IDR & OLED_SDA_Pin) != 0)//�Ժ��������I2C�豸ʱ��Ҫ�޸Ĵ˳���
	 {
	  re=1;
	 }
	 else re=0;
	
	OLED_SCLK_Clr();
	i2c_Delay();
	 return re;
}

//MCU����Ӧ���źţ�AckΪ1����Ӧ���źţ�Ϊ0ʱ������Ӧ���ź�
void IIC_Sent_Ack(u8 Ack)
{
	OLED_SDIN_Set();	//SDA�ͷ�����
	i2c_Delay();
	
	if(Ack==1)
	{
	OLED_SDIN_Clr();
	}
	else {OLED_SDIN_Set();}
	i2c_Delay();
	
	OLED_SCLK_Set();
	i2c_Delay();
	
	OLED_SCLK_Clr();
	i2c_Delay();
	
	OLED_SDIN_Set();	//SDA�ͷ�����
	i2c_Delay();
}

/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
		i2c_Delay();
			da=da<<1;
		OLED_SCLK_Set();
		i2c_Delay();
		OLED_SCLK_Clr();
		i2c_Delay();
		
		if (i == 7)
		 {
			 OLED_SDIN_Set(); // �ͷ�����
		 }
		 i2c_Delay();		
	}
}

unsigned char Recieve_IIC_Byte(void)
{
    int i = 0;
    unsigned char data = 0;

    OLED_SDIN_Set();//�ͷ�SDA���ߣ��Ա���������
    
    for(i = 0; i < 8; i++)
    {
        data <<= 1;
        OLED_SCLK_Clr();
        i2c_Delay();	
        OLED_SCLK_Set();
        if((OLED_GPIO_Port->IDR & OLED_SDA_Pin) != 0) 
        {
            data |= 0x01;//���λ��1
        }
        else
        {
            data &= 0xfe;//���λ��0
        }        
    }
   i2c_Delay();
   OLED_SCLK_Clr();

    return data;
}


/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	
//	IIC_Wait_Ack();
	if(IIC_Wait_Ack()!=0)
	{
//		printf("����д��ַ��Ӧ��\n");
	  goto fail;
	}

   Write_IIC_Byte(0x00);			//write command
//	IIC_Wait_Ack();
	if(IIC_Wait_Ack()!=0)
	{
//		printf("����������Ӧ��\n");
	  goto fail;
	}
   Write_IIC_Byte(IIC_Command); 
	
//	IIC_Wait_Ack();
		if(IIC_Wait_Ack()!=0)
	{
//		printf("���;���������Ӧ��\n");
	  goto fail;
	}
	
fail: IIC_Stop();

}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		if(IIC_Wait_Ack()!=0)
	{
//		printf("����д��ַ��Ӧ��\n");
	  goto fail2;
	}
   Write_IIC_Byte(0x40);			//write data
		if(IIC_Wait_Ack()!=0)
	{
//		printf("��������ָ����Ӧ��\n");
	  goto fail2;
	}
   Write_IIC_Byte(IIC_Data);
		if(IIC_Wait_Ack()!=0)
	{
//		printf("���;���������Ӧ��\n");
	  goto fail2;
	}
fail2:  IIC_Stop();
}


void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}

void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<4;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7�� Set Page Start Address for Page Addressing Mode
		OLED_WR_Byte (0x00,OLED_CMD);      //Set Lower Column Start Address for Page Addressing Mode
		OLED_WR_Byte (0x10,OLED_CMD);      //Set Higher Column Start Address for Page Addressing Mode ���п�ʼ��ַΪ0
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}


void OLED_Clear1(void)  
{  
	u8 i,n,m=0xff;	

	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7�� Set Page Start Address for Page Addressing Mode
		OLED_WR_Byte (0x00,OLED_CMD);      //Set Lower Column Start Address for Page Addressing Mode
		OLED_WR_Byte (0x10,OLED_CMD);      //Set Higher Column Start Address for Page Addressing Mode ���п�ʼ��ַΪ0
		for(n=0;n<128;n++)OLED_WR_Byte(m,OLED_DATA); 
	} //������ʾ
}

//��������

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);					//(0+y)ҳ   y�ܹ���4�У���Ӧ128*32���أ�ÿ��8����
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63   ������4
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
				
			for(i=0;i<8;i++) OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
				
			OLED_Set_Pos(x,y+1);
				
			for(i=0;i<8;i++) OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else 
		  {				
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
 static u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;//��������ѭ��
			}else enshow=1; 		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0') //�ַ����Ľ�������
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

//��ʾ���Ѻ�ˮͼ��
void OLED_Showsmallbmp(u8 x)
{ 
	int y,t;
	for(y=0;y<4;y++)
		{
		  OLED_Set_Pos(x,y);	
				for(t=0;t<32;t++)
			  {
				 OLED_WR_Byte(BMP1[y][t],OLED_DATA);
				}
				
     }	
		

}


//���Ѻ�ˮ
void OLED_Reminddrinking(void)
{

OLED_Showsmallbmp(50);
OLED_FillBlank(0,49,0,3);
OLED_FillBlank(82,127,0,3);

}

//����ȫ�ֱ���MODEѡ����ʾ����
void OLED_DisplayMode(u8 x)
{
switch(x)
  { 
	    case 0:OLED_Drinkwater_value();break;
	    case 1:OLED_Temperature_humidity();break;
		  case 2:OLED_Time();break;
//			case 3:OLED_ShowWeight(0);break;//�����޸�����Ĵ����������ʱΪ0		   
			        
		  
  }
}


//��ʾ���д�С������
static void OLED_ShowNum_Big(u8 Num,u8 x,u8 y)
{
	int t,z;
	for(z=0;z<3;z++)
	{
		OLED_Set_Pos(x,(z+y));		
    for(t=0;t<12;t++)
		 {OLED_WR_Byte(F12x24[(3*Num+z)][t],OLED_DATA);}	
   }		
}

//��ʾ�ָ����ţ���-
static void OLED_ShowFengefu(u8 Num,u8 x,u8 y)
{
	int t,z;
	for(z=0;z<3;z++)
	{
		OLED_Set_Pos(x,(z+y));		
    for(t=0;t<6;t++)
		 {OLED_WR_Byte(F6x24[(3*Num+z)][t],OLED_DATA);}	
   }		
}

//��ʾ����
static void OLED_DataNum(DataTime* Rtime, u8 x, u8 y)
{
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<2;t++)	//��ʾ�·�
	{
		temp=(Rtime->Mounth/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
//				OLED_ShowNum_Big(0,x,y);
				OLED_ShowNum(x,y,0,1,16);
				continue;//��������ѭ��
			}else enshow=1; 		 	 
		}
//		OLED_ShowNum_Big(temp,(x+12*t),y);
		OLED_ShowNum((x+8*t),y,temp,1,16);
	}
	//��ʾ�ָ�����
	OLED_ShowCHinese((x+8*2),y,26);//��
	

	for(t=0;t<2;t++)	//��ʾDay
	{
		temp=(Rtime->Day/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
				OLED_ShowNum((x+2*8+16+8*t),y,0,1,16);
				continue;//��������ѭ��
			}else enshow=1; 		 	 
		}
		OLED_ShowNum((x+2*8+16+8*t),y,temp,1,16);		
	}
	OLED_ShowCHinese((x+4*8+16),y,27);//��
}

//��ʾʱ������
static void OLED_TimeNum(DataTime* Rtime, u8 x, u8 y)
{
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<2;t++)	//��ʾСʱ
	{
		temp=(Rtime->Clk/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
				OLED_ShowNum(x,y,0,1,16);
				continue;//��������ѭ��
			}else enshow=1; 		 	 
		}
		OLED_ShowNum((x+8*t),y,temp,1,16);
	}
	//��ʾ�ָ�����
	OLED_ShowChar((x+8*2),y,':',16);
	
	for(t=0;t<2;t++)	//��ʾ����
	{
		temp=(Rtime->Min/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
				OLED_ShowNum((x+2*8+4),y,0,1,16);
				continue;//��������ѭ��
			}else enshow=1; 		 	 
		}
		OLED_ShowNum((x+8*(t+2)+4),y,temp,1,16);
	}
	
}





void OLED_Time(void)
{	

	OLED_showDataTime(&Overall_Time,10,2);
	OLED_ShowSignal(&Overall_Time,4,0);
	OLED_ShowBattery(&Overall_Time,110,0); 

}

 
//��ʾ��ǰʱ�䣬ʱ��ṹ��DataTime
void OLED_showDataTime(DataTime* Rtime, u8 x, u8 y)
{ 
	OLED_FillBlank(0,127,1,1);
	OLED_FillBlank(0,x,2,3);
	OLED_FillBlank(74,79,2,3);
	OLED_FillBlank(x+130+1,127,2,3);

	OLED_DataNum(Rtime,x,y);
	OLED_TimeNum(Rtime,(x+70),y);
}


void OLED_ShowSignal(DataTime* Rtime, u8 x, u8 y)
{
	  int t;
	
	OLED_FillBlank(11,109,0,0);
	
		OLED_Set_Pos(0,0);		
    for(t=0;t<11;t++)
		 {OLED_WR_Byte(F11x8[(Rtime->Signal)][t],OLED_DATA);}	
}

void OLED_ShowBattery(DataTime* Rtime, u8 x,u8 y)
{
		OLED_FillBlank(124,127,0,0);
	  int t;
		ADC_Battery();	
	
		OLED_Set_Pos(x,y);	
			
		if(Rtime->charge_state == 0)
		{
    for(t=0;t<14;t++)
		 {OLED_WR_Byte(F14x8[(Rtime->Battery)][t],OLED_DATA);}	
	  }
		else
		{
		 for(t=0;t<16;t++)
		 {OLED_WR_Byte(F16x8[0][t],OLED_DATA);}	 
		}	
	
}

void OLED_ShowTable(u8 x,u16 TotalValue,u16 DrinkWaterValue)
{
	u8 b;
	u16 i,t=0;
	char Table[100];//������״ͼ�ֿ�,Ԥ��4��������Ե���
	float percentage;
	percentage=((float)DrinkWaterValue/(float)TotalValue);
	
	if(percentage>=1)
	{
		percentage=1;
		b=(u8)(percentage*96);
	}
	else b=(u8)(percentage*96);
  
	
	//���ʵ�Ĳ���
	for(i=2;i<(b+2);i++)
	{
	Table[i]=0x5a;
	}
	
	//�����Ĳ���
	for(i=(b+2);i<98;i++)
	{
	Table[i]=0x42;
	}
	
	//����Ե����

	Table[0]=0x7e;
	Table[1]=0x42;
	Table[98]=0x42;
	Table[99]=0x7e;	

	
	OLED_Set_Pos(x,3);	

	 for(t=0;t<100;t++)
		{
				OLED_WR_Byte(Table[t],OLED_DATA);
//				adder+=1;
     }	
	
}
void OLED_Temperature_humidity(void)
{ 
		u8 temp;
		u8 line=2;
		SHT_get_temp();
		SHT_get_hum();
	

		OLED_ShowSignal(&Overall_Time,0,0);
		OLED_ShowBattery(&Overall_Time,110,0);
	
 		OLED_ShowCHinese(7,line,4);//�¶�ͼ��	
		if(Overall_Time.Temperature<0	)
		{
			temp=abs(Overall_Time.Temperature);
			OLED_ShowChar(23,line,'-',16);
			OLED_ShowNum(31,line,temp,2,16);//�¶�ֵ
			OLED_ShowCHinese(47,line,5);//�¶ȵ�λ
				OLED_FillBlank(0,6,line,line+1);
				OLED_FillBlank(62,64,line,line+1);
				OLED_FillBlank(113,127,line,line+1);
				OLED_FillBlank(0,127,line-1,line-1);
		}
		else {OLED_ShowNum(23,line,Overall_Time.Temperature,2,16);//�¶�ֵ
					OLED_ShowCHinese(39,line,5);//�¶ȵ�λ
					OLED_FillBlank(0,6,line,line+1);
					OLED_FillBlank(55,64,line,line+1);
					OLED_FillBlank(113,127,line,line+1);
					OLED_FillBlank(0,127,line-1,line-1);
		}	
		OLED_ShowCHinese(65,line,3);//ʪ��ͼ��	
		OLED_ShowNum(81,line,Overall_Time.humidity,2,16);//ʪ��ֵ
		OLED_ShowCHinese(97,line,6);//ʪ�ȵ�λ
		
//	OLED_FillBlank(0,127,0,0);

//	OLED_FillBlank(0,127,3,3);

}
 
//��ʾ���κ�ˮ��
void OLED_DrinkingWaterThistime(u16 Value)
{ 
	OLED_ShowSignal(&Overall_Time,4,0);
	OLED_ShowBattery(&Overall_Time,110,0);
	
	OLED_FillBlank(124,127,0,0);
	OLED_FillBlank(0,29,1,2);
	OLED_FillBlank(84,127,1,2);
	OLED_FillBlank(0,127,3,3);
	
	OLED_ShowCHinese(20+10,1,23);//��ˮͼ��	
	OLED_ShowNum(20+16+10,1,Value,4,16);//�ôκ�ˮ��
	OLED_ShowChar(68+10,1,'m',16);
	OLED_ShowChar(76+10,1,'l',16);
	

}

void OLED_SuggestDrinkingwater(u16 Value)//��ʾ�����ˮ������
{
	OLED_ShowSignal(&Overall_Time,4,0);
	OLED_ShowBattery(&Overall_Time,110,0);
	
	OLED_FillBlank(0,24,1,2);
	OLED_FillBlank(105,127,1,2);
	OLED_FillBlank(0,127,3,3);
	
	OLED_ShowCHinese(25,1,24);//��
	OLED_ShowCHinese(41,1,25);//��
	OLED_ShowNum(57,1,Value,4,16);//�����ˮ��
	OLED_ShowString(89,1,"ml",16);
	
	
};
//��ʾ��������
void OLED_ShowWelcome(void)
{
//	u8 a=25;
//	OLED_ShowCHinese(a,1,7);					//ϵ
//	OLED_ShowCHinese((a+16),1,8);			//ͳ
//	OLED_ShowCHinese((a+16*2),1,9);		//��
//	OLED_ShowCHinese((a+16*3),1,10);	//��
//	OLED_ShowCHinese((a+16*4),1,11);	//��
//	OLED_FillBlank(0,127,0,0);
//	OLED_FillBlank(0,24,1,2);
//	OLED_FillBlank(105,127,1,2);
//	OLED_FillBlank(0,127,3,3);
	
		u8 t,h,q,adder=0;
	
	OLED_Set_Pos(0,0);		
	for(q=0;q<4;q++)//��ʾȫ��
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//��ʾһ��
		{
			for(t=0;t<16;t++)
			{
				OLED_WR_Byte(BMP2[h+q*8][t],OLED_DATA);
				adder+=1;
			}	
		}
	}
}

void OLED_Black(void)
{
		u8 t,h,q,adder=0;
	
	OLED_Set_Pos(0,0);		
	for(q=0;q<4;q++)//��ʾȫ��
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//��ʾһ��
		{
			for(t=0;t<16;t++)
			{
				OLED_WR_Byte(0xFF,OLED_DATA);
				adder+=1;
			}	
		}
	}
}


//��ʾ���ڳ�����
void OLED_ShowCharging(void)
{
//	u8 a=25;
//	OLED_ShowCHinese(a,1,7);					//ϵ
//	OLED_ShowCHinese((a+16),1,8);			//ͳ
//	OLED_ShowCHinese((a+16*2),1,9);		//��
//	OLED_ShowCHinese((a+16*3),1,10);	//��
//	OLED_ShowCHinese((a+16*4),1,11);	//��
//	OLED_FillBlank(0,127,0,0);
//	OLED_FillBlank(0,24,1,2);
//	OLED_FillBlank(105,127,1,2);
//	OLED_FillBlank(0,127,3,3);
	
		u8 t,h,q,adder=0;
	
	OLED_Set_Pos(0,0);		
	for(q=0;q<4;q++)//��ʾȫ��
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//��ʾһ��
		{
			for(t=0;t<16;t++)
			{
				OLED_WR_Byte(BMP3[h+q*8][t],OLED_DATA);
				adder+=1;
			}	
		}
	}
}

void OLED_ShowCharge_Completed(void)//��ʾ�����ɽ���
{
	u8 t,h,q,adder=0;
	
	OLED_Set_Pos(0,0);		
	for(q=0;q<4;q++)//��ʾȫ��
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//��ʾһ��
		{
			for(t=0;t<16;t++)
			{
				OLED_WR_Byte(BMP4[h+q*8][t],OLED_DATA);
				adder+=1;
			}	
		}
	}
};

//��ʾ�����û�����ˮ��
void OLED_RemindPlaceCup(void)
{
		u8 a=25;
	OLED_ShowCHinese(a,1,12);					 	//��
	OLED_ShowCHinese((a+16),1,13);			//��
	OLED_ShowCHinese((a+16*2),1,14);		//��
	OLED_ShowCHinese((a+16*3),1,15);		//ˮ
	OLED_ShowCHinese((a+16*4),1,16);		//��
	OLED_FillBlank(0,127,0,0);
	OLED_FillBlank(0,24,1,2);
	OLED_FillBlank(105,127,1,2);
	OLED_FillBlank(0,127,3,3);
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	 	 
 	GPIO_InitTypeDef  GPIO_InitStructure; 
 	
 	RCC_Fun_Cmd(RCC_OLED_GPIO, ENABLE);	 //ʹ��PC�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = OLED_SCL_Pin | OLED_SDA_Pin;	 //PB6 PB7 od���  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //�������,ʵ��ʱӦΪGPIO_Mode_Out_OD
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//�ٶ�50MHz
 	GPIO_Init(OLED_GPIO_Port, &GPIO_InitStructure);	  //��ʼ��GPIOC2,6
 	GPIO_SetBits(OLED_GPIO_Port,OLED_SCL_Pin | OLED_SDA_Pin);	//PC2,PC6 ����ߵ�ƽ

  delay200ms(); //��һ��������Ļ��Ҫ��ʱ��������ʾ������

  OLED_WR_Byte(0xAE,OLED_CMD);//�ر���ʾ
	
	OLED_WR_Byte(0x40,OLED_CMD);//---set low column address    Set Display Start Line
	OLED_WR_Byte(0xB0,OLED_CMD);//---set high column address   Set GDDRAM Page Start Address��page1

	OLED_WR_Byte(0xC8,OLED_CMD);//-not offset remapped mode. Scan from COM[N-1] to COM0 Where N is the Multiplex ratio

	OLED_WR_Byte(0x81,OLED_CMD);//���öԱȶ�(����)
	OLED_WR_Byte(0xff,OLED_CMD);//256

	OLED_WR_Byte(0xa1,OLED_CMD);//���ض�������   A1h, X[0]=1b: column address 127 is mapped to SEG0

	OLED_WR_Byte(0xa6,OLED_CMD);//Set Normal/Inverse Display
	
	OLED_WR_Byte(0xa8,OLED_CMD);//��������·��  Set Multiplex Ratio  to N+1 MUX
	OLED_WR_Byte(0x1f,OLED_CMD);//31+1=32MUX
	
	OLED_WR_Byte(0xd3,OLED_CMD);//����ʾƫ��
	OLED_WR_Byte(0x00,OLED_CMD);
	
	OLED_WR_Byte(0xd5,OLED_CMD);//Set Display Clock Divide Ratio/Oscillator Frequency
	OLED_WR_Byte(0xf0,OLED_CMD);//DCLK=1,  Set the Oscillator Frequency, FOSC
	
	OLED_WR_Byte(0xd9,OLED_CMD);//Set Pre-charge Period 
	OLED_WR_Byte(0x22,OLED_CMD);
	
	OLED_WR_Byte(0xda,OLED_CMD);//Set COM Pins Hardware Configuration
	OLED_WR_Byte(0x02,OLED_CMD);//Sequential COM pin configuration  Disable COM Left/Right remap
	
	OLED_WR_Byte(0xdb,OLED_CMD);//Set VCOMH Deselect Level
	OLED_WR_Byte(0x49,OLED_CMD);
	
	OLED_WR_Byte(0x8d,OLED_CMD);//�Ҳ�������?
	OLED_WR_Byte(0x14,OLED_CMD);
	
	OLED_WR_Byte(0xaf,OLED_CMD);//Display ON in normal mode
	
	OLED_Clear();//����
}

void delay200ms(void)   //?? 0us
{
    unsigned char a,b,c;
    for(c=151;c>0;c--)
        for(b=107;b>0;b--)
            for(a=48;a>0;a--);
//    _nop_();  //if Keil,require use intrins.h
}


//��ʾ��ǰ��ˮ��
void OLED_Drinkwater_value(void)
{
	OLED_ShowSignal(&Overall_Time,4,0);
	OLED_ShowBattery(&Overall_Time,110,0);
	
  int m=2;
	
	
	OLED_FillBlank(0,127,1,1);
	OLED_FillBlank(0,10,1,2);
	OLED_FillBlank(115,127,1,2);
	OLED_FillBlank(0,127,3,3);	
	
//  OLED_ShowCHinese(14,m,0);
//	OLED_ShowCHinese(30,m,1);
//	OLED_ShowCHinese(46,m,2);	
	OLED_ShowNum(11,m,Overall_Time.DrinkValue,4,16);	
	OLED_ShowString(43,m,"ml/",16);	
	OLED_ShowNum(67,m,Overall_Time.WaterAmount,4,16);	
	OLED_ShowString(99,m,"ml",16);		
//  OLED_ShowTable(15,Overall_Time.WaterAmount,Overall_Time.DrinkValue);	
	
	

}

void TimeStruct_Init(void)
{
	Overall_Time.Battery=0;
	Overall_Time.Clk=9;
	Overall_Time.Day=31;
	Overall_Time.DrinkValue=600;
	Overall_Time.humidity=0;
	Overall_Time.Min=54;
	Overall_Time.Mounth=7;
	Overall_Time.Signal=4;
	Overall_Time.Temperature=0;
	Overall_Time.WaterAmount=1200;
	Overall_Time.Year=0;
	Overall_Time.charge_state=0;					//������Գ��Ч����Ԥ��Ϊ1********************
}

void OLED_ShowBattery_Voltage(void)
{
u16 tee;
tee=(1.1*Overall_Time.test_message/1024.0)*1000*1.0115;

	OLED_ShowCHinese(5,1,17);
	OLED_ShowCHinese(21,1,18);
	OLED_ShowCHinese(37,1,19);
	OLED_ShowCHinese(53,1,20);
	OLED_ShowNum(69,1,tee,5,16);
	
	OLED_FillBlank(0,127,0,0);
	OLED_FillBlank(0,4,1,2);
	OLED_FillBlank(109,127,1,2);
	OLED_FillBlank(0,127,3,3);
	
}

//��ʾ��������,�������Ϊ�����ĳ�����
void OLED_ShowWeight(u32 Jian)
{
	uint32_t temp;
	temp=Weighing();
	
//	temp=6666;
	
	OLED_FillBlank(0,127, 0, 0);
	OLED_FillBlank(0,127, 3, 3);
	OLED_FillBlank(0,24+4, 1, 2);
	OLED_FillBlank(45,45, 1, 2);
	OLED_FillBlank(102,127, 1, 2);
	
	OLED_ShowCHinese(24+5,1,21);OLED_ShowCHinese(41+5,1,22);
	OLED_ShowNum(57+5,1,temp-Jian,4,16);	
	OLED_ShowChar(89+5,1,'g',16);
}


//�����ʾ�հ׵ĵط�
void OLED_FillBlank(u8 x1, u8 x2, u8 y1, u8 y2)
{ 
	u8 i,k;
	for(i=y1;i<(y2+1);i++)
	{
		OLED_Set_Pos(x1,i);
		for(k=x1;k<(x2+1);k++)OLED_WR_Byte(0,OLED_DATA); 		
	}
}

