TARGET=sfmt.$(LIB_EXTENSION)
SRCS=$(wildcard ./deps/SFMT/SFMT.c) $(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)
INSTALL?=install
ifdef SFMT_COVERAGE
COVFLAGS=--coverage
endif

ifdef SFMT_FLAGS
DEFINE=$(SFMT_FLAGS)
else
DEFINE="-DSFMT_MEXP=19937"
endif


.PHONY: all install clean

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(WARNINGS) $(COVFLAGS) $(DEFINE) $(CPPFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS) $(PLATFORM_LDFLAGS) $(COVFLAGS)

install:
	$(INSTALL) $(TARGET) $(INST_LIBDIR)
	rm -f ./src/*.o ./*.so

clean:
	rm -f ./src/*.o ./*.so
