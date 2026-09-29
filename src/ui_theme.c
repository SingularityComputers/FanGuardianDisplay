#include "ui_theme.h"
#include "ui/screens.h"
#include "ui/styles.h"

// EEZ Studio color themes only switch colors, but the "button_bg" and "border_bg" theme colors
// also have a different opacity in each theme. The opacity of the styles using these colors is
// set here, indexed by theme. Index 0 (Default theme) must match the values set in EEZ Studio.
static const lv_opa_t button_bg_opa[] = {0, 50, 70};
static const lv_opa_t border_bg_opa[] = {255, 140, 100};

void ui_theme_set(uint8_t theme_index) {
  change_color_theme(theme_index);

  lv_opa_t button_bg = button_bg_opa[theme_index];
  lv_opa_t border_bg = border_bg_opa[theme_index];
  lv_style_set_bg_opa(get_style_button_MAIN_DEFAULT(), button_bg);
  lv_style_set_border_opa(get_style_button_MAIN_DEFAULT(), border_bg);
  lv_style_set_bg_opa(get_style_checkbox_INDICATOR_DEFAULT(), button_bg);
  lv_style_set_bg_opa(get_style_title_label_MAIN_DEFAULT(), border_bg);
  lv_style_set_border_opa(get_style_name_label_MAIN_DEFAULT(), border_bg);
  lv_style_set_bg_opa(get_style_slider_INDICATOR_DEFAULT(), border_bg);
  lv_style_set_bg_opa(get_style_switch_KNOB_DEFAULT(), border_bg);
  lv_style_set_bg_opa(get_style_textarea_MAIN_DEFAULT(), border_bg);

  lv_obj_report_style_change(NULL);
}
