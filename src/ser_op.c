/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月19日 星期二 13时02分24秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <termios.h>

#include "ser_op.h"

static int fd;

int Ser_open()
{
    struct termios tio;

    memset(&tio, 0, sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;  //8n1
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    fd = open("/dev/ttyS0", O_RDWR | O_NONBLOCK);
    if (fd != -1)
    {
        printf("Serial Port open\n");
        cfsetispeed(&tio, B115200);
        cfsetospeed(&tio, B115200);
        tcsetattr(fd, TCSANOW, &tio);
    }
    else
    {
        printf("Unable to open /dev/ttyS0\n");
    }

    return fd;
}

int Ser_read(unsigned char *buf, int buflen)
{
    int iRead = 0;
    int done = 0;

    if (buf[0] == 0x99)
    {
        buf++;
    }
    for (;buflen;)
    {
        iRead = read(fd, buf, buflen);
        if (-1 == iRead)
        {
            if (errno != EAGAIN)
            {
                perror("ser read");
                return 0;
            }
            else
            {
                printf("EGAIN");
            }
        }
        else if (0 == iRead)
        {
            printf("==0");
        }
        else
        {
            buflen -= iRead;
            buf += iRead;
            printf("iRead %d\n", iRead);
        }
        usleep(100);
    }

    return 1;
}

int Ser_write(unsigned char *buf, int buflen)
{
    int iWrite = -1;

    assert(fd > 0);
    assert(buf != NULL);
    assert(buflen > 0);

    if (buf[0] == 0xcc)
    {
        buf++;
        buflen--;
    }

    iWrite = write(fd, buf, buflen);
    if (iWrite < 0)
    {
        perror("ser write");
    }

    return iWrite;
}

int testser_main()
{
    int nRead = 0;
    unsigned char buf[1024];
    int i = 0;
    struct termios tio, tio_original;

    memset(&tio, 0, sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    int fd = open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);
    if (fd != -1)
    {
        printf("Serial Port open\n");
        cfsetospeed(&tio, B115200);
        cfsetispeed(&tio, B115200);

        tcsetattr(fd, TCSANOW, &tio);

    }
    else
    {
        printf("Unable to open /dev/ttyUSB0\n");
    }

    while (1)
    {
        memset(buf, 0, 1024);
        nRead = read(fd, buf, 64);
        if (nRead <= 0)
            continue;
        for (i = 0; i < nRead; i++)
        {
            printf("%x", buf[i]);
        }
        printf("\n");
    }

    return 0;
}
