#include "led_functions.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

Preferences preferences;
CRGB leds[MAXIMUM_NUMBER_OF_LEDS];

// Convert HSV to a 32-bit integer
uint32_t packHSV(uint16_t hue, uint8_t saturation, uint8_t value) {
    return ((uint32_t)hue << 16) | ((uint32_t)saturation << 8) | value;
}

// Convert packed HSV to lv_color_hsv_t
lv_color_hsv_t unpackHSV(uint32_t packedHSVColor) {
    lv_color_hsv_t hsvColor;
    hsvColor.h = (packedHSVColor >> 16) & 0xFFFF;
    hsvColor.s = (packedHSVColor >> 8) & 0xFF;
    hsvColor.v = packedHSVColor & 0xFF;
    return hsvColor;
}

void ledPatternAurora(CRGB* leds, byte led_count){
    // Define colors representing aurora-like hues
  CRGB auroraColors[] = {
    CRGB(0, 255, 0),
    CRGB(154, 115, 255),
    CRGB(0, 136, 181),
    CRGB(0, 214, 164),
    CRGB(255, 69, 249),
    CRGB(94, 0, 235),
    CRGB(16, 163, 0),
    CRGB(53, 212, 0)
    // Add more colors as needed
  };
  int numAuroraColors = sizeof(auroraColors) / sizeof(auroraColors[0]);
  
  for(int i=0; i<led_count; i++){
    if (i == led_count - 1) {
      fadeTowardColor(leds[i],leds[i-1],5);
    }
    else if (i==0){
      fadeTowardColor(leds[i],leds[i+1],5);
    }
    else{
      fadeTowardColor(leds[i],(leds[i+1]+leds[i-1])/2,5);
    }
    if(random16()<500){
        //CRGB color = CHSV( random8(), 255, 255);
        CRGB color = auroraColors[random8(numAuroraColors)];
        fadeTowardColor(leds[i], color, 255);
    }
  }
}

void ledPatternTwinkle(CRGB* leds, byte led_count){
  for(int i=0; i<led_count; i++){
    fadeTowardColor(leds[i],CRGB(0,0,0),16);
    if(random16()<50){
        fadeTowardColor(leds[i], CRGB(255,255,255), 255);
    }
  }
}

void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
  }
}

CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
  return cur;
}

void ledPatternRainbow(CRGB* leds, byte led_count) {
  static uint8_t hue = 0;
  hue++;
  fill_rainbow(leds, led_count, hue, 7);
}

void ledPatternSolid(CRGB* leds, byte led_count) {
  fill_solid(leds, led_count, CRGB(value_r, value_g, value_b));
  FastLED.show();
}

uint8_t brightness = 0;
int8_t fadeDirection = 1;
void ledPatternSolidFade(CRGB* leds, byte led_count) {
  brightness += fadeDirection;
  if (brightness == 0 || brightness == 255) {
    fadeDirection *= -1;
  }
  fill_solid(leds, led_count, CRGB(value_r, value_g, value_b) % brightness);
  FastLED.show();
}

// the Celsius ranges to apply transient colors
#define MIN_TEMP 25.0
#define MAX_TEMP 45.0
void ledPatternTemperature(CRGB* leds, byte led_count, float temperature) {
  temperature = constrain(temperature, MIN_TEMP, MAX_TEMP);

  // map the temperature to the color range
  float ratio = (float)(temperature - MIN_TEMP) / (MAX_TEMP - MIN_TEMP);

  // calculate the RGB values for the color gradient from blue to red
  uint8_t red = ratio * 255;
  uint8_t blue = (1 - ratio) * 255;

  for (int i = 0; i < led_count; i++) {
    leds[i] = blend(leds[i], CRGB(red, 0, blue), 16);  // Adjust blend amount for smoothness
  }
}


unsigned long previousRGBBlink = 0;
bool isLedOn = false;
lv_color_t rgb_color;
void ledPatternSolidAlerting(CRGB* leds, byte led_count, unsigned long currentMillis) {
  if (currentMillis - previousRGBBlink >= 1000) {
    isLedOn = !isLedOn;
    previousRGBBlink = currentMillis;
    if (isLedOn) {
      rgb_color = lv_color_hsv_to_rgb(alert_color.h, alert_color.s, alert_color.v);
    }
  }

  if (isLedOn) {
    fill_solid(leds, led_count, CRGB(rgb_color.ch.red, rgb_color.ch.green, rgb_color.ch.blue));
    FastLED.show();
  } else {
    fill_solid(leds, led_count, CRGB::Black); // Turn off the LED
    FastLED.show();
  }
}