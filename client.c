#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <ctype.h>
#include <netdb.h>
#include <getopt.h>

#define BUFSIZE 1024

void send_recv(int i, int sockfd)
{
        char send_buf[BUFSIZE];
        char recv_buf[BUFSIZE];
        int nbyte_recvd,s=0,k=0;
        if (i == 0){
                fgets(send_buf, BUFSIZE, stdin);
                if (strcmp(send_buf , "quit\n") == 0) {
                        exit(0);
                }
		else
                        send(sockfd, send_buf, strlen(send_buf)+1, 0);
        }

        else {
		nbyte_recvd = recv(sockfd, recv_buf, BUFSIZE, 0);
                recv_buf[nbyte_recvd] = '\0';
		printf("%s",recv_buf);
                fflush(stdout);
		fflush(stdin);
		printf("\n");

           }
}


void connect_request(int *sockfd, struct sockaddr_in *server_addr,char *host,char *port)
{
        if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("Socket");
                exit(1);
        }
        struct hostent *hp, *gethostbyname();
        server_addr->sin_family = AF_INET;
	hp = gethostbyname(host);
        memcpy(&server_addr->sin_addr, hp->h_addr, hp->h_length);
        server_addr->sin_port = htons(atoi(port));
        memset(server_addr->sin_zero, '\0', sizeof server_addr->sin_zero);
        if(connect(*sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr)) == -1) {
                perror("connect");
                exit(1);
        }
}

void usage()
{
  printf("dec_client [−h] [-s port-number] [−p file]\n");
  printf("-h  				print usage summary and exit\n");
  printf("-s connect to specified host 	Default is localhost\n");
  printf("-p portno.			port number. Default is 9090  \n");
  exit(1);
}

int main(int argc, char *argv[])
{
        int sockfd, fdmax, i,option=0,prev_ind;
        struct sockaddr_in server_addr;
        fd_set master;
        fd_set read_fds;
        char *host="localhost",*port="9090",c;
 	while(prev_ind = optind, (c =  getopt(argc, argv, ":h:s:p:")) != EOF) {
             if ( optind == prev_ind + 2 && *optarg == '-' ) {
                  c = ':';
                  -- optind;
             }
             switch (c) {
                    case 'h':
		      usage();
                      break;

		    case 's':
                      host=optarg;
                      break;

		    case 'p':
                      port=optarg;
		      break;

                    case ':':
                      printf("Missing option.\n");
                      exit(1);
                      break;
            }
    }

        connect_request(&sockfd, &server_addr,host,port);
        FD_ZERO(&master);
        FD_ZERO(&read_fds);
        FD_SET(0, &master);
        FD_SET(sockfd, &master);
        fdmax = sockfd;
        while(1){
                read_fds = master;
                if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
                        perror("select");
                        exit(4);
                }

                for(i=0; i <= fdmax; i++) {
                        if(FD_ISSET(i, &read_fds)) {
                                send_recv(i, sockfd); }
                }
        }
        printf("client-quited\n");
        close(sockfd);
        return 0;
}

