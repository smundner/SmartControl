CC = gcc
OBJECTS = main.o ethernet.o sql_handler.o arp_handler.o

TARGET = SmartGardenControl

GTKFLAGS =`pkg-config --cflags gtk+-3.0 --libs`
CFLAGS = -std=c11 -lpthread -ldl -lsqlite3
#CFLAGS += $(GTKFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(CFLAGS) 

main.o: main.c main.h
	$(CC) -c $(CFLAGS) $< -o $@
	


%.o: %.c %.h
	$(CC) -c $(CFLAGS)  $< -o $@
clean:
	rm $(TARGET) *.o
