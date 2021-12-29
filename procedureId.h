#ifndef CONFETTI_PROCEDURE_ID
#define CONFETTI_PROCEDURE_ID
const uint8_t MENU = 0;
const uint8_t CLOCK = 1;
const uint8_t SET_TIME = 2;
const uint8_t SET_DATE = 3;
const uint8_t SPIFFS_FM = 4;
const uint8_t MAXIMUM_ID = 5;

String menu_list[] = {
    "",
    "Clock",
    "Set Time",
    "Set Date",
    "File Manager",
};
#endif
