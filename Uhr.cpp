#include "Uhr.h"


RTC_DATA_ATTR int Welche = 1;

#define DARKMODE true

void Uhr::drawWatchFace() { 

  // reset step counter at midnight
    if (currentTime.Hour == 00 && currentTime.Minute == 00) {
        sensor.resetStepCounter();
        //turn off radios
        WiFi.mode(WIFI_OFF);
        btStop();
    }

    //time adjustment start
    //if you don't need a time adjustment, just delete all lines between time adjustment start and end
    //my watchy runs too fast (15 seconds and 500 ms a day), this code set back time each day by 15 seconds and 500 ms

    if (currentTime.Hour == 00 && currentTime.Minute == 30) {

        RTC.read(currentTime);
        int8_t sekunde = currentTime.Second;
        int8_t minute = currentTime.Minute;
        int8_t hour = currentTime.Hour;
        int8_t day = currentTime.Day;
        int8_t month = currentTime.Month;
        int8_t year = tmYearToY2k(currentTime.Year);

        delay(15500);

        tmElements_t tm;
        tm.Month = month;
        tm.Day = day;
        tm.Year = y2kYearToTm(year);
        tm.Hour = hour;
        tm.Minute = minute;
        tm.Second = sekunde;

        RTC.set(tm);
    }
    // time adjustment end

    uint8_t stundeA =  ((currentTime.Hour + 11) % 12) + 1;
    int minuteA =  currentTime.Minute;
    int8_t batLev = 0;
    float VBAT = getBatteryVoltage();

    if (VBAT > 4) {
        batLev = 3;
    }
    else if (VBAT > 3.80 && VBAT <= 4) {
        batLev = 2;
    }
    else if (VBAT > 3.40 && VBAT <= 3.80) {
        batLev = 1;
    }
    else if (VBAT <= 3.40) {
        batLev = 0;
    }

    uint32_t Schritt = sensor.getCounter();
    
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    int16_t  xc, yc, bk;
    uint16_t wc, hc;
    srand(stundeA + minuteA  + batLev + Schritt);
    const uint8_t* komet[] = { xkomet0, xkomet1, xkomet2, xkomet3, ykomet0, ykomet1, ykomet2, ykomet3 };

  if(Welche == 2){
    display.setFont(&FreeSans40pt7b);
        
        display.setCursor(0, 60);
        
        if (stundeA < 10) {
            display.print("0");
        }
        display.print(stundeA);
        display.print(":");
        if (minuteA < 10) {
            display.print("0");
        }
        display.println(minuteA);

        display.setFont(&FreeSans16pt7b);

        int16_t  x1, y1;
        uint16_t w, h;

        String dayOfWeek = dayShortStr(currentTime.Wday);
        display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
       
        display.setCursor(50, 125);
        display.println(dayOfWeek);

        String month = monthShortStr(currentTime.Month);
        display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
        display.setCursor(50, 95);
        display.println(month);

        display.setCursor(5, 95);
        if (currentTime.Day < 10) {
            display.print("0");
        }
        display.println(currentTime.Day);

        display.drawBitmap(5, 175, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(5, 145, akku, 30, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.setCursor(50, 195);
        display.println(Schritt);
        display.setCursor(50, 165);
        uint8_t Protz = min<int>(100 - ((4.20 - VBAT) * 100), 100);
        Protz = max<int>(Protz, 0);
        display.print(Protz);
        display.print(" %");
        
  } else {
    Schritt = Schritt / 50;
        for (int n = 0; n <= Schritt; n++) {
            xc = rand() % 200;
            yc = rand() % 200;
            display.drawPixel(xc, yc, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        }

        display.drawBitmap(80, 80, xsonne, 40, 40, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

        double winke = (stundeA * 30) + minuteA * 0.5;
        double lange = ((winke + 180) * 71) / 4068.0;
        double cx1 = 100 - (sin(lange) * 60);
        double cy1 = 100 + (cos(lange) * 60);
        display.drawBitmap(cx1 - 15, cy1 - 15, xerde, 30, 30, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

        double minke = minuteA * 6;
        double mange = ((minke + 180) * 71) / 4068.0;
        double cmx1 = (cx1 - (sin(mange) * 28));
        double cmy1 = (cy1 + (cos(mange) * 28));
        display.fillCircle(cmx1, cmy1, 6, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

        
        int mx1 = 5;
        bk = 0;
        if (stundeA >= 6) {
            bk = 4;
            mx1 = 165;
        }

        bk = batLev + bk;
        display.drawBitmap(mx1, 165, komet[bk], 30, 30, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
  }
  
}


void Uhr::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  if (IS_DOUBLE_TAP) {
    while (!sensor.getINT()) {
      }
   }


  if (IS_BTN_RIGHT_UP) {
    vibrate();
    Welche = Welche +1;
    if (Welche>2) {
      Welche =1;
    }
  RTC.read(currentTime);
    uint8_t stundeA =  ((currentTime.Hour + 11) % 12) + 1;
    int minuteA =  currentTime.Minute;
  drawWatchFace();
  display.setPartialWindow(0, 0, 200, 200);  
  showWatchFace(true);
  }
if (IS_BTN_RIGHT_DOWN) {
   }
}

void Uhr::vibrate() {
  
  sensor.enableFeature(BMA423_WAKEUP, false);
  pinMode(VIB_MOTOR_PIN, OUTPUT);
  
    delay(50);
    digitalWrite(VIB_MOTOR_PIN, true);
    delay(50);
    digitalWrite(VIB_MOTOR_PIN, false);
  
  sensor.enableFeature(BMA423_WAKEUP, true);
}



