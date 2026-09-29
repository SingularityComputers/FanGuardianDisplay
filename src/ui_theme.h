#ifndef _UI_THEME_H
#define _UI_THEME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// switches the color theme (THEME_ID_DEFAULT, THEME_ID_WARM, THEME_ID_COOL)
void ui_theme_set(uint8_t theme_index);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
