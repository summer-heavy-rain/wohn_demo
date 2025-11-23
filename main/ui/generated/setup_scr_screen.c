/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_root
    ui->screen_cont_root = lv_obj_create(ui->screen);
    lv_obj_set_pos(ui->screen_cont_root, 0, 0);
    lv_obj_set_size(ui->screen_cont_root, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_root, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_root, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_root, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_scene7
    ui->screen_cont_scene7 = lv_obj_create(ui->screen_cont_root);
    lv_obj_set_pos(ui->screen_cont_scene7, 0, 0);
    lv_obj_set_size(ui->screen_cont_scene7, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_scene7, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_scene7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_scene7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_7
    ui->screen_animimg_7 = lv_img_create(ui->screen_cont_scene7);
    lv_obj_add_flag(ui->screen_animimg_7, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_animimg_7, "S:/imgs/scene7.bin");
    lv_img_set_pivot(ui->screen_animimg_7, 50,50);
    lv_img_set_angle(ui->screen_animimg_7, 0);
    lv_obj_set_pos(ui->screen_animimg_7, -57, -240);
    lv_obj_set_size(ui->screen_animimg_7, 400, 480);

    //Write style for screen_animimg_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_animimg_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_animimg_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_animimg_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_animimg_7, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_title7
    ui->screen_title7 = lv_img_create(ui->screen_cont_scene7);
    lv_obj_add_flag(ui->screen_title7, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_title7, "S:/imgs/txt01.bin");
    lv_img_set_pivot(ui->screen_title7, 50,50);
    lv_img_set_angle(ui->screen_title7, 0);
    lv_obj_set_pos(ui->screen_title7, 0, 11);
    lv_obj_set_size(ui->screen_title7, 319, 152);

    //Write style for screen_title7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_title7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_title7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_title7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_title7, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_scene6
    ui->screen_cont_scene6 = lv_obj_create(ui->screen_cont_root);
    lv_obj_set_pos(ui->screen_cont_scene6, 0, 0);
    lv_obj_set_size(ui->screen_cont_scene6, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_scene6, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_cont_scene6, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_cont_scene6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_scene6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_6
    ui->screen_animimg_6 = lv_img_create(ui->screen_cont_scene6);
    lv_obj_add_flag(ui->screen_animimg_6, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_animimg_6, "S:/imgs/scene6.bin");
    lv_img_set_pivot(ui->screen_animimg_6, 50,50);
    lv_img_set_angle(ui->screen_animimg_6, 0);
    lv_obj_set_pos(ui->screen_animimg_6, -200, 0);
    lv_obj_set_size(ui->screen_animimg_6, 720, 320);

    //Write style for screen_animimg_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_animimg_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_animimg_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_animimg_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_animimg_6, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsB6
    ui->screen_bsB6 = lv_img_create(ui->screen_cont_scene6);
    lv_obj_add_flag(ui->screen_bsB6, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsB6, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsB6, 50,50);
    lv_img_set_angle(ui->screen_bsB6, 0);
    lv_obj_set_pos(ui->screen_bsB6, -2, 171);
    lv_obj_set_size(ui->screen_bsB6, 291, 69);

    //Write style for screen_bsB6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsB6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsB6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsB6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsB6, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsT6
    ui->screen_bsT6 = lv_img_create(ui->screen_cont_scene6);
    lv_obj_add_flag(ui->screen_bsT6, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsT6, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsT6, 50,50);
    lv_img_set_angle(ui->screen_bsT6, 0);
    lv_obj_set_pos(ui->screen_bsT6, -4, -1);
    lv_obj_set_size(ui->screen_bsT6, 291, 45);

    //Write style for screen_bsT6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsT6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsT6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsT6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsT6, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_scene5
    ui->screen_cont_scene5 = lv_obj_create(ui->screen_cont_root);
    lv_obj_set_pos(ui->screen_cont_scene5, 0, 0);
    lv_obj_set_size(ui->screen_cont_scene5, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_scene5, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_cont_scene5, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_cont_scene5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_scene5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_5
    ui->screen_animimg_5 = lv_img_create(ui->screen_cont_scene5);
    lv_obj_add_flag(ui->screen_animimg_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_animimg_5, "S:/imgs/scene5.bin");
    lv_img_set_pivot(ui->screen_animimg_5, 50,50);
    lv_img_set_angle(ui->screen_animimg_5, 0);
    lv_obj_set_pos(ui->screen_animimg_5, 0, 0);
    lv_obj_set_size(ui->screen_animimg_5, 381, 350);

    //Write style for screen_animimg_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_animimg_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_animimg_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_animimg_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_animimg_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsL5
    ui->screen_bsL5 = lv_img_create(ui->screen_cont_scene5);
    lv_obj_add_flag(ui->screen_bsL5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsL5, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsL5, 50,50);
    lv_img_set_angle(ui->screen_bsL5, 0);
    lv_obj_set_pos(ui->screen_bsL5, 180, -31);
    lv_obj_set_size(ui->screen_bsL5, 100, 290);

    //Write style for screen_bsL5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsL5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsL5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsL5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsL5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsR5
    ui->screen_bsR5 = lv_img_create(ui->screen_cont_scene5);
    lv_obj_add_flag(ui->screen_bsR5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsR5, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsR5, 50,50);
    lv_img_set_angle(ui->screen_bsR5, 0);
    lv_obj_set_pos(ui->screen_bsR5, 0, -26);
    lv_obj_set_size(ui->screen_bsR5, 30, 290);

    //Write style for screen_bsR5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsR5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsR5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsR5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsR5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_scene4
    ui->screen_cont_scene4 = lv_obj_create(ui->screen_cont_root);
    lv_obj_set_pos(ui->screen_cont_scene4, 0, 0);
    lv_obj_set_size(ui->screen_cont_scene4, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_scene4, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_cont_scene4, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_cont_scene4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_scene4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_4R
    ui->screen_animimg_4R = lv_img_create(ui->screen_cont_scene4);
    lv_obj_add_flag(ui->screen_animimg_4R, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_animimg_4R, "S:/imgs/scene4R.bin");
    lv_img_set_pivot(ui->screen_animimg_4R, 50,50);
    lv_img_set_angle(ui->screen_animimg_4R, 0);
    lv_obj_set_pos(ui->screen_animimg_4R, -224, 0);
    lv_obj_set_size(ui->screen_animimg_4R, 504, 240);

    //Write style for screen_animimg_4R, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_animimg_4R, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_animimg_4R, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_animimg_4R, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_animimg_4R, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_4L
    ui->screen_animimg_4L = lv_img_create(ui->screen_cont_scene4);
    lv_obj_add_flag(ui->screen_animimg_4L, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_animimg_4L, "S:/imgs/scene4L.bin");
    lv_img_set_pivot(ui->screen_animimg_4L, 50,50);
    lv_img_set_angle(ui->screen_animimg_4L, 0);
    lv_obj_set_pos(ui->screen_animimg_4L, -224, 0);
    lv_obj_set_size(ui->screen_animimg_4L, 504, 240);

    //Write style for screen_animimg_4L, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_animimg_4L, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_animimg_4L, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_animimg_4L, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_animimg_4L, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsR4
    ui->screen_bsR4 = lv_img_create(ui->screen_cont_scene4);
    lv_obj_add_flag(ui->screen_bsR4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsR4, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsR4, 50,50);
    lv_img_set_angle(ui->screen_bsR4, 0);
    lv_obj_set_pos(ui->screen_bsR4, -15, -21);
    lv_obj_set_size(ui->screen_bsR4, 150, 290);

    //Write style for screen_bsR4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsR4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsR4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsR4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsR4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsL4
    ui->screen_bsL4 = lv_img_create(ui->screen_cont_scene4);
    lv_obj_add_flag(ui->screen_bsL4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsL4, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsL4, 50,50);
    lv_img_set_angle(ui->screen_bsL4, 0);
    lv_obj_set_pos(ui->screen_bsL4, 265, -26);
    lv_obj_set_size(ui->screen_bsL4, 60, 290);

    //Write style for screen_bsL4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsL4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsL4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsL4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsL4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_scene3
    ui->screen_cont_scene3 = lv_obj_create(ui->screen_cont_root);
    lv_obj_set_pos(ui->screen_cont_scene3, 0, 0);
    lv_obj_set_size(ui->screen_cont_scene3, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_scene3, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_cont_scene3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_cont_scene3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_scene3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_3
    ui->screen_animimg_3 = lv_img_create(ui->screen_cont_scene3);
    lv_obj_add_flag(ui->screen_animimg_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_animimg_3, "S:/imgs/scene3.bin");
    lv_img_set_pivot(ui->screen_animimg_3, 50,50);
    lv_img_set_angle(ui->screen_animimg_3, 0);
    lv_obj_set_pos(ui->screen_animimg_3, -48, -13);
    lv_obj_set_size(ui->screen_animimg_3, 468, 390);

    //Write style for screen_animimg_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_animimg_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_animimg_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_animimg_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_animimg_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsR3
    ui->screen_bsR3 = lv_img_create(ui->screen_cont_scene3);
    lv_obj_add_flag(ui->screen_bsR3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsR3, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsR3, 50,50);
    lv_img_set_angle(ui->screen_bsR3, 0);
    lv_obj_set_pos(ui->screen_bsR3, 162, -33);
    lv_obj_set_size(ui->screen_bsR3, 174, 300);

    //Write style for screen_bsR3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsR3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsR3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsR3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsR3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bsL3
    ui->screen_bsL3 = lv_img_create(ui->screen_cont_scene3);
    lv_obj_add_flag(ui->screen_bsL3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_bsL3, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_bsL3, 50,50);
    lv_img_set_angle(ui->screen_bsL3, 0);
    lv_obj_set_pos(ui->screen_bsL3, -24, -33);
    lv_obj_set_size(ui->screen_bsL3, 57, 304);

    //Write style for screen_bsL3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_bsL3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_bsL3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bsL3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_bsL3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_scene2
    ui->screen_cont_scene2 = lv_obj_create(ui->screen_cont_root);
    lv_obj_set_pos(ui->screen_cont_scene2, 0, 0);
    lv_obj_set_size(ui->screen_cont_scene2, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_scene2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_cont_scene2, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_cont_scene2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_scene2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_2
    ui->screen_animimg_2 = lv_img_create(ui->screen_cont_scene2);
    lv_obj_add_flag(ui->screen_animimg_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_animimg_2, "S:/imgs/scene2.bin");
    lv_img_set_pivot(ui->screen_animimg_2, 50,50);
    lv_img_set_angle(ui->screen_animimg_2, 0);
    lv_obj_set_pos(ui->screen_animimg_2, -121, -100);
    lv_obj_set_size(ui->screen_animimg_2, 450, 405);

    //Write style for screen_animimg_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_animimg_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_animimg_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_animimg_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_animimg_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_blackslideRight
    ui->screen_blackslideRight = lv_img_create(ui->screen_cont_scene2);
    lv_obj_add_flag(ui->screen_blackslideRight, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_blackslideRight, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_blackslideRight, 50,50);
    lv_img_set_angle(ui->screen_blackslideRight, 0);
    lv_obj_set_pos(ui->screen_blackslideRight, -123, -95);
    lv_obj_set_size(ui->screen_blackslideRight, 210, 411);

    //Write style for screen_blackslideRight, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_blackslideRight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_blackslideRight, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_blackslideRight, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_blackslideRight, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_blackslideLeft
    ui->screen_blackslideLeft = lv_img_create(ui->screen_cont_scene2);
    lv_obj_add_flag(ui->screen_blackslideLeft, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_blackslideLeft, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_blackslideLeft, 50,50);
    lv_img_set_angle(ui->screen_blackslideLeft, 0);
    lv_obj_set_pos(ui->screen_blackslideLeft, 248, -94);
    lv_obj_set_size(ui->screen_blackslideLeft, 90, 405);

    //Write style for screen_blackslideLeft, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_blackslideLeft, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_blackslideLeft, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_blackslideLeft, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_blackslideLeft, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_scene1
    ui->screen_cont_scene1 = lv_obj_create(ui->screen_cont_root);
    lv_obj_set_pos(ui->screen_cont_scene1, 0, 0);
    lv_obj_set_size(ui->screen_cont_scene1, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_scene1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_cont_scene1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_cont_scene1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_scene1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_1
    ui->screen_animimg_1 = lv_animimg_create(ui->screen_cont_scene1);
    lv_animimg_set_src(ui->screen_animimg_1, (const void **) screen_animimg_1_imgs, 1);
    lv_animimg_set_duration(ui->screen_animimg_1, 3*1);
    lv_animimg_set_repeat_count(ui->screen_animimg_1, LV_ANIM_REPEAT_INFINITE);
    // lv_animimg_set_playback_time(ui->screen_animimg_1, 200);
    lv_animimg_start(ui->screen_animimg_1);
    lv_obj_set_pos(ui->screen_animimg_1, -42, 0);
    lv_obj_set_size(ui->screen_animimg_1, 404, 240);

    //Write codes screen_blackslideTop
    ui->screen_blackslideTop = lv_img_create(ui->screen_cont_scene1);
    lv_obj_add_flag(ui->screen_blackslideTop, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_blackslideTop, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_blackslideTop, 20,50);
    lv_img_set_angle(ui->screen_blackslideTop, 0);
    lv_obj_set_pos(ui->screen_blackslideTop, -18, 157);
    lv_obj_set_size(ui->screen_blackslideTop, 350, 83);

    //Write style for screen_blackslideTop, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_blackslideTop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_blackslideTop, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_blackslideTop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_blackslideTop, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_blackslideBottom
    ui->screen_blackslideBottom = lv_img_create(ui->screen_cont_scene1);
    lv_obj_add_flag(ui->screen_blackslideBottom, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_blackslideBottom, "S:/imgs/blackslide.bin");
    lv_img_set_pivot(ui->screen_blackslideBottom, 30,50);
    lv_img_set_angle(ui->screen_blackslideBottom, 0);
    lv_obj_set_pos(ui->screen_blackslideBottom, -35, 0);
    lv_obj_set_size(ui->screen_blackslideBottom, 350, 50);

    //Write style for screen_blackslideBottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_blackslideBottom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_blackslideBottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_blackslideBottom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_blackslideBottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

}
