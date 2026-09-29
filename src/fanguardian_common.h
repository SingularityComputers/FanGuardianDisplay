#ifndef _FANGUARDIAN_COMMON_H
#define _FANGUARDIAN_COMMON_H

#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// LovyanGFX class definition
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_FT5x06 _touch_instance;

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.freq_write = 30000000;
      cfg.pin_wr = 47;
      cfg.pin_rd = -1;
      cfg.pin_rs = 0;
      cfg.pin_d0 = 9;
      cfg.pin_d1 = 46;
      cfg.pin_d2 = 3;
      cfg.pin_d3 = 8;
      cfg.pin_d4 = 18;
      cfg.pin_d5 = 17;
      cfg.pin_d6 = 16;
      cfg.pin_d7 = 15;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = -1;
      cfg.pin_rst = 4;
      cfg.pin_busy = -1;
      cfg.memory_width = 320;
      cfg.memory_height = 480;
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = 45;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    {
      auto cfg = _touch_instance.config();
      cfg.i2c_port = 1;
      cfg.i2c_addr = 0x38;
      cfg.pin_sda = 6;
      cfg.pin_scl = 5;
      cfg.freq = 500000;
      cfg.x_min = 0;
      cfg.x_max = 320;
      cfg.y_min = 0;
      cfg.y_max = 480;

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};

extern uint8_t rgb_pattern_index;
extern uint8_t fan1_label_index;
extern uint8_t fan2_label_index;
extern uint8_t pump1_label_index;
extern uint8_t pump2_label_index;
extern uint8_t temp1_label_index;
extern uint8_t temp2_label_index;
extern uint8_t background_image_index;
extern uint8_t theme_index;
extern bool flip_screen;
extern LGFX tft;

LV_IMG_DECLARE( ui_img_aether_png);         // assets/Aether.png
LV_IMG_DECLARE( ui_img_afterburner_png);    // assets/Afterburner.png
LV_IMG_DECLARE( ui_img_divinity_png);       // assets/Divinity.png
LV_IMG_DECLARE( ui_img_final_hours_png);    // assets/Final Hours.png
LV_IMG_DECLARE( ui_img_gaia_png);           // assets/Gaia.png
LV_IMG_DECLARE( ui_img_jade_png);           // assets/Jade.png
LV_IMG_DECLARE( ui_img_napier_png);         // assets/Napier.png
LV_IMG_DECLARE( ui_img_nibiru_png);         // assets/Nibiru.png
LV_IMG_DECLARE( ui_img_plasmatic_png);      // assets/Plasmatic.png
LV_IMG_DECLARE( ui_img_singularity_png);    // assets/Singularity.png
LV_IMG_DECLARE( ui_img_war_png);            // assets/War.png

void toggle_visibility(bool show, int count, lv_obj_t* objs[]);
void led_setting_screen_dynamic_ui_events();
void set_screen_flip();
void set_background_image(uint8_t bg_index);

#endif
