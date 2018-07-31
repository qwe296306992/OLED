#include "oled.h"
#include "oledfont.h"  	
#include "bsp_adc.h" 
#include "SHT20.h"
#include "weigh.h"
 
extern DataTime Overall_Time;//使用Overall_Time作为全局变量参数，传递OLED显示参数




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
//	/*　
//	 	下面的时间是通过逻辑分析仪测试得到的。
//    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
//  
//		循环次数为10时，SCL频率 = 205KHz 
//		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
//	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
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

//CPU产生一个时钟，并读取器件的ACK应答信号,返回值为1时接受到应答
//返回0表示正确应答，1表示无器件响应
u8 IIC_Wait_Ack(void)
{
	u8 re;
	
	OLED_SDIN_Set();/* CPU释放SDA总线 */
	i2c_Delay();
	OLED_SCLK_Set() ;/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	
	 if((OLED_GPIO_Port->IDR & OLED_SDA_Pin) != 0)//以后添加其他I2C设备时需要修改此程序
	 {
	  re=1;
	 }
	 else re=0;
	
	OLED_SCLK_Clr();
	i2c_Delay();
	 return re;
}

//MCU发送应答信号，Ack为1产生应答信号，为0时产生非应答信号
void IIC_Sent_Ack(u8 Ack)
{
	OLED_SDIN_Set();	//SDA释放总线
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
	
	OLED_SDIN_Set();	//SDA释放总线
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
			 OLED_SDIN_Set(); // 释放总线
		 }
		 i2c_Delay();		
	}
}

unsigned char Recieve_IIC_Byte(void)
{
    int i = 0;
    unsigned char data = 0;

    OLED_SDIN_Set();//释放SDA总线，以便输入数据
    
    for(i = 0; i < 8; i++)
    {
        data <<= 1;
        OLED_SCLK_Clr();
        i2c_Delay();	
        OLED_SCLK_Set();
        if((OLED_GPIO_Port->IDR & OLED_SDA_Pin) != 0) 
        {
            data |= 0x01;//最低位置1
        }
        else
        {
            data &= 0xfe;//最低位置0
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
//		printf("发送写地址无应答\n");
	  goto fail;
	}

   Write_IIC_Byte(0x00);			//write command
//	IIC_Wait_Ack();
	if(IIC_Wait_Ack()!=0)
	{
//		printf("发送命令无应答\n");
	  goto fail;
	}
   Write_IIC_Byte(IIC_Command); 
	
//	IIC_Wait_Ack();
		if(IIC_Wait_Ack()!=0)
	{
//		printf("发送具体命令无应答\n");
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
//		printf("发送写地址无应答\n");
	  goto fail2;
	}
   Write_IIC_Byte(0x40);			//write data
		if(IIC_Wait_Ack()!=0)
	{
//		printf("发送数据指令无应答\n");
	  goto fail2;
	}
   Write_IIC_Byte(IIC_Data);
		if(IIC_Wait_Ack()!=0)
	{
//		printf("发送具体数据无应答\n");
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
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7） Set Page Start Address for Page Addressing Mode
		OLED_WR_Byte (0x00,OLED_CMD);      //Set Lower Column Start Address for Page Addressing Mode
		OLED_WR_Byte (0x10,OLED_CMD);      //Set Higher Column Start Address for Page Addressing Mode 即列开始地址为0
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


void OLED_Clear1(void)  
{  
	u8 i,n,m=0xff;	

	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7） Set Page Start Address for Page Addressing Mode
		OLED_WR_Byte (0x00,OLED_CMD);      //Set Lower Column Start Address for Page Addressing Mode
		OLED_WR_Byte (0x10,OLED_CMD);      //Set Higher Column Start Address for Page Addressing Mode 即列开始地址为0
		for(n=0;n<128;n++)OLED_WR_Byte(m,OLED_DATA); 
	} //更新显示
}

//坐标设置

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);					//(0+y)页   y总共有4行，对应128*32像素，每行8像素
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63   ？？？4
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
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
//m^n函数
 static u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
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
				continue;//跳出本次循环
			}else enshow=1; 		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0') //字符串的结束符号
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
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

//显示提醒喝水图标
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


//提醒喝水
void OLED_Reminddrinking(void)
{

OLED_Showsmallbmp(50);
OLED_FillBlank(0,49,0,3);
OLED_FillBlank(82,127,0,3);

}

