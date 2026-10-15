/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2026 Jakub Buczynski <KubaTaba1uga>
 */
#ifndef EBOOK_READER_READER_CORE_H
#define EBOOK_READER_READER_CORE_H
#include <stdbool.h>


#include "utils/err.h"
#include "utils/lvgl.h"

typedef lvgl_obj_t wdgt_power_t;
typedef struct _cairo_surface cairo_surface_t;

struct PowerView {
  wdgt_power_t screen;
  cairo_surface_t *screen_image;  
};

err_t power_off_view_init(struct PowerView *view);
void power_off_view_destroy(struct PowerView *view);

err_t wdgt_power_init(wdgt_power_t *out, const unsigned char *power_screen_data,
                     int power_screen_size);
void wdgt_power_destroy(wdgt_power_t *out);

#endif // EBOOK_READER_READER_CORE_H
