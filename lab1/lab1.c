#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *buffer = NULL;
  size_t size = 0;
  while (1) {
    printf("Please enter some text:");
    ssize_t len = getline(&buffer, &size, stdin);

    if (len < 0) {
      perror("getline failed");
      free(buffer);
      exit(1);
    }

    printf("Tokens:\n");

    char *saveptr = NULL;
    char *ret = strtok_r(buffer, " ", &saveptr);

    printf("%s\n", ret);

    while ((ret = strtok_r(NULL, " ", &saveptr))) {
      if (ret == NULL)
        break;
      printf("%s\n", ret);
    }
  }
  free(buffer);

  return 0;
}
