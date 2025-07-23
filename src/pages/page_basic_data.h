#ifndef PAGE_BASIC_DATA_H
#define PAGE_BASIC_DATA_H

#include "../menu.h"

// Bitmap definitions
static const unsigned char image_OIL_bits[] = {0x04,0x0a,0x0a,0x11,0x11,0x0e};
static const unsigned char image_SNOWFLAKE_bits[] = {0x15,0x0e,0x1b,0x0e,0x15};

extern bool button_select_pressed;
extern unsigned long last_button_time;
extern bool just_entered_page;
extern unsigned long page_enter_time;
extern const unsigned long DEBOUNCE_DELAY;

void drawPageBasicData() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  
  // RPM NUM
  u8g2.setFont(u8g2_font_profont15_tr);
  u8g2.drawStr(10, 24, "0000");

  // COOLANT NUM
  u8g2.drawStr(10, 40, "000");

  // Layer 4 copy
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(32, 40, "C");

  // RPM TEXT
  u8g2.drawStr(39, 24, "RPM");

  // OIL NUM
  u8g2.setFont(u8g2_font_profont15_tr);
  u8g2.drawStr(10, 55, "000");

  // SNOWFLAKE
  u8g2.drawXBM(38, 35, 5, 5, image_SNOWFLAKE_bits);

  // Layer 4 copy
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(32, 55, "C");

  // SPEED
  u8g2.setFont(u8g2_font_profont15_tr);
  u8g2.drawStr(67, 24, "000");

  // OIL
  u8g2.drawXBM(38, 49, 5, 6, image_OIL_bits);

  // KMPH
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(89, 24, "KM/H");

  // PLACE HOLDER2
  u8g2.setFont(u8g2_font_profont15_tr);
  u8g2.drawStr(67, 40, "000");

  // PLACE HOLDER1
  u8g2.drawStr(67, 55, "000");

  // Layer 13
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(10, 9, "Basic Data");

  u8g2.sendBuffer();
}

void handlePageBasicData() {
  unsigned long current_time = millis();
  
  // Clear the just_entered_page flag after a delay
  if (just_entered_page && (current_time - page_enter_time > DEBOUNCE_DELAY)) {
    just_entered_page = false;
  }
  
  // Only handle button if we haven't just entered the page and enough time has passed
  if (!just_entered_page && (current_time - last_button_time > DEBOUNCE_DELAY)) {
    if (digitalRead(5) == LOW && !button_select_pressed) { // BUTTON_SELECT
      button_select_pressed = true;
      last_button_time = current_time;
      current_state = STATE_MENU;
      Serial.println("Returning to menu from Basic Data page");
    }
    else if (digitalRead(5) == HIGH) {
      button_select_pressed = false;
    }
  }
}

#endif
