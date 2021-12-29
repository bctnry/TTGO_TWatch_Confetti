#ifndef CONFETTI_SET_TIME
#define CONFETTI_SET_TIME
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
    
    ttgo->tft->drawBitmap(30-12, 240-24, EXIT_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(150-12, 240-24, DATE_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(210-12, 240-24, DIAMOND_24, 24, 24, TFT_WHITE);
}
void enterSetTimeScreen() {
    clearMainUI();
    updateSetTimeUI();

    int16_t x, y;
    uint8_t key = 10;
    while (true) {
        waitTouchDown(x, y);
        waitTouchUp(x, y);

        // Cancel button.
        if (X_IN_FIRST_QUARTER(x) && y > 240-32) { requested = MENU; break; }

        // Date button.
        if (X_IN_THIRD_QUARTER(x) && y > 240-32) { requested = SET_DATE; break; }
        
        // OK button.
        if (X_IN_FOURTH_QUARTER(x) && y > 240-32) {
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
    }
    leave:
    clearMainUI();
}

#endif
