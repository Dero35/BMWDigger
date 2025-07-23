#ifndef INTRO_SCREEN_H
#define INTRO_SCREEN_H

#include "menu.h"

// BMW Logo bitmap data
static const unsigned char image_BMW_LOGO_bits[] = {
  0xc0,0x07,0x00,0x30,0x18,0x00,0x08,0x20,0x00,0xc4,0x47,0x00,0xe2,0x89,0x00,
  0xf2,0x91,0x00,0xf9,0x21,0x01,0xf9,0x21,0x01,0xf9,0x3f,0x01,0x09,0x3f,0x01,
  0x09,0x3f,0x01,0x12,0x9f,0x00,0x22,0x8f,0x00,0xc4,0x47,0x00,0x08,0x20,0x00,
  0x30,0x18,0x00,0xc0,0x07,0x00
};

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern AppState current_state;

// Intro screen variables
unsigned long intro_start_time = 0;
int progress = 0;
const unsigned long INTRO_DURATION = 4000; // 4 seconds

void drawIntroStatic() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  
  // TITLE
  u8g2.setFont(u8g2_font_t0_13_tr);
  u8g2.drawStr(6, 17, "BMW DIGGER");

  // AUTHOR
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(7, 27, "By: Dero33");

  // PROGRESS OUTLINE
  u8g2.drawFrame(12, 36, 104, 18);

  // BMW LOGO
  u8g2.drawXBM(80, 4, 17, 17, image_BMW_LOGO_bits);
  
  u8g2.sendBuffer();
}

void drawIntroDynamic() {
  // Clear the progress bar area
  u8g2.setDrawColor(0); // Black
  u8g2.drawBox(14, 38, 100, 14);
  
  // Draw progress bar based on current progress
  u8g2.setDrawColor(1); // White
  u8g2.drawBox(14, 38, progress, 14);
  
  // Clear and draw progress percentage
  u8g2.setDrawColor(0); // Black
  u8g2.drawBox(50, 55, 30, 10); // Clear percentage area
  
  u8g2.setDrawColor(1); // White
  u8g2.setFont(u8g2_font_4x6_tr);
  
  // Calculate percentage
  int percentage = (progress * 100) / 100;
  if (percentage > 100) percentage = 100;
  
  // Format percentage string
  char percent_str[6];
  sprintf(percent_str, "%d%%", percentage);
  u8g2.drawStr(54, 61, percent_str);
  
  u8g2.sendBuffer();
}

void handleIntroScreen() {
  unsigned long current_time = millis();
  
  // Initialize intro screen on first call
  if (intro_start_time == 0) {
    intro_start_time = current_time;
    drawIntroStatic();
    progress = 0;
    Serial.println("BMW DIGGER intro screen started");
  }
  
  // Update progress bar
  unsigned long elapsed = current_time - intro_start_time;
  progress = (elapsed * 100) / INTRO_DURATION; // Scale to progress bar width (100px)
  
  if (progress > 100) progress = 100;
  
  drawIntroDynamic();
  
  // Transition to menu after duration
  if (elapsed >= INTRO_DURATION) {
    current_state = STATE_MENU;
    intro_start_time = 0; // Reset for next time
    Serial.println("BMW DIGGER intro completed, transitioning to menu");
  }
}

#endif
