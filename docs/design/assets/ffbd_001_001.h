static inline void create_chunk(char *chunk, int chunk_size) {
  for (int i = 0; i < chunk_size; i++) {
    chunk[i] = '0' + (i % 10);
  }
  chunk[chunk_size - 1] = '\n';
}
