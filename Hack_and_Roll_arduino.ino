//Required Libraries
#include <Wire.h>
#include <Servo.h>
#include <DS3231.h>
#include <SoftwareSerial.h>

//Definitioins
#define BMP085_ADDRESS 0x77  // I2C address of BMP085
#define Address 0x1E //HIH definition
#define STEPS_PER_MOTOR_REVOLUTION 32   //Motor
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64  //2048 

void process_timedata(int i);
void process_degreedata(int i);
void receive_data();
void bmp_setup();
void hih_read();
void bmp_read();
void send_data();
void rain_read();
void process_data();
void motor();

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

//Objects
Servo myservo;

//Data String
String bluetoothData;
String receiveData;
int receivedSlider = 0;


//Global Variables
int dataReceiveFlag = 0;
char colon = ';';
int rain = 0;

int analogPin = 5; 
int val = 0;  

//Motor
int pos = 0; 
int degree;

//HIH4030
const int outPin = A4; // (7) pin for the analogue input 
int humiditySensor = 0; // variable to store the value coming from the sensor
float humidity;

//BMP
const unsigned char OSS = 0;  // Oversampling Setting
// Calibration values
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 

short temperature;
long pressure;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  //attachInterrupt(digitalPinToInterrupt(19), check, CHANGE);
  myservo.attach(9);
  pinMode(outPin, INPUT);
  
  bmp_setup();

  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014
  
  //rtc_setup();
}


void loop() {
 receivedSlider = 0;
 hih_read();
 bmp_read();
 send_data();
 //scheduleMovement();
 rain_read();
 motor();
 //receive_data();

 if (dataReceiveFlag == 1) {
 process_data();
}
}

void bmp_setup() {
  bmp085Calibration();
}

/*void rtc_setup {
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014
}*/


void hih_read() {
  humiditySensor = analogRead(outPin); // read the voltage on the out pin
  float v = (humidity*5.0)/1024.0;
  humidity = (v - 0.958)/0.0307;
  humidity = ((-1)*humidity);
  delay(10); 
}

void bmp_read() {
  temperature = bmp085GetTemperature(bmp085ReadUT());
  pressure = bmp085GetPressure(bmp085ReadUP());
  //delay(100);
}


// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;
  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8) >> 4);  
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write((byte)address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available());
    
  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write((byte)address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2);
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write((byte)0xF4);
  Wire.write((byte)0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write((byte)0xF4);
  Wire.write((byte)(0x34 + (OSS<<6)));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write((byte)0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3);
  msb  = Wire.read();
  lsb  = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}


void rain_read(){         
  val = analogRead(analogPin);     // read the input pin
     //Serial.println(val);
  if (val < 500)
     rain = 1; 
  else  
     rain = 0; 
}


void send_data() {
  bluetoothData.remove(0);
  //
  bluetoothData.concat("values");
  bluetoothData.concat(colon);
  //
  bluetoothData.concat(String(temperature * 0.1, 1));
  bluetoothData.concat(colon);
  bluetoothData.concat(String(pressure));
  bluetoothData.concat(colon);
  bluetoothData.concat(String(humidity));
  bluetoothData.concat(colon);
  //
  bluetoothData.concat(degree); // motor reading
  bluetoothData.concat(colon);
  //
  bluetoothData.concat(String(rain));
  bluetoothData.concat(colon);
  //bluetoothData.concat('\n');
  Serial.print(bluetoothData);  

  delay(20);

  receive_data();
  delay(20);
  //Serial.flush();
 }


void receive_data() {
  //Serial.print("Entered ISR");
    if (Serial.available() > 0) {
      receiveData = Serial.readString();
//      Serial.println(receiveData);
    }

    process_data();
    //dataReceiveFlag = 1;
    //Serial.print(dataReceiveFlag);
}


void process_data(){
  //Serial.print("Entered Process Data");
    int i = 0;

    if (receiveData[i] == 't') {
      i++; 
      if (receiveData[i] == ' '){
        i++;
        receivedSlider = 1;
        process_timedata(i);
      }
    }    

    if (receiveData[i] == 'd') {
      i++; 
      if (receiveData[i] == ' '){
        i++;
        process_degreedata(i);
      }
    } 

    dataReceiveFlag = 0;
    //Serial.print(dataReceiveFlag);
  }


