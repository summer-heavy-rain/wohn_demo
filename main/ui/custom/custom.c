/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
 #include <stdio.h>
 #include "lvgl.h"
 #include "custom.h"
 
 /*********************
  *      DEFINES
  *********************/
 
 /**********************
  *      TYPEDEFS
  **********************/
 
 /**********************
  *  STATIC PROTOTYPES
  **********************/
static void create_scene1(lv_ui *ui);
static void create_scene2(lv_ui *ui);
static void create_scene3(lv_ui *ui);
static void create_scene4(lv_ui *ui);
static void create_scene5(lv_ui *ui);
static void create_scene6(lv_ui *ui);
static void create_scene7(lv_ui *ui);
 
/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */

/**
 * 动画结束回调函数：隐藏容器
 * 通过动画对象的父对象自动找到对应的容器
 */
static lv_ui *s_ui_ctx = NULL;

static void anim_ready_hide_container(lv_anim_t *a) {
    // a->var 是被动画的对象（比如 screen_animimg_1）
    lv_obj_t *anim_obj = (lv_obj_t *)a->var;
    if (anim_obj != NULL) {
        // 获取动画对象的父容器（比如 screen_cont_scene1）
        lv_obj_t *container = lv_obj_get_parent(anim_obj);
        if (container != NULL) {
            lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void set_obj_opa(void *obj, int32_t value){
    lv_obj_set_style_opa((lv_obj_t *)obj, value, LV_PART_MAIN|LV_STATE_DEFAULT);
}

static void scene2_anim_ready(lv_anim_t *a) {
    anim_ready_hide_container(a);
    if (s_ui_ctx == NULL) {
        return;
    }

    /* 显示场景2并启动动画 */
    lv_obj_clear_flag(s_ui_ctx->screen_cont_scene2, LV_OBJ_FLAG_HIDDEN);
    create_scene2(s_ui_ctx);
}

static void scene3_anim_ready(lv_anim_t *a) {
    anim_ready_hide_container(a);
    if (s_ui_ctx == NULL) {
        return;
    }

    lv_obj_clear_flag(s_ui_ctx->screen_cont_scene3, LV_OBJ_FLAG_HIDDEN);
    create_scene3(s_ui_ctx);
}

static void scene4_anim_ready(lv_anim_t *a) {
    anim_ready_hide_container(a);
    if (s_ui_ctx == NULL) {
        return;
    }

    lv_obj_clear_flag(s_ui_ctx->screen_cont_scene3, LV_OBJ_FLAG_HIDDEN);
    create_scene4(s_ui_ctx);
}

static void scene5_anim_ready(lv_anim_t *a){
    anim_ready_hide_container(a);
    if (s_ui_ctx == NULL) {
        return;
    }

    lv_obj_clear_flag(s_ui_ctx->screen_cont_scene4, LV_OBJ_FLAG_HIDDEN);
    create_scene5(s_ui_ctx);
}

static void scene6_anim_ready(lv_anim_t *a){
    anim_ready_hide_container(a);
    if (s_ui_ctx == NULL) {
        return;
    }

    lv_obj_clear_flag(s_ui_ctx->screen_cont_scene5, LV_OBJ_FLAG_HIDDEN);
    create_scene6(s_ui_ctx);
}

static void scene7_anim_ready(lv_anim_t *a){
    anim_ready_hide_container(a);
    if (s_ui_ctx == NULL) {
        return;
    }

    lv_obj_clear_flag(s_ui_ctx->screen_cont_scene6, LV_OBJ_FLAG_HIDDEN);
    create_scene7(s_ui_ctx);
}


static void create_scene1(lv_ui *ui) {
    /* 场景1出场，先确保容器可见，其他场景隐藏 */
    lv_obj_clear_flag(ui->screen_cont_scene1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_cont_scene2, LV_OBJ_FLAG_HIDDEN);

    // animimg_1 动画：3000ms，这是最晚结束的动画
    ui_animation(
        ui->screen_animimg_1,
        4000,                /* duration */
        0,                   /* delay */
        0,                   /* start x */
        -80,                /* end x */
        lv_anim_path_linear, /* path */
        0,                   /* repeat_cnt (0 = no repeat) */
        0,                   /* repeat_delay */
        0,                   /* playback_time */
        0,                   /* playback_delay */
        (lv_anim_exec_xcb_t)lv_obj_set_x,
        NULL,                /* start_cb */
        scene2_anim_ready,  /* ready_cb: 动画结束时隐藏容器并启动场景2 */
        NULL                 /* deleted_cb */
    );

	if(ui->screen_blackslideTop) {
        ui_animation(
            ui->screen_blackslideTop,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }
    if(ui->screen_blackslideBottom) {
        ui_animation(
            ui->screen_blackslideBottom,
            800,
            1500,
            255,
            0,
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }

}

static void create_scene2(lv_ui *ui) {
    /* 确保场景2容器可见 */
    lv_obj_clear_flag(ui->screen_cont_scene2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_cont_scene3, LV_OBJ_FLAG_HIDDEN);

    ui_animation(
        ui->screen_animimg_2,
        4000,                /* duration */
        0,                   /* delay */
        -140,                   /* start y */
        -10,                /* end y */
        lv_anim_path_ease_in_out, /* path */
        0,                   /* repeat_cnt (0 = no repeat) */
        0,                   /* repeat_delay */
        0,                   /* playback_time */
        0,                   /* playback_delay */
        (lv_anim_exec_xcb_t)lv_obj_set_y,
        NULL,                /* start_cb */
        scene3_anim_ready,  /* ready_cb: 动画结束时隐藏容器 */
        NULL                 /* deleted_cb */
    );

    if(ui->screen_blackslideRight) {
        ui_animation(
            ui->screen_blackslideRight,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }
    if(ui->screen_blackslideLeft) {
        ui_animation(
            ui->screen_blackslideLeft,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }

}


static void create_scene3(lv_ui *ui) {

    lv_obj_clear_flag(ui->screen_cont_scene3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_cont_scene2, LV_OBJ_FLAG_HIDDEN);

    ui_animation(
        ui->screen_animimg_3,
        4000,                /* duration */
        0,                   /* delay */
        0,                   /* start y */
        -100,                /* end y */
        lv_anim_path_ease_in_out, /* path */
        0,                   /* repeat_cnt (0 = no repeat) */
        0,                   /* repeat_delay */
        0,                   /* playback_time */
        0,                   /* playback_delay */
        (lv_anim_exec_xcb_t)lv_obj_set_y,
        NULL,                /* start_cb */
        scene4_anim_ready,  /* ready_cb: 动画结束时隐藏容器 */
        NULL                 /* deleted_cb */
    );

    if(ui->screen_bsR3) {
        ui_animation(
            ui->screen_bsR3,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }

    if(ui->screen_bsL3) {
        ui_animation(
            ui->screen_bsL3,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }
}

static void create_scene4(lv_ui *ui){

    lv_obj_clear_flag(ui->screen_cont_scene4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_cont_scene3, LV_OBJ_FLAG_HIDDEN);

    ui_animation(
        ui->screen_animimg_4L,
        6000, 0,
        -224, -10,
        lv_anim_path_linear,
        0, 0, 0, 0,
        (lv_anim_exec_xcb_t)lv_obj_set_x,
        NULL,
        scene5_anim_ready,
        NULL
    );

    ui_animation(
        ui->screen_animimg_4R,
        6000, 0,
        -224, -10,
        lv_anim_path_linear,
        0, 0, 0, 0,
        (lv_anim_exec_xcb_t)lv_obj_set_x,
        NULL,
        NULL,
        NULL
    );

    ui_animation(
        ui->screen_bsR4,
        800,                 /* duration */
        1500,                /* delay */
        255,                 /* start opa */
        0,                   /* end opa */
        lv_anim_path_ease_in,
        0, 0, 0, 0,
        (lv_anim_exec_xcb_t)set_obj_opa,
        NULL, NULL, NULL
    );

    ui_animation(
        ui->screen_bsL4,
        800,                 /* duration */
        1500,                /* delay */
        255,                 /* start opa */
        0,                   /* end opa */
        lv_anim_path_ease_in,
        0, 0, 0, 0,
        (lv_anim_exec_xcb_t)set_obj_opa,
        NULL, NULL, NULL
    );

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui->screen_animimg_4L);
    lv_anim_set_values(&a, 255, 0);
    lv_anim_set_time(&a, 1700); // 1s
    lv_anim_set_playback_time(&a, 1700);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&a, set_obj_opa);
    lv_anim_start(&a);

}
static void create_scene5(lv_ui *ui){
    lv_obj_clear_flag(ui->screen_cont_scene5, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_cont_scene4, LV_OBJ_FLAG_HIDDEN);

    ui_animation(
        ui->screen_animimg_5,
        4000,                /* duration */
        0,                   /* delay */
        0,                   /* start y */
        -110,                /* end y */
        lv_anim_path_ease_in_out, /* path */
        0,                   /* repeat_cnt (0 = no repeat) */
        0,                   /* repeat_delay */
        0,                   /* playback_time */
        0,                   /* playback_delay */
        (lv_anim_exec_xcb_t)lv_obj_set_y,
        NULL,                /* start_cb */
        scene6_anim_ready,  /* ready_cb: 动画结束时隐藏容器 */
        NULL                 /* deleted_cb */
    );

    if(ui->screen_bsR5) {
        ui_animation(
            ui->screen_bsR5,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }

    if(ui->screen_bsL5) {
        ui_animation(
            ui->screen_bsL5,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }
}

static void create_scene6(lv_ui *ui){
    lv_obj_clear_flag(ui->screen_cont_scene6, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_cont_scene5, LV_OBJ_FLAG_HIDDEN);

    ui_animation(
        ui->screen_animimg_6,
        4000,                /* duration */
        0,                   /* delay */
        -360,                   /* start y */
        -200,                /* end y */
        lv_anim_path_ease_in_out, /* path */
        0,                   /* repeat_cnt (0 = no repeat) */
        0,                   /* repeat_delay */
        0,                   /* playback_time */
        0,                   /* playback_delay */
        (lv_anim_exec_xcb_t)lv_obj_set_x,
        NULL,                /* start_cb */
        scene7_anim_ready,  /* ready_cb: 动画结束时隐藏容器 */
        NULL                 /* deleted_cb */
    );

    if(ui->screen_bsT6) {
        ui_animation(
            ui->screen_bsT6,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }

    if(ui->screen_bsB6) {
        ui_animation(
            ui->screen_bsB6,
            800,                 /* duration */
            1500,                /* delay */
            255,                 /* start opa */
            0,                   /* end opa */
            lv_anim_path_ease_in,
            0, 0, 0, 0,
            (lv_anim_exec_xcb_t)set_obj_opa,
            NULL, NULL, NULL
        );
    }

}
static void create_scene7(lv_ui *ui){
    lv_obj_clear_flag(ui->screen_cont_scene7, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_cont_scene6, LV_OBJ_FLAG_HIDDEN);

    ui_animation(
        ui->screen_animimg_7,
        5800,                /* duration */
        0,                   /* delay */
        -240,                   /* start y */
        -30,                /* end y */
        lv_anim_path_ease_in_out, /* path */
        0,                   /* repeat_cnt (0 = no repeat) */
        0,                   /* repeat_delay */
        0,                   /* playback_time */
        0,                   /* playback_delay */
        (lv_anim_exec_xcb_t)lv_obj_set_y,
        NULL,                /* start_cb */
        NULL,  /* ready_cb: 动画结束时隐藏容器 */
        NULL                 /* deleted_cb */
    );

    ui_animation(
        ui->screen_title7,
        800,                 /* duration */
        3000,                /* delay */
        0,                 /* start opa */
        255,                   /* end opa */
        lv_anim_path_ease_in,
        0, 0, 0, 0,
        (lv_anim_exec_xcb_t)set_obj_opa,
        NULL, NULL, NULL
    );
}


 void custom_init(lv_ui *ui)
{
    s_ui_ctx = ui;

    // 创建场景1的动画，场景2会在场景1结束后自动触发
    create_scene1(ui);
   
    // 注意：不再需要全局变量保存容器指针
    // 回调函数会自动通过动画对象的父对象找到对应的容器
}
 
 