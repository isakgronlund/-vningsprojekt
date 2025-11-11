/*
* Name: övningsprojekt
* Author: Victor Huke
* Date: 2025-10-14
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>
#include <U8glib.h>



// Init constants

// Init global variables

// Construct objects
RTC_DS3231 rtc;
Servo myServo;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
const int buzzer=11;
int Timer = 0;

void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();
  myServo.attach(9);
  pinMode(4, INPUT);
  pinMode(3, INPUT);
 
  // Init Hardware
  rtc.begin();

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  u8g.setFont(u8g_font_osb21);
  pinMode(buzzer, OUTPUT);

}

void loop() {
 

  Serial.println(getTime());
  Serial.println(rtc.getTemperature());
  delay(200);

  //funktionen gör så att variablen Timer går upp 60 sekunder om Timer är noll
  if (digitalRead(3) == LOW) {
    if (Timer==0)
        Timer += 60;
    }
  //Funktionen gör att Timern går ner varje sekund och när den når värdet noll går ett larm igång i form av en pieso
  if (Timer > 0){
   Timer--;
    if (Timer==0){
    tone(buzzer, 2000);
  }

}
  delay(1000);
  oledDraw(getTime(), String(Timer));  //remove comment when the function is written
  servoWrite(getTemp());
}




//This function reads time from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss as String
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temprature as float 
*/
float getTemp() {
  return rtc.getTemperature();
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledDraw(String text, String Timer) {
  u8g.firstPage();
  do {
    u8g.drawStr(0, 25, text.c_str());
    u8g.drawStr(0, 60, Timer.c_str());
  } while (u8g.nextPage());
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  myServo.write(map(value, 15, 35, 0, 180));
}

