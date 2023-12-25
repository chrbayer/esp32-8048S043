#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "driver/gpio.h"

#include "lvgl.h"

#include "sdkconfig.h"

#include "lcd.h"

#define TAG _PROJECT_NAME_

//
// example lvgl graphics from https://github.com/espressif/esp-bsp/tree/master/examples/display
//
void example_lvgl_demo_ui(lv_obj_t *scr);

static void touch_event(lv_event_t *e)
{
uint8_t n = rand()%0xff;

  if (e->code == LV_EVENT_CLICKED)
  {
    lv_color_t color = LV_COLOR_MAKE16(n,n*2,n*3);
    lv_obj_t *label = (lv_obj_t*)e->user_data;
    lv_obj_set_style_text_color(label, color, 0);
  }
}

//
//
//
void app_main(void)
{
uint32_t n = 0;
char buf[12];

  //
  // initialize display, i2c and touch handler:
  //
  ESP_ERROR_CHECK(LCDInit());

  //
  // print a simple counter next to the Espressif demo
  //
  lv_obj_t *scr = lv_disp_get_scr_act(NULL);
  lv_color_t color = LV_COLOR_MAKE16(0xff, 0x00, 0x00);
  lv_obj_t *label = lv_label_create(scr);
  lv_obj_t *label2 = lv_label_create(scr);
  lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_32);
  lv_obj_add_style(label, &style, 0);
  lv_obj_add_style(label2, &style, 0);
  lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_align(label2, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_text_color(label, color, 0);
  lv_label_set_text_static(label, "");
  lv_label_set_text_static(label2, "Hello world");

  //
  // change counter color when the display was clicked
  //
  lv_obj_add_event_cb(scr, touch_event, LV_EVENT_CLICKED, label);

  //
  // turn on lcd backlight, to prevent displaying noise
  //
  gpio_set_level(LCD_PIN_BK_LIGHT, LCD_BK_LIGHT_ON_LEVEL);


  while(true)
  {
    vTaskDelay(pdMS_TO_TICKS(50));
    n++;

    sprintf(buf, "%" PRIu32, n);
    lv_label_set_text_static(label, buf);
  }
}
