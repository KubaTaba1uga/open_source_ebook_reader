/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2026 Jakub Buczynski <KubaTaba1uga>
 */
#include <assert.h>
#include <cairo.h>
#include <lvgl.h>
#include <stdio.h>

#include "display/core.h"
#include "display/display.h"
#include "utils/err.h"
#include "utils/graphic.h"
#include "utils/mem.h"
#include "utils/settings.h"
#include "utils/time.h"
#include "utils/log.h"

static void display_flush_callback(lv_display_t *display, const lv_area_t *area,
                                   uint8_t *px_map);

err_t display_png_init(display_t display, int x, int y) {
  lv_tick_set_cb(time_now);

  display->lv_disp = lv_display_create(x, y);
  if (!display->lv_disp) {
    goto error_out;
  }

  // Configure LVGL
  lv_indev_t *lv_indev =
      lv_evdev_create(LV_INDEV_TYPE_KEYPAD, settings_input_path);
  lv_indev_set_display(lv_indev, display->lv_disp);
  display->lv_ingroup = lv_group_create();
  lv_indev_set_group(lv_indev, display->lv_ingroup);

  display->render.len =
      (cairo_format_stride_for_width(cairo_color_format, x) * y);
  display->render.buf = mem_malloc(display->render.len);

  lv_display_set_color_format(display->lv_disp, lvgl_color_format);
  lv_display_set_user_data(display->lv_disp, display);
  lv_display_set_flush_cb(display->lv_disp, display_flush_callback);
  lv_display_set_buffers(display->lv_disp, display->render.buf, NULL,
                         display->render.len, LV_DISPLAY_RENDER_MODE_FULL);
  return 0;

error_out:
  mem_free(display);
  return err_o;
}

static void display_flush_callback(lv_display_t *display, const lv_area_t *area,
                                   uint8_t *px_map) {
  struct Display *mydisp = lv_display_get_user_data(display);
  trace_end(&mydisp->trace);

  char buf[128];
  static int i = 0;
  snprintf(buf, sizeof(buf), "screenshot_%d.png", i);
  i++;

  cairo_surface_t *cairo_surface = cairo_image_surface_create_for_data(
      px_map, cairo_color_format, display_get_x(mydisp), display_get_y(mydisp),
      cairo_format_stride_for_width(cairo_color_format, display_get_x(mydisp)));
  assert(cairo_surface != NULL);

  cairo_status_t cairo_status = cairo_surface_write_to_png(cairo_surface, buf);
  log_info("%s", cairo_status_to_string(cairo_status));

  lv_display_flush_ready(display);
}
