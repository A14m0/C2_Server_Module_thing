CPPC = g++

CFLAGS = -c -static -fpic

build: $(OBJECT_FILES)
	$(CPPC) $(CFLAGS) -o resolver.o main.cpp
	$(CPPC) -shared -o resolver.so resolver.o
