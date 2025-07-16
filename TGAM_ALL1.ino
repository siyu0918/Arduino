
// 通过UART串口显示信号值、注意力及放松度的值
#define BAUDRATE 57600
#define DEBUGOUTPUT 0

//校验相关变量
int   generatedChecksum = 0;
byte  checksum = 0; 

//接收数据长度和数据数组
byte  payloadLength = 0;
byte  payloadData[32] = {0};//总共接收32个自己的数据

//需要读取的信息变量
byte signalquality = 0;//信号质量
byte attention = 0;    //注意力值
byte meditation = 0;   //放松度值
unsigned int delta_wave = 0;
unsigned int theta_wave = 0;
unsigned int low_alpha_wave = 0;
unsigned int high_alpha_wave = 0;
unsigned int low_beta_wave = 0;
unsigned int high_beta_wave = 0;
unsigned int low_gamma_wave = 0;
unsigned int mid_gamma_wave = 0;

//初始化
void setup() 
{
  Serial.begin(BAUDRATE); 
}

//从串口读取一个字节数据
byte ReadOneByte() 
{
  int ByteRead;
  while(!Serial.available());
  ByteRead = Serial.read();
  return ByteRead;//返回读到的字节
}

//读取串口数据
void read_serial_data()
{
    //寻找数据包起始同步字节，2个
    if(ReadOneByte() == 0xAA)//先读一个
    {
      if(ReadOneByte() == 0xAA)//读第二个
      {
        payloadLength = ReadOneByte();//读取第三个，数据包字节的长度
        if(payloadLength == 0x20)//如果接收到的是大包数据才继续读取，小包数据则舍弃不读取
        {
          generatedChecksum = 0; //校验变量清0       
          for(int i = 0; i < payloadLength; i++)//连续读取32个字节
          {  
            payloadData[i] = ReadOneByte();//读取指定长度数据包中的数据
            generatedChecksum += payloadData[i];//计算数据累加和
          }         
          checksum = ReadOneByte();//读取校验字节  
          //校验
          generatedChecksum = (~generatedChecksum)&0xff;       
          //比较校验字节
          if(checksum == generatedChecksum)//数据接收正确，继续处理 
          {    
            signalquality = 0;//信号质量变量
            attention = 0;    //注意力值变量
            //赋值数据
            signalquality = payloadData[1];//信号值      
            attention = payloadData[29];//注意力值
            meditation = payloadData[31];//放松度值
            delta_wave = (payloadData[4] << 16) + (payloadData[5] << 8) + payloadData[6];
            theta_wave = (payloadData[7] << 16) + (payloadData[8] << 8) + payloadData[9];
            low_alpha_wave = (payloadData[10] << 16) + (payloadData[11] << 8) + payloadData[12];
            high_alpha_wave = (payloadData[13] << 16) + (payloadData[14] << 8) + payloadData[15];
            low_beta_wave = (payloadData[16] << 16) + (payloadData[17] << 8) + payloadData[18];
            high_beta_wave = (payloadData[19] << 16) + (payloadData[20] << 8) + payloadData[21];
            low_gamma_wave = (payloadData[22] << 16) + (payloadData[23] << 8) + payloadData[24];
            mid_gamma_wave = (payloadData[25] << 16) + (payloadData[26] << 8) + payloadData[27];   
            attention = payloadData[29];//注意力值
            meditation = payloadData[31];//放松度值
            #if !DEBUGOUTPUT         
            //打印信号质量
          //  Serial.print("SignalQuality: ");
           // Serial.print(signalquality, DEC);
            //打印注意力值
           // Serial.print("Attation: ");
            Serial.print(attention, DEC);Serial.print(",");
            //打印放松度值
           // Serial.print("Meditation: ");
           Serial.print(meditation, DEC);Serial.print(",");
            Serial.print(delta_wave, DEC);Serial.print(",");
            Serial.print(theta_wave, DEC);Serial.print(",");
            Serial.print(low_alpha_wave, DEC);Serial.print(",");
            Serial.print(high_alpha_wave, DEC);Serial.print(",");
            Serial.print(low_beta_wave, DEC);Serial.print(",");
            Serial.print(high_beta_wave, DEC);Serial.print(",");
            Serial.print(low_gamma_wave, DEC);Serial.print(",");
            Serial.print(mid_gamma_wave, DEC);Serial.print(",");
           
            //换行
            Serial.print("\n");       
            #endif              
          } 
        } 
      }
    }
}

//主循环
void loop() 
{
  read_serial_data();//读取串口数据 
}
