/**
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2026 Jakub Buczynski <KubaTaba1uga>
 */

/**
  Settings are specified at compilation time. Each setting has corresponding
  definition, if definition is not set default value is used.

  Some settings depend on each other, for example boot screen image depends
  on the display model, because different displays support different resolutions
  and colors so we need different pictures to make every possible configuration
  look beautiful. If you need such dependent setting use preprocessor directives
  to set appropriate value.

  DISPLAY_MODEL display model used with a device instance.
  DISPLAY_BOOT_SCREEN_PATH path to image displayed during boot.
 */

#include "settings.h"

#if EBK_DISPLAY_X11
#define EBK_DISPLAY_MODEL DisplayModelEnum_X11
#define EBK_DISPLAY_BOOT_SCREEN_PATH                                           \
  "/home/taba1uga/Github/open_source_ebook_reader/package/ebook_reader/data/"  \
  "poweroff_screen.png"
#elif EBK_DISPLAY_PNG
#define EBK_DISPLAY_MODEL DisplayModelEnum_PNG
#define EBK_DISPLAY_BOOT_SCREEN_PATH "/usr/assets/data/poweroff_screen.png"
#elif EBK_DISPLAY_IT8951
#define EBK_DISPLAY_MODEL DisplayModelEnum_IT8951
#define EBK_DISPLAY_BOOT_SCREEN_PATH "/usr/assets/data/poweroff_screen.png"
#else
#error "Unsupported display model"
#endif

#if !EBK_DB_PATH
#define EBK_DB_PATH "ebk.db"
#endif

const enum DisplayModelEnum settings_display_model = EBK_DISPLAY_MODEL;
const char *settings_boot_screen_path = EBK_DISPLAY_BOOT_SCREEN_PATH;
const char *settings_books_dir = "/mnt/sdcard";
const char *settings_input_path = "/dev/input/event0";
const char *settings_db_path = EBK_DB_PATH;
