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

  while (1) {
    struct sockaddr_in client; /* SAP du client */
    int client_len = sizeof(client);
    char buf_read[256], buf_write[256];

    ret = recvfrom(sock, buf_read, 256, 0
        , (struct sockaddr *) &client, &client_len);

    if (ret < 0) {
      printf("%s: recvfrom=%d: %s\n", argv[0],ret,strerror(errno));
      continue;
    }

    printf("serveur %s recu le msg %s de %s:%d\n", id,buf_read
        , inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    sprintf(buf_write, "serveur#%2s reponse%03d#", id,nb_reponse++);
    ret = sendto(sock, buf_write, strlen(buf_write), 0
        , (struct sockaddr *) &client, client_len);
    if (ret <= 0) {
      printf("%s: sendto=%d: %s\n", argv[0],ret,strerror(errno));
      continue;
    }
    //    sleep(2);
  }
  end();
  return 0;
}
