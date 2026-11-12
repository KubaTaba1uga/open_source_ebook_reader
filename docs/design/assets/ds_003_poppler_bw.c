#include <cairo.h>
#include <poppler.h>
#include <stdio.h>
#include <string.h> // for memset
#include <time.h>

int main(int argc, char *argv[]) {
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  GError *error = NULL;
  gchar *uri = g_filename_to_uri(
      argv[1], NULL, &error);
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

  PopplerPage *page = poppler_document_get_page(doc, atoi(argv[3]));
  if (!page) {
    fprintf(stderr, "Failed to load page.\n");
    g_object_unref(doc);
    return 1;
  }

  double width_pts, height_pts;
  poppler_page_get_size(page, &width_pts, &height_pts);
  
  double scale = 2.0; // Keep it at 72 DPI for simplicity
  int width = (int)(width_pts * scale);
  int height = (int)(height_pts * scale);

  // 1. Create a standard RGB24 surface
  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
  cairo_t *cr = cairo_create(surface);

  // Optional: fill with white background first
  cairo_scale(cr, scale, scale); // <-- key line  
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // 2. Render the PDF onto the RGB surface
  poppler_page_render(page, cr);

  // 3. Manually threshold the pixel data to pure black and white
  // The pixel buffer is in BGRA format on little-endian systems
  cairo_surface_flush(surface);
  unsigned char *data = cairo_image_surface_get_data(surface);
  int stride = cairo_image_surface_get_stride(surface);
  int threshold = 250; // Adjust this value (0-255) to control the cutoff

  for (int y = 0; y < height; y++) {
    unsigned char *row = data + y * stride;
    for (int x = 0; x < width; x++) {
      unsigned char *pixel = row + x * 4;
      // Calculate brightness (simple average, or use luminosity)
      int brightness = (pixel[2] + pixel[1] + pixel[0]) / 3;
      unsigned char bw = (brightness < threshold) ? 0x00 : 0xFF;
      // Set to pure black or white (BGRA format)
      pixel[0] = bw;   // B
      pixel[1] = bw;   // G
      pixel[2] = bw;   // R
      pixel[3] = 0xFF; // Alpha (opaque)
    }
  }
  cairo_surface_mark_dirty(surface);

  clock_gettime(CLOCK_REALTIME, &end);
  long long ns = ((long long)end.tv_sec * 1000000000LL + end.tv_nsec) -
                 ((long long)start.tv_sec * 1000000000LL + start.tv_nsec);
  printf("%.3f ms\n", (double)ns / 1e6);

  // 4. Save the thresholded result
  cairo_status_t status = cairo_surface_write_to_png(surface,argv[2]);
  if (status != CAIRO_STATUS_SUCCESS) {
    fprintf(stderr, "Save PNG failed: %s\n", cairo_status_to_string(status));
  }

  // 5. Cleanup
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  g_object_unref(page);
  g_object_unref(doc);

  return 0;
}
