#include <led_functions.h>
#include <fanguardian_common.h>
#include <Wire.h>

#include <lvgl.h>
#include "ui/ui.h"
#include "ui_theme.h"
#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_INA3221.h>

// RGB led driver pin
#define LED_PIN 10

// thermistors
#define THERMISTOR1_PIN 0
#define THERMISTOR2_PIN 1
#define THERMISTOR3_PIN 2
#define THERMISTOR4_PIN 3
#define ADC_RESOLUTION 12
#define NOMINAL_RESISTANCE 10000.0         // Nominal resistance of the thermistor (ohms)
#define NOMINAL_SERIES_RESISTANCE 10000.0  // Nominal resistance of the series resistor (ohms)
#define NOMINAL_TEMPERATURE 25.0           // Nominal temperature of the thermistor (Celsius)
#define B_COEFFICIENT 3950.0               // B coefficient of the thermistor
#define THERMISTOR_VREF 5                  // Ref voltage for measurement

// I2C pins
#define CUSTOM_SDA_PIN 11
#define CUSTOM_SCL_PIN 21

// I2C address for INA3221
#define INA3221_ADDRESS 0x40

// maximum number of concurrent scheduled tasks
#define MAX_TASKS 10

// debounce delay for fans (in milliseconds)
#define DEBOUNCE_DELAY 1000

// number of temp sensors
#define NUMBER_OF_TEMPS 4

volatile unsigned int fanRPMs[NUMBER_OF_FANS] = {0,0,0,0};
volatile unsigned long fanLastAlertTime[NUMBER_OF_FANS] = {0,0,0,0};
volatile bool fanIsAlerting[NUMBER_OF_FANS] = {false, false, false, false};
volatile float voltages[3] = {0.0,0.0,0.0};
bool isADSFailed = false;

// temperature variable
volatile float temps[4] = {0.0,0.0,0.0,0.0};
volatile bool newUARTData = false;
String receivedData = "";
Adafruit_ADS1115 ads;
Adafruit_INA3221 ina3221;

// LVGL variables
LGFX tft;
static const uint32_t screenWidth = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
lv_obj_t *ui_rpm_arcs[NUMBER_OF_FANS];
lv_obj_t *ui_rpm_labels[NUMBER_OF_FANS];

// function declarations
void initDisplay();
void initLED();
void nonBlockingDelay(uint8_t delayIndex, unsigned long delayTime, void (*functionToRun)());
void sendUARTMessage(const String& message);
void core0Task(void *parameter);
void core1Task(void *parameter);
void uartTask(void *parameter);
void readVoltages();
void readRPMs();
void ledController();
void rpmWatcher();
void loadSettingsFromNVFlash();
void initGuiElements();

// LVGL heap diagnostics: prints at each step of setup() whether the LVGL heap is intact.
// Enabled by -DDEBUG_LVGL_HEAP in platformio.ini, which also turns on LVGL's own heap check
// after every allocation (see lv_conf.h). That makes the UI slower, comment it out if not needed.
// No Serial.flush() here: on the USB serial port it waits until a PC reads the data, so
// without an open serial monitor the boot would hang.
static void check_lvgl_heap(const char * where) {
#ifdef DEBUG_LVGL_HEAP
  bool ok = lv_mem_test() == LV_RES_OK;
  lv_mem_monitor_t mon;
  lv_mem_monitor(&mon);
  Serial.printf("[heap] %-32s %s  used=%u biggest_free=%u\n", where, ok ? "OK" : "CORRUPT",
                (unsigned)(mon.total_size - mon.free_size), (unsigned)mon.free_biggest_size);
#endif
}

/* Display flushing */
void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/* Read the touchpad */
void touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t x, y;
  if (tft.getTouch(&x, &y)) {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = x;
    data->point.y = y;

  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
  // load user settings from NVRAM
  loadSettingsFromNVFlash();

  // initialize display
  initDisplay();

  uint32_t DISP_BUF_SIZE = 480 * 320;
  lv_color_t* buf1 = (lv_color_t*)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
  lv_color_t* buf2 = (lv_color_t*)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISP_BUF_SIZE);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = displayFlush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpadRead;
  lv_indev_drv_register(&indev_drv);

  // call UI screen initializations
  ui_init();

  // init peripherials
