#include "fanguardian_common.h"
#include "ui.h"
#include <lvgl.h>

void toggle_visibility(bool show, int count, lv_obj_t* objs[]) {
    for (int i = 0; i < count; i++) {
        if (show) {
            lv_obj_clear_flag(objs[i], LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(objs[i], LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void led_setting_screen_dynamic_ui_events() {
    switch (rgb_pattern_index) {
      case 0: {
        lv_obj_t* show[] = {objects.rgb_stripe_panel};
        lv_obj_t* hide[]= {objects.temp_sensor_selector_panel};
        toggle_visibility(true, 1, show);
        toggle_visibility(false, 1, hide);
        break;
      }
      case 1: {
        lv_obj_t* hide[]= {objects.rgb_stripe_panel, objects.temp_sensor_selector_panel};
        toggle_visibility(false, 2, hide);
        break;
      }
      case 2: {
        lv_obj_t* show[] = {objects.rgb_stripe_panel};
        lv_obj_t* hide[]= {objects.temp_sensor_selector_panel};
        toggle_visibility(true, 1, show);
        toggle_visibility(false, 1, hide);
        break;
      }
      case 3: {
        lv_obj_t* hide[]= {objects.rgb_stripe_panel, objects.temp_sensor_selector_panel};
        toggle_visibility(false, 2, hide);
        break;
      }
      case 4: {
        lv_obj_t* hide[] = {objects.rgb_stripe_panel};
        lv_obj_t* show[]= {objects.temp_sensor_selector_panel};
        toggle_visibility(true, 1, show);
        toggle_visibility(false, 1, hide);
        break;
      }
      case 5: {
        lv_obj_t* hide[]= {objects.rgb_stripe_panel, objects.temp_sensor_selector_panel};
        toggle_visibility(false, 2, hide);
        break;
      }
    }
}

void set_screen_flip() {
  if (flip_screen) {
    tft.setRotation(3);
  } else {
    tft.setRotation(1);
  }
  uint16_t calData[8] = {0,0,0,480,330,0,330,480};
  tft.setTouchCalibrate(calData);
}

void set_background_image(uint8_t bg_index) {
  const lv_img_dsc_t* img_data[] = {
    NULL,                     // Case 0
    &ui_img_aether_png,       // Case 1
    &ui_img_afterburner_png,  // Case 2
    &ui_img_divinity_png,     // Case 3
    &ui_img_final_hours_png,  // Case 4
    &ui_img_gaia_png,         // Case 5
    &ui_img_jade_png,         // Case 6
    &ui_img_napier_png,       // Case 7
    &ui_img_nibiru_png,       // Case 8
    &ui_img_plasmatic_png,    // Case 9
    &ui_img_singularity_png,  // Case 10
    &ui_img_war_png           // Case 11
  };

  lv_obj_t* screens[] = {
    objects.main_screen, objects.led_settings_screen, objects.alert_settings_screen,
    objects.settings_screen, objects.about_screen
  };

  for (int i = 0; i < sizeof(screens) / sizeof(screens[0]); i++) {
    lv_obj_set_style_bg_img_src(screens[i], img_data[bg_index], LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_img_opa(screens[i], LV_OPA_COVER, LV_PART_MAIN);
  }
}
