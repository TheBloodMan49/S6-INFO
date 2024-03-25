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

#define NBECHANGE 3

char* id = 0;
short sport = 0;
int sock = 0; /* socket de communication */

int main(int argc, char** argv) {
  init();
  struct sockaddr_in moi;      /* SAP du client */
  struct sockaddr_in serveur;  /* SAP du serveur */
  int nb_question = 0;
  int ret, len;

  if (argc != 4) {
    fprintf(stderr, "usage: %s id serveur port %d\n", argv[0], argc);
    exit(1);
  }
  id = argv[1];
  sport = atoi(argv[3]);
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "%s: socket %s\n", argv[0], strerror(errno));
    exit(1);
  }
  serveur.sin_family = AF_INET;
  serveur.sin_port = htons(sport);
#ifdef WIN32
  serveur.sin_addr.s_addr =inet_addr(argv[2]);
#else
  inet_aton(argv[2], (struct in_addr *) &serveur.sin_addr);
#endif

  if (connect(sock, (struct sockaddr *) &serveur, sizeof(serveur)) < 0) {
    fprintf(stderr, "%s: connect %s\n", argv[0], strerror(errno));
    perror("bind");
    exit(1);
  }
  len = sizeof(moi);
  getsockname(sock, (struct sockaddr *) &moi, &len);
  int done = 0;

  while (!done) {
    char buf_read[1<<8], buf_write[1<<8];
    scanf("%s", buf_write);
    // Convert C string to normal string
    buf_write[strlen(buf_write)] = '\n';
    if (send(sock, buf_write, strlen(buf_write), 0) < 0) {
      fprintf(stderr, "%s: send %s\n", argv[0], strerror(errno));
      exit(1);
    }
    ret = recv(sock, buf_read, 256, 0);
    if (ret <= 0) {
      printf("%s: read=%d: %s\n", argv[0], ret, strerror(errno));
      break;
    }
    // Convert incoming string to C string
    buf_read[ret] = '\0';
    //printf("cli %s recu de (%s:%4d) : %s\n", id, inet_ntoa(moi.sin_addr), ntohs(moi.sin_port), buf_read);
    switch (atoi(buf_read)) {
      case 0:
        printf("Correct !\n");
        done = 1;
        break;
      case 1:
        printf("More !\n");
        break;
      case -1:
        printf("Less !\n");
        break;
    }
  }
  close(sock);
  end();
}