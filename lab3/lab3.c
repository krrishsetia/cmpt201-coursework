#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 5

char *input_token();
void print(char **input, int *history);
void array_free(char **array);
void add_to_history(char *input, char **array, int *history);
void remove_oldest(char *input, char **array);

int main() {
  char **inputs = calloc(MAX_LEN, sizeof(char[25]));
  int *history = calloc(1, sizeof(int));

  while (1) {
    char *input = input_token();
    add_to_history(input, inputs, history);
    if (strcmp(input, "print") == 1) {
      print(inputs, history);
    }
  }
  free(history);
  array_free(inputs);
  return 0;
}

char *input_token() {
  char *buffer = NULL;
  size_t size = 0;
  printf("Please enter some text:");
  ssize_t len = getline(&buffer, &size, stdin);

  if (len < 0) {
    perror("getline failed");
    free(buffer);
    exit(1);
  }

  return buffer;
}

void print(char **input, int *history) {
  for (int i = 0; i < *history; i++) {
    printf("%s\n", input[i]);
  }
}

void array_free(char **array) {
  for (int i = 0; i < MAX_LEN; i++) {
    free(array[i]);
  }
  free(array);
}

void add_to_history(char *input, char **array, int *history) {
  if (*history == MAX_LEN) {
    remove_oldest(input, array);
  } else {
    array[*history] = input;
    *history = *history + 1;
  }
}

void remove_oldest(char *input, char **array) {
  free(array[0]);

  for (int i = 0; i < MAX_LEN - 1; i++) {
    array[i] = array[i + 1];
  }
  array[MAX_LEN - 1] = input;
}