void process_timedata(int i) {
  int day, day1, day2, month, month1, month2, hourtotal, hour1, hour2, minute, minute1, minute2, year1, year2, year3, year4, yeartotal;
   
   day1 = receiveData[i] - 48;
   i++;

   if (receiveData[i] == ':'){
    day = day1;
    i++;
   } else {
    day2 = receiveData[i] - 48;
    day = (day1*10) + day2;
    i = i + 2;
   }
   
  //Serial.print(day);
  //Serial.print('\n');

   month1 = receiveData[i] - 48;
   i++;

   if (receiveData[i] == ':'){
    month = month1;
    i++;
   } else {
    month2 = receiveData[i] - 48;
    month = (month1*10) + month2;
    i = i + 2;
   }

  //Serial.print(month);
  //Serial.print('\n');

   year1 = receiveData[i] - 48;
   i++;
   year2 = receiveData[i] - 48;
   i++;
   year3 = receiveData[i] - 48;
   i++;
   year4 = receiveData[i] - 48;

   yeartotal = (year1*1000) + (year2*100) + (year3*10) + year4;
   i= i + 2;

  //Serial.print(yeartotal);
  //Serial.print('\n');

   hour1 = receiveData[i] - 48;
   i++;

   if (receiveData[i] == ':'){
    hourtotal = hour1;
    i++;
   } else {
    hour2 = receiveData[i] - 48;
    hourtotal = (hour1*10) + hour2;
    i = i + 2;
   }

  //Serial.print(hourtotal);
  //Serial.print('\n');

   minute1 = receiveData[i] - 48;
   i++;

   if (receiveData[i] == ';'){
    minute = minute1;
    i++;
   } else {
    minute2 = receiveData[i] - 48;
    minute = (minute1*10) + minute2;
    i = i + 2;
   }

  //Serial.print(minute);
  //Serial.print('\n');
}  



void process_degreedata(int i) {
  int degree2;
  
  //Serial.print("Enter Degree Data");
  
  int degree1 = receiveData[i] - 48;
  i++;

  if (receiveData[i] == ';'){
    degree = degree1;
  } else {
    degree2 = receiveData[i] - 48;
    degree = (degree1*10) + degree2;
  }
  
  //Serial.print(degree);
  //Serial.print('\n');
}


void motor() {
//  Serial.print(degree);
  //Serial.println(degree);
  
  if (receivedSlider == 0 && rain == 1) {
     myservo.write(0);              
  } else if (receivedSlider == 1 && rain == 1) {
    myservo.write(degree); 
  } else if (receivedSlider == 0 && rain == 0) {
    myservo.write(90);  
  }else if (receivedSlider == 1 && rain == 0) {
    myservo.write(degree); 
  }
  
}

void check(){
  //Serial.print("Interrupt");
}

/*void scheduleMovement(){
  int scheduleDay, scheduleMonth, scheduleYear, scheduleHour, scheduleMinute;
  
  scheduleDay = (rtc.getDateStr(0) - 48)*10 + (rtc.getDateStr(1) - 48);
  scheduleMonth = (rtc.getDateStr(3) - 48)*10 + (rtc.getDateStr(4) - 48);
  scheduleYear = (rtc.getDateStr(6) - 48)*1000 + (rtc.getDateStr(7) - 48)*100 + (rtc.getDateStr(8) - 48)*10 + (rtc.getDateStr(9) - 48);
  scheduleHour = (rtc.getTimeStr(0) - 48)*10 + (rtc.getTimeStr(1) - 48);
  scheduleMinute = (rtc.getTimeStr(3) - 48)*10 + (rtc.getTimeStr(4) - 48);

  if (scheduleDay == day){
    if (scheduleMonth == month){
      if (scheduleYear == yeartotal) {
        if (scheduleHour == hourtotal) {
          if (scheduleMinute == minute) {
            motor();
          }
        }
      }
  }*/


