#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <bitmaps.h>

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;

public:
    LGFX(void) {
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.pin_sclk = 12;
            cfg.pin_mosi = 11;
            cfg.pin_miso = -1;
            cfg.pin_dc = 9;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }
        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs = 10;
            cfg.pin_rst = 8;
            cfg.pin_busy = -1;
            cfg.panel_width = 240;
            cfg.panel_height = 320;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = false;
            _panel_instance.config(cfg);
        }
        setPanel(&_panel_instance);
    }
};

LGFX tft;

#define BTN_1 4
#define BTN_2 5
#define PRIMARY_COLOR 0xEAE0

int cursor = 1;
int old_cursor = 1;

#define MENU_ITEMS_NUM 3
#define MENU_WIDTH 288
#define MENU_HEIGHT 32

void draw_loading_screen() {
    tft.fillScreen(0x0000);         
    tft.drawBitmap(68, 28, image_bmw_logo_own_bits, 183, 183, 0xEAE0);
}

void draw_button(int x, int y, const unsigned char* bitmap) {    
    tft.drawBitmap(x, y, bitmap, MENU_WIDTH, MENU_HEIGHT, PRIMARY_COLOR, 0x0000);
}

void draw_menu(bool force_all = false) {
    tft.startWrite();

    if (force_all) {
        if (cursor == 1) draw_button(15, 20, image_voltage_active_bits);
        else             draw_button(15, 20, image_voltage_n_active_bits);

        if (cursor == 2) draw_button(15, 64, image_rpm_active_bits);
        else             draw_button(15, 64, image_rpm_n_active_bits);

        if (cursor == 3) draw_button(15, 108, image_cool_active_bits);
        else             draw_button(15, 108, image_cool_n_active_bits);
    }
    else {
        if (old_cursor == 1) draw_button(15, 20, image_voltage_n_active_bits);
        if (old_cursor == 2) draw_button(15, 64, image_rpm_n_active_bits);
        if (old_cursor == 3) draw_button(15, 108, image_cool_n_active_bits);

        if (cursor == 1) draw_button(15, 20, image_voltage_active_bits);
        if (cursor == 2) draw_button(15, 64, image_rpm_active_bits);
        if (cursor == 3) draw_button(15, 108, image_cool_active_bits);
    } 

    tft.endWrite();
}

void setup() {
    Serial.begin(115200);
    delay(1000); 

    pinMode(BTN_1, INPUT_PULLUP);
    pinMode(BTN_2, INPUT_PULLUP);

    tft.init();
    tft.setRotation(3);

    draw_loading_screen();
    delay(3000);

    tft.fillScreen(0x0000);
    draw_menu(true);
}

void loop() {    
    if (digitalRead(BTN_2) == LOW) {
        delay(30);
        if (digitalRead(BTN_2) == LOW) {
            old_cursor = cursor;
            cursor--;
            if (cursor < 1) cursor = MENU_ITEMS_NUM;
            draw_menu(false);
        }
        while (digitalRead(BTN_2) == LOW) {delay(10);}
    }

    if (digitalRead(BTN_1) == LOW) {
        delay(30);
        if (digitalRead(BTN_1) == LOW) {
            old_cursor = cursor;
            cursor++;
            if (cursor > MENU_ITEMS_NUM) cursor = 1;
            draw_menu(false);
        }
        while (digitalRead(BTN_1) == LOW) {delay(10);}
    }
}