#ifdef DEBUG_LVGL_HEAP
  // room for all boot diagnostics, so printing them does not wait for the PC to read them
  Serial.setTxBufferSize(2048);
#endif
  Serial.begin(9600);
  Serial.println("Board started.");
  Serial0.begin(9600);
  Serial0.flush();
  check_lvgl_heap("after ui_init");

  // Configure Wire library with custom SDA and SCL pins
  Wire.begin(CUSTOM_SDA_PIN, CUSTOM_SCL_PIN);
  check_lvgl_heap("after Wire.begin");

  // Initialize the ADS1115
  if (!ads.begin()) {
    tft.setBrightness(255);                     // turn on backlight
    tft.fillScreen(tft.color565(0, 0, 0));      // Clear the screen
    tft.setTextColor(tft.color565(255, 0, 0));  // Set text color to red
    tft.setTextSize(1);                         // Set text size
    tft.setCursor(0, 0);                        // Print text to the top-left corner
    tft.print("ERROR: ADS1115 initialization failed!");
    isADSFailed = true;
    delay(10000);
  }
  ads.setGain(GAIN_ONE);
  check_lvgl_heap("after ADS1115");

  // Initialize INA3221
  ina3221.begin(0x40, &Wire);
  ina3221.reset();
  ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES);
  for (uint8_t i = 0; i < 3; i++) {
    ina3221.setShuntResistance(i, 100);
  }
  check_lvgl_heap("after INA3221");

  // Initialize WS2812B
  initLED();
  check_lvgl_heap("after initLED");

  // initialize GUI elements (checkbox, dropdown, slider etc.) state
  initGuiElements();
  check_lvgl_heap("after initGuiElements");

  // turn on backlight after all screen initializations done
  // this will hide garbage from framebuffer
  tft.setBrightness(255);

  // Create a task to run on Core 0
  xTaskCreatePinnedToCore(core0Task, "ScreenTask", 4096, NULL, 1, NULL, 0);

  // Create a task to run on Core 1
  xTaskCreatePinnedToCore(core1Task, "MainTask", 10000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(uartTask, "UartTask", 2048, NULL, 1, NULL, 1);
}

// global variable to track elapsed milliseconds
volatile unsigned long currentMillis = 0;
void loop() {
  currentMillis = millis();
  lv_timer_handler();
  ui_tick();
  delay(5);
}

/* loadSettingsFromNVFlash loads user settings from display board NVRAM */
void loadSettingsFromNVFlash() {
  preferences.begin("settings", true);

  flip_screen = preferences.getBool("flip_screen", false);

  value_r = preferences.getUChar("value_r", 0);
  value_g = preferences.getUChar("value_g", 0);
  value_b = preferences.getUChar("value_b", 0);

	fanAlertRPMs[0] = preferences.getUShort("fan_1_alert", 0);
  fanAlertRPMs[1] = preferences.getUShort("fan_2_alert", 0);
  fanAlertRPMs[2] = preferences.getUShort("fan_3_alert", 0);
  fanAlertRPMs[3] = preferences.getUShort("fan_4_alert", 0);
  
  alert_color = unpackHSV(preferences.getUInt("alert_color", 25700));
  rgb_pattern_index = preferences.getUChar("pattern",3);
  rgb_pattern_temp_sensor_index = preferences.getUChar("sensor_index", 0);
  rgb_led_alert_enabled = preferences.getBool("led_alert", false);
  rgb_led_count = preferences.getUChar("rgb_led_count", 32);
  fan1_label_index = preferences.getUChar("fan1_lbl_index", 0);
  fan2_label_index = preferences.getUChar("fan2_lbl_index", 0);
  pump1_label_index = preferences.getUChar("pump1_lbl_index", 0);
  pump2_label_index = preferences.getUChar("pump2_lbl_index", 0);
  temp1_label_index = preferences.getUChar("temp1_lbl_index", 0);
  temp2_label_index = preferences.getUChar("temp2_lbl_index", 0);
  background_image_index = preferences.getUChar("bg_img_index", 0);
  theme_index = preferences.getUChar("theme_index", 0);
  preferences.end();
}

