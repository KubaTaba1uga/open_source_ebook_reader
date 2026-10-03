#include <lvgl.h>
#include <stdio.h>

#include "display/core.h"
#include "display/display.h"
#include "event_queue/event_queue.h"
#include "src/display/lv_display.h"
#include "src/misc/lv_event.h"
#include "utils/err.h"
#include "utils/log.h"
#include "utils/mem.h"

static const int display_heigth = 1872;
static const int display_width = 1404; // Display is 1404 but lvgl in
                                       // i1 needs byte aligned values
                                       // 1404 % 8 != 0

static void event_cb(lv_event_t *lvgl_event);

err_t display_init(display_t *out, event_queue_t queue) {
  display_t display = *out = mem_malloc(sizeof(struct Display));
  *display = (struct Display){
      .ev_queue = queue,
  };

#if EBK_DISPLAY_IT8951
  err_o = display_it8951_init(display, display_width, display_heigth);
  ERR_TRY(err_o);
#elif EBK_DISPLAY_X11
  err_o = display_x11_init(display, display_width, display_heigth);
  ERR_TRY(err_o);
#elif EBK_DISPLAY_PNG
  err_o = display_png_init(display, display_width, display_heigth);
  ERR_TRY(err_o);
#else
#error "No display selected!"
#endif

  lv_display_set_user_data(display->lv_disp, display);
  lv_display_add_event_cb(display->lv_disp, event_cb, LV_EVENT_ALL, display);

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
  return display_width;
};
int display_get_y(display_t display) {
  return display_heigth;
}

void display_set_trace(display_t display) {
  display->trace = trace_start("render display");
};

void display_panic(display_t display) {
  if (display->private.panic) {
    display->private.panic(display);
  }
}

static void event_cb(lv_event_t *lvgl_event) {
  display_t display = lv_event_get_user_data(lvgl_event);

  if (lv_event_get_code(lvgl_event) == LV_EVENT_FLUSH_FINISH) {
    event_queue_push(display->ev_queue, Events_DISPLAY_RENDERED, NULL);
  }
}
