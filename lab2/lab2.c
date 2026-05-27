#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void token(char *output) {}

int main() {
  char *input = NULL;
  pid_t cpid, w;
  int wstatus;
  while (1) {
    printf("Enter a program to run.\n");
    char *buffer = NULL;
    size_t size = 0;
    ssize_t len = getline(&buffer, &size, stdin);

    if (len < 0) {
      perror("getline failed");
      free(buffer);
      exit(1);
    }
    char *saveptr = NULL;
    char *ret = strtok_r(buffer, " ", &saveptr);

    input = ret;
    input[len - 1] = 0;
    cpid = fork();
    if (cpid == -1)
      perror("fork failed");

    if (cpid == 0) {
      int error = execl(input, input, (char *)NULL);
      if (error == -1)
        perror("execl failed");
    }

    w = waitpid(cpid, &wstatus, WCONTINUED);
    if (w == -1) {
      perror("waitpid failed");
    }
    free(buffer);
  }
  return 0;
}
