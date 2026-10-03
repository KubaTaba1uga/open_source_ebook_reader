#include <cairo.h>

#include "core.h"
#include "power/core.h"
#include "utils/err.h"
#include "utils/graphic.h"
#include "utils/settings.h"

err_t power_off_view_init(struct PowerView *view) {
  cairo_surface_t *cairo_surface =
      cairo_image_surface_create_from_png(settings_boot_screen_path);
  if (!cairo_surface) {
    err_o = err_errnof(EINVAL, "Cannot load png from %s",
                       settings_boot_screen_path);
    goto error_out;
  }

  uint8_t *buf = cairo_image_surface_get_data(cairo_surface);
  wdgt_power_t power = NULL;
  err_o = wdgt_power_init(&power, buf,
                          graphic_calc_screen_buf_size(
                              cairo_image_surface_get_width(cairo_surface),
                              cairo_image_surface_get_height(cairo_surface)));
  ERR_TRY(err_o);

  view->screen = power;
  view->screen_image = cairo_surface;

  return 0;

error_out:
  return err_o;
}

void power_off_view_destroy(struct PowerView *view) {
  wdgt_power_destroy(&view->screen);
  if (view->screen_image) {
    cairo_surface_destroy(view->screen_image);
    view->screen_image = NULL;
  }
}
