#ifndef MENU_H
#define MENU_H

#include <U8g2lib.h>

// Menu states
enum AppState {
  STATE_INTRO,
  STATE_MENU,
  STATE_PAGE_INFO,
  STATE_PAGE_BASIC_DATA,
  STATE_PAGE_OIL_TEMP,
  STATE_PAGE_RPM
};

// External variables
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern AppState current_state;
extern int item_selected;

// Function declarations
void drawMenu();
void handleMenuButtons();

#endif
