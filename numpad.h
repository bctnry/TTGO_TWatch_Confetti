#ifndef CONFETTI_INPUT_NUMPAD
#define CONFETTI_INPUT_NUMPAD

char numpad_input[255] = {};
uint8_t numpad_input_i = 0;

void updateNumPadDisplayBoxUI() {
    ttgo->tft->fillRect(0, 32, 240, 42, TFT_WHITE);
    ttgo->tft->setTextColor(TFT_BLACK, TFT_WHITE);
    if (numpad_input_i <= 30) {
        ttgo->tft->setTextFont(4);
        for (uint8_t i = 0; i < MIN(numpad_input_i, 240/16); i++) {
            ttgo->tft->setCursor(i * 16, 32);
            snprintf(buf, sizeof(buf), "%c", numpad_input[i]);
            ttgo->tft->print(buf);
        }
        if (numpad_input_i > 240/16) {
            for (uint8_t i = 15; i < numpad_input_i; i++) {
                ttgo->tft->setCursor((i-15) * 16, 32+16);
                snprintf(buf, sizeof(buf), "%c", numpad_input[i]);
                ttgo->tft->print(buf);
            }
        }
    } else {
        ttgo->tft->setTextFont(2);
        for (uint8_t i = 0; i < MIN(numpad_input_i, 29); i++) {
            ttgo->tft->setCursor(i * 8, 32);
            snprintf(buf, sizeof(buf), "%c", numpad_input[i]);
            ttgo->tft->print(buf);
        }
        if (numpad_input_i > 29) {
            for (uint8_t i = 29; i < numpad_input_i; i++) {
                ttgo->tft->setCursor((i-29) * 8, 32+16);
                snprintf(buf, sizeof(buf), "%c", numpad_input[i]);
                ttgo->tft->print(buf);
            }
        }
        
    }
}

void updateNumPadUI() {
    updateNumPadDisplayBoxUI();
    
    ttgo->tft->setTextFont(6);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    for (uint8_t j = 0; j < 3; j++) {
        for(uint8_t i = 0; i < 3; i++) {
            ttgo->tft->setCursor(i * 60, j * 40 + 72);
            snprintf(buf, sizeof(buf), "%d", ((j*3+(i+1))));
            ttgo->tft->print(buf);
        }
    }
    ttgo->tft->setCursor(3 * 60, 2 * 40 + 72);
    ttgo->tft->print("0");
    ttgo->tft->setTextFont(4);
    ttgo->tft->setCursor(3 * 60, 72);
    ttgo->tft->print("*");
    ttgo->tft->setCursor(3 * 60, 40 + 72);
    ttgo->tft->print("#");
  
    ttgo->tft->drawBitmap(30-12, 240-24, EXIT_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(150-12, 240-24, LEFT_ARROW_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(210-12, 240-24, DIAMOND_24, 24, 24, TFT_WHITE);
}

char checkNumPadInputKey(uint16_t x, uint16_t y) {
    uint8_t x_quarter = (
        X_IN_FIRST_QUARTER(x)? 0
        : X_IN_SECOND_QUARTER(x)? 1
        : X_IN_THIRD_QUARTER(x)? 2
        : 3
    );
    uint8_t y_part = (
        y <= 40+72? 0
        : y <= 80+72? 1
        : 2
    );
    return "123*456#7890"[y_part*4+x_quarter];
}

char* enterNumPadScreen() {
    clearMainUI();
    updateTitleUI("Numpad Demo");
    updateNumPadUI();
    
    int16_t x, y;
    uint8_t target = 0;
    while (true) {
        waitTouchDown(x, y);
        waitTouchUp(x, y);
        
        // Back button.
        if (X_IN_FIRST_QUARTER(x) && y > 240-32) {
            current_menu_selection = CLOCK;
            numpad_input_i = 0;
            numpad_input[0] = 0;
            clearMainUI();
            return NULL;
        }

        // backspace button.
        if (X_IN_THIRD_QUARTER(x) && y > 240-32 && numpad_input_i > 0) {
            numpad_input_i--;
            updateNumPadDisplayBoxUI();
            continue;
        }

        // confirm button.
        if (X_IN_FOURTH_QUARTER(x) && y > 240-32) {
            numpad_input[numpad_input_i] = 0;
            break;
        }

        if (numpad_input_i > 58) { continue; }
        numpad_input[numpad_input_i++] = checkNumPadInputKey(x, y);
        updateNumPadDisplayBoxUI();
    }
    clearMainUI();
    return numpad_input;
}

#endif
