/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2026 Jakub Buczynski <KubaTaba1uga>
 */
#ifndef EBOOK_READER_DISPLAY_CORE_H
#define EBOOK_READER_DISPLAY_CORE_H
#include <lvgl.h>

#include "display/display.h"
#include "event_queue/event_queue.h"
#include "utils/time.h"

struct Display {
  const char *id;
  lv_group_t *lv_ingroup;
  lv_display_t *lv_disp;
  struct {
    unsigned char *buf;
    int len;
  } render;
  struct Trace trace;
  struct {
    void *data;
    void (*destroy)(void *data);
    void (*panic)(display_t self);
  } private;
  event_queue_t ev_queue;
};

err_t display_it8951_init(display_t display, int x, int y);
err_t display_x11_init(display_t display, int x, int y);
err_t display_png_init(display_t display, int x, int y);

#endif
