/* serveur_UDP.c (serveur UDP) */


#ifdef WIN32 /* si vous �tes sous Windows */

#include <winsock2.h>


#elif defined (linux) /* si vous �tes sous Linux */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */


#endif
#include <unistd.h> /* close */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void init(void)
{
#ifdef WIN32
  WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
  WSACleanup();
#endif
}

char* id = 0;
short port = 0;
int sock = 0; /* socket de communication */
int nb_reponse = 0;

int main(int argc, char** argv) {
  init();
  int ret;
  struct sockaddr_in serveur; /* SAP du serveur */

  if (argc!=3) {
    fprintf(stderr,"usage: %s id port\n", argv[0]);
    exit(1);
  }
  id = argv[1];
  port = atoi(argv[2]);
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr,"%s: socket %s\n", argv[0],strerror(errno));
    exit(1);
  }
  serveur.sin_family = AF_INET;
  serveur.sin_port = htons(port);
  serveur.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *) &serveur, sizeof(serveur)) < 0) {
    fprintf(stderr,"%s: bind %s\n", argv[0],strerror(errno));
    exit(1);
  }

  int tries = 0;

  while (1) {
    struct sockaddr_in client; /* SAP du client */
    int client_len = sizeof(client);
    char buf_read[256], buf_write[256];

    char answer[50];
    int ans_len;
    char word[50];

    printf("Enter a word: ");
    scanf("%s", answer);
    ans_len = strlen(answer);

    for (int i = 0; i < ans_len; i++) {
      word[i] = '_';
    }
    word[ans_len] = '\0';

    ret = recvfrom(sock, buf_read, 256, 0
        , (struct sockaddr *) &client, &client_len);

    if (ret < 0) {
      printf("%s: recvfrom=%d: %s\n", argv[0],ret,strerror(errno));
      continue;
    }

    sprintf(buf_write, "%s\n", word);

    while (1) {
      ret = sendto(sock, buf_write, strlen(buf_write), 0
          , (struct sockaddr *) &client, client_len);
      if (ret <= 0) {
        printf("%s: sendto=%d: %s\n", argv[0],ret,strerror(errno));
        continue;
      }

      ret = recvfrom(sock, buf_read, 256, 0
          , (struct sockaddr *) &client, &client_len);

      if (ret < 0) {
        printf("%s: recvfrom=%d: %s\n", argv[0],ret,strerror(errno));
        continue;
      }

      for (int i = 0; i < strlen(buf_read); i++) {
        if (buf_read[i] == '\n') {
          buf_read[i] = '\0';
        }
      }

      // Reset game
      if (strcmp(buf_read, "reset") == 0) {
        break;
      }

      // If we input a single letter
      printf("Received: %s, length %d\n", buf_read, strlen(buf_read));
      if (strlen(buf_read) == 1){
        char c = buf_read[0];
        int found = 0;
        for (int i = 0; i < ans_len; i++) {
          if (answer[i] == c) {
            word[i] = c;
            found = 1;
          }
        }
        if (!found) {
          tries++;
        }
        sprintf(buf_write, "%s\n", word);
      } else {
        // Several letter -> try for word
        if (strcmp(answer, buf_read) == 0) {
          strcpy(word, answer);
          sprintf(buf_write, "=\n\0");
        } else {
          tries++;
          sprintf(buf_write, "@\n\0");
        }
      }

    }

  }
  end();
  return 0;
}
