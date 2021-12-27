#include "config.h"
#include <soc/rtc.h>

TTGOClass* ttgo;
bool irq = 0;
char buf[128];
uint32_t targetTime = 0;

uint8_t hh, mm, ss, mmonth, dday;
uint16_t yyear;
uint8_t day_of_week;

char day_of_week_str[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

void updateInitUI() {
    ttgo->tft->setTextFont(2);
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->println("TTGO_TWatch_Confetti");
}

void updateTitleUI(const char* string) {  
    ttgo->tft->setTextFont(2);
    ttgo->tft->setCursor(0, 16);
    ttgo->tft->print(string);
}

const uint16_t time_ui_base = 96;

void updateTimeUI() {
    RTC_Date tnow = ttgo->rtc->getDateTime();
    uint16_t base = time_ui_base;
    ttgo->tft->setTextFont(2);
    ttgo->tft->setCursor(0, base);
    ttgo->tft->setTextFont(6);
    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;
    day_of_week = ttgo->rtc->getDayOfWeek(dday, mmonth, yyear);
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hh, mm, ss);
    ttgo->tft->print(buf);
    ttgo->tft->setTextFont(6);
    ttgo->tft->setCursor(0, base+48);
    snprintf(buf, sizeof(buf), "%d.%d\n%02d ", yyear, mmonth, dday);
    ttgo->tft->print(buf);
    ttgo->tft->setTextFont(4);
    ttgo->tft->setCursor(64, 208);
    snprintf(buf, sizeof(buf), "%s %d%%", day_of_week_str[day_of_week], ttgo->power->getBattPercentage());
    ttgo->tft->println(buf);
}

void setup() {
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    ttgo->power->clearIRQ();
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);
    
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    ttgo->power->clearIRQ();
    
    ttgo->rtc->check();
    ttgo->rtc->syncToSystem();
    targetTime = millis() + 1000;
    updateInitUI();
    updateTitleUI("Version 2021.12.28");
    updateTimeUI();
}

uint8_t requested = 0;
const uint8_t SET_TIME_REQUESTED = 1;
const uint8_t SET_DATE_REQUESTED = 2;

void updateSetTimeUI() {
    updateInitUI();
    updateTitleUI("Configuration - Set Time");
    ttgo->tft->setCursor(0, 32+48);
    ttgo->tft->setTextFont(6);
    snprintf(buf, sizeof(buf), " %02d  %02d  %02d", hh, mm, ss);
    ttgo->tft->println(buf);
    ttgo->tft->setTextFont(4);
    ttgo->tft->setCursor(40, 32+24);
    ttgo->tft->print("+");
    ttgo->tft->setCursor(40, 32+96);
    ttgo->tft->print("-");
    
    ttgo->tft->setCursor(120, 32+24);
    ttgo->tft->print("+");
    ttgo->tft->setCursor(120, 32+96);
    ttgo->tft->print("-");

    
    ttgo->tft->setCursor(200, 32+24);
    ttgo->tft->print("+");
    ttgo->tft->setCursor(200, 32+96);
    ttgo->tft->print("-");

    ttgo->tft->setCursor(0, 32+96+96-16);
    ttgo->tft->print("Cancel");
    ttgo->tft->setCursor(104, 32+96+96-16);
    ttgo->tft->print("Date");
    ttgo->tft->setCursor(200-16, 32+96+96-16);
    ttgo->tft->print("OK");
}
void enterSetTimeScreen() {
    ttgo->tft->fillScreen(TFT_BLACK);
    updateSetTimeUI();

    uint8_t field = 0;
    int16_t x, y;
    uint8_t key = 10;
    while (true) {
        while (!ttgo->getTouch(x, y)) { }


        // Date button.
        if (x < 80 && y > 240-32) { while (ttgo->getTouch(x, y)) { } goto leave; }
        if (x < 160 && y > 240-32) { while (ttgo->getTouch(x, y)) { } requested = SET_DATE_REQUESTED; break; }
        
        // OK button.
        if (x > 160 && y > 240-32) {
            while (ttgo->getTouch(x, y)) { }
            ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, ss);
            break;
        }

        // + button.
        if (y < 32+48) {
            if (x < 80) { hh++; if (hh > 23) { hh = 0; } }
            else if (x < 160) { mm++; if (mm > 59) { mm = 0; } }
            else { ss++; if (ss > 59) { ss = 0; } }
        }

        // - button.
        if (y > 32+96) {
            if (x < 80) { hh--; if (hh == 255) { hh = 23; } }
            else if (x < 160) { mm--; if (mm == 255) { mm = 59; } }
            else { ss--; if (ss == 255) { ss = 59; } }
        }
        
        ttgo->tft->setCursor(0, 32+48);
        ttgo->tft->setTextFont(6);
        snprintf(buf, sizeof(buf), " %02d  %02d  %02d", hh, mm, ss);
        ttgo->tft->print(buf);
        while (ttgo->getTouch(x, y)) { }
        field++;
    }
    leave:
    ttgo->tft->fillScreen(TFT_BLACK);
}

