CXX=g++
# option par défaut de compilation: -std=c++0x si c++11 n'est pas reconnu
CXXFLAGS=-pipe -std=c++11 -Wall -Wextra -Wconversion -pedantic -O3
# liste de l'ensemble des fichiers sources cpp
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
# bibliothèques supplémentaires (statiques) avec lesquelles compiler
LDLIBS=
LDFLAGS=-pipe -O3
# nom de l'exécutable
EXE=prog

.PHONY: clean mrproper

all:	clean depend $(EXE)

$(EXE): depend $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# makedepend: le package xutils-dev doit être installé
#EDIT personnel :(sous Ubuntu/Debian c'est valide)
depend:
	@makedepend -- $(CXXFLAGS) -- -Y $(SRCS) 2> /dev/null

clean:
#rm -f $(OBJS)
	rm -f *.o

mrproper:	clean
	rm -f $(EXE)

# DO NOT DELETE THIS LINE

test-set.o: Set.hpp
