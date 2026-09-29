#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_LOGO_SCREEN = 1,
    SCREEN_ID_MAIN_SCREEN = 2,
    SCREEN_ID_LED_SETTINGS_SCREEN = 3,
    SCREEN_ID_ALERT_SETTINGS_SCREEN = 4,
    SCREEN_ID_SETTINGS_SCREEN = 5,
    SCREEN_ID_ABOUT_SCREEN = 6,
    _SCREEN_ID_LAST = 6
};

typedef struct _objects_t {
    lv_obj_t *logo_screen;
    lv_obj_t *main_screen;
    lv_obj_t *led_settings_screen;
    lv_obj_t *alert_settings_screen;
    lv_obj_t *settings_screen;
    lv_obj_t *about_screen;
    lv_obj_t *sc_logo;
    lv_obj_t *panel_fan1;
    lv_obj_t *arc_fan1;
    lv_obj_t *label_fan1;
    lv_obj_t *value_fan1;
    lv_obj_t *rpm_fan1;
    lv_obj_t *voltages_label;
    lv_obj_t *panel_fan2;
    lv_obj_t *arc_fan2;
    lv_obj_t *label_fan2;
    lv_obj_t *value_fan2;
    lv_obj_t *rpm_fan2;
    lv_obj_t *panel_fan3;
    lv_obj_t *arc_fan3;
    lv_obj_t *label_pump1;
    lv_obj_t *value_fan3;
    lv_obj_t *rpm_fan3;
    lv_obj_t *panel_fan4;
    lv_obj_t *arc_fan4;
    lv_obj_t *label_pump2;
    lv_obj_t *value_fan4;
    lv_obj_t *rpm_fan4;
    lv_obj_t *panel12_volts;
    lv_obj_t *volts12_value;
    lv_obj_t *volts12_label;
    lv_obj_t *panel5_volts;
    lv_obj_t *volts5_value;
    lv_obj_t *volts5_label;
    lv_obj_t *panel3_v3_volts;
    lv_obj_t *volts3_v3_value;
    lv_obj_t *volts3_v3_label;
    lv_obj_t *panel_ambient1;
    lv_obj_t *ambient1_label;
    lv_obj_t *temp3_value;
    lv_obj_t *temp3_label;
    lv_obj_t *panel_ambient2;
    lv_obj_t *ambient2_label;
    lv_obj_t *temp4_value;
    lv_obj_t *temp4_label;
    lv_obj_t *panel_t1;
    lv_obj_t *arc_t1;
    lv_obj_t *label_t1;
    lv_obj_t *value_t1;
    lv_obj_t *metric_t1;
    lv_obj_t *panel_t2;
    lv_obj_t *arc_t2;
    lv_obj_t *label_t2;
    lv_obj_t *value_t2;
    lv_obj_t *rpmt2;
    lv_obj_t *settings_button;
    lv_obj_t *settings_label;
    lv_obj_t *tap_to_switch_label;
    lv_obj_t *next_button;
    lv_obj_t *next_label;
    lv_obj_t *rgb_panel_title;
    lv_obj_t *led_effect_label;
    lv_obj_t *led_effect_dropdown;
    lv_obj_t *num_of_leds_label;
    lv_obj_t *num_of_leds;
    lv_obj_t *rgb_stripe_panel;
    lv_obj_t *red_slider;
    lv_obj_t *green_slider;
    lv_obj_t *blue_slider;
    lv_obj_t *red_slider_value;
    lv_obj_t *green_slider_value;
    lv_obj_t *blue_slider_value;
    lv_obj_t *temp_sensor_selector_panel;
    lv_obj_t *temp_sensor_list_label;
    lv_obj_t *temp_sensor_list_dropdown;
    lv_obj_t *back_button;
    lv_obj_t *back_label;
    lv_obj_t *save_button;
    lv_obj_t *save_label;
    lv_obj_t *led_settings_screen_kb;
    lv_obj_t *tresholds_panel;
    lv_obj_t *min_rpm_alert_label;
    lv_obj_t *fan1_label;
    lv_obj_t *fan1_min;
    lv_obj_t *fan2_label;
    lv_obj_t *fan2_min;
    lv_obj_t *pump1_label;
    lv_obj_t *pump1_min;
    lv_obj_t *pump2_label;
    lv_obj_t *pump2_min;
    lv_obj_t *fan1_label_dropdown;
    lv_obj_t *fan2_label_dropdown;
    lv_obj_t *pump1_label_dropdown;
    lv_obj_t *pump2_label_dropdown;
    lv_obj_t *temp2_label;
    lv_obj_t *temp1_label;
    lv_obj_t *temp1_label_dropdown;
    lv_obj_t *temp2_label_dropdown;
    lv_obj_t *rgb_panel2;
    lv_obj_t *rgb_panel2_title;
    lv_obj_t *colorwheel2;
    lv_obj_t *enable_led_alert;
    lv_obj_t *next_button1;
    lv_obj_t *next_label1;
    lv_obj_t *back_button2;
    lv_obj_t *back_label2;
    lv_obj_t *save_button2;
    lv_obj_t *save_label2;
    lv_obj_t *alert_settings_screen_kb;
    lv_obj_t *theme_label;
    lv_obj_t *bg_dropdown;
    lv_obj_t *flip_screen_check_box;
    lv_obj_t *back_button4;
    lv_obj_t *back_label5;
    lv_obj_t *rgb_panel2_title2;
    lv_obj_t *save_button3;
    lv_obj_t *save_label3;
    lv_obj_t *next_button3;
    lv_obj_t *next_label3;
    lv_obj_t *theme_dropdown;
    lv_obj_t *color_theme_label;
    lv_obj_t *fw_label;
    lv_obj_t *panel3;
    lv_obj_t *about_label;
    lv_obj_t *github_url_label;
    lv_obj_t *label3;
    lv_obj_t *back_button1;
    lv_obj_t *back_label1;
} objects_t;

extern objects_t objects;

void create_screen_logo_screen();
void tick_screen_logo_screen();

void create_screen_main_screen();
void tick_screen_main_screen();

void create_screen_led_settings_screen();
void tick_screen_led_settings_screen();

void create_screen_alert_settings_screen();
void tick_screen_alert_settings_screen();

void create_screen_settings_screen();
void tick_screen_settings_screen();

void create_screen_about_screen();
void tick_screen_about_screen();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

// Color themes

enum Themes {
    THEME_ID_DEFAULT,
    THEME_ID_WARM,
    THEME_ID_COOL,
};
enum Colors {
    COLOR_ID_TEXT,
    COLOR_ID_LABEL_TEXT,
    COLOR_ID_DROPDOWN_BG,
    COLOR_ID_BUTTON_BG,
    COLOR_ID_BORDER_BG,
};
void change_color_theme(uint32_t themeIndex);
extern uint32_t theme_colors[3][5];
extern uint32_t active_theme_index;

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/