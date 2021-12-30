#ifndef CONFETTI_MENU
#define CONFETTI_MENU

uint8_t current_menu_selection = 1;
// NOTE: base 1, [x, y)
uint8_t menu_upperbound = 1;
uint8_t menu_lowerbound = MIN(MAXIMUM_ID, 8);

void updateMenuBound() {
  if (MAXIMUM_ID <= 8) { return; }
  if (current_menu_selection >= menu_lowerbound) {
    menu_lowerbound = current_menu_selection + 1;
    menu_upperbound = menu_lowerbound - 7;    
  } else if (current_menu_selection < menu_upperbound) {
    menu_upperbound = current_menu_selection;
    menu_lowerbound = current_menu_selection + 7;
  }
}

void updateMenuUI() {
    ttgo->tft->setTextFont(4);

    
    // i starts from 1 to skip `Menu`.
    for (uint8_t i = menu_upperbound; i < menu_lowerbound; i++) {

        ttgo->tft->setTextColor(
            (i == current_menu_selection? TFT_BLACK : TFT_WHITE),
            (i == current_menu_selection? TFT_WHITE : TFT_BLACK)
        );
        ttgo->tft->fillRect(0, 32 + (i-menu_upperbound)*26, 240, 26, TFT_BLACK);
        ttgo->tft->drawString(menu_list[i].c_str(), 0, 32 + (i-menu_upperbound) * 26);
    }


    ttgo->tft->drawBitmap(30-12, 240-24, EXIT_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(90-12, 240-24, LEFT_BUTTON_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(150-12, 240-24, RIGHT_BUTTON_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(210-12, 240-24, DIAMOND_24, 24, 24, TFT_WHITE);
    
}
void enterMenuScreen() {
    clearMainUI();
    updateTitleUI("Menu");
    updateMenuUI();

    int16_t x, y;
    uint8_t target = 0;
    while (true) {
        waitTouchDown(x, y);
        waitTouchUp(x, y);


        // Back button.
        if (X_IN_FIRST_QUARTER(x) && y > 240-32) {
            current_menu_selection = CLOCK;
            goto leave;
        }
        
        // up button.
        else if (X_IN_SECOND_QUARTER(x) && y > 240-32) {
            current_menu_selection -= 1;
            if (current_menu_selection == 0) { current_menu_selection = MAXIMUM_ID-1; }
            updateMenuBound();
            updateMenuUI();
        }
        // down button.
        else if (X_IN_THIRD_QUARTER(x) && y > 240-32) {
            current_menu_selection += 1;
            if (current_menu_selection >= MAXIMUM_ID) { current_menu_selection = 1; }
            updateMenuBound();
            updateMenuUI();
        }
        // select button.
        else if (X_IN_FOURTH_QUARTER(x) && y > 240-32) {
            goto leave;            
        }
    }
    leave:
    requested = current_menu_selection;
    clearMainUI();
}
#endif
