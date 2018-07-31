#include "SHT20.h"


extern DataTime	Overall_Time;

//软复位SHT20
void SHT_iic_Init(void)
{
	IIC_Start();
	Write_IIC_Byte(WRITE);
	
	    if(IIC_Wait_Ack()==0)
    {
        Write_IIC_Byte(REST);//发送软复位指令
				IIC_Wait_Ack();

    }
		
	IIC_Stop();
}

//读取转换后的SHT温度值，并将值更新到Overall_Time全局变量中
void SHT_get_temp(void) 
{
    unsigned int dat= 0;
    unsigned char Hbyte = 0;
    unsigned char Lbyte = 0;
    float temp = 0;
		int	result=0;

    IIC_Start();
    Write_IIC_Byte(WRITE);
	
    if(IIC_Wait_Ack()==0)
    {
				Write_IIC_Byte(NO_HOLD_AT_START);
        if(IIC_Wait_Ack()==0)
        {
            do
            {
                IIC_Start();
                Write_IIC_Byte(READ);
                
            }while(IIC_Wait_Ack()==1);

            Hbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(Ask_sinal);

            Lbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(NAsk_sinal);	//省略检验和传输

            IIC_Stop();

            Lbyte &= 0xfc;
            dat = (Hbyte<<8)|Lbyte;

            temp = (dat*175.72)/65536-46.85;
						result=(int)(temp);
						Overall_Time.Temperature=result;
						return;
            
        }
    }
		
		 IIC_Stop();//非应答也要发送停止信号
		 Overall_Time.Temperature=result;
}

//带返回值温度
int SHT_get_temp_Revalue(void) 
{
    unsigned int dat= 0; 
    unsigned char Hbyte = 0;
    unsigned char Lbyte = 0;
    float temp = 0;
		int	result=0;

    IIC_Start();
    Write_IIC_Byte(WRITE);
	
    if(IIC_Wait_Ack()==0)
    {
				Write_IIC_Byte(NO_HOLD_AT_START);
        if(IIC_Wait_Ack()==0)
        {
            do
            {
                IIC_Start();
                Write_IIC_Byte(READ);
                
            }while(IIC_Wait_Ack()==1);

            Hbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(Ask_sinal);

            Lbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(NAsk_sinal);	//省略检验和传输

            IIC_Stop();

            Lbyte &= 0xfc;
            dat = (Hbyte<<8)|Lbyte;

            temp = (dat*175.72)/65536-46.85;
						result=(int)(temp);
//						Overall_Time.Temperature=result;
						return result;
            
        }
    }
		
		 IIC_Stop();//非应答也要发送停止信号
//		 Overall_Time.Temperature=result;
}


//读取转换后的湿度值,并将值更新到Overall_Time全局变量中
void SHT_get_hum(void)  
{
    unsigned int dat= 0;
    unsigned char Hbyte = 0;
    unsigned char Lbyte = 0;
    float hum = 0;
		u8 result;

    IIC_Start();
    Write_IIC_Byte(WRITE);
	
    if(IIC_Wait_Ack()==0)
    {
				Write_IIC_Byte(NO_HOLD_AH_START);
        if(IIC_Wait_Ack()==0)
        {
            do
            {
                IIC_Start();
                Write_IIC_Byte(READ);
                
            }while(IIC_Wait_Ack()==1);

            Hbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(Ask_sinal);

            Lbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(NAsk_sinal);	//省略检验和传输

            IIC_Stop();

            Lbyte &= 0xfc;
            dat = (Hbyte<<8)|Lbyte;

            hum = (float)((dat*125.0)/65536.0-6);
						result=(u8)(hum);
						Overall_Time.humidity=result;
						return;
            
        }
    }
		
		 IIC_Stop();//非应答也要发送停止信号
		Overall_Time.humidity=result;
}

//读取转换后的湿度值,并将值更新到Overall_Time全局变量中
u8 SHT_get_hum_Revalue(void)  
{
    unsigned int dat= 0;
    unsigned char Hbyte = 0;
    unsigned char Lbyte = 0;
    float hum = 0;
		u8 result;

    IIC_Start();
    Write_IIC_Byte(WRITE);
	
    if(IIC_Wait_Ack()==0)
    {
				Write_IIC_Byte(NO_HOLD_AH_START);
        if(IIC_Wait_Ack()==0)
        {
            do
            {
                IIC_Start();
                Write_IIC_Byte(READ);
                
            }while(IIC_Wait_Ack()==1);

            Hbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(Ask_sinal);

            Lbyte = Recieve_IIC_Byte();
            IIC_Sent_Ack(NAsk_sinal);	//省略检验和传输

            IIC_Stop();

            Lbyte &= 0xfc;
            dat = (Hbyte<<8)|Lbyte;

            hum = (float)((dat*125.0)/65536.0-6);
						result=(u8)(hum);
//						Overall_Time.humidity=result;
						return result;
            
        }
    }
		
		 IIC_Stop();//非应答也要发送停止信号
//		Overall_Time.humidity=result;
}


//调整精度为8位湿度，12位温度
void Set_REG(void)
{
	u8 temp;
	
	//开始读取用户寄存器
	IIC_Start();
	Write_IIC_Byte(WRITE);
	
	if(IIC_Wait_Ack()==0)
	{
		Write_IIC_Byte(ReadREG);
		if(IIC_Wait_Ack()==0)
		{
			IIC_Start();
			Write_IIC_Byte(READ);
			if(IIC_Wait_Ack()==0)
			{
				temp=Recieve_IIC_Byte();
				IIC_Sent_Ack(NAsk_sinal);
				
			}
		}
	}
	
	temp=temp|(0x01);
	temp=temp&(~(0x80));
	
	//开始写入用户寄存器
	IIC_Start();
	Write_IIC_Byte(WRITE);
	
	if(IIC_Wait_Ack()==0)
	{
		Write_IIC_Byte(WriteREG);
		if(IIC_Wait_Ack()==0)
		{
			Write_IIC_Byte(temp);
			IIC_Wait_Ack();
			IIC_Stop();
		}
		
	}
	
}






