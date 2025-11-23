#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "st7789_driver.h"

#define LCD_SPI_HOST    SPI2_HOST

static const char* TAG = "st7789";

//lcd操作句柄
static esp_lcd_panel_io_handle_t lcd_io_handle = NULL;

//刷新完成回调函数
static lcd_flush_done_cb    s_flush_done_cb = NULL;

//背光GPIO
static gpio_num_t   s_bl_gpio = -1;

static bool notify_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    if(s_flush_done_cb)
        s_flush_done_cb(user_ctx);
    return false;
}


/** st7789初始化
 * @param st7789_cfg_t  接口参数
 * @return 成功或失败
*/
esp_err_t st7789_driver_hw_init(st7789_cfg_t* cfg)
{
    ESP_LOGI(TAG, "开始初始化ST7789 LCD驱动...");
    ESP_LOGI(TAG, "配置参数: 宽=%d, 高=%d, 旋转=%d, SPI频率=%lu Hz", 
             (int)cfg->width, (int)cfg->height, (int)cfg->spin, (unsigned long)cfg->spi_fre);
    
    //初始化SPI
    ESP_LOGI(TAG, "初始化SPI总线...");
    spi_bus_config_t buscfg = {
        .sclk_io_num = cfg->clk,        //SCLK引脚
        .mosi_io_num = cfg->mosi,       //MOSI引脚
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .flags = SPICOMMON_BUSFLAG_MASTER , //SPI主模式
        .max_transfer_sz = cfg->width * 40 * sizeof(uint16_t),  //DMA单次传输最大字节，最大32768
    };
    esp_err_t ret = spi_bus_initialize(LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI总线初始化失败: %s", esp_err_to_name(ret));
        return ret;
    }
    ESP_LOGI(TAG, "SPI总线初始化成功");

    s_flush_done_cb = cfg->done_cb; //设置刷新完成回调函数
    s_bl_gpio = cfg->bl;    //设置背光GPIO
    
    //初始化GPIO(BL)
    ESP_LOGI(TAG, "初始化背光GPIO: %d", cfg->bl);
    gpio_config_t bl_gpio_cfg = 
    {
        .pull_up_en = GPIO_PULLUP_DISABLE,          //禁止上拉
        .pull_down_en = GPIO_PULLDOWN_DISABLE,      //禁止下拉
        .mode = GPIO_MODE_OUTPUT,                   //输出模式
        .intr_type = GPIO_INTR_DISABLE,             //禁止中断
        .pin_bit_mask = (1ULL<<cfg->bl)                //GPIO脚
    };
    ret = gpio_config(&bl_gpio_cfg);
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "背光GPIO配置失败: %s", esp_err_to_name(ret));
        return ret;
    }

    //初始化复位脚
    if(cfg->rst >= 0)
    {
        ESP_LOGI(TAG, "初始化复位GPIO: %d", cfg->rst);
        gpio_config_t rst_gpio_cfg = 
        {
            .pull_up_en = GPIO_PULLUP_DISABLE,          //禁止上拉
            .pull_down_en = GPIO_PULLDOWN_DISABLE,      //禁止下拉
            .mode = GPIO_MODE_OUTPUT,                   //输出模式
            .intr_type = GPIO_INTR_DISABLE,             //禁止中断
            .pin_bit_mask = (1ULL<<cfg->rst)                //GPIO脚
        };
        ret = gpio_config(&rst_gpio_cfg);
        if(ret != ESP_OK) {
            ESP_LOGE(TAG, "复位GPIO配置失败: %s", esp_err_to_name(ret));
            return ret;
        }
    }

    //创建基于spi的lcd操作句柄
    ESP_LOGI(TAG, "创建LCD SPI IO句柄...");
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = cfg->dc,         //DC引脚，低电平表示发送命令，高电平表示传输数据
        .cs_gpio_num = cfg->cs,         //CS引脚
        .pclk_hz = (uint32_t)cfg->spi_fre,        //SPI时钟频率 (添加类型转换确保类型匹配)
        .lcd_cmd_bits = 8,              //命令长度
        .lcd_param_bits = 8,            //参数长度
        .spi_mode = 0,                  //使用SPI0模式
        .trans_queue_depth = 10,        //表示可以缓存的spi传输事务队列深度
        .on_color_trans_done = notify_flush_ready,   //刷新完成回调函数
        .user_ctx = cfg->cb_param,                                    //回调函数参数
        .flags = {    // 以下为 SPI 时序的相关参数，需根据 LCD 驱动 IC 的数据手册以及硬件的配置确定
            .sio_mode = 0,    // 通过一根数据线（MOSI）读写数据，0: Interface I 型，1: Interface II 型
        },
    };
    
    ret = esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io_config, &lcd_io_handle);
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "创建LCD SPI IO句柄失败: %s", esp_err_to_name(ret));
        return ret;
    }
    ESP_LOGI(TAG, "LCD SPI IO句柄创建成功");
    
    //硬件复位
    if(cfg->rst >= 0)
    {
        ESP_LOGI(TAG, "执行硬件复位...");
        gpio_set_level(cfg->rst, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
        gpio_set_level(cfg->rst, 1);
        vTaskDelay(pdMS_TO_TICKS(20));
        ESP_LOGI(TAG, "硬件复位完成");
    }

    /*向LCD写入初始化命令*/
    ESP_LOGI(TAG, "发送LCD初始化命令...");
    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_SWRESET, NULL, 0);    //软件复位
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "发送软件复位命令失败: %s", esp_err_to_name(ret));
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(150));
    
    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_SLPOUT, NULL, 0);     //退出休眠模式
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "发送退出休眠命令失败: %s", esp_err_to_name(ret));
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(200));
    
    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_COLMOD, (uint8_t[]) {0x55,}, 1);  //选择RGB数据格式
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "设置颜色格式失败: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, 0xb0, (uint8_t[]) {0x00, 0xF0}, 2);
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "设置偏压失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_INVON, NULL, 0);     //颜色翻转
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "设置颜色翻转失败: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_NORON, NULL, 0);     //普通显示模式
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "设置普通显示模式失败: %s", esp_err_to_name(ret));
        return ret;
    }
    
    uint8_t spin_type = 0;
    switch(cfg->spin)
    {
        case 0:
            spin_type = 0x00;   //不旋转
            break;
        case 1:
            spin_type = 0x60;   //顺时针90
            break;
        case 2:
            spin_type = 0xC0;   //180
            break;
        case 3:
            spin_type = 0xA0;   //顺时针270,（逆时针90）
            break;
        default:break;
    }
    ESP_LOGI(TAG, "设置屏幕旋转: 旋转类型=%02x", spin_type);
    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_MADCTL, (uint8_t[]) {spin_type,}, 1);   //屏旋转方向
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "设置屏幕旋转失败: %s", esp_err_to_name(ret));
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(150));
    
    ret = esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_DISPON, NULL, 0);    //打开显示
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "打开显示失败: %s", esp_err_to_name(ret));
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(300));
    ESP_LOGI(TAG, "ST7789 LCD驱动初始化完成");
    return ESP_OK;
}

