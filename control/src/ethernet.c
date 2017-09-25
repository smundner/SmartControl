#include "ethernet.h"
#include "sql_handler.h"
#include "arp_handler.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



#define SEARCH_PORT 5998
#define RECORDER_PORT 6000
#define OUTPUT_PORT 5999
#define BUFF_SIZE 255

typedef struct {
	uint8_t dI;
	uint8_t dO;
	uint16_t aI[4];
}inOut;

const int y = 1;
int bcast_sock=0;

int json_get_int_from_oject(char *_json, char *pos){
	int ret = 0;
	char *json = malloc(strlen(_json));
	memcpy(json,_json,strlen(_json));
	
	char *temp = strstr(json,pos);
	
	if(temp!=NULL){
		
		strtok(temp,":");
		temp = strtok(NULL,",");
		if(!strcmp(temp,"")) return 0;
		ret = atoi(temp);
	}
	
	return ret;
}

void findeMemberSend(){
	struct hostent *he;
	struct sockaddr_in remoteAddr;
	he = gethostbyname("255.255.255.255");
	if (he== NULL){
		printf("can't convert ip address\n");
		return;
		}
	remoteAddr.sin_family = he->h_addrtype;
	memcpy(&remoteAddr.sin_addr, he->h_addr_list[0],he->h_length);
	remoteAddr.sin_port = htons(SEARCH_PORT);
	printf("sending Broadcast \n");
	sendto(bcast_sock,"\n",strlen("/n"),0,(struct sockaddr *) &remoteAddr,sizeof(remoteAddr));
	}

void *findMemberRecvTask(void *arg){
	char puffer[BUFF_SIZE];
	struct sockaddr_in recAddr;
	int len = sizeof(recAddr);
	while(1){
		static int i = 1;
		memset(puffer,0,BUFF_SIZE);
		recvfrom(bcast_sock,puffer,BUFF_SIZE,0,(struct sockaddr *) &recAddr,&len);
		char mac[18];
		
		char *ip = inet_ntoa(recAddr.sin_addr);
		int dI, dO, aI,aO;
		
		if(arp_get_mac(inet_ntoa(recAddr.sin_addr),mac)==1){
			dI = json_get_int_from_oject(puffer,"DI");
			dO = json_get_int_from_oject(puffer,"DO");
			aI = json_get_int_from_oject(puffer,"AI");
			aO = json_get_int_from_oject(puffer,"AO");

			db_addDevice(ip,mac,dI,dO,aI,aO);
			
			
		}
	}
}

void findMembersInit(){
	printf("start finde Member\n");
	struct sockaddr_in addr;
	struct in_addr ipv4addr_remote;
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SEARCH_PORT);
	bcast_sock = socket(AF_INET, SOCK_DGRAM,0);
	setsockopt(bcast_sock,SOL_SOCKET,SO_REUSEADDR, &y, sizeof(int));
	setsockopt(bcast_sock,SOL_SOCKET,SO_BROADCAST, &y, sizeof(int));
	bind(bcast_sock,(struct sockaddr*) &addr, sizeof(addr));
	
	}
	
	
void *data_rec(void *arg){
	printf("start data recorder\n");
	struct sockaddr_in addr, remoteAddr,recAddr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(RECORDER_PORT);
	int record_sock = socket(AF_INET, SOCK_DGRAM,0);
	setsockopt(record_sock,SOL_SOCKET,SO_REUSEADDR, &y, sizeof(int));
	bind(record_sock,(struct sockaddr*) &addr, sizeof(addr));
	inOut buff;
	int len = sizeof(recAddr);
	while(1){
		static int i = 1;
		
		memset(&buff,0,sizeof(inOut));
		recvfrom(record_sock,&buff,sizeof(inOut),0,(struct sockaddr *) &recAddr,&len);
		char *ip =inet_ntoa(recAddr.sin_addr);
		printf("%05i %s %i %i %i %i %i %i\n",i++,inet_ntoa(recAddr.sin_addr),buff.dI,buff.dO,buff.aI[0],buff.aI[1],buff.aI[2],buff.aI[3]);
		
		//db_update_di(ip,);
		}
		
	}

void set_remote_output(int remote, char output){
	int sock;
	struct sockaddr_in address;
	
	sock = socket(AF_INET,SOCK_STREAM,0);
	address.sin_family = AF_INET;
	address.sin_port = htons(OUTPUT_PORT);
	inet_aton("192.168.0.230",&address.sin_addr);
	connect(sock,(struct sockaddr *) &address,sizeof(address));
	send(sock,&output,sizeof(output),0);
	close(sock);
	}
