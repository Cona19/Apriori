CC = g++
CFLAGS = -g -Wall -O2

all: apriori

OBJS = ./src/main.o ./src/unit_test.o
SRCS = /src/$(OBJS: .o=.cpp)
BIN = /bin/
INCLUDE = ./src
LIBNAME =

CPPFLAGS += -I$(INCLUDE)

apriori: $(OBJS)
    $(CC) -o $(BIN)apriori $(OBJS) $(CPPFLAGS) $(LIBNAME)

clean:
    rm -rf $(OBJS)

allclean:
    rm -rf $(OBJS) $(BIN)*

