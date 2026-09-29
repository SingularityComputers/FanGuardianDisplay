#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: button
//

void init_style_button_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][3]));
    lv_style_set_bg_opa(style, 0);
    lv_style_set_border_color(style, lv_color_hex(theme_colors[active_theme_index][4]));
    lv_style_set_border_opa(style, 255);
};

lv_style_t *get_style_button_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: checkbox
//

void init_style_checkbox_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][3]));
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_checkbox_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_checkbox_INDICATOR_DEFAULT(style);
    }
    return style;
};

void init_style_checkbox_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(theme_colors[active_theme_index][0]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_checkbox_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_checkbox_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_checkbox(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_checkbox_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_checkbox_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_checkbox(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_checkbox_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_checkbox_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: title_label
//

void init_style_title_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][4]));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_text_color(style, lv_color_hex(theme_colors[active_theme_index][1]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_title_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_title_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_title_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_title_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_title_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_title_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: name_label
//

void init_style_name_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(theme_colors[active_theme_index][4]));
    lv_style_set_border_opa(style, 255);
    lv_style_set_text_color(style, lv_color_hex(theme_colors[active_theme_index][0]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_name_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_name_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_name_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_name_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_name_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_name_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: text_label
//

void init_style_text_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(theme_colors[active_theme_index][0]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_text_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_text_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_text_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_text_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_text_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_text_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: slider
//

void init_style_slider_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][4]));
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_slider_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_slider_INDICATOR_DEFAULT(style);
    }
    return style;
};

void add_style_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_slider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

void remove_style_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_slider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

//
// Style: switch
//

void init_style_switch_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][4]));
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_switch_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_switch_KNOB_DEFAULT(style);
    }
    return style;
};

void init_style_switch_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][2]));
    lv_style_set_bg_opa(style, 150);
};

lv_style_t *get_style_switch_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_switch_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_switch(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_switch_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_switch_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_switch(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_switch_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_switch_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: textarea
//

void init_style_textarea_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][4]));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_text_color(style, lv_color_hex(theme_colors[active_theme_index][1]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_textarea_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_textarea_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_textarea(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_textarea_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_textarea(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_textarea_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: dropdown
//

void init_style_dropdown_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[active_theme_index][2]));
    lv_style_set_bg_opa(style, 150);
    lv_style_set_text_color(style, lv_color_hex(theme_colors[active_theme_index][1]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_dropdown_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_dropdown_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_dropdown(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_dropdown_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_dropdown(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_dropdown_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_button,
        add_style_checkbox,
        add_style_title_label,
        add_style_name_label,
        add_style_text_label,
        add_style_slider,
        add_style_switch,
        add_style_textarea,
        add_style_dropdown,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_button,
        remove_style_checkbox,
        remove_style_title_label,
        remove_style_name_label,
        remove_style_text_label,
        remove_style_slider,
        remove_style_switch,
        remove_style_textarea,
        remove_style_dropdown,
    };
    remove_style_funcs[styleIndex](obj);
}