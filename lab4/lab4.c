#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE 64
#define handle_error(err)

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? (uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

struct header {
  uint64_t size;
  struct header *next;
};

int main() {
  void *output = sbrk(256);

  struct header *block1 = (struct header *)output;
  print_out("pointer is: %p\n", block1, sizeof(block1));

  struct header *block2 = (struct header *)(output + 128);
  print_out("pointer is: %p\n", block2, sizeof(block2));

  block1->size = 128;
  block2->size = 128;
  block1->next = NULL;
  block2->next = block1;

  print_out("block 1 size: %d\n", (void *)block1->size, sizeof(block1->size));
  print_out("block 1 next: %p\n", block1->next, sizeof(block1->next));
  print_out("block 2 size: %d\n", (void *)block2->size, sizeof(block2->size));
  print_out("block 2 next: %p\n", block2->next, sizeof(block2->next));

  uint8_t *block1_mem = (uint8_t *)block1 + 1;
  memset(block1_mem, 0, 120);
  for (int i = 0; i < 120; i++) {
    print_out("block 1 memory: %d\n", (void *)(uintptr_t)(block1_mem[i]), sizeof(uint64_t));
  }
  uint8_t *block2_mem = (uint8_t *)block2 + 1;
  memset(block2_mem, 1, 120);
  for (int i = 0; i < 120; i++) {
    print_out("block 2 memory: %d\n", (void *)(uintptr_t)(block2_mem[i]), sizeof(uint64_t));
  }

  return 0;
}
