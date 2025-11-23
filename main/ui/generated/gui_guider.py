# Copyright 2025 NXP
# NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import SDL
import utime as time
import usys as sys
import lvgl as lv
import lodepng as png
import ustruct
import fs_driver

lv.init()
SDL.init(w=280,h=240)

# Register SDL display driver.
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytearray(280*240*4)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 280
disp_drv.ver_res = 240
disp_drv.register()

# Regsiter SDL mouse driver
indev_drv = lv.indev_drv_t()
indev_drv.init()
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

# Below: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

COLOR_SIZE = lv.color_t.__SIZE__
COLOR_IS_SWAPPED = hasattr(lv.color_t().ch,'green_h')

class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)

# Parse PNG file header
# Taken from https://github.com/shibukawa/imagesize_py/blob/ffef30c1a4715c5acf90e8945ceb77f4a2ed2d45/imagesize.py#L63-L85

def get_png_info(decoder, src, header):
    # Only handle variable image types

    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV

    data = lv.img_dsc_t.__cast__(src).data
    if data == None:
        return lv.RES.INV

    png_header = bytes(data.__dereference__(24))

    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = ustruct.unpack(">LL", png_header[start:start+8])
        except ustruct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV

    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA

    return lv.RES.OK

def convert_rgba8888_to_bgra8888(img_view):
    for i in range(0, len(img_view), lv.color_t.__SIZE__):
        ch = lv.color_t.__cast__(img_view[i:i]).ch
        ch.red, ch.blue = ch.blue, ch.red

# Read and parse PNG file

def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.__cast__(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = png_width.int_val * png_height.int_val * 4
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)

    if COLOR_SIZE == 4:
        convert_rgba8888_to_bgra8888(img_view)
    else:
        raise lodepng_error("Error: Color mode not supported yet!")

    dsc.img_data = img_data
    return lv.RES.OK

# Above: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_zoom(v)

