# Makefile for the smash program
CPP = g++
CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CPP)
OBJS = smash.o commands.o signals.o job.o
RM = rm -f
$TARGET = smash
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.h
smash.o: smash.cpp commands.h
signals.o: signals.cpp signals.h
job.o: job.cpp job.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) $(OBJS)

