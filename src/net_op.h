/*************************************************************************
	> File Name: net_op.h
	> Author: 
	> Mail: 
	> Created Time: 2017年09月20日 星期三 13时24分00秒
 ************************************************************************/

#ifndef _NET_OP_H
#define _NET_OP_H

int Net_send(unsigned char *buf, int buflen);
int Net_rcv(int fd, unsigned char *buf, int buflen);
int Net_bind();

#endif
