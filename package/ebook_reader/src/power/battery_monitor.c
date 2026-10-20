#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "power/core.h"
#include "src/lv_api_map_v8.h"
#include "utils/log.h"
#include "utils/settings.h"

float power_battery_monitor_adc_read(const char *raw_file,
                                     const char *scale_file,
                                     const char *offset_file, bool verbose);
static int power_battery_monitor_value_read(void);
static void power_battery_monitor_timer(lv_timer_t *timer);

err_t power_battery_monitor_init(struct PowerBatteryMonitor *battery_monitor,
                                 void (*timer_cb)(int battery_pct, void *data),
                                 void *timer_data) {
  battery_monitor->timer_cb = timer_cb;
  battery_monitor->timer_data = timer_data;
  battery_monitor->timer =
      lv_timer_create(power_battery_monitor_timer, 60 * 1000, battery_monitor);

  int battery_pct = power_battery_monitor_value_read();
  log_info("Battery = %d%%", battery_pct);

  timer_cb(battery_pct, timer_data);

  return 0;
}

void power_battery_monitor_destroy(
    struct PowerBatteryMonitor *battery_monitor) {
  if (battery_monitor->timer) {
    lv_timer_del(battery_monitor->timer);
  }

  *battery_monitor = (struct PowerBatteryMonitor){0};
}

static int power_battery_monitor_value_read(void) {
  float adc_value = power_battery_monitor_adc_read(
      settings_adc_raw_path, settings_adc_scale_path, settings_adc_offset_path,
      false);
  adc_value -= 0.14;
  if (adc_value < 0) {
    adc_value = 0;
  }

  return (adc_value / 1.21) * 100;
}

float power_battery_monitor_adc_read(const char *raw_file,
                                     const char *scale_file,
                                     const char *offset_file, bool verbose) {
  char buf[128] = {0};
  size_t ret_code;
  FILE *offset_fp;
  FILE *scale_fp;
  FILE *raw_fp;
  float scale;
  int offset;
  int raw;

  raw_fp = fopen(raw_file, "r");
  if (!raw_fp) {
    goto error_out;
  }

  ret_code = fread(buf, 1, sizeof(buf) - 1, raw_fp);
  if (ferror(raw_fp)) {
    (void)ret_code;
    fclose(raw_fp);
    goto error_out;
  }

  raw = atoi(buf);
  fclose(raw_fp);
  memset(buf, 0, sizeof(buf));

  scale_fp = fopen(scale_file, "r");
  if (!scale_fp) {
    goto error_out;
  }

  ret_code = fread(buf, 1, sizeof(buf) - 1, scale_fp);
  if (ferror(scale_fp)) {
    (void)ret_code;
    fclose(scale_fp);
    goto error_out;
  }

  scale = atof(buf);
  fclose(scale_fp);
  memset(buf, 0, sizeof(buf));

  offset_fp = fopen(offset_file, "r");
  if (!offset_fp) {
    puts(offset_file);
    goto error_out;
  }

  ret_code = fread(buf, 1, sizeof(buf) - 1, offset_fp);
  if (ferror(offset_fp)) {
    (void)ret_code;
    fclose(offset_fp);
    goto error_out;
  }

  offset = atoi(buf);
  fclose(offset_fp);
  memset(buf, 0, sizeof(buf));

  if (verbose) {
    fprintf(stderr, "(raw + offset) * scale = (%d + %d) * %g = %g\n", raw,
            offset, scale, (float)(raw + offset) * scale);
  }

  return ((float)(raw + offset) * scale) / 1000;

error_out:
  return -1;
}

static void power_battery_monitor_timer(lv_timer_t *timer) {
  struct PowerBatteryMonitor *battery_monitor = lv_timer_get_user_data(timer);

  int battery_pct = power_battery_monitor_value_read();
  
  battery_monitor->timer_cb(battery_pct, battery_monitor->timer_data);
}
