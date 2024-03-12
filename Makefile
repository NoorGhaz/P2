C       = gcc
CFLAGS  = -g -Wall -std=c11
TARGET  = P1
OBJS    = main.o scanner.o testScanner.o

all: $(TARGET)

$(TARGET): $(OBJS)
        $(CC) -o $(TARGET) $(OBJS)

main.o: main.c
        $(CC) $(CFLAGS) -c main.c

scanner.o: scanner.c scanner.h token.h
        $(CC) $(CFLAGS) -c scanner.c

testScanner.o: testScanner.c testScanner.h token.h
    	$(CC) $(CFLAGS) -c testScanner.c

clean:
    	/bin/rm -f *.o $(TARGET)
