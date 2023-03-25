# A make file that should work on most Unix-like platforms

# PREFIX is environment variable, if not set, use the default value
ifeq ($(PREFIX),)
    PREFIX := /opt/ooRexx
endif

LIBP = lib
LIB1 = rxgetopt

REXX_CFLAGS := $(shell ooRexx-config  --cflags)

OTHR_CFLAGS  = -fPIC

REXX_LFLAGS := $(shell ooRexx-config --libs)
REXX_RPATH  := $(shell ooRexx-config --rpath)

OTHR_LFLAGS :=

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	EXT = dylib
	OTHR_LFLAGS := -dynamiclib
else
  EXT = so
  OTHR_LFLAGS := -shared -export-dynamic -nostartfiles
endif

# What we want to build.
all: lib$(LIB1).$(EXT)

$(LIB1).o: $(LIB1).cpp
	$(CXX) -c $(LIB1).cpp $(REXX_CFLAGS) $(OTHR_CFLAGS) -o $(LIB1).o

$(LIBP)$(LIB1).$(EXT): $(LIB1).o
	$(CXX) $(LIB1).o $(REXX_LFLAGS) $(OTHR_LFLAGS) -o $(LIBP)$(LIB1).$(EXT)

install:
	cp $(LIBP)$(LIB1).$(EXT) $(PREFIX)/lib/

clean:
	rm -f *.o  *.so *.dylib
