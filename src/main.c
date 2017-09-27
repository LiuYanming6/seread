#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>

#include "net_op.h"
#include "ser_op.h"
#include "epoll_op.h"

#define MAXEVENTS 64

int main (int argc, char *argv[])
{
    int net_fd;
    int ser_fd;
    int efd;
    int iRet;
    struct epoll_event event;
    struct epoll_event *events;
    unsigned char buf[128];

    efd = Epoll_init();
    net_fd = Net_bind();
    Epoll_add(net_fd, NULL);
    ser_fd = Ser_open();
    Epoll_add(ser_fd, NULL);

    /* Buffer where events are returned */
    events = calloc (MAXEVENTS, sizeof event);

    /* The event loop */
    while (1)
    {
        int n, i;
        memset(buf, 0, sizeof(buf));

        printf("epoll_wait\n");
        n = epoll_wait (efd, events, MAXEVENTS, -1);
        if (n == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < n; i++)
        {
            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN)))
            {
                /* An error has occured on this fd, or the socket is not
                ready for reading (why were we notified then?) */
                fprintf (stderr, "epoll error\n");
                close (events[i].data.fd);
                continue;
            }
            else if (net_fd == events[i].data.fd)
            {
                iRet = Net_rcv(net_fd, buf, sizeof(buf));
                printf("net_fd %s\n", buf);
                if ((iRet > 0) && (buf[0] == 0xcc))
                {
                    Ser_write(buf+1, iRet);
                }
            }
            else if (ser_fd == events[i].data.fd)
            {
                printf("ser_fd\n");
                buf[0] = 0x99;
                iRet = Ser_read(buf+1, 9);
                if (iRet > 0)
                {
                    Net_send(buf, 10);
                }
            }
        }
    }

  free (events);
  close (net_fd);
  close (ser_fd);

  return EXIT_SUCCESS;
}
