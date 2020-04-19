# Makefile for Bank distributed program
CPPFLAGS = -g -Wall -pedantic
CCC = g++ $(CPPFLAGS)
CCLINK = $(CCC)
LINKS = -lpthread
OBJS = Account.o Bank.o ATM.o main.o
RM = rm -f

# Creating the Bank executable
all: Bank

Bank: $(OBJS)
	$(CCLINK) -o Bank $(OBJS) $(LINKS)
	
# Creating object files using default rules

main.o: main.cpp Bank.h Atm.h
	$(CCC) -c $*.cpp
	
Account.o : Account.cpp Account.h
	$(CCC) -c $*.cpp

Bank.o : Bank.cpp Bank.h Account.h
	$(CCC) -c $*.cpp
	
Atm.o : Atm.cpp Atm.h Account.h
	$(CCC) -c $*.cpp
	

	
# Cleaning old files before creating new ones
clean:
	$(RM) Bank *.o *~ "#"* core.* 

