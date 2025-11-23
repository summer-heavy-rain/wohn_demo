#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "lv_port.h"
#include "driver/gpio.h"
#include "st7789_driver.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "sdcard.h"
#include "lv_port_fs.h"

#define LVGL_TASK_STACK_SIZE   4096
#define LVGL_TASK_PRIORITY     5
#define LVGL_TASK_CORE         1

lv_ui guider_ui;

static void lvgl_task(void *arg)
{
    const TickType_t delay = pdMS_TO_TICKS(5);
    while (1) {
        lv_timer_handler();
        vTaskDelay(delay);
    }
}

void app_main(void)
{
    ESP_LOGI("main", "启动应用程序...");
    
    // 初始化LVGL和硬件
    ESP_LOGI("main", "初始化LVGL...");
    lv_port_init();
    
    ESP_LOGI("main", "初始化SD卡...");
    sd_card_init(); 
    
    ESP_LOGI("main", "初始化文件系统...");
    lv_port_fs_init();
    
    ESP_LOGI("main", "打开LCD背光...");
    st7789_lcd_backlight(1);
    
    // 创建一个临时屏幕以验证显示是否正常
    ESP_LOGI("main", "创建测试屏幕验证显示...");
    lv_obj_t *test_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(test_screen, lv_color_hex(0xFF0000), LV_PART_MAIN); // 设置为红色背景
    lv_scr_load(test_screen);
    
    // 等待1秒让用户看到红色背景
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    ESP_LOGI("main", "加载主界面...");
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);

    xTaskCreatePinnedToCore(
        lvgl_task,
        "lvgl",
        LVGL_TASK_STACK_SIZE,
        NULL,
        LVGL_TASK_PRIORITY,
        NULL,
        LVGL_TASK_CORE);
}
