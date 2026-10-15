/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2026 Jakub Buczynski <KubaTaba1uga>
 */
#include <lvgl.h>

#include "display/core.h"
#include "display/display.h"
#include "src/display/lv_display.h"
#include "utils/mem.h"
#include "utils/time.h"

err_t display_x11_init(display_t display, int x, int y) {
  lv_display_t *lv_display = lv_x11_window_create("ebook_reader", x, y);
  if (!lv_display) {
    err_o = err_errnos(errno, "Cannot initialize X11 display");
    goto error_out;
  }
  lv_x11_inputs_create(lv_display, NULL);

  display->lv_disp = lv_display;
  display->lv_ingroup = lv_group_get_default();

  return 0;

error_out:
  mem_free(display);
  return err_o;
}
