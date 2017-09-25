#ifndef __SQL_HANDLER_H__
#define __SQL_HANDLER_H__

#define _GNU_SOURCE

enum FUNC_TYPE{DIGITAL_IN, DIGITAL_OUT, ANALOG_IN, ANALOG_OUT};

void db_add_function(int dev,enum FUNC_TYPE func, int number);
void db_init(void);
void db_addDevice(char *ip, char *mac, int digitalIn, int digitalOut, int analogIn, int analogOut);
void db_update_di(char *ip,int data);
#endif
