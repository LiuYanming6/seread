/*************************************************************************
	> File Name: ser_op.h
	> Author: 
	> Mail: 
	> Created Time: 2017年09月20日 星期三 13时28分36秒
 ************************************************************************/

#ifndef _SER_OP_H
#define _SER_OP_H

int Ser_open();
int Ser_read(unsigned char *buf, int buflen);
int Ser_write(unsigned char *buf, int buflen);

#endif
