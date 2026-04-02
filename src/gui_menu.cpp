#include "gui_screens.h"
#include <Arduino.h>

LV_IMG_DECLARE(voltage_active);
LV_IMG_DECLARE(voltage_n_active);

LV_IMG_DECLARE(rpm_active);
LV_IMG_DECLARE(rpm_n_active);

LV_IMG_DECLARE(cool_active);
LV_IMG_DECLARE(cool_n_active);

lv_obj_t * screen_menu;

static void event_handler_btn_voltage(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        Serial.println("Kliknieto Voltage");
    }
}

static void event_handler_btn_rpm(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        Serial.println("Kliknieto RPM");
        lv_indev_set_group(indev_keypad, group_rpm);
        lv_scr_load_anim(screen_rpm, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    }
}

static void event_handler_btn_coolant(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        Serial.println("Kliknieto Coolant");
    }
}

void gui_menu_init(void) {
    screen_menu = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen_menu, lv_color_hex(0x000000), 0);

    static lv_style_t style_img_btn;
    lv_style_init(&style_img_btn);
    lv_style_set_bg_opa(&style_img_btn, LV_OPA_0); 
    lv_style_set_border_opa(&style_img_btn, LV_OPA_0); 
    lv_style_set_shadow_opa(&style_img_btn, LV_OPA_0);

    group_menu = lv_group_create();

    // Voltage btn    
    lv_obj_t * btn_voltage = lv_btn_create(screen_menu);
    lv_obj_remove_style_all(btn_voltage); 
    lv_obj_set_size(btn_voltage, 288, 32); 
    lv_obj_align(btn_voltage, LV_ALIGN_TOP_MID, 0, 20);
    
    lv_obj_set_style_bg_img_src(btn_voltage, &voltage_n_active, LV_STATE_DEFAULT); 
    lv_obj_set_style_bg_img_src(btn_voltage, &voltage_active, LV_STATE_FOCUSED); 
    lv_obj_set_style_bg_img_src(btn_voltage, &voltage_active, LV_STATE_PRESSED);

    lv_obj_add_event_cb(btn_voltage, event_handler_btn_voltage, LV_EVENT_CLICKED, NULL); 
    lv_group_add_obj(group_menu, btn_voltage);

    // RPM btn
    lv_obj_t * btn_rpm = lv_btn_create(screen_menu);
    lv_obj_remove_style_all(btn_rpm);
    lv_obj_set_size(btn_rpm, 288, 32);
    lv_obj_align(btn_rpm, LV_ALIGN_TOP_MID, 0, 64);

    lv_obj_set_style_bg_img_src(btn_rpm, &rpm_n_active, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(btn_rpm, &rpm_active, LV_STATE_FOCUSED);
    lv_obj_set_style_bg_img_src(btn_rpm, &rpm_active, LV_STATE_PRESSED);

    lv_obj_add_event_cb(btn_rpm, event_handler_btn_rpm, LV_EVENT_CLICKED, NULL);
    lv_group_add_obj(group_menu, btn_rpm);

    // Coolant btn
    lv_obj_t * btn_cool = lv_btn_create(screen_menu);
    lv_obj_remove_style_all(btn_cool);
    lv_obj_set_size(btn_cool, 288, 32);
    lv_obj_align(btn_cool, LV_ALIGN_TOP_MID, 0, 108);

    lv_obj_set_style_bg_img_src(btn_cool, &cool_n_active, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(btn_cool, &cool_active, LV_STATE_FOCUSED);
    lv_obj_set_style_bg_img_src(btn_cool, &cool_active, LV_STATE_PRESSED);

    lv_obj_add_event_cb(btn_cool, event_handler_btn_coolant, LV_EVENT_CLICKED, NULL);
    lv_group_add_obj(group_menu, btn_cool);
}