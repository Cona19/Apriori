CC = g++

CPPFLAGS = -g -Wall -O0

OBJS = ./src/main.o ./src/unit_test.o
SRCS = ./src/$(OBJS: .o=.cpp)
BIN = ./bin/
INCLUDE = ./src
LIBNAME =

CPPFLAGS += -I$(INCLUDE)

all: apriori

apriori: $(OBJS)
	$(CC) -o $(BIN)apriori $(OBJS) $(CPPFLAGS) $(LIBNAME)

clean:
	rm -rf $(OBJS)

allclean:
	rm -rf $(OBJS) $(BIN)*

