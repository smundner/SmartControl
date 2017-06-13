#include "main.h"
#include "ethernet.h"
#include <stdio.h>
int main(int argc, char *argv[]){
	printf("Start programm\n");
	
	//findMembers();
	set_remote_output(1,3);
	data_rec();
	return 0;
}
