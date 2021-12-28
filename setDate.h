#ifndef CONFETTI_SET_DATE
#define CONFETTI_SET_DATE
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


        // Cancel button.
        if (x < 80 && y > 240-32) { while (ttgo->getTouch(x, y)) { } requested = MENU; break; }

        // Time button.
        if (x < 160 && y > 240-32) { while (ttgo->getTouch(x, y)) { } requested = SET_TIME; break; }
        
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
#endif
