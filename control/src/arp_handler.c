#include "arp_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define buffer_size 79

int arp_get_mac(char *_ip, char *ret){
	char *buffer = NULL;
	size_t read_len;
	ssize_t read;
	int return_val=0;
	int j=0;
	FILE *arp = fopen("/proc/net/arp","r");
	if(!arp){
		printf("cant open file\n");
		return -1;
	}
	
	
	getline(&buffer,&read_len,arp);
	while((read = getline(&buffer,&read_len,arp)) != -1){
		
		if(strstr(buffer,_ip) != NULL){
			char *token, *saveptr; 
			for(buffer;;buffer=NULL){
				token = strtok_r(buffer," ",&saveptr);
				if(token == NULL)break;
				
				if(j++==3){
				
				strcpy(ret,token);
				return_val = 1;
				}
			}
			break;
		}
	}
	
	
	if(buffer)free(buffer);
	fclose(arp);
	
	
	return return_val;
}
