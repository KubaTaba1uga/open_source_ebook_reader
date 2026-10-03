#include "display/display.h"
#include "display/core.h"
#include "src/display/lv_display.h"
#include "utils/err.h"
#include "utils/mem.h"
#include <stdio.h>

static const int display_heigth = 1872;
static const int display_width = 1404; // Display is 1404 but lvgl in
                                       // i1 needs byte aligned values
                                       // 1404 % 8 != 0

err_t display_init(display_t *out) {
  display_t display = *out = mem_malloc(sizeof(struct Display));
  *display = (struct Display){0};
  
  puts(__func__);
#if EBK_DISPLAY_IT8951
  err_o = display_it8951_init(display, display_width, display_heigth);
  ERR_TRY(err_o);
#elif EBK_DISPLAY_X11
  err_o = display_x11_init(display, display_width, display_heigth);
  ERR_TRY(err_o);
#elif EBK_DISPLAY_PNG
  err_o = display_png_init(display);
  ERR_TRY(err_o);
#else
#error "No display selected!"
#endif

  lv_display_set_user_data(display->lv_disp, display);
  
  return 0;

error_out:
  *out = NULL;
  return err_o;
}

void display_destroy(display_t *out) {
  if (mem_is_null_ptr(out)) {
    return;
  }

  display_t display = *out;
  
  if (display->private.destroy && display->private.data) {
    display->private.destroy(display->private.data);
  }

  if (display->render.buf) {
    mem_free(display->render.buf);
  }

  if (display->lv_ingroup) {
    lv_group_delete(display->lv_ingroup);
  }

  if (display->lv_disp) {
    lv_display_delete(display->lv_disp);
  }

  mem_free(*out);
  *out = NULL;
}

void display_add_to_ingroup(display_t display, void *wx) {
  lv_group_add_obj(display->lv_ingroup, wx);
}

void display_del_from_ingroup(display_t _, void *wx) {
  lv_group_remove_obj(wx);
}

int display_get_x(display_t display) {
  return lv_display_get_horizontal_resolution(NULL);
};
int display_get_y(display_t display) {
  return lv_display_get_vertical_resolution(NULL);
}

void display_set_trace(display_t display) {
  display->trace = trace_start("render display");
};

void display_panic(display_t display) {
  if (display->private.panic) {
    display->private.panic(display);
  }
}
