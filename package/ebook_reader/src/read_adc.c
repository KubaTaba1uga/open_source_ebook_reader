#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

float power_battery_monitor_adc_read(const char *raw_file,
                                     const char *scale_file,
                                     const char *offset_file, bool verbose);
static void help_print(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  static const struct option long_options[] = {
      {"help", no_argument, NULL, 'h'},
      {"verbose", no_argument, NULL, 'v'},
      {NULL, 0, NULL, 0}};
  const char *offset_file;
  const char *scale_file;
  const char *raw_file;
  bool verbose = false;
  float adc_value;
  int opt;

  while ((opt = getopt_long(argc, argv, "hv:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'h':
      help_print(argc, argv);
      return 0;

    case 'v':
      verbose = true;
      break;

    default:
      return 1;
    }
  }

  if ((optind + 3) > argc) {
    help_print(argc, argv);
    return 1;
  }

  raw_file = argv[optind];
  scale_file = argv[optind + 1];
  offset_file = argv[optind + 2];

  adc_value = power_battery_monitor_adc_read(raw_file, scale_file, offset_file, verbose);
  if (adc_value == -1) {
    return EXIT_FAILURE;
  }

  fprintf(stdout, "%g\n", adc_value);
  fflush(stderr);
  fflush(stdout);

  return EXIT_SUCCESS;
}

static void help_print(int argc, char *argv[]) {
  printf("Usage: %s [OPTIONS] <RAW_FILE> <SCALE_FILE> <OFFSET_FILE>\n"
         "\n"
         "Description:\n"
         "  Read ADC value.\n"
         "\n"
         "Options:\n"
         "  -h, --help           Show this help message\n"
         "\n"
         "Examples:\n"
         "  %s in_voltage6_raw in_voltage_scale in_voltage_offset\n"
         "  %s -h\n",
         argv[0], argv[0], argv[0]);
}

