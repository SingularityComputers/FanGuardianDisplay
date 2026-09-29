#ifndef _LED_FUNCTIONS_H
#define _LED_FUNCTIONS_H

#include <stdint.h>
#include "ui/ui.h"
#include <lvgl.h>
#include <Preferences.h>
#include <FastLED.h>
extern Preferences preferences;

#ifdef __cplusplus
extern "C" {
#endif

#define MAXIMUM_NUMBER_OF_LEDS 200
#define NUMBER_OF_FANS 4
extern CRGB leds[MAXIMUM_NUMBER_OF_LEDS];
extern uint8_t value_r;
extern uint8_t value_g;
extern uint8_t value_b;
extern lv_color_t rgb_stripe_color;
volatile extern uint16_t fanAlertRPMs[NUMBER_OF_FANS];
extern lv_color_hsv_t alert_color;
extern uint8_t rgb_pattern_index;
extern uint8_t rgb_pattern_temp_sensor_index;
extern bool rgb_led_alert_enabled;
extern uint16_t rgb_led_count;

#ifdef __cplusplus
} /*extern "C"*/
#endif

uint32_t packHSV(uint16_t hue, uint8_t saturation, uint8_t value);
lv_color_hsv_t unpackHSV(uint32_t packedHSVColor);
void ledPatternAurora(CRGB* leds, byte led_count);
void ledPatternTwinkle(CRGB* leds, byte led_count);
void ledPatternRainbow(CRGB* leds, byte led_count);
void ledPatternSolid(CRGB* leds, byte led_count);
void ledPatternSolidFade(CRGB* leds, byte led_count);
void ledPatternSolidAlerting(CRGB* leds, byte led_count, unsigned long currentMillis);
void ledPatternTemperature(CRGB* leds, byte led_count, float temperature);
CRGB fadeTowardColor(CRGB& cur, const CRGB& target, uint8_t amount);

#endif