//根据全局变量MODE选择显示画面
void OLED_DisplayMode(u8 x)
{
switch(x)
  { 
	    case 0:OLED_Drinkwater_value();break;
	    case 1:OLED_Temperature_humidity();break;
		  case 2:OLED_Time();break;
//			case 3:OLED_ShowWeight(0);break;//！需修改这里的传入参数，暂时为0		   
			        
		  
  }
}


//显示三行大小的数字
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

//显示分隔符号：，-
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

//显示日期
static void OLED_DataNum(DataTime* Rtime, u8 x, u8 y)
{
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<2;t++)	//显示月份
	{
		temp=(Rtime->Mounth/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
//				OLED_ShowNum_Big(0,x,y);
				OLED_ShowNum(x,y,0,1,16);
				continue;//跳出本次循环
			}else enshow=1; 		 	 
		}
//		OLED_ShowNum_Big(temp,(x+12*t),y);
		OLED_ShowNum((x+8*t),y,temp,1,16);
	}
	//显示分隔符号
	OLED_ShowCHinese((x+8*2),y,26);//月
	

	for(t=0;t<2;t++)	//显示Day
	{
		temp=(Rtime->Day/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
				OLED_ShowNum((x+2*8+16+8*t),y,0,1,16);
				continue;//跳出本次循环
			}else enshow=1; 		 	 
		}
		OLED_ShowNum((x+2*8+16+8*t),y,temp,1,16);		
	}
	OLED_ShowCHinese((x+4*8+16),y,27);//日
}

