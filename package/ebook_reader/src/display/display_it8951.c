/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2026 Jakub Buczynski <KubaTaba1uga>
 */
#include <EPD_IT8951.h>
#include <assert.h>
#include <cairo.h>
#include <errno.h>
#include <lvgl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "DEV_Config.h"
#include "display/core.h"
#include "display/display.h"
#include "utils/err.h"
#include "utils/graphic.h"
#include "utils/log.h"
#include "utils/mem.h"
#include "utils/settings.h"
#include "utils/time.h"

struct DisplayIT8951 {
  IT8951_Dev_Info dev_info;
  UDOUBLE init_mem_addr;
};

static void display_it8951_flush_callback(lv_display_t *display,
                                          const lv_area_t *area,
                                          uint8_t *px_map);
static void display_it8951_destroy(void *data);
static void display_it8951_panic(display_t display);

err_t display_it8951_init(display_t display, int x, int y) {
  display->lv_disp = lv_display_create(x, y);
  if (!display->lv_disp) {
    err_o = err_errnos(EINVAL, "Cannot create LVGL display");
    goto error_out;
  }

  struct DisplayIT8951 *it8951 = mem_malloc(sizeof(struct DisplayIT8951));
  *it8951 = (struct DisplayIT8951){0};
  display->private.data = it8951;
  display->private.destroy = display_it8951_destroy;
  display->private.panic = display_it8951_panic;

  if (DEV_Module_Init() != 0) {
    err_o = err_errnos(EINVAL, "Cannot initialize ET8951 driver");
    goto error_it8951_cleanup;
  }

  // Configure IT8951
  UWORD VCOM = (UWORD)(fabs(-1.50) * 1000);
  it8951->dev_info = EPD_IT8951_Init(VCOM);
  it8951->init_mem_addr =
      it8951->dev_info.Memory_Addr_L | (it8951->dev_info.Memory_Addr_H << 16);

  printf("it8951->dev_info.Panel_W=%d\n", it8951->dev_info.Panel_W);
  printf("it8951->dev_info.Panel_H=%d\n", it8951->dev_info.Panel_H);
  printf("ui_display_it8951_width=%d\n", x);
  printf("ui_display_it8951_heigth=%d\n", y);

  EPD_IT8951_Clear_Refresh(it8951->dev_info, it8951->init_mem_addr, INIT_Mode);
  
  // Configure LVGL
  lv_indev_t *lv_indev =
      lv_evdev_create(LV_INDEV_TYPE_KEYPAD, settings_input_path);
  lv_indev_set_display(lv_indev, display->lv_disp);
  display->lv_ingroup = lv_group_create();
  lv_indev_set_group(lv_indev, display->lv_ingroup);

  display->render.len =
      (it8951->dev_info.Panel_W * it8951->dev_info.Panel_H * 4);
  display->render.buf = mem_malloc(display->render.len);

  lv_display_set_color_format(display->lv_disp, lvgl_color_format);
  lv_display_set_flush_cb(display->lv_disp, display_it8951_flush_callback);
  lv_display_set_buffers(display->lv_disp, display->render.buf, NULL,
                         display->render.len, LV_DISPLAY_RENDER_MODE_FULL);

  return 0;

error_it8951_cleanup:
  lv_display_delete(display->lv_disp);  
  mem_free(it8951);
  memset(&display->private, 0, sizeof(display->private));
error_out:
  return err_o;
}

static void display_it8951_destroy(void *data) {
  if (!data) {
    return;
  }

  DEV_Module_Exit();
  mem_free(data);
}

unsigned char *dd_wvs75v2b_rotate(int width, int heigth, unsigned char *buf,
                                  int buf_len) {
  unsigned char *dst = calloc(buf_len, 1);
  for (int y = 0; y < heigth; y++) {
    int src_row = y * width;
    for (int x = 0; x < width; x++) {
      int src_bit = src_row + (width - 1 - x);
      int dst_bit = x * heigth + y;
      int v = (buf[src_bit >> 3] >> (src_bit & 7)) & 1;
      if (v) {
        dst[dst_bit >> 3] |= (1 << (dst_bit & 7));
      }
    }
  }

  return dst;
}

static void display_it8951_refresh(UBYTE *Frame_Buf, UWORD X, UWORD Y, UWORD W,
                                   UWORD H, UBYTE Mode,
                                   UDOUBLE Target_Memory_Addr,
                                   bool Packed_Write) {
  EPD_IT8951_1bp_Refresh(Frame_Buf, X, Y, W, H, GC16_Mode, Target_Memory_Addr,
                         Packed_Write);
}

static void display_it8951_flush_callback(lv_display_t *display,
                                          const lv_area_t *area,
                                          uint8_t *px_map) {
  struct Display *mydisp = lv_display_get_user_data(display);
  struct DisplayIT8951 *it8951 = mydisp->private.data;

  uint8_t *dst = graphic_argb32_to_i1(
      it8951->dev_info.Panel_W, it8951->dev_info.Panel_H, px_map,
      cairo_format_stride_for_width(cairo_color_format,
                                    it8951->dev_info.Panel_W));

  unsigned char *final = dd_wvs75v2b_rotate(
      it8951->dev_info.Panel_H, it8951->dev_info.Panel_W, dst,
      it8951->dev_info.Panel_W * it8951->dev_info.Panel_H / 8);

  display_it8951_refresh(final, 0, 0, it8951->dev_info.Panel_W,
                         it8951->dev_info.Panel_H, GC16_Mode,
                         it8951->init_mem_addr, true);

  free(dst);
  free(final);
  lv_display_flush_ready(display);  
}

static void display_it8951_panic(display_t display) {
  EPD_IT8951_Reset();
  EPD_IT8951_Sleep();
}
