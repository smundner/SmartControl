#include "main.h"
#include "ethernet.h"
#include "sql_handler.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int main(int argc, char *argv[]){
	pthread_t bc_recv_task,data_rec_task;
	
	printf("Start programm\n");
	
	findMembersInit();
	db_init();
	pthread_create(&bc_recv_task,NULL,findMemberRecvTask,NULL);
	findeMemberSend();
	pthread_create(&data_rec_task,NULL,data_rec,NULL);
	
	
	while(1) sleep(10);
	
	return 0;
}
