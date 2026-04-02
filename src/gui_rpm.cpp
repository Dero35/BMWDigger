#include "gui_screens.h"
#include <Arduino.h>

lv_obj_t * screen_rpm;
lv_obj_t * blocks[7];
lv_obj_t * label_rpm_val;
lv_obj_t * label_speed_val;

static void event_handler_rpm_back(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Serial.println("Powrót do menu");        
        lv_indev_set_group(indev_keypad, group_menu);
        lv_scr_load_anim(screen_menu, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    }
}

void gui_rpm_init(void) {
    screen_rpm = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen_rpm, lv_color_hex(0x000000), 0);
    
    group_rpm = lv_group_create();
    lv_group_add_obj(group_rpm, screen_rpm);
    
    lv_obj_add_event_cb(screen_rpm, event_handler_rpm_back, LV_EVENT_CLICKED, NULL);

    lv_obj_t * bar_cont = lv_obj_create(screen_rpm);
    lv_obj_remove_style_all(bar_cont);
    lv_obj_set_size(bar_cont, 320, 40);
    lv_obj_align(bar_cont, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(bar_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(bar_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_column(bar_cont, 2, 0);
    
    static lv_style_t style_off;
    lv_style_init(&style_off);
    lv_style_set_bg_color(&style_off, lv_color_hex(0x333333));
    lv_style_set_bg_opa(&style_off, LV_OPA_COVER);
    lv_style_set_radius(&style_off, 0); 
    lv_style_set_border_width(&style_off, 0);

    int heights[] = {12, 16, 20, 24, 28, 32, 36};
    for (int i=0; i<7; i++) {
        blocks[i] = lv_obj_create(bar_cont);
                
        lv_obj_remove_style_all(blocks[i]); 
        
        lv_obj_add_style(blocks[i], &style_off, 0);
        lv_obj_set_size(blocks[i], (i == 6 ? 26 : 47), heights[i]);
    }
    
    label_rpm_val = lv_label_create(screen_rpm);
    lv_label_set_text(label_rpm_val, "0000");
    lv_obj_set_style_text_color(label_rpm_val, lv_color_hex(0xFF6500), 0);
    lv_obj_set_style_text_font(label_rpm_val, &lv_font_montserrat_48, 0);     
    lv_obj_align(label_rpm_val, LV_ALIGN_TOP_LEFT, 20, 30); 
    
    lv_obj_t * label_rpm_unit = lv_label_create(screen_rpm);
    lv_label_set_text(label_rpm_unit, "RPM");
    lv_obj_set_style_text_color(label_rpm_unit, lv_color_hex(0xFF6500), 0);
    lv_obj_set_style_text_font(label_rpm_unit, &lv_font_montserrat_24, 0);    
    lv_obj_align_to(label_rpm_unit, label_rpm_val, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, -5);
    
    label_speed_val = lv_label_create(screen_rpm);
    lv_label_set_text(label_speed_val, "000");
    lv_obj_set_style_text_color(label_speed_val, lv_color_hex(0xFF6500), 0);
    lv_obj_set_style_text_font(label_speed_val, &lv_font_montserrat_48, 0);     
    lv_obj_align_to(label_speed_val, label_rpm_val, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    
    lv_obj_t * label_speed_unit = lv_label_create(screen_rpm);
    lv_label_set_text(label_speed_unit, "km / h");
    lv_obj_set_style_text_color(label_speed_unit, lv_color_hex(0xFF6500), 0);
    lv_obj_set_style_text_font(label_speed_unit, &lv_font_montserrat_24, 0);    
    lv_obj_align_to(label_speed_unit, label_speed_val, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, -5);
}

void gui_rpm_update(int rpm_val, int speed_val) {    
    lv_label_set_text_fmt(label_rpm_val, "%04d", rpm_val);
    lv_label_set_text_fmt(label_speed_val, "%03d", speed_val);

    for (int i = 0; i < 7; i++) {
        int threshold = i * 1000;
        
        if (rpm_val > threshold) {                                                
            lv_obj_set_style_bg_color(blocks[i], lv_color_hex(0xFF6500), 0);             
            lv_obj_set_style_bg_opa(blocks[i], LV_OPA_COVER, 0);
        } else {            
            lv_obj_set_style_bg_color(blocks[i], lv_color_hex(0x333333), 0);
            lv_obj_set_style_bg_opa(blocks[i], LV_OPA_COVER, 0);
        }
    }
}