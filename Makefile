CC=gcc
PROG:=bvm
CPPFLAGS:=-Iinclude

OBJS:=$(patsubst %.c,%.o,$(wildcard src/*.c)) \
$(patsubst %.cpp,%.o,$(wildcard src/*.cpp)) \
$(patsubst %.c,%.o,$(wildcard src/*/*.c)) \
$(patsubst %.cpp,%.o,$(wildcard src/*/*.cpp))

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $@
clean:
	rm -f $(OBJS)
	rm -f $(PROG)

