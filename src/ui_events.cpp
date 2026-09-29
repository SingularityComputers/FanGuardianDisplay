#include "led_functions.h"
#include "fanguardian_common.h"
#include "ui.h"
#include "actions.h"
#include "ui_theme.h"
#include "lvgl.h"

char label_text[16];
lv_color_t rgb_stripe_color;
uint8_t value_r = 127;
uint8_t value_g = 127;
uint8_t value_b = 127;
volatile uint16_t fanAlertRPMs[NUMBER_OF_FANS] = {0,0,0,0};
lv_color_hsv_t alert_color;
uint8_t rgb_pattern_index = 0;
uint8_t rgb_pattern_temp_sensor_index = 0;
uint8_t fan1_label_index = 0;
uint8_t fan2_label_index = 0;
uint8_t pump1_label_index = 0;
uint8_t pump2_label_index = 0;
uint8_t temp1_label_index = 0;
uint8_t temp2_label_index = 0;
uint8_t background_image_index = 0;
uint8_t theme_index = 0;
bool rgb_led_alert_enabled = false;
bool flip_screen = false;
uint16_t rgb_led_count = 32;

void action_color_slider_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    value_r = lv_slider_get_value(objects.red_slider);
    value_g = lv_slider_get_value(objects.green_slider);
    value_b = lv_slider_get_value(objects.blue_slider);

    snprintf(label_text, sizeof(label_text), "%d", value_r);
    lv_label_set_text(objects.red_slider_value, label_text);

    snprintf(label_text, sizeof(label_text), "%d", value_g);
    lv_label_set_text(objects.green_slider_value, label_text);

    snprintf(label_text, sizeof(label_text), "%d", value_b);
    lv_label_set_text(objects.blue_slider_value, label_text);
  }
}

void action_alert_color_changed(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    alert_color = lv_colorwheel_get_hsv(objects.colorwheel2);
  }
}

void action_save_led_settings(lv_event_t * e)
{
  preferences.begin("settings", false);
	preferences.putUChar("value_r", value_r);
  preferences.putUChar("value_g", value_g);
  preferences.putUChar("value_b", value_b);
  preferences.putUChar("pattern", rgb_pattern_index);
  preferences.putUChar("sensor_index", rgb_pattern_temp_sensor_index);
  preferences.putUChar("rgb_led_count", rgb_led_count);
  preferences.end();
}

void action_save_led_settings_and_go_to_main_screen(lv_event_t * e)
{
  loadScreen(SCREEN_ID_MAIN_SCREEN);
  action_save_led_settings(e);
}

void action_save_alert_settings(lv_event_t * e)
{
  preferences.begin("settings", false);
  fanAlertRPMs[0] = static_cast<uint16_t>(atoi(lv_textarea_get_text(objects.fan1_min)));
  fanAlertRPMs[1] = static_cast<uint16_t>(atoi(lv_textarea_get_text(objects.fan2_min)));
  fanAlertRPMs[2] = static_cast<uint16_t>(atoi(lv_textarea_get_text(objects.pump1_min)));
  fanAlertRPMs[3] = static_cast<uint16_t>(atoi(lv_textarea_get_text(objects.pump2_min)));
  preferences.putUShort("fan_1_alert", fanAlertRPMs[0]);
  preferences.putUShort("fan_2_alert", fanAlertRPMs[1]);
  preferences.putUShort("fan_3_alert", fanAlertRPMs[2]);
  preferences.putUShort("fan_4_alert", fanAlertRPMs[3]);
  preferences.putUInt("alert_color", packHSV((uint16_t)alert_color.h, 100, 100));
  preferences.putBool("led_alert", rgb_led_alert_enabled);
  preferences.putUChar("fan1_lbl_index", fan1_label_index);
  preferences.putUChar("fan2_lbl_index", fan2_label_index);
  preferences.putUChar("pump1_lbl_index", pump1_label_index);
  preferences.putUChar("pump2_lbl_index", pump2_label_index);
  preferences.putUChar("temp1_lbl_index", temp1_label_index);
  preferences.putUChar("temp2_lbl_index", temp2_label_index);
  preferences.end();
}

void action_led_alert_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    rgb_led_alert_enabled = lv_obj_get_state(target) & LV_STATE_CHECKED ? true : false;
  }
}

void action_led_effect_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    rgb_pattern_index = lv_dropdown_get_selected(target);
    led_setting_screen_dynamic_ui_events();
  }
}

void action_num_of_leds_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    rgb_led_count = static_cast<uint16_t>(atoi(lv_textarea_get_text(target)));
    if (rgb_led_count > MAXIMUM_NUMBER_OF_LEDS) {
      rgb_led_count = MAXIMUM_NUMBER_OF_LEDS;
      snprintf(label_text, sizeof(label_text), "%d", MAXIMUM_NUMBER_OF_LEDS);
      lv_textarea_set_text(target, label_text);
    }
  }
}

void action_temp_sensor_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    rgb_pattern_temp_sensor_index = lv_dropdown_get_selected(target);
  }
}

void action_fan2_label_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    fan2_label_index = lv_dropdown_get_selected(target);
    lv_dropdown_get_selected_str(target, label_text, sizeof(label_text));
    lv_label_set_text(objects.label_fan2, label_text);
  }
}

void action_pump1_label_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    pump1_label_index = lv_dropdown_get_selected(target);
    lv_dropdown_get_selected_str(target, label_text, sizeof(label_text));
    lv_label_set_text(objects.label_pump1, label_text);
  }
}

void action_pump2_label_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    pump2_label_index = lv_dropdown_get_selected(target);
    lv_dropdown_get_selected_str(target, label_text, sizeof(label_text));
    lv_label_set_text(objects.label_pump2, label_text);
  }
}

void action_fan1_label_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    fan1_label_index = lv_dropdown_get_selected(target);
    lv_dropdown_get_selected_str(target, label_text, sizeof(label_text));
    lv_label_set_text(objects.label_fan1, label_text);
  }
}

void action_temp1_label_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    temp1_label_index = lv_dropdown_get_selected(target);
    lv_dropdown_get_selected_str(target, label_text, sizeof(label_text));
    lv_label_set_text(objects.label_t1, label_text);
  }
}

void action_temp2_label_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    temp2_label_index = lv_dropdown_get_selected(target);
    lv_dropdown_get_selected_str(target, label_text, sizeof(label_text));
    lv_label_set_text(objects.label_t2, label_text);
  }
}

void action_background_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    background_image_index = lv_dropdown_get_selected(target);
    set_background_image(background_image_index);
  }
}

void action_flip_screen_changed(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    flip_screen = lv_obj_has_state(target, LV_STATE_CHECKED);
    set_screen_flip();
    lv_obj_invalidate(lv_scr_act());
    lv_refr_now(NULL);
  }
}

void action_save_settings(lv_event_t * e)
{
  preferences.begin("settings", false);
  preferences.putBool("flip_screen", flip_screen);
  preferences.putUChar("bg_img_index", background_image_index);
  preferences.putUChar("theme_index", theme_index);
  preferences.end();
}

void action_theme_changed(lv_event_t * e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);

  if (event_code == LV_EVENT_VALUE_CHANGED) {
    theme_index = lv_dropdown_get_selected(target);
    ui_theme_set(theme_index);
  }
}
