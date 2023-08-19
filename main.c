#include "query.h"
#include <fcntl.h>
#include <plibsys.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define PLAYERCOUNT_TYPE uint16_t // so fun, it wouldn't overflow right?
#define RETRYS 10

#define SOCKET_PATH "./.sck"
#define BUFFER_SIZE 1024
#define BUCKET_SIZE 32
#define RESPONSEBUFFER_SIZE 13 * BUCKET_SIZE

typedef struct s_env_ {
  char *ip;
  uint16_t port;
} s_env;

// response: e,cr,max (maxof 5+5+3 = 13)
typedef struct s_res_ {
  uint8_t e;
  PLAYERCOUNT_TYPE cr;
  PLAYERCOUNT_TYPE max;
} s_res;

typedef struct s_db_ {
  s_res bucket[BUCKET_SIZE]; // wtf
  int8_t cursor; // instead of pushing every value back, we make sure the order
                 // is weird enougg
} s_db;

void dwrite(s_db *q, s_res *r) {
  q->bucket[q->cursor] = *r;
  if (q->cursor++ >= BUCKET_SIZE - 1) // wtf
    q->cursor = 0;
}

#define spute(s, r) sprintf(s, "%s:%d,%d,%d", s, r.e, r.cr, r.max);
#define dreads_splice(mi, ma)                                                  \
  for (size_t i = mi; i < ma; i++)                                             \
    spute((char *)s, q->bucket[i]);

void dreads(s_db *q, char *s) {
  if (q->cursor >= BUCKET_SIZE - 1) {
    dreads_splice(q->cursor + 1, BUCKET_SIZE);
    dreads_splice(0, q->cursor + 1);
  } else
    dreads_splice(0, BUCKET_SIZE);
}

void qwrite(s_res *res, s_env *env) {

  int ret = msq_init(env->ip, env->port);

  if (ret != 0) {
    printf("socc %s\n", msq_get_error(ret));
    *res = (s_res){255, 0, 0};
    return 0;
  }
  //   int jumps = 0;
  // RETRY:
  //   if (ret != 0) {
  //     printf("Cannot INIT after retry %d times. %s\n", jumps,
  //     msq_get_error(ret)); if (jumps < RETRYS) {
  //       jumps++;
  //       msq_free();
  //       goto RETRY;
  //     } else
  //       return (s_res){1, 0, 0};
  //   }
  msq_basic_stats_t bp;
  ret = msq_get_basic_stats(&bp);
  if (ret != 0)
    *res = (s_res){ret, 0, 0};
  else {
    *res = (s_res){ret, bp.num_players, bp.max_players};
    msq_free_basic_stats(&bp);
  }
  msq_free();
}

#define swbuf p_socket_send(client_socket, buffer, strlen(buffer), NULL);

int main(int argc, char **argv) {
  s_env env = {"127.0.0.1", 25565};

  int server_sock, client_sock;
  struct sockaddr_un server_addr, client_addr;
  socklen_t client_addr_size;
  char *buffer;

  s_db compact;
  s_res res;

  // Create a Unix domain stream socket
  server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_sock == -1) {
    perror("socket");
    return 0;
  }

  // Set the server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  // Bind the socket to the server address
  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind");
    return 0;
  }

  // Listen for incoming connections
  if (listen(server_sock, 5) == -1) {
    perror("listen");
    return 0;
  }

  printf("Listening for incoming connections...\n");

  while (1) {
    // Accept an incoming connection
    client_addr_size = sizeof(client_addr);
    client_sock =
        accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_sock == -1) {
      perror("accept");
      return 0;
    }

    // Receive data from the client
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_sock, buffer, sizeof(buffer), 0) == -1) {
      perror("recv");
      return 0;
    }

    printf("Received message: %s\n", buffer);

    // Check if the received message contains "hello"
    if (strstr(buffer, "query") != NULL) {
      sprintf(buffer, "");
      dreads(&compact, buffer);
      if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
        // perror("c");
      }
    } else if (strstr(buffer, "cron") != NULL) {
      qwrite(&res, &env);
      dwrite(&compact, &res);
      sprintf(buffer, "ok");
      if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
      }
    }

    // Close the client socket
    close(client_sock);
  }

  // Close the server socket and remove the socket file
  close(server_sock);
  unlink(SOCKET_PATH);

  return 0;

  return 0;
}