def anim_img_rotate_cb(obj, v):
    obj.set_angle(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.font_load(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.img_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = e.get_current_target()
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RES.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    target = e.get_target()
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = target.get_screen()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = target.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, target), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    target = e.get_current_target()
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if target.get_pressed_date(date) == lv.RES.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.clear_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            target.delete()

# Create screen
screen = lv.obj()
screen.set_size(280, 240)
screen.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_root
screen_cont_root = lv.obj(screen)
screen_cont_root.set_pos(0, 0)
screen_cont_root.set_size(280, 240)
screen_cont_root.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_root, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_root.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_root.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_root.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_root.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_root.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_root.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_root.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_root.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_scene7
screen_cont_scene7 = lv.obj(screen_cont_root)
screen_cont_scene7.set_pos(0, 0)
screen_cont_scene7.set_size(280, 240)
screen_cont_scene7.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_scene7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_scene7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene7.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene7.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene7.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene7.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene7.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene7.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_animimg_7
screen_animimg_7 = lv.img(screen_cont_scene7)
screen_animimg_7.set_src("B:MicroPython/_scene7_alpha_400x480.bin")
screen_animimg_7.add_flag(lv.obj.FLAG.CLICKABLE)
screen_animimg_7.set_pivot(50,50)
screen_animimg_7.set_angle(0)
screen_animimg_7.set_pos(-59, -122)
screen_animimg_7.set_size(400, 480)
# Set style for screen_animimg_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_animimg_7.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_7.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_7.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_title
screen_title = lv.img(screen_cont_scene7)
screen_title.set_src("B:MicroPython/_txt01_alpha_319x152.bin")
screen_title.add_flag(lv.obj.FLAG.CLICKABLE)
screen_title.set_pivot(50,50)
screen_title.set_angle(0)
screen_title.set_pos(-19, 11)
screen_title.set_size(319, 152)
screen_title.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for screen_title, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_title.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_title.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_title.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_scene6
screen_cont_scene6 = lv.obj(screen_cont_root)
screen_cont_scene6.set_pos(0, 0)
screen_cont_scene6.set_size(280, 240)
screen_cont_scene6.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_scene6.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_scene6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_scene6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_animimg_6
screen_animimg_6 = lv.img(screen_cont_scene6)
screen_animimg_6.set_src("B:MicroPython/_scene6_alpha_720x320.bin")
screen_animimg_6.add_flag(lv.obj.FLAG.CLICKABLE)
screen_animimg_6.set_pivot(50,50)
screen_animimg_6.set_angle(0)
screen_animimg_6.set_pos(-200, 0)
screen_animimg_6.set_size(720, 320)
# Set style for screen_animimg_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_animimg_6.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_6.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsB6
screen_bsB6 = lv.img(screen_cont_scene6)
screen_bsB6.set_src("B:MicroPython/_blackslide_alpha_291x69.bin")
screen_bsB6.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsB6.set_pivot(50,50)
screen_bsB6.set_angle(0)
screen_bsB6.set_pos(-2, 171)
screen_bsB6.set_size(291, 69)
# Set style for screen_bsB6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsB6.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsB6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsB6.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsT6
screen_bsT6 = lv.img(screen_cont_scene6)
screen_bsT6.set_src("B:MicroPython/_blackslide_alpha_291x45.bin")
screen_bsT6.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsT6.set_pivot(50,50)
screen_bsT6.set_angle(0)
screen_bsT6.set_pos(-4, -1)
screen_bsT6.set_size(291, 45)
# Set style for screen_bsT6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsT6.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsT6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsT6.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_scene5
screen_cont_scene5 = lv.obj(screen_cont_root)
screen_cont_scene5.set_pos(0, 0)
screen_cont_scene5.set_size(280, 240)
screen_cont_scene5.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_scene5.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_scene5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_scene5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_animimg_5
screen_animimg_5 = lv.img(screen_cont_scene5)
screen_animimg_5.set_src("B:MicroPython/_scene5_alpha_381x350.bin")
screen_animimg_5.add_flag(lv.obj.FLAG.CLICKABLE)
screen_animimg_5.set_pivot(50,50)
screen_animimg_5.set_angle(0)
screen_animimg_5.set_pos(0, 0)
screen_animimg_5.set_size(381, 350)
# Set style for screen_animimg_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_animimg_5.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_5.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsL5
screen_bsL5 = lv.img(screen_cont_scene5)
screen_bsL5.set_src("B:MicroPython/_blackslide_alpha_100x290.bin")
screen_bsL5.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsL5.set_pivot(50,50)
screen_bsL5.set_angle(0)
screen_bsL5.set_pos(180, -31)
screen_bsL5.set_size(100, 290)
# Set style for screen_bsL5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsL5.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsL5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsL5.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsR5
screen_bsR5 = lv.img(screen_cont_scene5)
screen_bsR5.set_src("B:MicroPython/_blackslide_alpha_30x290.bin")
screen_bsR5.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsR5.set_pivot(50,50)
screen_bsR5.set_angle(0)
screen_bsR5.set_pos(0, -26)
screen_bsR5.set_size(30, 290)
# Set style for screen_bsR5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsR5.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsR5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsR5.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_scene4
screen_cont_scene4 = lv.obj(screen_cont_root)
screen_cont_scene4.set_pos(0, 0)
screen_cont_scene4.set_size(280, 240)
screen_cont_scene4.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_scene4.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_scene4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_scene4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_animimg_4R
screen_animimg_4R = lv.img(screen_cont_scene4)
screen_animimg_4R.set_src("B:MicroPython/_scene3R_alpha_504x240.bin")
screen_animimg_4R.add_flag(lv.obj.FLAG.CLICKABLE)
screen_animimg_4R.set_pivot(50,50)
screen_animimg_4R.set_angle(0)
screen_animimg_4R.set_pos(-224, 0)
screen_animimg_4R.set_size(504, 240)
# Set style for screen_animimg_4R, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_animimg_4R.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_4R.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_4R.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_animimg_4L
screen_animimg_4L = lv.img(screen_cont_scene4)
screen_animimg_4L.set_src("B:MicroPython/_scene3L_alpha_504x240.bin")
screen_animimg_4L.add_flag(lv.obj.FLAG.CLICKABLE)
screen_animimg_4L.set_pivot(50,50)
screen_animimg_4L.set_angle(0)
screen_animimg_4L.set_pos(-224, 0)
screen_animimg_4L.set_size(504, 240)
# Set style for screen_animimg_4L, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_animimg_4L.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_4L.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_4L.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsR4
screen_bsR4 = lv.img(screen_cont_scene4)
screen_bsR4.set_src("B:MicroPython/_blackslide_alpha_150x290.bin")
screen_bsR4.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsR4.set_pivot(50,50)
screen_bsR4.set_angle(0)
screen_bsR4.set_pos(-15, -21)
screen_bsR4.set_size(150, 290)
# Set style for screen_bsR4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsR4.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsR4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsR4.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsL4
screen_bsL4 = lv.img(screen_cont_scene4)
screen_bsL4.set_src("B:MicroPython/_blackslide_alpha_60x290.bin")
screen_bsL4.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsL4.set_pivot(50,50)
screen_bsL4.set_angle(0)
screen_bsL4.set_pos(265, -26)
screen_bsL4.set_size(60, 290)
# Set style for screen_bsL4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsL4.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsL4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsL4.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_scene3
screen_cont_scene3 = lv.obj(screen_cont_root)
screen_cont_scene3.set_pos(0, 0)
screen_cont_scene3.set_size(280, 240)
screen_cont_scene3.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_scene3.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_scene3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_scene3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_animimg_3
screen_animimg_3 = lv.img(screen_cont_scene3)
screen_animimg_3.set_src("B:MicroPython/_scene3_alpha_468x390.bin")
screen_animimg_3.add_flag(lv.obj.FLAG.CLICKABLE)
screen_animimg_3.set_pivot(50,50)
screen_animimg_3.set_angle(0)
screen_animimg_3.set_pos(-48, -13)
screen_animimg_3.set_size(468, 390)
# Set style for screen_animimg_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_animimg_3.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_3.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsR3
screen_bsR3 = lv.img(screen_cont_scene3)
screen_bsR3.set_src("B:MicroPython/_blackslide_alpha_174x300.bin")
screen_bsR3.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsR3.set_pivot(50,50)
screen_bsR3.set_angle(0)
screen_bsR3.set_pos(162, -33)
screen_bsR3.set_size(174, 300)
# Set style for screen_bsR3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsR3.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsR3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsR3.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_bsL3
screen_bsL3 = lv.img(screen_cont_scene3)
screen_bsL3.set_src("B:MicroPython/_blackslide_alpha_57x304.bin")
screen_bsL3.add_flag(lv.obj.FLAG.CLICKABLE)
screen_bsL3.set_pivot(50,50)
screen_bsL3.set_angle(0)
screen_bsL3.set_pos(-24, -33)
screen_bsL3.set_size(57, 304)
# Set style for screen_bsL3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_bsL3.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsL3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_bsL3.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_scene2
screen_cont_scene2 = lv.obj(screen_cont_root)
screen_cont_scene2.set_pos(0, 0)
screen_cont_scene2.set_size(280, 240)
screen_cont_scene2.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_scene2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_scene2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_scene2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_animimg_2
screen_animimg_2 = lv.img(screen_cont_scene2)
screen_animimg_2.set_src("B:MicroPython/_scene2_alpha_450x405.bin")
screen_animimg_2.add_flag(lv.obj.FLAG.CLICKABLE)
screen_animimg_2.set_pivot(50,50)
screen_animimg_2.set_angle(0)
screen_animimg_2.set_pos(-121, -100)
screen_animimg_2.set_size(450, 405)
# Set style for screen_animimg_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_animimg_2.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_animimg_2.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_blackslideRight
screen_blackslideRight = lv.img(screen_cont_scene2)
screen_blackslideRight.set_src("B:MicroPython/_blackslide_alpha_210x411.bin")
screen_blackslideRight.add_flag(lv.obj.FLAG.CLICKABLE)
screen_blackslideRight.set_pivot(50,50)
screen_blackslideRight.set_angle(0)
screen_blackslideRight.set_pos(-123, -95)
screen_blackslideRight.set_size(210, 411)
# Set style for screen_blackslideRight, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_blackslideRight.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideRight.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideRight.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_blackslideLeft
screen_blackslideLeft = lv.img(screen_cont_scene2)
screen_blackslideLeft.set_src("B:MicroPython/_blackslide_alpha_90x405.bin")
screen_blackslideLeft.add_flag(lv.obj.FLAG.CLICKABLE)
screen_blackslideLeft.set_pivot(50,50)
screen_blackslideLeft.set_angle(0)
screen_blackslideLeft.set_pos(248, -94)
screen_blackslideLeft.set_size(90, 405)
# Set style for screen_blackslideLeft, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_blackslideLeft.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideLeft.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideLeft.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_scene1
screen_cont_scene1 = lv.obj(screen_cont_root)
screen_cont_scene1.set_pos(0, 0)
screen_cont_scene1.set_size(280, 240)
screen_cont_scene1.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_scene1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_scene1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_scene1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_scene1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_animimg_1
screen_animimg_1 = lv.animimg(screen_cont_scene1)
screen_animimg_1_imgs = [None]*1
screen_animimg_1_imgs[0] = "B:MicroPython/screen_animimg_1scene1_404x240.bin"
screen_animimg_1.set_src(screen_animimg_1_imgs, 1, False)
screen_animimg_1.set_duration(3*1)
screen_animimg_1.set_repeat_count(lv.ANIM_REPEAT.INFINITE)
screen_animimg_1.set_playback_time(200)
screen_animimg_1.start()
screen_animimg_1.set_pos(-42, 0)
screen_animimg_1.set_size(404, 240)

# Create screen_blackslideTop
screen_blackslideTop = lv.img(screen_cont_scene1)
screen_blackslideTop.set_src("B:MicroPython/_blackslide_alpha_350x83.bin")
screen_blackslideTop.add_flag(lv.obj.FLAG.CLICKABLE)
screen_blackslideTop.set_pivot(20,50)
screen_blackslideTop.set_angle(0)
screen_blackslideTop.set_pos(-18, 157)
screen_blackslideTop.set_size(350, 83)
# Set style for screen_blackslideTop, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_blackslideTop.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideTop.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideTop.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_blackslideBottom
screen_blackslideBottom = lv.img(screen_cont_scene1)
screen_blackslideBottom.set_src("B:MicroPython/_blackslide_alpha_350x50.bin")
screen_blackslideBottom.add_flag(lv.obj.FLAG.CLICKABLE)
screen_blackslideBottom.set_pivot(30,50)
screen_blackslideBottom.set_angle(0)
screen_blackslideBottom.set_pos(-35, 0)
screen_blackslideBottom.set_size(350, 50)
# Set style for screen_blackslideBottom, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_blackslideBottom.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideBottom.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_blackslideBottom.set_style_clip_corner(True, lv.PART.MAIN|lv.STATE.DEFAULT)

screen.update_layout()

# content from custom.py

# Load the default screen
lv.scr_load(screen)

while SDL.check():
    time.sleep_ms(5)

