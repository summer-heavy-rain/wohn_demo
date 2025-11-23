#include "lvgl.h"
#include <stdio.h>
#include <errno.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "sdcard.h"

// 修改前
#define LV_FS_ROOT   MOUNT_POINT"/"   // "/sdcard/"

// 修改后
#define LV_FS_ROOT   MOUNT_POINT       // "/sdcard"
static const char *TAG_FS = "lv_fs";

static void * my_open_cb(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode)
{
    /* path 是盘符后的相对路径，比如 "imgs/_blackslide_alpha_30x290.bin" */
    static char full_path[256];
    snprintf(full_path, sizeof(full_path), LV_FS_ROOT"%s", path);

    ESP_LOGI(TAG_FS, "尝试打开文件: %s", full_path);
    const char *fmode = (mode == LV_FS_MODE_WR) ? "wb" :
                        (mode == LV_FS_MODE_RD) ? "rb" : "rb+";
    FILE * f = fopen(full_path, fmode);
    if(f == NULL) {
        ESP_LOGE(TAG_FS, "文件打开失败: %s, 错误: %s", full_path, strerror(errno));
    }
    return f;
}

static lv_fs_res_t my_close_cb(lv_fs_drv_t * drv, void * file_p)
{
    FILE * f = (FILE *)file_p;
    fclose(f);
    return LV_FS_RES_OK;
}

static lv_fs_res_t my_read_cb(lv_fs_drv_t * drv, void * file_p,
                              void * buf, uint32_t btr, uint32_t * br)
{
    FILE * f = (FILE *)file_p;
    size_t r = fread(buf, 1, btr, f);
    if(br) *br = r;
    if(r == 0 && ferror(f)) {
        ESP_LOGE(TAG_FS, "文件读取失败，错误: %s", strerror(errno));
        return LV_FS_RES_FS_ERR;
    }
    return LV_FS_RES_OK;
}

static lv_fs_res_t my_seek_cb(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence)
{
    FILE * f = (FILE *)file_p;
    int w;
    if(whence == LV_FS_SEEK_SET) w = SEEK_SET;
    else if(whence == LV_FS_SEEK_CUR) w = SEEK_CUR;
    else w = SEEK_END;

    if(fseek(f, pos, w) != 0) return LV_FS_RES_FS_ERR;
    return LV_FS_RES_OK;
}

static lv_fs_res_t my_tell_cb(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    FILE * f = (FILE *)file_p;
    long p = ftell(f);
    if(p < 0) return LV_FS_RES_FS_ERR;
    *pos_p = (uint32_t)p;
    return LV_FS_RES_OK;
}

void lv_port_fs_init(void)
{
    static lv_fs_drv_t drv;
    lv_fs_drv_init(&drv);

    drv.letter = 'S';              // 盘符，以后路径用 "S:/..."
    drv.open_cb = my_open_cb;
    drv.close_cb = my_close_cb;
    drv.read_cb = my_read_cb;
    drv.seek_cb = my_seek_cb;
    drv.tell_cb = my_tell_cb;

    lv_fs_drv_register(&drv);
}