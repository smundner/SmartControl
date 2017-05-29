CC = gcc
OBJECTS = main.o ethernet.o gui.o
TARGET = SmartGarden
CFLAGS = -std=c11
GTKFLAGS =`pkg-config --cflags gtk+-3.0 --libs`

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(CFLAGS) $(GTKFLAGS)

main.o: main.c main.h
	$(CC) -c $(CFLAGS) $(GTKFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(GTKFLAGS) $< -o $@
clean:
	rm $(TARGET) *.o
