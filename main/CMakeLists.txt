idf_component_register(
  SRCS main.c lcd.c
  INCLUDE_DIRS .
  REQUIRES lvgl esp_lcd driver esp_lcd_touch_gt911
)
