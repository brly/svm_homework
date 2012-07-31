
# objects 
OBJS = 	main.o \
	svm.o \
	libsvm.o

OUTPUT = application

LIBS = 

$(OUTPUT): $(OBJS)
	$(CXX) -g -O2 -o $(OUTPUT) $(OBJS) $(LIBS)

# compile option
CXXFLAGS = -Wall -std=c++0x -ggdb

# suffix
.SUFFIXES: .h .cc. o
.c.o:
	$(CXX) -c $(CXXFLAGS) $(LIBS) $<

# header dependency
.h.cc:

main.o: svm.h

libsvm.o: libsvm.h 

# make clean rule
.PHONY: clean
clean:
	$(RM) $(OBJS) $(OUTPUT)

