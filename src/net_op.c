/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#include "net_op.h"

static char *broStr = "255.255.255.255";
static struct sockaddr_in serv_addr;


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int create_socket()
{
    int sockfd, iRet;
    struct sockaddr_in serv_addr;
    const int opt = 1;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =  INADDR_BROADCAST;
    serv_addr.sin_port = htons(60000);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        error("ERROR opening socket");
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&opt,sizeof(opt)) < 0)//设置套接字类型
    {
        error("setsockopt");
    }

    return sockfd;
}
int Net_send(unsigned char *buf, int buflen)
{
    int sockfd, iRet;

    assert(buf != NULL);
    assert(buflen > 0);

    sockfd = create_socket();

    iRet = sendto(sockfd, buf, buflen, 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));  
    if(iRet < 0)  
    {  
        error("send error:");  
    } 

    return iRet;
}

int Net_rcv(int fd, unsigned char *buf, int buflen)
{
    int iRet = 0;
    iRet = read(fd, buf, buflen);
    if (iRet < 0)
    {
        perror("ERROR in read");
    }

    return iRet;
}

int Net_bind()
{
    int sockfd;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =  INADDR_BROADCAST;
    serv_addr.sin_port = htons(60000);
    sockfd = create_socket();

    if (bind(sockfd, (struct sockaddr *) &serv_addr, 
             sizeof(serv_addr)) < 0) 
    {
        error("ERROR on binding");
    }

    return sockfd;
}

int test_main(int argc, char *argv[])
{
    int sockfd;
    int iRet;
    u_char buf[1024] = {0};

    if ((argc == 2) && (argv[1][1] == 's'))
    {
        sockfd = Net_bind();
        iRet = Net_rcv(sockfd, buf, 1024);
        printf("%s\n", buf);
    }
    else
    {
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =  INADDR_BROADCAST;
    serv_addr.sin_port = htons(60000);
        Net_send(broStr, strlen(broStr));
    }

    close(sockfd);
    return 0; 
}
