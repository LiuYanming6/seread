/*************************************************************************
	> File Name: epoll_op.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月20日 星期三 14时06分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include "epoll_op.h"

static int efd;

static int make_socket_non_blocking (int sfd)
{
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

int Epoll_add(int fd, void *pFunc)
{
    int iRet;
    struct epoll_event event = {0};

    //make_socket_non_blocking(fd);

    event.data.fd = fd;
    event.events = EPOLLIN;
    iRet = epoll_ctl (efd, EPOLL_CTL_ADD, fd, &event);
    if (iRet == -1)
    {
        perror ("epoll_ctl");
        abort ();
    }

    printf("add fd %d\n", fd);
}

int Epoll_init()
{

    efd = epoll_create1 (0);
    if (efd == -1)
    {
        perror ("epoll_create");
        abort ();
    }
    return efd;
}

