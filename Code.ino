#include <DS3231.h> // DS3231 RTC Library
#include <Wire.h> // I2C Connection Library
#include <LiquidCrystal.h> // LCD Library
#include <EEPROM.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Arduino pins to LCD

#define bt_time A0 // Button to set time
#define bt_increment A1 // Button to increment values
#define bt_decrement A2 // Button to decrement values
#define bt_schedule A3 // Button to set automation schedule

#define AutomationRelay 8 // Relay for automation control
#define StatusLight 13 // Status indicator light

// Init DS3231
DS3231 rtc(SDA, SCL);

// Init a Time-data structure
Time t; // Time structure for RTC

int CurrentHour = 0, CurrentMinute = 0, CurrentSecond = 0, CurrentDay = 0, CurrentMonth = 0, CurrentYear = 0;
int SetDayOfWeek;
String DayOfWeek = " ";

int AutomationStartHour = 0, AutomationStartMinute = 0, AutomationEndHour = 0, AutomationEndMinute = 0;
int ScheduleMode = 0, SetSchedule = 0, AutomationActive = 1;
int Automation1StartHour, Automation1StartMinute, Automation1EndHour, Automation1EndMinute;
int Automation2StartHour, Automation2StartMinute, Automation2EndHour, Automation2EndMinute;
int Automation3StartHour, Automation3StartMinute, Automation3EndHour, Automation3EndMinute;
int Automation4StartHour, Automation4StartMinute, Automation4EndHour, Automation4EndMinute;

int Automation1Active, Automation2Active, Automation3Active;

int StopSchedule = 0, CurrentAutomation = 0, ButtonFlag = 0;

void setup() {
  rtc.begin(); // Start I2C connection with RTC

  pinMode(bt_time, INPUT_PULLUP);
  pinMode(bt_increment, INPUT_PULLUP);
  pinMode(bt_decrement, INPUT_PULLUP);
  pinMode(bt_schedule, INPUT_PULLUP);

  pinMode(AutomationRelay, OUTPUT);
  digitalWrite(AutomationRelay, HIGH);

  pinMode(StatusLight, OUTPUT);

  lcd.begin(16, 2); // Initialize LCD (16 columns, 2 rows)
  lcd.setCursor(0, 0);
  lcd.print("Home Automation ");
  lcd.setCursor(0, 1);
  lcd.print("3-Schedules/Day ");
  delay(2000);
  lcd.clear();

  StopSchedule = EEPROM.read(50);
  if (StopSchedule == 0) {
  } else {
    CurrentAutomation = 1; WriteEeprom(); delay(20);
    CurrentAutomation = 2; WriteEeprom(); delay(20);
    CurrentAutomation = 3; WriteEeprom(); delay(20);
    CurrentAutomation = 4; WriteEeprom(); delay(20);
    CurrentAutomation = 0;
  }
  EEPROM.write(50, 0);

  ReadEeprom();
  // Set RTC for first-time use (uncomment to set initial time)
  // rtc.setDOW(2); // Set Day-of-Week to MONDAY
  // rtc.setTime(00, 9, 50);
  // rtc.setDate(12, 11, 2017);
}

