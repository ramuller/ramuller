#include <EEPROM.h>

// DS3231_Serial_Easy
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my DS3231-library to
// quickly send time and date information over a serial link
//
// To use the hardware I2C (TWI) interface of the Arduino you must connect
// the pins as follows:
//
// Arduino Uno/2009:
// ----------------------
// DS3231:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//          SCL pin   -> Arduino Analog 5 or the dedicated SCL pin
//
// Arduino Leonardo:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 2 or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 3 or the dedicated SCL pin
//
// Arduino Mega:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL pin
//
// Arduino Due:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA1 (Digital 70) pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL1 (Digital 71) pin
//
// The internal pull-up resistors will be activated when using the
// hardware I2C interfaces.
//
// You can connect the DS3231 to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access
// to the pins used, and you will also have to use appropriate, external
// pull-up resistors on the data and clock signals.
//
#include <Arduino.h>
#include <util/eu_dst.h>
#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);
int DST;

Time utime;
time_t k_time;

void mySetDate();

void setup()
{
  // Setup Serial connection
  Serial.begin(57600);
  // Uncomment the next line if you are using an Arduino Leonardo
  //while (!Serial) {}

  DST = 1;
  EEPROM.put(0, DST);

  // Initialize the rtc object
  rtc.begin();

  // The following lines can be uncommented to set the date and time
#if 1
  rtc.setTime(23, 59, 58);     // Set the time to hh:mm:ss (24hr format)
  rtc.setDate(10, 10, 2018);   // Set the date to day, month, year
  // rtc.setDate(23, 03, 2019);   // Set the date to day, month, year
  // rtc.setDate(24, 03, 2018);   // Set the date to day, month, year
  rtc.setDOW();
#endif
}

// Day Of Week Sunday 0 ... Saturday 6
int dow(Time *ti)

{ 
  if(ti->dow == SUNDAY)
    return 0;
  return ti->dow;
}

static myprint(char *s, int num)
{
  Serial.print(s);
  Serial.println(num);
}

uint8_t dst(Time *t)
{
  uint8_t dst = 0; // Normal time
  if(t->mon < 10 && t->mon > 3)  // summer time anyway
    dst = 1;
  else if(t->mon == 3 && (t->date - dow(t) >= 24)) // After last Sunday in March
    dst = 1;
  else if(t->mon == 10 && (t->date  - dow(t) < 24)) // Before last Sunday in Oct
    dst = 1;
  myprint("Last DST day ", t->date - dow(t));
  return dst;
}

void loop()
{
  int v;
  
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");

  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());

  utime = rtc.getTime();
  k_time = rtc.getUnixTime(utime) - 946684800;
  Serial.print("DOW : ");
  Serial.println(dow(&utime));
  Serial.print("date : ");
  Serial.println(utime.date);
  v = dst(&utime);
  myprint("DST : ", v);
  myprint("HOUR : ", (utime.hour + v) % 24);

  EEPROM.get(0, DST);
  Serial.println(DST);

  // Wait one second before repeating :)
  delay (1000);
}
