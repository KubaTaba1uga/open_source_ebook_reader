#include <cairo.h>
#include <getopt.h>
#include <poppler.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void help_print(int argc, char *argv[]);
static int gen_pdf(const char *pdf_path, const char *png_path, int page_no,
                   float scale_no);

int main(int argc, char *argv[]) {
  const struct option long_options[] = {{"help", no_argument, NULL, 'h'},
                                        {"verbose", no_argument, NULL, 'v'},
                                        {"page", required_argument, NULL, 'p'},
                                        {"scale", required_argument, NULL, 's'},
                                        {NULL, 0, NULL, 0}};
  bool verbose = false;
  double scale = 1.0;
  int page = 0;
  int opt;

  if (argc < 2) {
    help_print(argc, argv);
    return 1;
  }

  while ((opt = getopt_long(argc, argv, "hvps:", long_options, NULL)) != -1) {
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
    case 's':
      scale = atof(optarg);
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
    printf("Scale: %f\n", scale);
    printf("Page: %d\n", page);
    
  }

  return gen_pdf(input_pdf, output_png, page, scale);
}

static void help_print(int argc, char *argv[]) {
  printf("Usage: %s [OPTIONS] <IN PDF> <OUT PNG>\n"
         "\n"
         "Description:\n"
         "  Generate PDF page as PNG.\n"
         "\n"
         "Options:\n"
         "  -h, --help           Show this help message\n"
         "  -v, --verbose        Enable verbose output\n"
         "  -p, --page <PAGE>    Specify page\n"
         "  -s, --scale <SCALE>  Specify scale\n"
         "\n"
         "Examples:\n"
         "  %s input.pdf output.png\n"
         "  %s -v input.pdf output.png\n",
         argv[0], argv[0], argv[0]);
}

static int gen_pdf(const char *pdf_path, const char *png_path, int page_no,
                   float scale_no) {
  // 2. Open PDF
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

  // 3. Load first page
  PopplerPage *page = poppler_document_get_page(doc, page_no);
  if (!page) {
    fprintf(stderr, "Failed to load page.\n");
    g_object_unref(doc);
    return 1;
  }

  // 4. Get page dimensions (in points, 72 DPI)
  double width_pts, height_pts;
  poppler_page_get_size(page, &width_pts, &height_pts);

  int width = (int)(width_pts * scale_no);
  int height = (int)(height_pts * scale_no);

  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
  cairo_t *cr = cairo_create(surface);

  cairo_scale(cr, scale_no, scale_no);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

cairo_font_options_t *options = cairo_font_options_create();
cairo_font_options_set_antialias(options, CAIRO_ANTIALIAS_GRAY);
cairo_set_font_options(cr, options);
cairo_font_options_destroy(options);
  /* poppler_page_set_render_antialias(page, CAIRO_ANTIALIAS_GRAY);   */
  poppler_page_render(page,
                      cr); // Poppler draws in points, Cairo scales to pixels

  clock_gettime(CLOCK_REALTIME, &end);
  long long ns = ((long long)end.tv_sec * 1000000000LL + end.tv_nsec) -
                 ((long long)start.tv_sec * 1000000000LL + start.tv_nsec);
  printf("%.3f ms\n", (double)ns / 1e6);

  // 6. Save as PNG
  // No need for cairo_surface_flush/mark_dirty: Cairo itself drew via the
  // context, so the surface is already in a consistent state.
  cairo_status_t status = cairo_surface_write_to_png(surface, png_path);
  if (status != CAIRO_STATUS_SUCCESS) {
    fprintf(stderr, "Save PNG failed: %s\n", cairo_status_to_string(status));
  }

  // 7. Cleanup
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  g_object_unref(page);
  g_object_unref(doc);

  return 0;
}
