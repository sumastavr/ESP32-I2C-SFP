#include <Arduino.h>
#include <Wire.h>
#include <M5Core2.h>

int textColor=YELLOW;
#define INFOADDR 0x50
#define DDMADDR 0x51

float cal_rxpower[5]; 
uint8_t raw_buffer[127];

const bool FS_SFP=false;

void setup(){
  M5.begin(true,true,true,true); //Init M5Core2(Initialization of external I2C is also included).  初始化M5Core2(初始化外部I2C也包含在内)
  Wire.begin(33, 32); //Detect internal I2C, if this sentence is not added, it will detect external I2C.  检测内部I2C,若不加此句为检测外部I2C
  M5.Lcd.setTextColor(YELLOW);  //Set the font color to yellow.  设置字体颜色为黄色
  M5.Lcd.setTextSize(2);  //Set the font size to 2.  设置字体大小为2
  M5.Lcd.println("I2C Tester"); //Print a string on the screen.  在屏幕上打印字符串
  M5.Lcd.fillScreen( BLACK ); //Make the screen full of black (equivalent to clear() to clear the screen).  使屏幕充满黑色(等效clear()清屏)
}

void loop(){
 
Wire.beginTransmission(INFOADDR);    
Wire.write(92);                               
Wire.requestFrom(INFOADDR,1);         
byte slaveByte2 = Wire.read();        
Wire.endTransmission();

//Serial.print("0x");
//Serial.print(slaveByte2,HEX);
//Serial.print(" : 0b");
//Serial.println(slaveByte2,BIN);

 if(slaveByte2 & 0x40){

    Wire.beginTransmission(DDMADDR);    // Get the slave's attention, tell it we're sending a command byte
    Wire.write(0);
    Wire.requestFrom(DDMADDR,127);

    for(int x=0;x<127;x++){
      raw_buffer[x]= Wire.read();
      //Serial.print(x);
      //Serial.print(" : ");
      //Serial.println(raw_buffer[x]);
    }

  Serial.println();
  Wire.endTransmission();

  if(FS_SFP){
    Serial.print("Rx P:");
    Serial.print(((raw_buffer[12] << 8) + raw_buffer[13])/(float) 10000,DEC);

    Serial.print(" \t Tx P:");
    Serial.print(((raw_buffer[10] << 8) + raw_buffer[11])/(float) 10000,DEC);

    Serial.print(" \t Temp:");
    Serial.println(((raw_buffer[4] << 8) + raw_buffer[5])/(float) 256,DEC);
  }else{
    Serial.print("Rx P:");
    Serial.print(((raw_buffer[104] << 8) + raw_buffer[105])/(float) 10000,DEC);

    Serial.print(" \t Tx P:");
    Serial.print(((raw_buffer[102] << 8) + raw_buffer[103])/(float) 10000,DEC);

    Serial.print(" \t Temp:");
    Serial.println(((raw_buffer[96] << 8) + raw_buffer[97])/(float) 256,DEC);
  }
  delay(500);

 }
}
