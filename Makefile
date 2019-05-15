BIN_FILES  = publisher broker subscriber server

CC = gcc


CCGLAGS =	-Wall  -g

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread


all: CFLAGS=$(CCGLAGS)
all: $(BIN_FILES)
.PHONY : all

publisher: publisher.o lines.o 
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

broker: broker.o lines.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

subscriber:
	javac *.java

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o
	rm -f *.class

.SUFFIXES:
.PHONY : clean