void loop() {
  t = rtc.getTime();
  DayOfWeek = rtc.getDOWStr(1);

  if (ScheduleMode == 0) {
    CurrentHour = t.hour, DEC;
    CurrentMinute = t.min, DEC;
    CurrentSecond = t.sec, DEC;
    CurrentDay = t.date, DEC;
    CurrentMonth = t.mon, DEC;
    CurrentYear = t.year, DEC;
  }

  if (SetSchedule == 0) {
    lcd.setCursor(0, 0);
    lcd.print((CurrentHour / 10) % 10);
    lcd.print(CurrentHour % 10);
    lcd.print(":");
    lcd.print((CurrentMinute / 10) % 10);
    lcd.print(CurrentMinute % 10);
    lcd.print(":");
    lcd.print((CurrentSecond / 10) % 10);
    lcd.print(CurrentSecond % 10);

    lcd.setCursor(1, 1);
    lcd.print(DayOfWeek);
    lcd.print(" ");
    lcd.print((CurrentDay / 10) % 10);
    lcd.print(CurrentDay % 10);
    lcd.print("/");
    lcd.print((CurrentMonth / 10) % 10);
    lcd.print(CurrentMonth % 10);
    lcd.print("/");
    lcd.print((CurrentYear / 1000) % 10);
    lcd.print((CurrentYear / 100) % 10);
    lcd.print((CurrentYear / 10) % 10);
    lcd.print(CurrentYear % 10);
  }

  setupClock();
  setAutomation();
  delay(100);
  blinking();

  // Automation 1 ON
  if (Automation1Active == 1 && AutomationActive == 1 && CurrentHour == Automation1StartHour && CurrentMinute == Automation1StartMinute) {
    digitalWrite(AutomationRelay, LOW);
  }
  // Automation 1 OFF
  if (Automation1Active == 1 && AutomationActive == 1 && CurrentHour == Automation1EndHour && CurrentMinute == Automation1EndMinute) {
    digitalWrite(AutomationRelay, HIGH);
  }

  // Automation 2 ON
  if (Automation2Active == 1 && AutomationActive == 1 && CurrentHour == Automation2StartHour && CurrentMinute == Automation2StartMinute) {
    digitalWrite(AutomationRelay, LOW);
  }
  // Automation 2 OFF
  if (Automation2Active == 1 && AutomationActive == 1 && CurrentHour == Automation2EndHour && CurrentMinute == Automation2EndMinute) {
    digitalWrite(AutomationRelay, HIGH);
  }

  // Automation 3 ON
  if (Automation3Active == 1 && AutomationActive == 1 && CurrentHour == Automation3StartHour && CurrentMinute == Automation3StartMinute) {
    digitalWrite(AutomationRelay, LOW);
  }
  // Automation 3 OFF
  if (Automation3Active == 1 && AutomationActive == 1 && CurrentHour == Automation3EndHour && CurrentMinute == Automation3EndMinute) {
    digitalWrite(AutomationRelay, HIGH);
  }

  delay(100);
  digitalWrite(StatusLight, LOW);
}

void blinking() {
  // BLINKING SCREEN FOR TIME AND DATE SETTING
  if (SetSchedule < 2 && ScheduleMode == 1) { lcd.setCursor(0, 0); lcd.print(" "); }
  if (SetSchedule < 2 && ScheduleMode == 2) { lcd.setCursor(3, 0); lcd.print(" "); }
  if (SetSchedule < 2 && ScheduleMode == 3) { lcd.setCursor(6, 0); lcd.print(" "); }
  if (SetSchedule < 2 && ScheduleMode == 4) { lcd.setCursor(1, 1); lcd.print(" "); }
  if (SetSchedule < 2 && ScheduleMode == 5) { lcd.setCursor(5, 1); lcd.print(" "); }
  if (SetSchedule < 2 && ScheduleMode == 6) { lcd.setCursor(8, 1); lcd.print(" "); }
  if (SetSchedule < 2 && ScheduleMode == 7) { lcd.setCursor(11, 1); lcd.print(" "); }

  // BLINKING SCREEN FOR AUTOMATION SETTINGS
  if (ScheduleMode == 0 && SetSchedule == 1 && CurrentAutomation == 0) { lcd.setCursor(2, 1); lcd.print(" "); }
  if (ScheduleMode == 0 && SetSchedule == 2 && CurrentAutomation == 0) { lcd.setCursor(6, 1); lcd.print(" "); }
  if (ScheduleMode == 0 && SetSchedule == 3 && CurrentAutomation == 0) { lcd.setCursor(10, 1); lcd.print(" "); }
  if (ScheduleMode == 0 && SetSchedule == 4 && CurrentAutomation == 0) { lcd.setCursor(13, 1); lcd.print(" "); }

  if (ScheduleMode == 0 && SetSchedule == 1 && CurrentAutomation > 0) { lcd.setCursor(11, 0); lcd.print(" "); }
  if (ScheduleMode == 0 && SetSchedule == 2 && CurrentAutomation > 0) { lcd.setCursor(14, 0); lcd.print(" "); }
  if (ScheduleMode == 0 && SetSchedule == 3 && CurrentAutomation > 0) { lcd.setCursor(11, 1); lcd.print(" "); }
  if (ScheduleMode == 0 && SetSchedule == 4 && CurrentAutomation > 0) { lcd.setCursor(14, 1); lcd.print(" "); }
}

