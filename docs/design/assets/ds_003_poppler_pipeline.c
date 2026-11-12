/*
  Compile:
  export PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig/:$PKG_CONFIG_PATH
  gcc docs/design/assets/ds_003_poppler_pipeline.c `MagickWand-config --cflags
  --cppflags`  $(pkg-config --cflags --libs poppler-glib cairo glib-2.0 gio-2.0)
  `MagickWand-config --ldflags --libs` -o pdf_poppler

  Cross compile:
  source build/buildroot/host/environment-setup
  arm-linux-gcc -O3 -pipe -mcpu=cortex-a7 -mfpu=neon -mfloat-abi=hard -mthumb ~/Github/open_source_ebook_reader/docs/design/assets/ds_003_poppler_pipeline.c  $(pkg-config --cflags --libs poppler-glib cairo glib-2.0 gio-2.0 MagickWand) -o pdf_poppler

*/
#include <cairo.h>
#include <getopt.h>
#include <poppler.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <MagickWand/MagickWand.h>

static void help_print(int argc, char *argv[]);
static int gen_pdf(const char *pdf_path, const char *png_path, int page_no,
                   float upscale_no, int thresh_x, int thresh_y);

int main(int argc, char *argv[]) {
  const struct option long_options[] = {
      {"help", no_argument, NULL, 'h'},
      {"verbose", no_argument, NULL, 'v'},
      {"page", required_argument, NULL, 'p'},
      {"upscale", required_argument, NULL, 'u'},
      {"thresh_x", required_argument, NULL, 'x'},
      {"thresh_y", required_argument, NULL, 'y'},
      {NULL, 0, NULL, 0}};
  double upscale = 2.0;
  bool verbose = false;
  int thresh_x = 25;
  int thresh_y = 50;
  int page = 0;
  int opt;

  if (argc < 2) {
    help_print(argc, argv);
    return 1;
  }

  while ((opt = getopt_long(argc, argv, "hvpuxy:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'h':
      help_print(argc, argv);
      return 0;
    case 'v':
      verbose = true;
      break;
    case 'p':
      page = atoi(optarg);
      break;
    case 'u':
      upscale = atof(optarg);
      break;
    case 'x':
      thresh_x = atoi(optarg);
      break;
    case 'y':
      thresh_y = atoi(optarg);
      break;
    default:
      return 1;
    }
  }

  if (optind >= argc - 1) {
    fprintf(stderr, "Missing input pdf and output png\n");
    return 1;
  }

  const char *input_pdf = argv[optind];
  const char *output_png = argv[optind + 1];

  if (verbose) {
    printf("Input: %s\n", input_pdf);
    printf("Output: %s\n", output_png);
    printf("Verbose: %s\n", verbose ? "yes" : "no");
    printf("Thresh: x=%d,y=%d\n", thresh_x, thresh_y);
    printf("Upscale: %f\n", upscale);
    printf("Page: %d\n", page);
  }

  return gen_pdf(input_pdf, output_png, page, upscale, thresh_x, thresh_y);
}

static void help_print(int argc, char *argv[]) {
  printf("Usage: %s [OPTIONS] <IN PDF> <OUT PNG>\n"
         "\n"
         "Description:\n"
         "  Generate PDF page as PNG.\n"
         "\n"
         "Options:\n"
         "  -h, --help             Show this help message\n"
         "  -v, --verbose          Enable verbose output\n"
         "  -p, --page <PAGE>      Specify page\n"
         "  -s, --scale <SCALE>    Specify scale\n"
         "  -u, --upscale <SCALE>  Specify up scale\n"
         "\n"
         "Examples:\n"
         "  %s input.pdf output.png\n"
         "  %s -v input.pdf output.png\n",
         argv[0], argv[0], argv[0]);
}

static int gen_pdf(const char *pdf_path, const char *png_path, int page_no,
                   float upscale_no, int thresh_x, int thresh_y) {
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  GError *error = NULL;
  gchar *uri = g_filename_to_uri(pdf_path, NULL, &error);
  if (!uri) {
    fprintf(stderr, "Failed to create URI: %s\n", error->message);
    g_error_free(error);
    return 1;
  }

  PopplerDocument *doc = poppler_document_new_from_file(uri, NULL, &error);
  g_free(uri);
  if (!doc) {
    fprintf(stderr, "Failed to load PDF: %s\n", error->message);
    g_error_free(error);
    return 1;
  }

  PopplerPage *page = poppler_document_get_page(doc, page_no);
  if (!page) {
    fprintf(stderr, "Failed to load page.\n");
    g_object_unref(doc);
    return 1;
  }

  // UPSCALE
  double width_pts, height_pts;
  poppler_page_get_size(page, &width_pts, &height_pts);

  int width = (int)(width_pts * upscale_no);
  int height = (int)(height_pts * upscale_no);

  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
  cairo_t *cr = cairo_create(surface);

  cairo_scale(cr, upscale_no, upscale_no);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  poppler_page_render(page, cr);
  int stride = cairo_image_surface_get_stride(surface);
  unsigned char *cairo_data = cairo_image_surface_get_data(surface);

  // CONVERT BW
  unsigned char *rgb_buffer = malloc(width * height * 3);

  for (int y = 0; y < height; y++) {
    unsigned char *src = cairo_data + y * stride;
    unsigned char *dst = rgb_buffer + y * width * 3;
    for (int x = 0; x < width; x++) {
      dst[x * 3 + 0] = src[x * 4 + 2]; // R
      dst[x * 3 + 1] = src[x * 4 + 1]; // G
      dst[x * 3 + 2] = src[x * 4 + 0]; // B
    }
  }

  MagickWand *wand = NewMagickWand();

  MagickConstituteImage(wand, width, height, "RGB", CharPixel, rgb_buffer);
  MagickAdaptiveThresholdImage(wand, thresh_x, thresh_y, -50);
  MagickAutoThresholdImage(wand, OTSUThresholdMethod);

  clock_gettime(CLOCK_REALTIME, &end);
  long long ns = ((long long)end.tv_sec * 1000000000LL + end.tv_nsec) -
                 ((long long)start.tv_sec * 1000000000LL + start.tv_nsec);
  printf("%.3f ms\n", (double)ns / 1e6);

  MagickWriteImage(wand, png_path);

  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  g_object_unref(page);
  g_object_unref(doc);
  wand = DestroyMagickWand(wand);
  MagickWandTerminus();

  free(rgb_buffer);

  return 0;
}
