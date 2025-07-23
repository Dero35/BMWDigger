#ifndef PAGE_INFO_H
#define PAGE_INFO_H

#include "../menu.h"

extern bool button_select_pressed;
extern unsigned long last_button_time;
extern bool just_entered_page;
extern unsigned long page_enter_time;
extern const unsigned long DEBOUNCE_DELAY;

void drawPageInfo() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  
  // Title
  u8g2.drawStr(10, 15, "Info Page");
  
  // Content
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(10, 30, "System Information");
  u8g2.drawStr(10, 40, "and status display.");
  
  // Back instruction
  u8g2.drawStr(10, 55, "Press SELECT to go back");
  
  u8g2.sendBuffer();
}

void handlePageInfo() {
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
      Serial.println("Returning to menu from Info page");
    }
    else if (digitalRead(5) == HIGH) {
      button_select_pressed = false;
    }
  }
}

#endif