void setupClock() {
  if (ScheduleMode == 8) {
    lcd.setCursor(0, 0);
    lcd.print("Set Time Done ");
    lcd.setCursor(0, 1);
    lcd.print("Set Date Done ");
    delay(1000);
    rtc.setTime(CurrentHour, CurrentMinute, CurrentSecond);
    rtc.setDate(CurrentDay, CurrentMonth, CurrentYear);
    lcd.clear();
    ScheduleMode = 0;
  }

  if (SetSchedule > 0) { AutomationActive = 0; }

  if (digitalRead(bt_time) == 0 && ButtonFlag == 0) {
    ButtonFlag = 1;
    if (SetSchedule > 0) {
      WriteEeprom();
      SetSchedule = 1;
      CurrentAutomation = 4;
    } else {
      ScheduleMode = ScheduleMode + 1;
    }
    digitalWrite(StatusLight, HIGH);
  }

  if (digitalRead(bt_schedule) == 0 && ButtonFlag == 0) {
    ButtonFlag = 1;
    if (ScheduleMode > 0) {
      ScheduleMode = 8;
    } else {
      SetSchedule = SetSchedule + 1;
      if (SetSchedule > 4) {
        SetSchedule = 1;
        WriteEeprom();
        CurrentAutomation = CurrentAutomation + 1;
        ReadEeprom();
      }
    }
    lcd.clear();
    digitalWrite(StatusLight, HIGH);
  }

  if (SetSchedule == 1 && CurrentAutomation == 4) {
    lcd.setCursor(0, 0);
    lcd.print("Set Sched Done ");
    lcd.setCursor(0, 1);
    lcd.print("-EEPROM Updated-");
    delay(2000);
    lcd.clear();
    SetSchedule = 0;
    CurrentAutomation = 0;
    AutomationActive = 1;
  }

  if (digitalRead(bt_time) == 1 && digitalRead(bt_schedule) == 1) { ButtonFlag = 0; }

  if (digitalRead(bt_increment) == 0) {
    if (SetSchedule < 2 && ScheduleMode == 1) CurrentHour = CurrentHour + 1;
    if (SetSchedule < 2 && ScheduleMode == 2) CurrentMinute = CurrentMinute + 1;
    if (SetSchedule < 2 && ScheduleMode == 3) CurrentSecond = CurrentSecond + 1;
    if (SetSchedule < 2 && ScheduleMode == 4) SetDayOfWeek = SetDayOfWeek + 1;
    if (SetSchedule < 2 && ScheduleMode == 5) CurrentDay = CurrentDay + 1;
    if (SetSchedule < 2 && ScheduleMode == 6) CurrentMonth = CurrentMonth + 1;
    if (SetSchedule < 2 && ScheduleMode == 7) CurrentYear = CurrentYear + 1;
    // Automation settings
    if (CurrentAutomation == 0 && ScheduleMode == 0 && SetSchedule == 1) Automation1Active = 1;
    if (CurrentAutomation == 0 && ScheduleMode == 0 && SetSchedule == 2) Automation2Active = 1;
    if (CurrentAutomation == 0 && ScheduleMode == 0 && SetSchedule == 3) Automation3Active = 1;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 1) AutomationStartHour = AutomationStartHour + 1;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 2) AutomationStartMinute = AutomationStartMinute + 1;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 3) AutomationEndHour = AutomationEndHour + 1;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 4) AutomationEndMinute = AutomationEndMinute + 1;

    if (CurrentHour > 23) CurrentHour = 0;
    if (CurrentMinute > 59) CurrentMinute = 0;
    if (CurrentSecond > 59) CurrentSecond = 0;
    if (SetDayOfWeek > 7) SetDayOfWeek = 0;
    if (CurrentDay > 31) CurrentDay = 0;
    if (CurrentMonth > 12) CurrentMonth = 0;
    if (CurrentYear > 2030) CurrentYear = 2000;
    if (AutomationStartHour > 23) AutomationStartHour = 0;
    if (AutomationStartMinute > 59) AutomationStartMinute = 0;
    if (AutomationEndHour > 23) AutomationEndHour = 0;
    if (AutomationEndMinute > 59) AutomationEndMinute = 0;
    rtc.setDOW(SetDayOfWeek);
    digitalWrite(StatusLight, HIGH);
  }

  if (digitalRead(bt_decrement) == 0) {
    if (SetSchedule < 2 && ScheduleMode == 1) CurrentHour = CurrentHour - 1;
    if (SetSchedule < 2 && ScheduleMode == 2) CurrentMinute = CurrentMinute - 1;
    if (SetSchedule < 2 && ScheduleMode == 3) CurrentSecond = CurrentSecond - 1;
    if (SetSchedule < 2 && ScheduleMode == 4) SetDayOfWeek = SetDayOfWeek - 1;
    if (SetSchedule < 2 && ScheduleMode == 5) CurrentDay = CurrentDay - 1;
    if (SetSchedule < 2 && ScheduleMode == 6) CurrentMonth = CurrentMonth - 1;
    if (SetSchedule < 2 && ScheduleMode == 7) CurrentYear = CurrentYear - 1;
    // Automation settings
    if (CurrentAutomation == 0 && ScheduleMode == 0 && SetSchedule == 1) Automation1Active = 0;
    if (CurrentAutomation == 0 && ScheduleMode == 0 && SetSchedule == 2) Automation2Active = 0;
    if (CurrentAutomation == 0 && ScheduleMode == 0 && SetSchedule == 3) Automation3Active = 0;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 1) AutomationStartHour = AutomationStartHour - 1;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 2) AutomationStartMinute = AutomationStartMinute - 1;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 3) AutomationEndHour = AutomationEndHour - 1;
    if (CurrentAutomation > 0 && ScheduleMode == 0 && SetSchedule == 4) AutomationEndMinute = AutomationEndMinute - 1;

    if (CurrentHour < 0) CurrentHour = 23;
    if (CurrentMinute < 0) CurrentMinute = 59;
    if (CurrentSecond < 0) CurrentSecond = 59;
    if (SetDayOfWeek < 0) SetDayOfWeek = 7;
    if (CurrentDay < 0) CurrentDay = 31;
    if (CurrentMonth < 0) CurrentMonth = 12;
    if (CurrentYear < 2000) CurrentYear = 2030;
    if (AutomationStartHour < 0) AutomationStartHour = 23;
    if (AutomationStartMinute < 0) AutomationStartMinute = 59;
    if (AutomationEndHour < 0) AutomationEndHour = 23;
    if (AutomationEndMinute < 0) AutomationEndMinute = 59;
    rtc.setDOW(SetDayOfWeek);
    digitalWrite(StatusLight, HIGH);
  }
}

