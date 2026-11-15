#include <mupdf/fitz.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
  // 1. Create MuPDF context (holds exception stack and caches)
  fz_context *ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
  if (!ctx) {
    fprintf(stderr, "Failed to create MuPDF context.\n");
    return 1;
  }

  // Register default document handlers
  fz_try(ctx) fz_register_document_handlers(ctx);
  fz_catch(ctx) {
    fprintf(stderr, "Failed to register handlers: %s\n",
            fz_caught_message(ctx));
    fz_drop_context(ctx);
    return 1;
  }

  // 2. Open PDF
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  fz_document *doc = NULL;
  fz_try(ctx) doc =
      fz_open_document(ctx, "/mnt/sdcard/mistakes_were_made_but_not_by_me.pdf");
  fz_catch(ctx) {
    fprintf(stderr, "Failed to load PDF: %s\n", fz_caught_message(ctx));
    fz_drop_context(ctx);
    return 1;
  }

  // 3. Load first page (page numbering starts from 0)
  fz_page *page = NULL;
  fz_try(ctx) page = fz_load_page(ctx, doc, 0);
  fz_catch(ctx) {
    fprintf(stderr, "Failed to load page: %s\n", fz_caught_message(ctx));
    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
    return 1;
  }

  // 4. Render page to pixmap (default 72 DPI, identity transform)
  // Target: 150 DPI
  float zoom = 150.0f / 72.0f;

  // fz_scale(sx, sy) builds a scaling matrix
  fz_matrix ctm = fz_scale(zoom, zoom);

  // If you also want to crop/translate, multiply by a translation matrix:
  // ctm = fz_concat(fz_translate(-offset_x, -offset_y), ctm);

  fz_pixmap *pix =
      fz_new_pixmap_from_page(ctx, page, ctm, fz_device_rgb(ctx), 0);
  /* fz_pixmap *pix = NULL; */
  /*     fz_try(ctx) */
  /*         pix = fz_new_pixmap_from_page(ctx, page, fz_identity,
   * fz_device_rgb(ctx), 0); */
  fz_catch(ctx) {
    fprintf(stderr, "Failed to render page: %s\n", fz_caught_message(ctx));
    fz_drop_page(ctx, page);
    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
    return 1;
  }

  clock_gettime(CLOCK_REALTIME, &end);
  long long ns = ((long long)end.tv_sec * 1000000000LL + end.tv_nsec) -
                 ((long long)start.tv_sec * 1000000000LL + start.tv_nsec);
  printf("%.3f ms\n", (double)ns / 1e6);

  // 5. Save as PNG
  fz_try(ctx) fz_save_pixmap_as_png(ctx, pix, "output.png");
  fz_catch(ctx) {
    fprintf(stderr, "Save PNG failed: %s\n", fz_caught_message(ctx));
  }

  // 6. Cleanup
  fz_drop_pixmap(ctx, pix);
  fz_drop_page(ctx, page);
  fz_drop_document(ctx, doc);
  fz_drop_context(ctx);

  return 0;
}
