#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_show_main_screen_after_logo(lv_event_t * e);
extern void action_toggle_ambient_panel(lv_event_t * e);
extern void action_go_to_led_settings_screen(lv_event_t * e);
extern void action_go_to_alert_settings_screen(lv_event_t * e);
extern void action_led_effect_changed(lv_event_t * e);
extern void action_show_keyboard(lv_event_t * e);
extern void action_hide_keyboard(lv_event_t * e);
extern void action_num_of_leds_changed(lv_event_t * e);
extern void action_color_slider_changed(lv_event_t * e);
extern void action_temp_sensor_changed(lv_event_t * e);
extern void action_save_led_settings_and_go_to_main_screen(lv_event_t * e);
extern void action_save_led_settings(lv_event_t * e);
extern void action_fan1_label_changed(lv_event_t * e);
extern void action_fan2_label_changed(lv_event_t * e);
extern void action_pump1_label_changed(lv_event_t * e);
extern void action_pump2_label_changed(lv_event_t * e);
extern void action_temp1_label_changed(lv_event_t * e);
extern void action_temp2_label_changed(lv_event_t * e);
extern void action_alert_color_changed(lv_event_t * e);
extern void action_led_alert_changed(lv_event_t * e);
extern void action_go_to_settings_screen(lv_event_t * e);
extern void action_save_alert_settings(lv_event_t * e);
extern void action_background_changed(lv_event_t * e);
extern void action_flip_screen_changed(lv_event_t * e);
extern void action_save_settings(lv_event_t * e);
extern void action_go_to_about_screen(lv_event_t * e);
extern void action_theme_changed(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/