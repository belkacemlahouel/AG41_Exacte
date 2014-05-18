CXX = g++
# CCFLAGS = -Werror -Wall -O3
# CCFLAGS = -Wall -Werror -ansi -pedantic -fPIC -Wextra -g

TARGET   = challenge
SRCFILES = main.cpp Probleme.cpp Produit.cpp Client.cpp Transport.cpp
HFILES   = Probleme.h Client.h Produit.h Transport.h

all:
	$(CXX) $(CCFLAGS) $(SRCFILES) -o $(TARGET)

clean:
	@echo "Cleaning .o and ~ files in current directory"
	rm -rf *.o *~ $(TARGET).dSYM

clear: clean
	@echo "Cleaning target file"
	rm -rf challenge
