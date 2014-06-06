# make exec clean

GXX = g++

CCFLAGS = -g 
# CCFLAGS = -Wall -Werror -ansi -pedantic -fPIC -Wextra -g -O3

TARGET 		= challenge
SRCFILES 	= Client.cpp Produit.cpp Batch.cpp Probleme.cpp Parser.cpp main.cpp
HFILES		= Tools.h Client.h Produit.h Batch.h Probleme.h Parser.h

exec: all
	./$(TARGET)

val: all
	valgrind ./$(TARGET)

all:
	$(GXX) $(CCFLAGS) $(SRCFILES) -o $(TARGET)

clean:
	rm -rf *.o *~

clear: clean
	rm -rf $(TARGET)*