void setAutomation() {
  // Display automation settings
  if (ScheduleMode == 0 && SetSchedule > 0 && CurrentAutomation > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Sched");
    lcd.print(CurrentAutomation);
    lcd.print(" Start:");
    lcd.setCursor(11, 0);
    lcd.print((AutomationStartHour / 10) % 10);
    lcd.print(AutomationStartHour % 10);
    lcd.print(":");
    lcd.print((AutomationStartMinute / 10) % 10);
    lcd.print(AutomationStartMinute % 10);

    lcd.setCursor(0, 1);
    lcd.print("Sched");
    lcd.print(CurrentAutomation);
    lcd.print(" End: ");
    lcd.setCursor(11, 1);
    lcd.print((AutomationEndHour / 10) % 10);
    lcd.print(AutomationEndHour % 10);
    lcd.print(":");
    lcd.print((AutomationEndMinute / 10) % 10);
    lcd.print(AutomationEndMinute % 10);
  }

  if (ScheduleMode == 0 && SetSchedule > 0 && CurrentAutomation == 0) {
    lcd.setCursor(0, 0);
    lcd.print(" S1 S2 S3 ");
    lcd.setCursor(0, 1);
    if (Automation1Active == 1) { lcd.print(" A"); } else { lcd.print(" D"); }
    if (Automation2Active == 1) { lcd.print(" A"); } else { lcd.print(" D"); }
    if (Automation3Active == 1) { lcd.print(" A"); } else { lcd.print(" D"); }
  }
}

