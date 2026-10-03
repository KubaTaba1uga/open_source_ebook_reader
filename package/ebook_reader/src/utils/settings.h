/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2026 Jakub Buczynski <KubaTaba1uga>
 */
#ifndef SETTINGS_H
#define SETTINGS_H

enum DisplayModelEnum {
  DisplayModelEnum_X11 = 0,
  DisplayModelEnum_PNG,
  DisplayModelEnum_IT8951,
  DisplayModelEnum_MAX,
};

extern const enum DisplayModelEnum settings_display_model;
extern const char *settings_boot_screen_path;
extern const char *settings_input_path;
extern const char *settings_books_dir;
extern const char *settings_db_path;

#endif // SETTINGS_H
