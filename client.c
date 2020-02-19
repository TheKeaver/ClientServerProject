
/**
 Group: Robin Naggi, Keith Petitt, Nick Sundvall
 Lab 2 - Factoid Server
 Course: 386 fall 2018
 Current Date: 09/18/2018
 Compiler: GCC
 Language: C
 Sources Consulted: StackOverflow,YouTube,Google
 Program Outline: Consult write-up
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 5432
#define MAX_LINE 256

int main(int argc, char * argv[])
{
    FILE *fp;
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    int s, port;
    int len;
    char temp[30];
    char sen[] = "send\n" ;
    int flag = 0;

    if (argc==3)
    {
        host = argv[1];
        port = atoi(argv[2]);
    }
    else
    {
        fprintf(stderr, "Worng input! (./client hostName PortNumber)\n");
        exit(1);
    }
    /* translate host name into peer's IP address */
    hp = gethostbyname(host);
    if (!hp)
    {
        fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
        exit(1);
    }
    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(port);
    /* active open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    }

    printf("Enter user command, GET/PUT/DELETE + \"Conversion/Constant/Definition\" + \"FACTP/0.9\"\n");
  	fgets(buf,sizeof(buf),stdin);

  	while(!(flag == -1)) { // Loop handles variable extra inputs, terminated by the 1
  	printf("Enter the next line of the command, terminate and send with \"send\"\n");
  	fgets(temp,sizeof(temp),stdin);

  	if((strcmp(temp,sen))==0) {
  	  flag = -1;
  	  break; }

  	if( (MAX_LINE-1) < (strlen(buf) + strlen(temp)) ) {
  	  printf("Input too long, sending partial message\n");
  	  flag = -1;
  	  strncat(buf, temp, (MAX_LINE - strlen(buf)-1));
      break;
    }
  	else {
  	  strcat(buf,temp) ; }
  	}

    buf[MAX_LINE-1] = '\0' ;
    len = strlen(buf) + 1 ;
  	send(s, buf, len, 0 ) ;
    if(len = recv(s, buf, MAX_LINE, 0))
      printf("\nClient received\n%s\n", buf);
    close(s);
    exit(1);
}
