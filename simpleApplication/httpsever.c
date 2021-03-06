/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[512];
     char reb[512];
     struct sockaddr_in serv_addr, cli_addr;
     int n, rblen;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     if(listen(sockfd,10) < 0)
               error("ERROR on listen");
     clilen = sizeof(cli_addr);
     while (1) {
          
          newsockfd = accept(sockfd, 
                      (struct sockaddr *) &cli_addr, 
                      &clilen);
          if (newsockfd < 0) 
               error("ERROR on accept");
          bzero(buffer,512);
 
          n = read(newsockfd,buffer,511);
          if (n < 0) error("ERROR reading from socket");
          printf("Here is the message: %s\n",buffer);
          sprintf(reb, "HTTP/1.1 200 OK\nContent-type: text/plain\n\n%s\n", 
                                        buffer);
          n = write(newsockfd, reb, strlen(reb));
          if (n < 0) error("ERROR writing to socket");
          close(newsockfd);
     }
     close(sockfd);
     return 0; 
}


