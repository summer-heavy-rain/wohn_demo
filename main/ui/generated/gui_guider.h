/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_cont_root;
	lv_obj_t *screen_cont_scene7;
	lv_obj_t *screen_animimg_7;
	lv_obj_t *screen_title7;
	lv_obj_t *screen_cont_scene6;
	lv_obj_t *screen_animimg_6;
	lv_obj_t *screen_bsB6;
	lv_obj_t *screen_bsT6;
	lv_obj_t *screen_cont_scene5;
	lv_obj_t *screen_animimg_5;
	lv_obj_t *screen_bsL5;
	lv_obj_t *screen_bsR5;
	lv_obj_t *screen_cont_scene4;
	lv_obj_t *screen_animimg_4R;
	lv_obj_t *screen_animimg_4L;
	lv_obj_t *screen_bsR4;
	lv_obj_t *screen_bsL4;
	lv_obj_t *screen_cont_scene3;
	lv_obj_t *screen_animimg_3;
	lv_obj_t *screen_bsR3;
	lv_obj_t *screen_bsL3;
	lv_obj_t *screen_cont_scene2;
	lv_obj_t *screen_animimg_2;
	lv_obj_t *screen_blackslideRight;
	lv_obj_t *screen_blackslideLeft;
	lv_obj_t *screen_cont_scene1;
	lv_obj_t *screen_animimg_1;
	lv_obj_t *screen_blackslideTop;
	lv_obj_t *screen_blackslideBottom;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_scene7_alpha_400x480);
LV_IMG_DECLARE(_txt01_alpha_319x152);
LV_IMG_DECLARE(_scene6_alpha_720x320);
LV_IMG_DECLARE(_blackslide_alpha_291x69);
LV_IMG_DECLARE(_blackslide_alpha_291x45);
LV_IMG_DECLARE(_scene5_alpha_381x350);
LV_IMG_DECLARE(_blackslide_alpha_100x290);
LV_IMG_DECLARE(_blackslide_alpha_30x290);
LV_IMG_DECLARE(_scene3R_alpha_504x240);
LV_IMG_DECLARE(_scene3L_alpha_504x240);
LV_IMG_DECLARE(_blackslide_alpha_150x290);
LV_IMG_DECLARE(_blackslide_alpha_60x290);
LV_IMG_DECLARE(_scene3_alpha_468x390);
LV_IMG_DECLARE(_blackslide_alpha_174x300);
LV_IMG_DECLARE(_blackslide_alpha_57x304);
LV_IMG_DECLARE(_scene2_alpha_450x405);
LV_IMG_DECLARE(_blackslide_alpha_210x411);
LV_IMG_DECLARE(_blackslide_alpha_90x405);
#include "extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(screen_animimg_1scene1);
LV_IMG_DECLARE(_blackslide_alpha_350x83);
LV_IMG_DECLARE(_blackslide_alpha_350x50);



#ifdef __cplusplus
}
#endif
#endif
