#ifndef __ETHERNET_H__
#define __ETHERNET_H__

//#define _BSD_SOURCE 
#define _GNU_SOURCE
void findMembersInit(void);
void findeMemberSend(void);
void *findMemberRecvTask(void *arg);
void *data_rec(void *arg);
void set_remote_output(int remote, char output);
#endif
