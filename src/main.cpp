#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
// #include <bitmaps.h> // Zakładamy, że obrazki przekonwertowałeś na .c w folderze src
#include <lvgl.h>

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

#define BTN_1 5
#define BTN_2 4

LV_IMG_DECLARE(voltage_active);
LV_IMG_DECLARE(voltage_n_active);

LV_IMG_DECLARE(rpm_active);
LV_IMG_DECLARE(rpm_n_active);

LV_IMG_DECLARE(cool_active);
LV_IMG_DECLARE(cool_n_active);

static const uint32_t screenWidth = 320;
static const uint32_t screenHeight = 240;

// --- OPTYMALIZACJA: Podwójne buforowanie ---
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf_1[screenWidth * screenHeight / 10];
static lv_color_t buf_2[screenWidth * screenHeight / 10];

void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    // OPTYMALIZACJA: Wypychamy paczkę przez DMA
    tft.pushPixelsDMA((lgfx::rgb565_t *)&color_p->full, w * h);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
    static uint32_t last_key = 0;

    if (digitalRead(BTN_1) == LOW) {
        data->state = LV_INDEV_STATE_PR;
        last_key = LV_KEY_PREV;
    }
    else if (digitalRead(BTN_2) == LOW) {
        data->state = LV_INDEV_STATE_PR;
        last_key = LV_KEY_NEXT;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

uint32_t custom_tick_get(void) {
    return millis();
}

void setup() {
    Serial.begin(115200);

    pinMode(BTN_1, INPUT_PULLUP);
    pinMode(BTN_2, INPUT_PULLUP);

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(0x0000);

    lv_init();
    
    // Inicjalizacja podwójnego bufora
    lv_disp_draw_buf_init(&draw_buf, buf_1, buf_2, screenWidth * screenHeight / 10);

    // Register screen
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Register btns
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = button_read;
    lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);

    // Group
    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);
    lv_indev_set_group(my_indev, g);

    static lv_style_t style_img_btn;
    lv_style_init(&style_img_btn);
    lv_style_set_bg_opa(&style_img_btn, LV_OPA_0); 
    lv_style_set_border_opa(&style_img_btn, LV_OPA_0); 
    lv_style_set_shadow_opa(&style_img_btn, LV_OPA_0);
    
    // Tło ekranu na czarno
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);

    // Voltage btn    
    lv_obj_t * btn_voltage = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn_voltage); 
    lv_obj_set_size(btn_voltage, 288, 32); 
    lv_obj_align(btn_voltage, LV_ALIGN_TOP_MID, 0, 20);
    
    lv_obj_set_style_bg_img_src(btn_voltage, &voltage_n_active, LV_STATE_DEFAULT); 
    lv_obj_set_style_bg_img_src(btn_voltage, &voltage_active, LV_STATE_FOCUSED); 
    lv_obj_set_style_bg_img_src(btn_voltage, &voltage_active, LV_STATE_PRESSED); 

    // RPM btn
    lv_obj_t * btn_rpm = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn_rpm);
    lv_obj_set_size(btn_rpm, 288, 32);
    lv_obj_align(btn_rpm, LV_ALIGN_TOP_MID, 0, 64);

    lv_obj_set_style_bg_img_src(btn_rpm, &rpm_n_active, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(btn_rpm, &rpm_active, LV_STATE_FOCUSED);
    lv_obj_set_style_bg_img_src(btn_rpm, &rpm_active, LV_STATE_PRESSED);

    // Coolant btn
    lv_obj_t * btn_cool = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn_cool);
    lv_obj_set_size(btn_cool, 288, 32);
    lv_obj_align(btn_cool, LV_ALIGN_TOP_MID, 0, 108);

    lv_obj_set_style_bg_img_src(btn_cool, &cool_n_active, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(btn_cool, &cool_active, LV_STATE_FOCUSED);
    lv_obj_set_style_bg_img_src(btn_cool, &cool_active, LV_STATE_PRESSED);
}

void loop() {
    lv_timer_handler();
    delay(5);
}