/*
Check content of the file at index 131070:
  hexdump -s 131070 -c file
*/

#include <fcntl.h>
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

  int file_desc = open(file_path, O_WRONLY | O_APPEND | O_CREAT, mode);
  if (file_desc == -1) {
    printf("Cannot open.\n");
    return EXIT_FAILURE;
  }

  char chunk[128 * 1024];
  create_chunk(chunk, sizeof(chunk));

  if (write(file_desc, chunk, sizeof(chunk)) == -1) {
    printf("Cannot write.\n");
    goto error_file_cleanup;
  };

#ifdef USE_FSYNC
  fsync(file_desc);
#endif
  
  close(file_desc);

  return EXIT_SUCCESS;

error_file_cleanup:
  close(file_desc);

  return EXIT_FAILURE;
}
