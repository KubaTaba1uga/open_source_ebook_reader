#include <lvgl.h>
#include <stdio.h>
#include <stdlib.h>

#include "power/core.h"
#include "src/misc/lv_event.h"
#include "utils/err.h"
#include "utils/graphic.h"
#include "utils/log.h"
#include "utils/lvgl.h"
#include "utils/mem.h"

static void event_cb(lvgl_event_t lvgl_event);

err_t wdgt_power_init(wdgt_power_t *out, const unsigned char *power_screen_data,
                      int power_screen_size) {
  wdgt_power_t power_screen = *out = lvgl_img_create(lv_screen_active());

  lv_img_dsc_t *dsc = mem_malloc(sizeof(lv_img_dsc_t));
  *dsc = (lv_img_dsc_t){0};
  dsc->header.cf = lvgl_color_format;
  dsc->header.w = lv_display_get_horizontal_resolution(NULL);
  dsc->header.h = lv_display_get_vertical_resolution(NULL);
  dsc->data_size = power_screen_size;
  dsc->data = power_screen_data;
  lv_image_set_src(power_screen, dsc);
  lv_obj_set_user_data(power_screen, dsc);

  lv_obj_add_event_cb(power_screen, event_cb, LV_EVENT_ALL, NULL);

  return 0;
}

void wdgt_power_destroy(wdgt_power_t *out) {
  if (mem_is_null_ptr(out)) {
    return;
  }

  lv_img_dsc_t *dsc = lv_obj_get_user_data(*out);
  mem_free(dsc);
  lv_obj_del(*out);
  *out = NULL;
}

static void event_cb(lvgl_event_t lvgl_event) {

  log_info("%s: %s", __func__,
           lv_event_code_get_name(lv_event_get_code(lvgl_event)));

  /* if (lv_event_get_code(lvgl_event) == LV_EVENT_DRAW_POST_END){ */
    /* exit(0); */
    /* }   */
}
