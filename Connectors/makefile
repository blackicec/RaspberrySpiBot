##

##

Compiler=gcc
CFlags=-Wall -pedantic
NoLinkFlag=-c
ExecutableName=-o SpiBotServer.exe

# build an executable named myprog from myprog.c
all: SpiBotServer.exe

SpiBotServer.exe: spiBotServer.o motorController.o
	$(Compiler) $(CFlags) -lwiringPi -pthread spiBotServer.o motorController.o $(ExecutableName)

spiBotServer.o: spiBotServer.c
	$(Compiler) $(CFlags) $(NoLinkFlag) -pthread spiBotServer.c

motorController.o: motorController.c motorController.h
	$(Compiler) $(CFlags) $(NoLinkFlag) -lwiringPi motorController.c

clean:
	rm -rf SpiBotServer.exe *.o
