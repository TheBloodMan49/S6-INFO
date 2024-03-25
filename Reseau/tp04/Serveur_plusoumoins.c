/* serveur_TCP.c (serveur TCP) */

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
#include <time.h>

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
short port = 0;
int sock = 0; /* socket de communication */
int nb_reponse = 0;

int main(int argc, char** argv) {
  srand(time(NULL));
  init();
  struct sockaddr_in serveur; /* SAP du serveur */

  if (argc != 3) {
    fprintf(stderr,"usage: %s id port\n",argv[0]);
    exit(1);
  }
  id = argv[1];
  port = atoi(argv[2]);
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr,"%s: socket %s\n", argv[0],strerror(errno));
    exit(1);
  }
  serveur.sin_family = AF_INET;
  serveur.sin_port = htons(port);
  serveur.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock,(struct sockaddr *)&serveur,sizeof(serveur)) < 0) {
    fprintf(stderr,"%s: bind %s\n", argv[0],strerror(errno));
    exit(1);
  }
  if (listen(sock, 5) != 0) {
    fprintf(stderr,"%s: listen %s\n", argv[0],strerror(errno));
    exit(1);
  }
  while (1) {
    struct sockaddr_in client; /* SAP du client */
    int len = sizeof(client);
    int sock_pipe; /* socket de dialogue */
    char buf_read[1<<8], buf_write[1<<8];
    int ret, done = 0;

    sock_pipe = accept(sock, (struct sockaddr *)&client,  &len);
    int random = rand() % 100 + 1;
    while (!done) {
      ret = recv(sock_pipe, buf_read, sizeof(buf_read), 0);
      if (ret == -1) {
        fprintf(stderr,"%s: recv %s\n", argv[0],strerror(errno));
        exit(1);
      }
      if (ret == 0) {
        done = 1;
      } else {
        //Convert string
        buf_read[ret] = '\0';
        //printf("Received: %s\n", buf_read);
        int number = atoi(buf_read);
        //printf("Number: %d\n", number);
        if (number < random) {
          strcpy(buf_write, "1\n");
        } else if (number > random) {
          strcpy(buf_write, "-1\n");
        } else {
          strcpy(buf_write, "0\n");
          done = 1;
        }
        ret = send(sock_pipe, buf_write, strlen(buf_write), 0);
        if (ret == -1) {
          fprintf(stderr,"%s: send %s\n", argv[0],strerror(errno));
          exit(1);
        }
    }

#ifdef WIN32
      Sleep(2000)
#else
      sleep(2);

#endif
    }
    close(sock_pipe);
  }
  end();
  return 0;
}
