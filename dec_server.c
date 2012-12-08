/*
 * soc.c - program to open sockets to remote machines
 *
 * $Author: kensmith $
 * $Id: soc.c 6 2009-07-03 03:18:54Z kensmith $
 */

static char svnid[] = "$Id: soc.c 6 2009-07-03 03:18:54Z kensmith $";

#define	BUF_LEN	8192

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<netinet/in.h>
#include	<inttypes.h>
//#include	<sampler.c>

char *progname;
char buf[BUF_LEN];

void usage();
//int setup_client();
int setup_server();

int s, sock, ch, server, done, bytes, aflg,main_len;
int soctype = SOCK_STREAM;
char *host = NULL;
char *port = NULL;
extern char *optarg;
extern int optind;

int
main(int argc,char *argv[])
{
	fd_set ready;
	struct sockaddr_in msgfrom;
	int msgsize;
	union {
		uint32_t addr;
		char bytes[4];
	} fromaddr;

	if ((progname = rindex(argv[0], '/')) == NULL)
		progname = argv[0];
	else
		progname++;
	while ((ch = getopt(argc, argv, "adsp:h:")) != -1)
		switch(ch) {
			case 'a':
				aflg++;		/* print address in output */
				break;
			case 'd':
				soctype = SOCK_DGRAM;
				break;
			case 's':
				server = 1;
				break;
			case 'p':
				port = optarg;
				break;
			case 'h':
				host = optarg;
				break;
			case '?':
			default:
				usage();
		}
	argc -= optind;
	if (argc != 0)
		usage();
	if (!server && (host == NULL || port == NULL))
		usage();
	if (server && host != NULL)
		usage();
/*
 * Create socket on local host.
 */
	if ((s = socket(AF_INET, soctype, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	//if (!server)
		//sock = setup_client();
	//else
		//sock = setup_server();
/*
 * Set up select(2) on both socket and terminal, anything that comes
 * in on socket goes to terminal, anything that gets typed on terminal
 * goes out socket...
 */
	/*while (!done) {
		FD_ZERO(&ready);
		FD_SET(sock, &ready);
		FD_SET(fileno(stdin), &ready);
		if (select((sock + 1), &ready, 0, 0, 0) < 0) {
			perror("select");
			exit(1);
		}
		if (FD_ISSET(fileno(stdin), &ready)) {
			if ((bytes = read(fileno(stdin), buf, BUF_LEN)) <= 0)
				done++;
			send(sock, buf, bytes, 0);
		}
		msgsize = sizeof(msgfrom);
		if (FD_ISSET(sock, &ready)) {
			if ((bytes = recvfrom(sock, buf, BUF_LEN, 0, (struct sockaddr *)&msgfrom, &msgsize)) <= 0) {
				done++;
			} else if (aflg) {
				fromaddr.addr = ntohl(msgfrom.sin_addr.s_addr);
				fprintf(stderr, "%d.%d.%d.%d: ", 0xff & (unsigned int)fromaddr.bytes[0],
			    	0xff & (unsigned int)fromaddr.bytes[1],
			    	0xff & (unsigned int)fromaddr.bytes[2],
			    	0xff & (unsigned int)fromaddr.bytes[3]);
			}
			//buf[bytes+1]='\0';
			//printf("Client sent: %s", buf);
			//pass call to some other function that breaks up this string and
			write(fileno(stdout), buf, bytes);
		}
	}*/
	//close(sock);
	return(0);
}


/*
 * setup_server() - set up socket for mode of soc running as a server.
 */

int
setup_server() {
	struct sockaddr_in serv,remote;
	struct servent *se;
	int newsock, len;

	len = sizeof(remote);

	memset((void *)&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	if (port == NULL)
		serv.sin_port = htons(9990);
	else if (isdigit(*port))
		serv.sin_port = htons(atoi(port));
	/*else {
		if ((se = getservbyname(port, (char *)NULL)) < (struct servent *) 0) {
			perror(port);
			exit(1);
		}
		serv.sin_port = se->s_port;
	}*/
	if (bind(s, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
		perror("bind");
		exit(1);
	}
	if (getsockname(s, (struct sockaddr *) &remote, &len) < 0) {
		perror("getsockname");
		exit(1);
	}
	fprintf(stderr, "Port number is %d\n", ntohs(remote.sin_port));
	listen(s, 1);
	newsock = s;
	if (soctype == SOCK_STREAM) {
		fprintf(stderr, "Entering accept() waiting for connection.\n");
		newsock = accept(s, (struct sockaddr *) &remote, &len);

	}
	return(newsock);
}
/*
 * usage - print usage string and exit
 */

void
usage()
{
	fprintf(stderr, "usage: %s -h host -p port\n", progname);
	fprintf(stderr, "usage: %s -s [-p port]\n", progname);
	exit(1);
}
