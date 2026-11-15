#include "fpdfview.h"
#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  // 1. Initialize PDFium
  FPDF_InitLibrary();

  // 2. Open PDF
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  FPDF_DOCUMENT doc = FPDF_LoadDocument(
      "/mnt/sdcard/mistakes_were_made_but_not_by_me.pdf", NULL);
  if (!doc) {
    fprintf(stderr, "Failed to load PDF.\n");
    FPDF_DestroyLibrary();
    return 1;
  }

  // 3. Load first page
  FPDF_PAGE page = FPDF_LoadPage(doc, 0);
  if (!page) {
    fprintf(stderr, "Failed to load page.\n");
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();
    return 1;
  }

  // 4. Get page dimensions and render to PDFium bitmap
  double scale = 2.0;
  int width = (int)(FPDF_GetPageWidth(page) * scale);
  int height = (int)(FPDF_GetPageHeight(page) * scale);

  FPDF_BITMAP bitmap = FPDFBitmap_Create(width, height, 0);
  FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
  FPDF_RenderPageBitmap(bitmap, page, 0, 0, width, height, 0, 0);

  /* int width = (int)FPDF_GetPageWidth(page); */
  /* int height = (int)FPDF_GetPageHeight(page); */
  /* FPDF_BITMAP bitmap = FPDFBitmap_Create(width, height, 0); */
  /* FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF); */
  /* FPDF_RenderPageBitmap(bitmap, page, 0, 0, width, height, 0, 0); */

  // 5. Get PDFium pixel buffer (BGRA format)
  unsigned char *pdf_buffer = FPDFBitmap_GetBuffer(bitmap);
  int stride = FPDFBitmap_GetStride(bitmap);

  // 6. Create Cairo image surface and copy pixels directly
  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);

  // Must flush before writing pixel data directly
  cairo_surface_flush(surface);

  unsigned char *cairo_data = cairo_image_surface_get_data(surface);
  int cairo_stride = cairo_image_surface_get_stride(surface);

  // Copy row by row (Cairo RGB24 on little-endian is BGRA byte order, matching
  // PDFium)
  for (int y = 0; y < height; y++) {
    memcpy(cairo_data + y * cairo_stride, pdf_buffer + y * stride, width * 4);
  }

  // Notify Cairo that data has been modified
  cairo_surface_mark_dirty(surface);

  clock_gettime(CLOCK_REALTIME, &end);
  long long ns = ((long long)end.tv_sec * 1000000000LL + end.tv_nsec) -
                 ((long long)start.tv_sec * 1000000000LL + start.tv_nsec);
  printf("%.3f ms\n", (double)ns / 1e6);

  // 7. Save as PNG
  cairo_status_t status = cairo_surface_write_to_png(surface, "output.png");
  if (status != CAIRO_STATUS_SUCCESS) {
    fprintf(stderr, "Save PNG failed: %s\n", cairo_status_to_string(status));
  }

  return 0;
}
