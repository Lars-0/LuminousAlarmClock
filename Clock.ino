#include <Wire.h>
#include <DS1307.h>
#include <Time.h> // not used
#include <TimeAlarms.h> // not used
/*
about set time:
format: year,month,day,week,hour,min,sec
example: 14,03,25,02,13,55,10   2014.03.25 tuesday 13:55:10
*/
String comdata = "";
int mark=0;
//store the current time data
int rtc[7];
//store the set time data
byte rr[7];
//light pin
int ledPin =  13;
//initial light
void setup()
{
  DDRC |= _BV(2) | _BV(3); // POWER:Vcc Gnd
  PORTC |= _BV(3); // VCC PINC3
  pinMode(ledPin, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  //initial baudrate
  Serial.begin(9600); // toggle this to turn on / off serial connection.
  //get current time
  RTC.get(rtc, true);
  //if time is wrong reset to default time
  //if (rtc[6] < 12) { // If second is less than 12 reset it???
    //stop rtc time
    RTC.stop();
    RTC.set(DS1307_SEC, 00);
    RTC.set(DS1307_MIN, 02);
    RTC.set(DS1307_HR, 20);
    RTC.set(DS1307_DOW, 2);
    RTC.set(DS1307_DATE, 02);
    RTC.set(DS1307_MTH, 11);
    RTC.set(DS1307_YR, 15);
    //start rtc time
    RTC.start();
  //}*/
  //RTC.SetOutput(LOW);
  //RTC.SetOutput(HIGH);
  //RTC.SetOutput(DS1307_SQW1HZ);
  //RTC.SetOutput(DS1307_SQW4KHZ);
  //RTC.SetOutput(DS1307_SQW8KHZ);
  RTC.SetOutput(DS1307_SQW32KHZ);
  RTC.start();
}
 
void loop()
{
  int i;
  //get current time 
  RTC.get(rtc, true);
  //print current time format : year month day week hour min sec
  /*
  //blink the light
  Serial.println();
  Serial.println("Blink!");
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  //
  */
  int j = 0;
  //read all the data
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
    mark = 1;
  }
  //if data is all collected,then parse it
  if (mark == 1)
  {
    Serial.println(comdata);
    Serial.println(comdata.length());
    //parse data
    for (int i = 0; i < comdata.length() ; i++)
    {
      //if the byte is ',' jump it,and parse next value
      if (comdata[i] == ',')
      {
        j++;
      }
      else
      {
        rr[j] = rr[j] * 10 + (comdata[i] - '0');
      }
    }
    comdata = String("");
    RTC.stop();
    RTC.set(DS1307_SEC, rr[6]);
    RTC.set(DS1307_MIN, rr[5]);
    RTC.set(DS1307_HR, rr[4]);
    RTC.set(DS1307_DOW, rr[3]);
    RTC.set(DS1307_DATE, rr[2]);
    RTC.set(DS1307_MTH, rr[1]);
    RTC.set(DS1307_YR, rr[0]);
    RTC.start();
    mark = 0;
  }
 
  if( rtc[2] >= 7 && rtc[2] < 21 ){
    LightsOn();
    PrintTime();
  }else{
    LightsOff();
    //PrintTime();
  }
  //Blink();
  delay(1000);
}
void Blink() {

  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH); 
  digitalWrite(11, HIGH); 
  digitalWrite(10, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);// turn the LED off by making the voltage LOW
  delay(1000); 
  Serial.println("Blink!"); 
}
void Breathe(){
  // Breathing goes here. Probably not worth doing unless they are all on PWM.
}
void LightsOn(){
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH); 
  digitalWrite(11, HIGH); 
  digitalWrite(10, HIGH); // turn the LED on (HIGH is the voltage level)
  Serial.println("Lights On!");
}
void LightsOff(){
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);// turn the LED off by making the voltage LOW
 // Serial.println("Lights Off!");
}
void PrintTime(){
    int i;
    for (i = 0; i < 7; i++)
  {
    Serial.print(rtc[i]);
    Serial.print(" ");
  }
  Serial.println();
}