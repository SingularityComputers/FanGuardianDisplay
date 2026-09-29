// EEZ Studio actions that only change the UI (screen navigation, on-screen keyboards)
#include "ui/ui.h"
#include "ui/actions.h"

void action_go_to_led_settings_screen(lv_event_t * e)
{
  loadScreen(SCREEN_ID_LED_SETTINGS_SCREEN);
}

void action_go_to_alert_settings_screen(lv_event_t * e)
{
  loadScreen(SCREEN_ID_ALERT_SETTINGS_SCREEN);
}

void action_go_to_settings_screen(lv_event_t * e)
{
  loadScreen(SCREEN_ID_SETTINGS_SCREEN);
}

void action_go_to_about_screen(lv_event_t * e)
{
  loadScreen(SCREEN_ID_ABOUT_SCREEN);
}

static void show_main_screen(lv_timer_t * timer)
{
  loadScreen(SCREEN_ID_MAIN_SCREEN);
}

void action_show_main_screen_after_logo(lv_event_t * e)
{
  lv_timer_t * timer = lv_timer_create(show_main_screen, 1500, NULL);
  lv_timer_set_repeat_count(timer, 1);
}

static lv_obj_t * keyboard_of(lv_obj_t * textarea)
{
  if (lv_obj_get_screen(textarea) == objects.led_settings_screen) {
    return objects.led_settings_screen_kb;
  }
  return objects.alert_settings_screen_kb;
}

void action_show_keyboard(lv_event_t * e)
{
  lv_obj_t * textarea = lv_event_get_current_target(e);
  lv_obj_t * keyboard = keyboard_of(textarea);

  lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
  lv_keyboard_set_textarea(keyboard, textarea);
}

void action_hide_keyboard(lv_event_t * e)
{
  lv_obj_add_flag(keyboard_of(lv_event_get_current_target(e)), LV_OBJ_FLAG_HIDDEN);
}

void action_toggle_ambient_panel(lv_event_t * e)
{
  lv_obj_t * clicked = lv_event_get_current_target(e);
  lv_obj_t * other = clicked == objects.panel_ambient1 ? objects.panel_ambient2 : objects.panel_ambient1;

  lv_obj_clear_flag(other, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(clicked, LV_OBJ_FLAG_HIDDEN);
}