void updateSetDateUI() {
    updateInitUI();
    updateTitleUI("Configuration - Set Date");
    ttgo->tft->setCursor(0, 32+48);
    ttgo->tft->setTextFont(6);
    snprintf(buf, sizeof(buf), "%d%02d%02d", yyear, mmonth, dday);
    ttgo->tft->println(buf);
    ttgo->tft->setTextFont(4);
    ttgo->tft->setCursor(40, 32+24);
    ttgo->tft->print("+");
    ttgo->tft->setCursor(40, 32+96);
    ttgo->tft->print("-");
    
    ttgo->tft->setCursor(120, 32+24);
    ttgo->tft->print("+");
    ttgo->tft->setCursor(120, 32+96);
    ttgo->tft->print("-");

    
    ttgo->tft->setCursor(200, 32+24);
    ttgo->tft->print("+");
    ttgo->tft->setCursor(200, 32+96);
    ttgo->tft->print("-");

    ttgo->tft->setCursor(0, 32+96+96-16);
    ttgo->tft->print("Cancel");
    ttgo->tft->setCursor(104, 32+96+96-16);
    ttgo->tft->print("Time");
    ttgo->tft->setCursor(200-16, 32+96+96-16);
    ttgo->tft->print("OK");
}
uint8_t normal_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
bool isLeapYear(uint8_t x) {
    return (x % 100 == 0? x % 400 == 0 : x % 4 == 0);
}
void enterSetDateScreen() {
    ttgo->tft->fillScreen(TFT_BLACK);
    updateSetDateUI();

    uint8_t field = 0;
    int16_t x, y;
    uint8_t key = 10;
    while (true) {
        while (!ttgo->getTouch(x, y)) { }


        // Date button.
        if (x < 80 && y > 240-32) { while (ttgo->getTouch(x, y)) { } goto leave; }
        if (x < 160 && y > 240-32) { while (ttgo->getTouch(x, y)) { } requested = SET_TIME_REQUESTED; break; }
        
        // OK button.
        if (x > 160 && y > 240-32) {
            while (ttgo->getTouch(x, y)) { }
            ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, ss);
            break;
        }

        // + button.
        if (y < 32+48) {
            if (x < 80) {
                yyear++;
                if (dday == 29 && !isLeapYear(yyear)) { dday = 28; }
            }
            else if (x < 160) { mmonth++; if (mmonth > 12) { mmonth = 1; } }
            else {
                dday++;
                if (
                  (isLeapYear(yyear) && mmonth == 2 && dday > 29)
                  || (dday > normal_month[mmonth])
                ) {
                  dday = 1;
                }
            }
        }

        // - button.
        if (y > 32+96) {
            if (x < 80) { yyear--; }
            else if (x < 160) { mmonth--; if (mmonth < 1) { mmonth = 12; } }
            else {
                dday--;
                if (dday < 1) {
                    dday = isLeapYear(yyear) && mmonth == 2? 29 : normal_month[mmonth];
                }
            }
        }
        
        ttgo->tft->setCursor(0, 32+48);
        ttgo->tft->setTextFont(6);
        snprintf(buf, sizeof(buf), "%d%02d%02d", yyear, mmonth, dday);
        ttgo->tft->print(buf);
        while (ttgo->getTouch(x, y)) { }
        field++;
    }
    leave:
    ttgo->tft->fillScreen(TFT_BLACK);
}

void dispatchSetScreen() {
    switch (requested) {
        case SET_TIME_REQUESTED: requested = 0; enterSetTimeScreen(); break;
        case SET_DATE_REQUESTED: requested = 0; enterSetDateScreen(); break;
        default: break;
    }
}

void loop() {
    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        updateInitUI();
        updateTitleUI("Version 2021.12.28");
        updateTimeUI();
    }

    int16_t x, y;
    if (ttgo->getTouch(x, y)) {
        requested = 1;
        while (requested) {
            while (ttgo->getTouch(x, y)) { }
            dispatchSetScreen();
        }
    }

    if (irq) {
        irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isPEKShortPressIRQ()) {
            ttgo->power->clearIRQ();
            ttgo->displaySleep();
            ttgo->powerOff();
            ttgo->power->setPowerOutPut(AXP202_LDO3, false);
            ttgo->power->setPowerOutPut(AXP202_LDO4, false);
            ttgo->power->setPowerOutPut(AXP202_LDO2, false);
            ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
            ttgo->power->setPowerOutPut(AXP202_DCDC2, false);
          
            esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
            esp_deep_sleep_start();
        }
        ttgo->power->clearIRQ();
    }
}
