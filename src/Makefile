

TARGET=seread

LIBS=
SRCS=*.c 
OBJS=$(SRCS:.c=.o)
	
all:$(TARGET) 

$(TARGET): *.c
	$(CC) $^ -o $@ $(LIBS)


clean:
	rm -rf *.o $(TARGET)

