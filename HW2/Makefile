CC=g++
CFLAGS=-std=c++11 -g -Wall -Werror -pedantic-errors -DNDEBUG -pthread
CCLINK=$(CC)
OBJS=main.o account.o Bank.o ATM.o
RM=rm -rf
TARGET=Bank

Bank: $(OBJS)
	$(CCLINK) $(CFLAGS) -o Bank $(OBJS)

account.o: account.cpp account.h
Bank.o: Bank.cpp Bank.h account.h Log-File.h
ATM.o: ATM.cpp ATM.h Bank.h Bank.h account.h Log-File.h
main.o: main.cpp account.h Bank.h Log-File.h

clean:
	$(RM) $(OBJS) $(TARGET)