void ReadEeprom() {
  Automation1StartHour = EEPROM.read(11); Automation1StartMinute = EEPROM.read(12); Automation1EndHour = EEPROM.read(13); Automation1EndMinute = EEPROM.read(14);
  Automation2StartHour = EEPROM.read(21); Automation2StartMinute = EEPROM.read(22); Automation2EndHour = EEPROM.read(23); Automation2EndMinute = EEPROM.read(24);
  Automation3StartHour = EEPROM.read(31); Automation3StartMinute = EEPROM.read(32); Automation3EndHour = EEPROM.read(33); Automation3EndMinute = EEPROM.read(34);
  Automation4StartHour = EEPROM.read(41); Automation4StartMinute = EEPROM.read(42); Automation4EndHour = EEPROM.read(43); Automation4EndMinute = EEPROM.read(44);

  if (CurrentAutomation == 1) {
    AutomationStartHour = Automation1StartHour; AutomationStartMinute = Automation1StartMinute; AutomationEndHour = Automation1EndHour; AutomationEndMinute = Automation1EndMinute;
  }
  if (CurrentAutomation == 2) {
    AutomationStartHour = Automation2StartHour; AutomationStartMinute = Automation2StartMinute; AutomationEndHour = Automation2EndHour; AutomationEndMinute = Automation2EndMinute;
  }
  if (CurrentAutomation == 3) {
    AutomationStartHour = Automation3StartHour; AutomationStartMinute = Automation3StartMinute; AutomationEndHour = Automation3EndHour; AutomationEndMinute = Automation3EndMinute;
  }
  if (CurrentAutomation == 4) {
    AutomationStartHour = Automation4StartHour; AutomationStartMinute = Automation4StartMinute; AutomationEndHour = Automation4EndHour; AutomationEndMinute = Automation4EndMinute;
  }

  Automation1Active = EEPROM.read(1);
  Automation2Active = EEPROM.read(2);
  Automation3Active = EEPROM.read(3);
}

void WriteEeprom() {
  if (CurrentAutomation == 1) {
    EEPROM.write(11, AutomationStartHour); EEPROM.write(12, AutomationStartMinute); EEPROM.write(13, AutomationEndHour); EEPROM.write(14, AutomationEndMinute);
  }
  if (CurrentAutomation == 2) {
    EEPROM.write(21, AutomationStartHour); EEPROM.write(22, AutomationStartMinute); EEPROM.write(23, AutomationEndHour); EEPROM.write(24, AutomationEndMinute);
  }
  if (CurrentAutomation == 3) {
    EEPROM.write(31, AutomationStartHour); EEPROM.write(32, AutomationStartMinute); EEPROM.write(33, AutomationEndHour); EEPROM.write(34, AutomationEndMinute);
  }
  if (CurrentAutomation == 4) {
    EEPROM.write(41, AutomationStartHour); EEPROM.write(42, AutomationStartMinute); EEPROM.write(43, AutomationEndHour); EEPROM.write(44, AutomationEndMinute);
  }

  EEPROM.write(1, Automation1Active);
  EEPROM.write(2, Automation2Active);
  EEPROM.write(3, Automation3Active);
}