/* initDisplay initializes display */
void initDisplay() {
  tft.begin();
  set_screen_flip();
  tft.setBrightness(0);   // init with backlight off, this avoids showing framebuffer garbage 
}

/* initLED initializes the RGB leds */
void initLED() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, rgb_led_count);
  ledPatternSolid(leds, rgb_led_count);
}

void initGuiElements() {
  // create custom keyboard map
  static const char * kb_mapx[] = {"1", "2", "3", "\n", 
                                  "4", "5", "6", "\n",
                                  "7", "8", "9", "\n",
                                  LV_SYMBOL_BACKSPACE, "0", NULL, NULL
                                };
  static const lv_btnmatrix_ctrl_t kb_ctrlx[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
  lv_keyboard_set_map(objects.led_settings_screen_kb, LV_KEYBOARD_MODE_USER_1, kb_mapx, kb_ctrlx);
  lv_keyboard_set_map(objects.alert_settings_screen_kb, LV_KEYBOARD_MODE_USER_1, kb_mapx, kb_ctrlx);
  check_lvgl_heap("after keyboard maps");

  // set initial state for UI elements on LED setting screen
  led_setting_screen_dynamic_ui_events();

  check_lvgl_heap("after LED screen panels");
  // set pointers of arc_fan objects
  ui_rpm_arcs[0] = objects.arc_fan1;
  ui_rpm_arcs[1] = objects.arc_fan2;
  ui_rpm_arcs[2] = objects.arc_fan3;
  ui_rpm_arcs[3] = objects.arc_fan4;

  // set pointers of value_fan objects
  ui_rpm_labels[0] = objects.value_fan1;
  ui_rpm_labels[1] = objects.value_fan2;
  ui_rpm_labels[2] = objects.value_fan3;
  ui_rpm_labels[3] = objects.value_fan4;

  // set initial state for lv_* components on all screens
  char tmp_label_text[16];
  lv_slider_set_value(objects.red_slider, value_r, LV_ANIM_ON);
  lv_slider_set_value(objects.green_slider, value_g, LV_ANIM_ON);
  lv_slider_set_value(objects.blue_slider, value_b, LV_ANIM_ON);
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", value_r);
  lv_label_set_text(objects.red_slider_value, tmp_label_text);
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", value_g);
  lv_label_set_text(objects.green_slider_value, tmp_label_text);
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", value_b);
  lv_label_set_text(objects.blue_slider_value, tmp_label_text);
  check_lvgl_heap("after sliders");
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", static_cast<int>(fanAlertRPMs[0]));
  lv_textarea_set_text(objects.fan1_min, tmp_label_text);
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", static_cast<int>(fanAlertRPMs[1]));
  lv_textarea_set_text(objects.fan2_min, tmp_label_text);
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", static_cast<int>(fanAlertRPMs[2]));
  lv_textarea_set_text(objects.pump1_min, tmp_label_text);
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", static_cast<int>(fanAlertRPMs[3]));
  lv_textarea_set_text(objects.pump2_min, tmp_label_text);
  check_lvgl_heap("after alert textareas");
  lv_colorwheel_set_hsv(objects.colorwheel2, alert_color);
  lv_dropdown_set_selected(objects.led_effect_dropdown, rgb_pattern_index);
  lv_dropdown_set_selected(objects.temp_sensor_list_dropdown, rgb_pattern_temp_sensor_index);
  check_lvgl_heap("after colorwheel, LED dropdowns");
  if (rgb_led_alert_enabled) {
    lv_obj_add_state(objects.enable_led_alert, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(objects.enable_led_alert, LV_STATE_CHECKED);
  }
  snprintf(tmp_label_text, sizeof(tmp_label_text), "%d", static_cast<uint16_t>(rgb_led_count));
  lv_textarea_set_text(objects.num_of_leds, tmp_label_text);
  check_lvgl_heap("after LED switch, LED count");
  lv_dropdown_set_selected(objects.fan1_label_dropdown, fan1_label_index);
  lv_dropdown_set_selected(objects.fan2_label_dropdown, fan2_label_index);
  lv_dropdown_set_selected(objects.pump1_label_dropdown, pump1_label_index);
  lv_dropdown_set_selected(objects.pump2_label_dropdown, pump2_label_index);
  lv_dropdown_set_selected(objects.temp1_label_dropdown, temp1_label_index);
  lv_dropdown_set_selected(objects.temp2_label_dropdown, temp2_label_index);
  check_lvgl_heap("after label dropdowns");
  lv_dropdown_get_selected_str(objects.fan1_label_dropdown,tmp_label_text, sizeof(tmp_label_text));
  lv_label_set_text(objects.label_fan1, tmp_label_text);
  check_lvgl_heap("after FAN #1 label");
  lv_dropdown_get_selected_str(objects.fan2_label_dropdown, tmp_label_text, sizeof(tmp_label_text));
  lv_label_set_text(objects.label_fan2, tmp_label_text);
  check_lvgl_heap("after FAN #2 label");
  lv_dropdown_get_selected_str(objects.pump1_label_dropdown, tmp_label_text, sizeof(tmp_label_text));
  lv_label_set_text(objects.label_pump1, tmp_label_text);
  lv_dropdown_get_selected_str(objects.pump2_label_dropdown, tmp_label_text, sizeof(tmp_label_text));
  lv_label_set_text(objects.label_pump2, tmp_label_text);
  lv_dropdown_get_selected_str(objects.temp1_label_dropdown, tmp_label_text, sizeof(tmp_label_text));
  lv_label_set_text(objects.label_t1, tmp_label_text);
  lv_dropdown_get_selected_str(objects.temp2_label_dropdown, tmp_label_text, sizeof(tmp_label_text));
  lv_label_set_text(objects.label_t2, tmp_label_text);
  check_lvgl_heap("after all sensor labels");
  if(flip_screen) {
    lv_obj_add_state(objects.flip_screen_check_box, LV_STATE_CHECKED); 
  } else {
    lv_obj_clear_state(objects.flip_screen_check_box, LV_STATE_CHECKED);
  }
  lv_dropdown_set_selected(objects.bg_dropdown, background_image_index);
  set_background_image(background_image_index);
  check_lvgl_heap("after background image");
  lv_dropdown_set_selected(objects.theme_dropdown, theme_index);
  ui_theme_set(theme_index);
}

/* readTEMPs reads temperature values from thermistor */
void readTEMPs() {
  if (!isADSFailed) {
    int16_t rawValue;
    float voltage;
    float resistance;

    // Use Steinhart-Hart equation to calculate temperature
    for(int i = 0; i < NUMBER_OF_TEMPS; i++) {
      rawValue = ads.readADC_SingleEnded(i);
      voltage = ads.computeVolts(rawValue);

      // do not measure near boundaries
      if ((voltage > 0.8) && (voltage < 4.0)) {
        resistance = (voltage / (THERMISTOR_VREF - voltage)) * NOMINAL_RESISTANCE;
        temps[i] = resistance / NOMINAL_RESISTANCE;
        temps[i] = log(temps[i]);
        temps[i] /= B_COEFFICIENT;
        temps[i] += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
        temps[i] = 1.0 / temps[i];
        temps[i] -= 273.15;
      } else {
        temps[i] = -273.15;
      }
    }
  }
}

/* screenUpdater updates screen widget properties */
void screenUpdater() {
  if (temps[0] > 0.0) {
    lv_obj_clear_flag(objects.panel_t1, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(objects.value_t1, "%.1f", temps[0]);
    lv_arc_set_value(objects.arc_t1, temps[0]);
  } else {
    lv_obj_add_flag(objects.panel_t1, LV_OBJ_FLAG_HIDDEN);
  }

  if (temps[1] > 0.0) {
    lv_obj_clear_flag(objects.panel_t2, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(objects.value_t2, "%.1f", temps[1]);
    lv_arc_set_value(objects.arc_t2, temps[1]);
  } else {
    lv_obj_add_flag(objects.panel_t2, LV_OBJ_FLAG_HIDDEN);
  }

  if (temps[2] > 0.0) {
    lv_label_set_text_fmt(objects.temp3_value, "%.1f", temps[2]);
  } else {
    lv_label_set_text(objects.temp3_value, "--");
  }

  if (temps[3] > 0.0) {
    lv_label_set_text_fmt(objects.temp4_value, "%.1f", temps[3]);
  } else {
    lv_label_set_text(objects.temp4_value, "--");
  }

  lv_label_set_text_fmt(objects.value_fan1, "%d", fanRPMs[0]);
  lv_arc_set_value(objects.arc_fan1, fanRPMs[0]);
  lv_label_set_text_fmt(objects.value_fan2, "%d", fanRPMs[1]);
  lv_arc_set_value(objects.arc_fan2, fanRPMs[1]);
  lv_label_set_text_fmt(objects.value_fan3, "%d", fanRPMs[2]);
  lv_arc_set_value(objects.arc_fan3, fanRPMs[2]);
  lv_label_set_text_fmt(objects.value_fan4, "%d", fanRPMs[3]);
  lv_arc_set_value(objects.arc_fan4, fanRPMs[3]);

  lv_label_set_text_fmt(objects.volts12_value, "%.2f", voltages[0]);
  lv_label_set_text_fmt(objects.volts5_value, "%.2f", voltages[1]);
  lv_label_set_text_fmt(objects.volts3_v3_value, "%.2f", voltages[2]);
}

/* core0Task runs screen tasks on Core0 */
void core0Task(void *parameter) {
  while (1) {
    nonBlockingDelay(0,1000,screenUpdater);
    nonBlockingDelay(1,500,rpmWatcher);
    nonBlockingDelay(2,50,ledController);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

/* core1Task runs read metric tasks on Core1 */
void core1Task(void *parameter) {
  while (1) {
    nonBlockingDelay(3,500,readTEMPs);
    nonBlockingDelay(4,1000,readRPMs);
    nonBlockingDelay(5,500,readVoltages);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

/* uartTask is an UART receiver running on Core1 */
void uartTask(void *parameter) {
  while (1) {
    if (Serial0.available()) {
      char c = Serial0.read();
      if (c == '#') {
        receivedData = "";
      }
      receivedData += c;
      if (c == '\n') {
        newUARTData = true;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/* nonBlockingDelay schedules functions to run in different time */
struct DelayInfo {
  unsigned long previousMillis;
  unsigned long customDelayMillis;
  void (*delayedFunction)();
};
volatile DelayInfo delays[MAX_TASKS];
void nonBlockingDelay(uint8_t delayIndex, unsigned long delayTime, void (*functionToRun)()) {
  if (delayIndex >= 0 && delayIndex < MAX_TASKS) {
    //unsigned long currentMillis = millis();

    // Check if the desired interval has passed
    if (currentMillis - delays[delayIndex].previousMillis >= delayTime) {
      // Save the last time the action was performed
      delays[delayIndex].previousMillis = currentMillis;
      delays[delayIndex].customDelayMillis = 0; // Reset custom delay time

      // Call the provided function
      if (functionToRun != nullptr) {
        functionToRun();
      }
    }
  }
}

/* readRPMs reads fan RPMs from UART */
volatile unsigned long previousRPMDataMs;
void readRPMs() {

  // read FAN and Pump RPMs array (result should look like #RF,0,0,0,0)
  sendUARTMessage("#RF");

  // Process data if new data is available
  if (newUARTData) {
    newUARTData = false;
    
    Serial.println("Received: " + receivedData);

    // if got readfans response
    if (receivedData.startsWith("#RF")) {
      previousRPMDataMs = millis();
      Serial.println("RF command detected");

      // remove #RF,
      receivedData.remove(0, 4);

      // update fanRPMs
      uint8_t commaIndex = 0;
      for (uint8_t i = 0; i < 4; ++i) {
        commaIndex = receivedData.indexOf(',');
        if (commaIndex != -1) {
          fanRPMs[i] = receivedData.substring(0, commaIndex).toInt();
          receivedData.remove(0, commaIndex + 1);
        } else {
          fanRPMs[i] = receivedData.toInt();  // Last RPM value
        }
      }
    }
    receivedData = "";
  }
}

/* readVoltages reads voltage measurement from the 3 channels */
void readVoltages() {
  if (!isADSFailed) {
    voltages[0] = ina3221.getBusVoltage(0);
    voltages[1] = ina3221.getBusVoltage(1);
    voltages[2] = ina3221.getBusVoltage(2);
  }
}

/* sendUARTMessage sends message over UART0 */
void sendUARTMessage(const String& message) {
  Serial0.println(message);
  Serial.printf("DEBUG: %s sent\n", message);
  Serial0.flush();
}

/* rpmWatcher watches the fan RPM values and alerts */
unsigned long blinkMillis = 0;
unsigned long blinkIntervalMillis = 0;
void rpmWatcher() {
  // set 0 if there is no RPM data for 3 sec    
  if (currentMillis - previousRPMDataMs >= 3000) {
    for (uint8_t i = 0; i < NUMBER_OF_FANS; i++) {
      fanRPMs[i] = 0;
    }
  }

  // blinker for all consumer
  static bool shouldBlink = false;
  shouldBlink = !shouldBlink;

  // threshold checker
  for (int i = 0; i < NUMBER_OF_FANS; i++) {
    if (fanRPMs[i] < fanAlertRPMs[i]) {
      // Check if enough time has passed since the last alert
      if (millis() - fanLastAlertTime[i] >= DEBOUNCE_DELAY && !fanIsAlerting[i]) {
        // Update last alert time
        fanLastAlertTime[i] = millis();
        fanIsAlerting[i] = true;
      }
    } else {
      lv_obj_set_style_arc_color(ui_rpm_arcs[i], lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
      lv_obj_set_style_text_color(ui_rpm_labels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
      fanIsAlerting[i] = false;
    }

    if (fanIsAlerting[i]) {
      if (shouldBlink) {
        lv_obj_set_style_arc_color(ui_rpm_arcs[i], lv_color_hex(0xFF0000), LV_PART_INDICATOR);
        lv_obj_set_style_text_color(ui_rpm_labels[i], lv_color_hex(0xFF0000), LV_PART_MAIN);
      } else {
        lv_obj_set_style_arc_color(ui_rpm_arcs[i], lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
        lv_obj_set_style_text_color(ui_rpm_labels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);
      }
    }
  }
}

/* isAlerting checks if there is any alert happened */
bool isAlerting() {
    for (int i = 0; i < NUMBER_OF_FANS; i++) {
        if (fanIsAlerting[i]) {
            return true;
        }
    }
    return false;
}

/* ledController drives the RGB stripe */
void ledController() {  
  if (rgb_led_alert_enabled && isAlerting()) {
    ledPatternSolidAlerting(leds, rgb_led_count, currentMillis);
  } else {
    switch (rgb_pattern_index) {
      case 0: {
        ledPatternSolid(leds, rgb_led_count);
        break;
      }
      case 1: {
        ledPatternAurora(leds, rgb_led_count);
        vTaskDelay(pdMS_TO_TICKS(50));
        FastLED.show();
        break;
      }
      case 2: {
        ledPatternSolidFade(leds, rgb_led_count);
        vTaskDelay(pdMS_TO_TICKS(5));
        FastLED.show();
        break;
      }
      case 3: {
        ledPatternRainbow(leds, rgb_led_count);
        vTaskDelay(pdMS_TO_TICKS(20));
        FastLED.show();
        break;
      }
      case 4: {
        ledPatternTemperature(leds, rgb_led_count, temps[rgb_pattern_temp_sensor_index]);
        vTaskDelay(pdMS_TO_TICKS(5));
        FastLED.show();
        break;
      }
      case 5: {
        ledPatternTwinkle(leds, rgb_led_count);
        vTaskDelay(pdMS_TO_TICKS(5));
        FastLED.show();
        break;
      }
    }
  }
}
