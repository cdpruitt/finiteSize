COMPILER= clang++ 
CFLAGS= -lstdc++ -I$(shell root-config --incdir) -std=c++11 -ggdb
SOURCES = dataPoint.cpp dataSet.cpp CrossSection.cpp simpleSim.cpp io.cpp
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)

LINKOPTION = $(shell root-config --libs)

TARGETS = simpleSim
all: $(addprefix $(BIN), $(TARGETS))

simpleSim: $(SOURCES)
	$(COMPILER) $(CFLAGS) -o simpleSim $(SOURCES) $(LINKOPTION)

%.o : %.cpp $(HEADERS)
	$(COMPILER) $(CFLAGS) $< -o $@

#.PHONY: all

clean:
	rm -f *.o
