#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ffbd_001_001.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Invalid args.\n");
    return EXIT_FAILURE;
  }

  const char *file_path = argv[1];
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  int file_desc = open(file_path, O_RDONLY, mode);
  if (file_desc == -1) {
    perror("Cannot open");
    return EXIT_FAILURE;
  }

  const int end_offset = lseek(file_desc, 0, SEEK_END);

  if (lseek(file_desc, 0, SEEK_SET) == -1) {
    perror("Cannot lseek");
    goto error_file_cleanup;
  };

  int chunk_i = 0;
  char chunk[128 * 1024];
  char expected_chunk[128 * 1024];
  create_chunk(expected_chunk, sizeof(expected_chunk));
  while (chunk_i < end_offset) {
    if (read(file_desc, chunk, sizeof(chunk)) == -1) {
      perror("Cannot read");
      goto error_file_cleanup;
    };

    for (int i = 0; i < sizeof(chunk); i++) {
      if (chunk[i] != expected_chunk[i]) {
        printf("Integrity error on index %d, should be '%d' and was '%d'\n", i,
               expected_chunk[i], chunk[i]);
        goto error_file_cleanup;
      };
    }

    chunk_i += sizeof(chunk);
    lseek(file_desc, chunk_i, SEEK_SET);
  }

  close(file_desc);

  printf("All good\n");

  return EXIT_SUCCESS;

error_file_cleanup:
  close(file_desc);

  return EXIT_FAILURE;
}
