#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: button
lv_style_t *get_style_button_MAIN_DEFAULT();
void add_style_button(lv_obj_t *obj);
void remove_style_button(lv_obj_t *obj);

// Style: checkbox
lv_style_t *get_style_checkbox_INDICATOR_DEFAULT();
lv_style_t *get_style_checkbox_MAIN_DEFAULT();
void add_style_checkbox(lv_obj_t *obj);
void remove_style_checkbox(lv_obj_t *obj);

// Style: title_label
lv_style_t *get_style_title_label_MAIN_DEFAULT();
void add_style_title_label(lv_obj_t *obj);
void remove_style_title_label(lv_obj_t *obj);

// Style: name_label
lv_style_t *get_style_name_label_MAIN_DEFAULT();
void add_style_name_label(lv_obj_t *obj);
void remove_style_name_label(lv_obj_t *obj);

// Style: text_label
lv_style_t *get_style_text_label_MAIN_DEFAULT();
void add_style_text_label(lv_obj_t *obj);
void remove_style_text_label(lv_obj_t *obj);

// Style: slider
lv_style_t *get_style_slider_INDICATOR_DEFAULT();
void add_style_slider(lv_obj_t *obj);
void remove_style_slider(lv_obj_t *obj);

// Style: switch
lv_style_t *get_style_switch_KNOB_DEFAULT();
lv_style_t *get_style_switch_MAIN_DEFAULT();
void add_style_switch(lv_obj_t *obj);
void remove_style_switch(lv_obj_t *obj);

// Style: textarea
lv_style_t *get_style_textarea_MAIN_DEFAULT();
void add_style_textarea(lv_obj_t *obj);
void remove_style_textarea(lv_obj_t *obj);

// Style: dropdown
lv_style_t *get_style_dropdown_MAIN_DEFAULT();
void add_style_dropdown(lv_obj_t *obj);
void remove_style_dropdown(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/