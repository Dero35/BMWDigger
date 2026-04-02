#ifndef GUI_SCREENS_H
#define GUI_SCREENS_H

#include <lvgl.h>

extern lv_obj_t * screen_menu;
extern lv_obj_t * screen_rpm;

extern lv_indev_t * indev_keypad;
extern lv_group_t * group_menu;
extern lv_group_t * group_rpm;

void gui_menu_init(void);
void gui_rpm_init(void);
void gui_rpm_update(int rpm_val, int speed_val);

#endif