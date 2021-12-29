#include "config.h"
#include <soc/rtc.h>
#include "globalState.h"

#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED true

TTGOClass* ttgo;
bool irq = 0;
char buf[128];
uint32_t targetTime = 0;

void updateTitleUI(const char* string);
void clearMainUI();

#include"common.h"
#include"icons.h"

// Init UI
void updateInitUI() {
    ttgo->tft->setTextFont(2);
    ttgo->tft->setCursor(0, 0);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->println("TTGO_TWatch_Confetti");
}

void updateTitleUI(const char* string) {
    ttgo->tft->fillRect(0, 16, 240, 16, TFT_BLACK);
    ttgo->tft->setTextFont(2);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setCursor(0, 16);
    ttgo->tft->print(string);
}

void clearMainUI() {
    ttgo->tft->fillRect(0, 32, 240, 240-32, TFT_BLACK);
}


// Time UI

const uint16_t time_ui_base = 96;
char day_of_week_str[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
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

// Setup

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
    updateTitleUI("Version 2021.12.29");
    updateTimeUI();

    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
      ttgo->tft->setTextFont(2);
      ttgo->tft->setCursor(0, 32);
      ttgo->tft->println("SPIFFS Mount Failed");
    }    
}

#include"procedureId.h"

#include"menu.h"
#include"setTime.h"
#include"setDate.h"
#include"spiffs.h"


void dispatchSetScreen() {
    switch (requested) {
        case MENU: requested = 1; enterMenuScreen(); break;
        case CLOCK: requested = 1; break;
        case SET_TIME: requested = 1; enterSetTimeScreen(); break;
        case SET_DATE: requested = 1; enterSetDateScreen(); break;
        case SPIFFS_FM: requested = 1; enterSPIFFSScreen(); break;
        default: {
          // no corresponding app.
          requested = 1; break;
        }
    }
}

void loop() {
    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        updateInitUI();
        updateTitleUI("Version 2021.12.30");
        updateTimeUI();
    }

    int16_t x, y;
    if (ttgo->getTouch(x, y)) {
        requested = 0;
        while (requested != CLOCK) {
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
