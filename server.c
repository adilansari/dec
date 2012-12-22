#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "sampler.c"

#define BUFSIZE 1024
#define PORT 9090

void usage()
{
  printf("dec_server [−h] [-p port-number] [−i file]\n");
  printf("-h  			print usage summary and exit\n");
  printf("-p portno.		port number. Default is 9090  \n");
  printf("-i input file 	log all request to a given file. Default is stdout\n");
  exit(1);
}



void send_recv(int i, fd_set *master, int sockfd, int fdmax,FILE *fp,int option_file)
{
        int nbytes_recvd;
        char recv_buf[BUFSIZE], buf[BUFSIZE];
        if ((nbytes_recvd = recv(i, recv_buf, BUFSIZ, 0)) <= 0) {
                if (nbytes_recvd == 0) {
                        printf("socket %d hung up\n", i);
                }else {
                        perror("recv");
                }
                close(i);
                FD_CLR(i, master);
        }else {
	      fflush(stdout);
	      char *ptr=NULL,final[2000];
	      final[0]='\0';
              ptr=recv_buf;
              ptr[strlen(ptr)]= '\0';
              //char newstr[500]= "response from server: ";
              tokenize(ptr);
            // send(i, newstr2, strlen(newstr2)+1,0);

      }
}

void connection_accept(fd_set *master, int *fdmax, int sockfd, struct sockaddr_in *client_addr)
{
        socklen_t addrlen;
        int newsockfd;

        addrlen = sizeof(struct sockaddr_in);
        if((newsockfd = accept(sockfd, (struct sockaddr *)client_addr, &addrlen)) == -1) {
                perror("accept");
                exit(1);
        }else {
                FD_SET(newsockfd, master);
                if(newsockfd > *fdmax){
                        *fdmax = newsockfd;
                }
                printf("new connection from %s on port %d \n",inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
        }
}

void connect_request(int *sockfd, struct sockaddr_in *my_addr,char *port)
{
        int yes = 1;

        if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("Socket");
                exit(1);
        }

        my_addr->sin_family = AF_INET;
        my_addr->sin_port = htons(atoi(port));
        my_addr->sin_addr.s_addr = INADDR_ANY;
        memset(my_addr->sin_zero, '\0', sizeof my_addr->sin_zero);

        if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                perror("setsockopt");
                exit(1);
        }

        if (bind(*sockfd, (struct sockaddr *)my_addr, sizeof(struct sockaddr)) == -1) {
                perror("Unable to bind");
                exit(1);
        }
        if (listen(*sockfd, 10) == -1) {
                perror("listen");
                exit(1);
        }
        printf("\nTCPServer Waiting for client\n");
        fflush(stdout);
}



int main(int argc,char *argv[])
{
        fd_set master;
        fd_set read_fds;
        int fdmax, i,prev_ind,option_file=0;
        int sockfd= 0;
	char *port="9090",*f,c;
        struct sockaddr_in my_addr, client_addr;
	FILE *fp;
	while(prev_ind = optind, (c =  getopt(argc, argv, ":h:p:i:")) != EOF) {
             if ( optind == prev_ind + 2 && *optarg == '-' ) {
                  c = ':';
                  -- optind;
             }
             switch (c) {
                    case 'h':
		      usage();
                      break;

		    case 'p':
                      port=optarg;
                      break;

		    case 'i':
                      option_file=1;
                      f=optarg;
		      break;

                    case ':':
                      printf("Missing option.\n");
                      exit(1);
                      break;
            }
        }

        if(option_file==1) {
	    fp=fopen(f,"w+");
        }
        FD_ZERO(&master);
        FD_ZERO(&read_fds);
        connect_request(&sockfd, &my_addr,port);
        FD_SET(sockfd, &master);

        fdmax = sockfd;
        while(1){
                read_fds = master;
                if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
                        perror("select");
                        exit(4);
                }

                for (i = 0; i <= fdmax; i++){
                        if (FD_ISSET(i, &read_fds)){
                                if (i == sockfd)
                                        connection_accept(&master, &fdmax, sockfd, &client_addr);
                                else
                                        send_recv(i, &master, sockfd, fdmax,fp,option_file);
                        }
                }
        }
        return 0;
}
