#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 0;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  printf("New client created! ID %d on socket FD %d\n", client->client_id, client->cfd);
  char buf[256];

  for (;;) {
    int closed = read(client->cfd, buf, 256);
    if (closed == 0)
      break;
    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    printf("MSG #  %d, client id %d:  %s", total_message_count, client->client_id, buf);
    pthread_mutex_unlock(&count_mutex);
  }

  close(client->cfd);

  printf("Ending thread for client %d\n", client->client_id);

  // TODO: print the message received from client
  // write(STDOUT_FILENO, );
  // TODO: increase total_message_count per message

  return NULL;
}

int main() {
  struct sockaddr_in addr, peer_addr;
  int sfd;
  socklen_t peer_addr_len;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    int cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
    pthread_t thread;
    struct client_info *info = malloc(sizeof(struct client_info));
    pthread_mutex_lock(&client_id_mutex);
    client_id_counter++;
    info->client_id = client_id_counter;
    info->cfd = cfd;
    pthread_mutex_unlock(&client_id_mutex);
    pthread_create(&thread, NULL, handle_client, (void *)info);
    pthread_detach(thread);
    // TODO: create a new thread when a new connection is encountered
    // TODO: call handle_client() when launching a new thread, and provide
    // client_info
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