/** st7789写入显示数据
 * @param x1,x2,y1,y2:显示区域
 * @return 无
*/
void st7789_flush(int x1,int x2,int y1,int y2,void *data)
{
    static int lcd_flush_count = 0;
    lcd_flush_count++;
    if(lcd_flush_count % 100 == 0) { // 改为每100次调用才输出一次日志
        ESP_LOGI(TAG, "LCD硬件刷新: x1=%d, x2=%d, y1=%d, y2=%d, 数据长度=%d字节", 
                 x1, x2, y1, y2, (x2 - x1) * (y2 - y1) * 2);
    }
    
    // define an area of frame memory where MCU can access
    if(x2 <= x1 || y2 <= y1)
    {
        ESP_LOGE(TAG, "刷新区域无效: x1=%d, x2=%d, y1=%d, y2=%d", x1, x2, y1, y2);
        if(s_flush_done_cb)
            s_flush_done_cb(NULL);
        return;
    }
    
    // 检查data是否有效
    if(data == NULL) {
        ESP_LOGE(TAG, "刷新数据为空指针");
        if(s_flush_done_cb)
            s_flush_done_cb(NULL);
        return;
    }
    
    esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_CASET, (uint8_t[]) {
        (x1 >> 8) & 0xFF,
        x1 & 0xFF,
        ((x2 - 1) >> 8) & 0xFF,
        (x2 - 1) & 0xFF,
    }, 4);
    esp_lcd_panel_io_tx_param(lcd_io_handle, LCD_CMD_RASET, (uint8_t[]) {
        (y1 >> 8) & 0xFF,
        y1 & 0xFF,
        ((y2 - 1) >> 8) & 0xFF,
        (y2 - 1) & 0xFF,
    }, 4);
    // transfer frame buffer
    size_t len = (x2 - x1) * (y2 - y1) * 2;
    esp_err_t ret = esp_lcd_panel_io_tx_color(lcd_io_handle, LCD_CMD_RAMWR, data, len);
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "传输数据失败: %s", esp_err_to_name(ret));
    }
    
    // 注意：回调函数由LVGL的disp_flush处理，这里不再重复调用
    // 避免重复通知LVGL刷新完成导致的显示问题
    
    // 注意：回调函数由LVGL的disp_flush处理，这里不再重复调用
    // 避免重复通知LVGL刷新完成导致的显示问题
    return ;
}

/** 控制背光
 * @param enable 是否使能背光
 * @return 无
*/
void st7789_lcd_backlight(bool enable)
{
    if(enable)
    {
        gpio_set_level(s_bl_gpio,1);
    }
    else
    {
    }
}
