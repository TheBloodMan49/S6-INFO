/* client_UDP.c (client UDP) */

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


#define NB_REQUETES 600

char* id = 0;
short sport = 0;
int sock = 0; /* socket de communication */

int main(int argc, char** argv) {
  init();
  struct sockaddr_in moi; /* SAP du client */
  struct sockaddr_in serveur; /* SAP du serveur */
  int nb_question = 0;
  int ret,len;
  int serveur_len = sizeof(serveur);
  char buf_read[256], buf_write[256];

  if (argc != 4) {
    fprintf(stderr,"usage: %s id host port\n", argv[0]);
    exit(1);
  }
  id = argv[1];
  sport = atoi(argv[3]);
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr,"%s: socket %s\n",argv[0],strerror(errno));
    exit(1);
  }
  len = sizeof(moi);
  getsockname(sock, (struct sockaddr *)&moi, &len);
  serveur.sin_family = AF_INET;
  serveur.sin_port = htons(sport);
#ifdef WIN32
  serveur.sin_addr.s_addr =inet_addr(argv[2]);
#else
  inet_aton(argv[2], (struct in_addr *)&serveur.sin_addr);
#endif

  int quit = 0;

  sprintf(buf_write, "\n");
  sendto(sock, buf_write, strlen(buf_write), 0, (struct sockaddr *)&serveur, sizeof(serveur));

  while (!quit) {

    ret = recvfrom(sock, buf_read, 256, 0, (struct sockaddr *)&serveur, &serveur_len);
    if (ret < 0) {
      printf("recvfrom=%d: %s\n", ret, strerror(errno));
      continue;
    }
    printf("Server: %s", buf_read);

    printf("Your try: ");
    scanf("%49s", buf_write);
    int len = strlen(buf_write);
    buf_write[len] = '\n';
    buf_write[len+1] = '\0';
    ret = sendto(sock, buf_write, strlen(buf_write), 0, (struct sockaddr *)&serveur, sizeof(serveur));
    if (ret <= 0) {
      printf("sendto=%d: %s\n", ret, strerror(errno));
      continue;
    }

  }

  close(sock);
  end();
  return 0;
}
