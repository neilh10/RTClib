#include <Arduino.h>

// Date and time functions using a PCF2127 RTC connected via I2C and Wire lib/
//Tested with an Adafruit Feather M4 express and board with a PCF2127T on PlatformIO/VSC
//Uses https://github.com/neilh10/RTClib:release1
#include "RTClib.h"

RTC_PCF2127 rtcExtPh;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
uint8_t ledState=0;

//#define SerialUSB Serial
//#define SerialTty Serial1
//B031r1 M4 Serial4 
#define SerialTty Serial4
//#define SerialTty Serial1

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  while (!SerialTty) {
    delay(1);  // for Leonardo/Micro/Zero
  }

  SerialTty.begin(115200);
  if (! rtcExtPh.begin()) {
    SerialTty.println("Couldn't find RTC");
    Serial.flush(); // ensure above text prints with nRF52 native USB Serial
    delay(100);
    while (1);
  }
  rtcExtPh.init();

  RTC_PCF2127::ErrorNum err_ret = rtcExtPh.initialized();
  if (RTC_PCF2127::NO_ERROR != err_ret) {

    SerialTty.print("RTC is NOT running! err=");
    SerialTty.println(err_ret);
    // following line sets the RTC to the date & time this sketch was compiled
    // rtcExtPh.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2019 at 0am you would call:
    rtcExtPh.init();
    rtcExtPh.adjust(DateTime(2019, 1, 1, 0, 0, 0));
  } 
 
  SerialTty.println("****Restarting***");
}

void loop () {
  ledState++;
  digitalWrite(LED_BUILTIN, ledState&1);
  DateTime now = rtcExtPh.now();
  SerialTty.print(now.year(), DEC);
  SerialTty.print('/');
  SerialTty.print(now.month(), DEC);
  SerialTty.print('/');
  SerialTty.print(now.day(), DEC);
  SerialTty.print(" (");
  SerialTty.print(daysOfTheWeek[now.dayOfTheWeek()]);
  SerialTty.print(") ");
  SerialTty.print(now.hour(), DEC);
  SerialTty.print(':');
  SerialTty.print(now.minute(), DEC);
  SerialTty.print(':');
  SerialTty.print(now.second(), DEC);
  SerialTty.println();

  #if 0
  //Possible other tests - I find way too distracting
  SerialTty.print(" since midnight 1/1/1970 = ");
  SerialTty.print(now.unixtime());
  SerialTty.print("s = ");
  SerialTty.print(now.unixtime() / 86400L);
  SerialTty.println("d");

  // calculate a date which is 7 days, 12 hours and 30 seconds into the future
  //DateTime future (now + TimeSpan(7,12,30,6));

  SerialTty.print(" now + 7d + 12h + 30m + 6s: ");
  SerialTty.print(future.year(), DEC);
  SerialTty.print('/');
  SerialTty.print(future.month(), DEC);
  SerialTty.print('/');
  SerialTty.print(future.day(), DEC);
  SerialTty.print(' ');
  SerialTty.print(future.hour(), DEC);
  SerialTty.print(':');
  SerialTty.print(future.minute(), DEC);
  SerialTty.print(':');
  SerialTty.print(future.second(), DEC);
  SerialTty.println();

  SerialTty.println();
  #endif //0

  delay(2000);

}