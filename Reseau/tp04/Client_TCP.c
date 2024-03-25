/* Client_TCP.c (Client TCP) */

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
  int ret,len;

  if (argc != 4) {
    fprintf(stderr,"usage: %s id serveur port %d\n",argv[0], argc);
    exit(1);
  }
  id = argv[1];
  sport = atoi(argv[3]);
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr,"%s: socket %s\n", argv[0],strerror(errno));
    exit(1);
  }
  serveur.sin_family = AF_INET;
  serveur.sin_port = htons(sport);
#ifdef WIN32
  serveur.sin_addr.s_addr =inet_addr(argv[2]);
#else
  inet_aton(argv[2], (struct in_addr *)&serveur.sin_addr);
#endif

  if (connect(sock, (struct sockaddr *)&serveur, sizeof(serveur)) < 0) {
    fprintf(stderr,"%s: connect %s\n", argv[0],strerror(errno));
    perror("bind");
    exit(1);
  }
  len = sizeof(moi);
  getsockname(sock,(struct sockaddr *)&moi, &len);
  for (nb_question = 0; nb_question < NBECHANGE; nb_question++) {
    char buf_read[1<<8], buf_write[1<<8];

    sprintf(buf_write,"#%2s=%03d",id,nb_question);
    printf("client %2s: (%s,%4d) envoie a ",id
	   , inet_ntoa(moi.sin_addr), ntohs(moi.sin_port));
    printf(" (%s,%4d) : %s\n", inet_ntoa(serveur.sin_addr), ntohs(serveur.sin_port),buf_write);
    ret = send(sock, buf_write, strlen(buf_write)+1, 0);
    if (ret <= strlen(buf_write)) {
      printf("%s: erreur dans write (num=%d, mess=%s)\n",argv[0],ret,strerror(errno));
      continue;
    }
    printf("client %2s: (%s,%4d) recoit de ", id, inet_ntoa(moi.sin_addr),ntohs(moi.sin_port));
    ret = recv(sock, buf_read, 256, 0);
    if (ret <= 0) {
      printf("%s:erreur dans read (num=%d,mess=%s)\n", argv[0],ret,strerror(errno));
      continue;
    }
    printf("(%s,%4d):%s\n",inet_ntoa(serveur.sin_addr), ntohs(serveur.sin_port),buf_read);
  }
  close(sock);
  end();
  return 0;
}

