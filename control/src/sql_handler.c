#include "sql_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

sqlite3 *db;

void db_init(){
	char *err_msg = 0;
	int rc;
	sqlite3_open("control.db",&db);
	char *sql =	"CREATE TABLE IF NOT EXISTS Device(ID ROWID, IP TEXT,MAC TEXT, DI INT, DO INT, AI INT, AO INT, Name TEXT);"\
				"CREATE TABLE IF NOT EXISTS DigitalIn(ID ROWID, DEVID INT, DEVINID INT, Status INT, Name TEXT);"\
				"CREATE TABLE IF NOT EXISTS AnalogIn(ID ROWID, DEVID INT, DEVINID INT, Value INT, Name TEXT, Scaling REAL);"\
				"CREATE TABLE IF NOT EXISTS DigitalOut(ID ROWID, DEVID INT, DEVOUTID INT, Status INT, Name TEXT);"\
				"CREATE TABLE IF NOT EXISTS AnalogOut(ID ROWID, DEVID INT, DEVOUTID INT, Value INT, NAME TEXT);";
	rc = sqlite3_exec(db,sql,0,0,&err_msg);
	
	if(rc != SQLITE_OK){
		fprintf(stderr,"SQL Error : %s\n",err_msg);
		sqlite3_free(err_msg);
		}
	}

void db_addDevice(char ip[], char mac[], int digitalIn, int digitalOut, int analogIn, int analogOut){
	
	char *sql = NULL;
	char *err_msg = 0;
	sqlite3_stmt *res;
	int rc = 0;
	int device_op = 0;
	int test = asprintf(&sql,"SELECT IP, MAC FROM Device;");
	if(test == -1){
		printf("fehler\n");
		return;
	}
	
	rc = sqlite3_prepare_v2(db,sql,-1,&res,NULL);
	if(rc != SQLITE_OK) {
		printf("sql_error \n%s\n",sql);
		return;}
	
	while((rc = sqlite3_step(res)) == SQLITE_ROW){
		
		const unsigned char *sql_ip = sqlite3_column_text(res,0);
		const unsigned char *sql_mac = sqlite3_column_text(res,1);
		printf("Device ip %s and mac %s found\n", sql_ip, sql_mac);
		
		if((strcmp(sql_ip, ip) + strcmp(sql_mac, mac))==0){
			device_op = 1;
			rc = SQLITE_DONE;
			break;
		}else if(strcmp(sql_mac, mac)==0){
			device_op = 2;
			rc = SQLITE_DONE;
			break;
		}
	
	}
	
	if(rc != SQLITE_DONE){
		fprintf(stderr,"SQL Error line 60 : %s\n",err_msg);
		sqlite3_free(err_msg);
	}
	sqlite3_finalize(res);
	printf("device op %i\n",device_op);
	
	if(sql!=NULL)free(sql);
	
	switch(device_op){
		case 0:
			asprintf(&sql,"INSERT INTO Device(IP,MAC,DI,DO,AI,AO) VALUES('%s','%s',%i,%i,%i,%i);", ip, mac, digitalIn, digitalOut, analogIn, analogOut);
			break;
		case 1:
			return;
			break;
		case 2:
			asprintf(&sql,"UPDATE Device SET IP='%s' WHERE MAC='%s';",ip,mac, mac);
			break;
	}

	rc = sqlite3_exec(db,sql,0,0,&err_msg);
	
	if(rc != SQLITE_OK){
		fprintf(stderr,"SQL Error : %s\n",err_msg);
		sqlite3_free(err_msg);
	}
	
	if(sql!=NULL) free(sql);
	
	if(device_op==0){
		int id=0;
		asprintf(&sql,"SELECT MAX(ROWID) FROM DEVICE");
		sqlite3_prepare_v2(db,sql,-1,&res,NULL);
		if(sqlite3_step(res)==SQLITE_ROW){
			id= sqlite3_column_int(res,0);
			
		}
		
		sqlite3_finalize(res);
		if(sql!=NULL)free(sql);
		
		if(digitalIn>0)db_add_function(id,DIGITAL_IN,digitalIn);
		if(digitalOut>0)db_add_function(id,DIGITAL_OUT,digitalOut);
		if(analogIn>0)db_add_function(id,ANALOG_IN,analogIn);
		if(analogOut>0)db_add_function(id,ANALOG_OUT,analogOut);
		}
}

void db_add_function(int dev,enum FUNC_TYPE func, int number){
		char *sql = NULL;
		sqlite3_stmt *res;
		printf("%i",func);
		switch(func){
			case DIGITAL_IN:
				asprintf(&sql,"INSERT INTO DigitalIn(DEVID,DEVINID) VALUES(%i,?1);",dev);
				printf("%s",sql);
				break;  
			case DIGITAL_OUT:
				asprintf(&sql,"INSERT INTO DigitalOut(DEVID,DEVOUTID) VALUES(%i,?1);",dev);
				printf("%s",sql);
				break;  
			case ANALOG_IN:
				asprintf(&sql,"INSERT INTO AnalogIn(DEVID,DEVINID) VALUES(%i,?1);",dev);
				printf("%s",sql);
				break;  
			case ANALOG_OUT:
				asprintf(&sql,"INSERT INTO AnalogOut(DEVID,DEVOUTID) VALUES(%i,?1);",dev);
				printf("%s",sql);
				break;
		}
		
		if(sql!=NULL){
			printf("%s\n",sql);
			for(int i = 0;i<number;i++){
				char *errmsg = NULL;
				int rc = sqlite3_prepare_v2(db,sql,-1,&res,NULL);
				sqlite3_bind_int(res,1,i+1);
				rc = sqlite3_step(res);
				if(rc!=SQLITE_DONE){
					fprintf(stderr,"SQL Error: %s",errmsg);
					sqlite3_free(errmsg);	
				}
			}
			free(sql);
		
		}
	}