//显示时间数字
static void OLED_TimeNum(DataTime* Rtime, u8 x, u8 y)
{
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<2;t++)	//显示小时
	{
		temp=(Rtime->Clk/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
				OLED_ShowNum(x,y,0,1,16);
				continue;//跳出本次循环
			}else enshow=1; 		 	 
		}
		OLED_ShowNum((x+8*t),y,temp,1,16);
	}
	//显示分隔符号
	OLED_ShowChar((x+8*2),y,':',16);
	
	for(t=0;t<2;t++)	//显示分钟
	{
		temp=(Rtime->Min/oled_pow(10,1-t))%10;
		if(enshow==0&&t<1)
		{
			if(temp==0)
			{
				OLED_ShowNum((x+2*8+4),y,0,1,16);
				continue;//跳出本次循环
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

 
//显示当前时间，时间结构体DataTime
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
	char Table[100];//定义柱状图字库,预留4个留给边缘表格
	float percentage;
	percentage=((float)DrinkWaterValue/(float)TotalValue);
	
	if(percentage>=1)
	{
		percentage=1;
		b=(u8)(percentage*96);
	}
	else b=(u8)(percentage*96);
  
	
	//表格实心部分
	for(i=2;i<(b+2);i++)
	{
	Table[i]=0x5a;
	}
	
	//表格空心部分
	for(i=(b+2);i<98;i++)
	{
	Table[i]=0x42;
	}
	
	//表格边缘部分

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
	
 		OLED_ShowCHinese(7,line,4);//温度图标	
		if(Overall_Time.Temperature<0	)
		{
			temp=abs(Overall_Time.Temperature);
			OLED_ShowChar(23,line,'-',16);
			OLED_ShowNum(31,line,temp,2,16);//温度值
			OLED_ShowCHinese(47,line,5);//温度单位
				OLED_FillBlank(0,6,line,line+1);
				OLED_FillBlank(62,64,line,line+1);
				OLED_FillBlank(113,127,line,line+1);
				OLED_FillBlank(0,127,line-1,line-1);
		}
		else {OLED_ShowNum(23,line,Overall_Time.Temperature,2,16);//温度值
					OLED_ShowCHinese(39,line,5);//温度单位
					OLED_FillBlank(0,6,line,line+1);
					OLED_FillBlank(55,64,line,line+1);
					OLED_FillBlank(113,127,line,line+1);
					OLED_FillBlank(0,127,line-1,line-1);
		}	
		OLED_ShowCHinese(65,line,3);//湿度图标	
		OLED_ShowNum(81,line,Overall_Time.humidity,2,16);//湿度值
		OLED_ShowCHinese(97,line,6);//湿度单位
		
//	OLED_FillBlank(0,127,0,0);

//	OLED_FillBlank(0,127,3,3);

}
 
//显示本次喝水量
void OLED_DrinkingWaterThistime(u16 Value)
{ 
	OLED_ShowSignal(&Overall_Time,4,0);
	OLED_ShowBattery(&Overall_Time,110,0);
	
	OLED_FillBlank(124,127,0,0);
	OLED_FillBlank(0,29,1,2);
	OLED_FillBlank(84,127,1,2);
	OLED_FillBlank(0,127,3,3);
	
	OLED_ShowCHinese(20+10,1,23);//饮水图标	
	OLED_ShowNum(20+16+10,1,Value,4,16);//该次喝水量
	OLED_ShowChar(68+10,1,'m',16);
	OLED_ShowChar(76+10,1,'l',16);
	

}

void OLED_SuggestDrinkingwater(u16 Value)//显示建议喝水量界面
{
	OLED_ShowSignal(&Overall_Time,4,0);
	OLED_ShowBattery(&Overall_Time,110,0);
	
	OLED_FillBlank(0,24,1,2);
	OLED_FillBlank(105,127,1,2);
	OLED_FillBlank(0,127,3,3);
	
	OLED_ShowCHinese(25,1,24);//建
	OLED_ShowCHinese(41,1,25);//议
	OLED_ShowNum(57,1,Value,4,16);//建议喝水量
	OLED_ShowString(89,1,"ml",16);
	
	
};
//显示开机界面
void OLED_ShowWelcome(void)
{
//	u8 a=25;
//	OLED_ShowCHinese(a,1,7);					//系
//	OLED_ShowCHinese((a+16),1,8);			//统
//	OLED_ShowCHinese((a+16*2),1,9);		//启
//	OLED_ShowCHinese((a+16*3),1,10);	//动
//	OLED_ShowCHinese((a+16*4),1,11);	//中
//	OLED_FillBlank(0,127,0,0);
//	OLED_FillBlank(0,24,1,2);
//	OLED_FillBlank(105,127,1,2);
//	OLED_FillBlank(0,127,3,3);
	
		u8 t,h,q,adder=0;
	
	OLED_Set_Pos(0,0);		
	for(q=0;q<4;q++)//显示全部
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//显示一行
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
	for(q=0;q<4;q++)//显示全部
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//显示一行
		{
			for(t=0;t<16;t++)
			{
				OLED_WR_Byte(0xFF,OLED_DATA);
				adder+=1;
			}	
		}
	}
}


//显示正在充电界面
void OLED_ShowCharging(void)
{
//	u8 a=25;
//	OLED_ShowCHinese(a,1,7);					//系
//	OLED_ShowCHinese((a+16),1,8);			//统
//	OLED_ShowCHinese((a+16*2),1,9);		//启
//	OLED_ShowCHinese((a+16*3),1,10);	//动
//	OLED_ShowCHinese((a+16*4),1,11);	//中
//	OLED_FillBlank(0,127,0,0);
//	OLED_FillBlank(0,24,1,2);
//	OLED_FillBlank(105,127,1,2);
//	OLED_FillBlank(0,127,3,3);
	
		u8 t,h,q,adder=0;
	
	OLED_Set_Pos(0,0);		
	for(q=0;q<4;q++)//显示全部
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//显示一行
		{
			for(t=0;t<16;t++)
			{
				OLED_WR_Byte(BMP3[h+q*8][t],OLED_DATA);
				adder+=1;
			}	
		}
	}
}

void OLED_ShowCharge_Completed(void)//显示充电完成界面
{
	u8 t,h,q,adder=0;
	
	OLED_Set_Pos(0,0);		
	for(q=0;q<4;q++)//显示全部
	{
		OLED_Set_Pos(0,q);		
		for(h=0;h<8;h++)//显示一行
		{
			for(t=0;t<16;t++)
			{
				OLED_WR_Byte(BMP4[h+q*8][t],OLED_DATA);
				adder+=1;
			}	
		}
	}
};

//显示提醒用户放置水杯
void OLED_RemindPlaceCup(void)
{
		u8 a=25;
	OLED_ShowCHinese(a,1,12);					 	//请
	OLED_ShowCHinese((a+16),1,13);			//放
	OLED_ShowCHinese((a+16*2),1,14);		//置
	OLED_ShowCHinese((a+16*3),1,15);		//水
	OLED_ShowCHinese((a+16*4),1,16);		//杯
	OLED_FillBlank(0,127,0,0);
	OLED_FillBlank(0,24,1,2);
	OLED_FillBlank(105,127,1,2);
	OLED_FillBlank(0,127,3,3);
}

