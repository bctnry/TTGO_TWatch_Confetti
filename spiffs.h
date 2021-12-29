#ifndef CONFETTI_SPIFFS
#define CONFETTI_SPIFFS

uint16_t current_file_list_selection = 0;
char current_working_directory[256] = "/";
void updateSPIFFSUI() {
    ttgo->tft->setTextFont(2);

    ttgo->tft->drawString(current_working_directory, 0, 32);
    
    for (uint8_t i = 0; i < 10; i++) {
        ttgo->tft->setTextColor(
            (i == current_file_list_selection? TFT_BLACK : TFT_WHITE),
            (i == current_file_list_selection? TFT_WHITE : TFT_BLACK)
        );
        ttgo->tft->fillRect(0, 48 + i*16, 240, 16, TFT_BLACK);
        ttgo->tft->drawBitmap(0, 48 + i*16, (i%2? FOLDER_16 : FILE_16), 16, 16, TFT_WHITE);
        ttgo->tft->drawString(i%2? "Folder" : "File", 16, 48 + i*16);
    }

  
    ttgo->tft->drawBitmap(30-12, 240-24, EXIT_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(90-12, 240-24, LEFT_BUTTON_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(150-12, 240-24, RIGHT_BUTTON_24, 24, 24, TFT_WHITE);
    ttgo->tft->drawBitmap(210-12, 240-24, DIAMOND_24, 24, 24, TFT_WHITE);
}

void enterSPIFFSScreen() {
    clearMainUI();
    updateTitleUI("SPIFFS File Manager");
    updateSPIFFSUI();
    
    int16_t x, y;
    uint8_t target = 0;
    while (true) {
        waitTouchDown(x, y);
        waitTouchUp(x, y);

        
        // Back button.
        if (X_IN_FIRST_QUARTER(x) && y > 240-32) {
            current_menu_selection = CLOCK;
            break;
        }
        
        // up button.
        else if (X_IN_SECOND_QUARTER(x) && y > 240-32) {
            /*
            current_menu_selection -= 1;
            if (current_menu_selection == 0) { current_menu_selection = MAXIMUM_ID-1; }
            updateMenuBound();
            updateMenuUI();
            */
        }
        // down button.
        else if (X_IN_THIRD_QUARTER(x) && y > 240-32) {
            /*
            current_menu_selection += 1;
            if (current_menu_selection >= MAXIMUM_ID) { current_menu_selection = 1; }
            updateMenuBound();
            updateMenuUI();
            */
        }
        // select button.
        else if (X_IN_FOURTH_QUARTER(x) && y > 240-32) {
            // TODO: finish this.
            break;
        }
    }
    clearMainUI();
}
#endif
