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


        // Cancel button.
        if (x < 80 && y > 240-32) { while (ttgo->getTouch(x, y)) { } requested = MENU; break; }

        // Date button.
        if (x < 160 && y > 240-32) { while (ttgo->getTouch(x, y)) { } requested = SET_DATE; break; }
        
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

#endif