//初始化SSD1306					    
void OLED_Init(void)
{ 	 	 
 	GPIO_InitTypeDef  GPIO_InitStructure; 
 	
 	RCC_Fun_Cmd(RCC_OLED_GPIO, ENABLE);	 //使能PC端口时钟

	GPIO_InitStructure.GPIO_Pin = OLED_SCL_Pin | OLED_SDA_Pin;	 //PB6 PB7 od输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出,实际时应为GPIO_Mode_Out_OD
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//速度50MHz
 	GPIO_Init(OLED_GPIO_Port, &GPIO_InitStructure);	  //初始化GPIOC2,6
 	GPIO_SetBits(OLED_GPIO_Port,OLED_SCL_Pin | OLED_SDA_Pin);	//PC2,PC6 输出高电平

  delay200ms(); //第一次启动屏幕需要延时，否则显示有问题

  OLED_WR_Byte(0xAE,OLED_CMD);//关闭显示
	
	OLED_WR_Byte(0x40,OLED_CMD);//---set low column address    Set Display Start Line
	OLED_WR_Byte(0xB0,OLED_CMD);//---set high column address   Set GDDRAM Page Start Address：page1

	OLED_WR_Byte(0xC8,OLED_CMD);//-not offset remapped mode. Scan from COM[N-1] to COM0 Where N is the Multiplex ratio

	OLED_WR_Byte(0x81,OLED_CMD);//设置对比度(亮度)
	OLED_WR_Byte(0xff,OLED_CMD);//256

	OLED_WR_Byte(0xa1,OLED_CMD);//段重定向设置   A1h, X[0]=1b: column address 127 is mapped to SEG0

	OLED_WR_Byte(0xa6,OLED_CMD);//Set Normal/Inverse Display
	
	OLED_WR_Byte(0xa8,OLED_CMD);//设置驱动路数  Set Multiplex Ratio  to N+1 MUX
	OLED_WR_Byte(0x1f,OLED_CMD);//31+1=32MUX
	
	OLED_WR_Byte(0xd3,OLED_CMD);//无显示偏移
	OLED_WR_Byte(0x00,OLED_CMD);
	
	OLED_WR_Byte(0xd5,OLED_CMD);//Set Display Clock Divide Ratio/Oscillator Frequency
	OLED_WR_Byte(0xf0,OLED_CMD);//DCLK=1,  Set the Oscillator Frequency, FOSC
	
	OLED_WR_Byte(0xd9,OLED_CMD);//Set Pre-charge Period 
	OLED_WR_Byte(0x22,OLED_CMD);
	
	OLED_WR_Byte(0xda,OLED_CMD);//Set COM Pins Hardware Configuration
	OLED_WR_Byte(0x02,OLED_CMD);//Sequential COM pin configuration  Disable COM Left/Right remap
	
	OLED_WR_Byte(0xdb,OLED_CMD);//Set VCOMH Deselect Level
	OLED_WR_Byte(0x49,OLED_CMD);
	
	OLED_WR_Byte(0x8d,OLED_CMD);//找不到命令?
	OLED_WR_Byte(0x14,OLED_CMD);
	
	OLED_WR_Byte(0xaf,OLED_CMD);//Display ON in normal mode
	
	OLED_Clear();//清屏
}

void delay200ms(void)   //?? 0us
{
    unsigned char a,b,c;
    for(c=151;c>0;c--)
        for(b=107;b>0;b--)
            for(a=48;a>0;a--);
//    _nop_();  //if Keil,require use intrins.h
}


//显示当前喝水量
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
	Overall_Time.charge_state=0;					//如需调试充电效果可预设为1********************
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

//显示重量界面,传入参数为被减的初试量
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


//填充显示空白的地方
void OLED_FillBlank(u8 x1, u8 x2, u8 y1, u8 y2)
{ 
	u8 i,k;
	for(i=y1;i<(y2+1);i++)
	{
		OLED_Set_Pos(x1,i);
		for(k=x1;k<(x2+1);k++)OLED_WR_Byte(0,OLED_DATA); 		
	}
}

