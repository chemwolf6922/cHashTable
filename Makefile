CC=gcc

CFLAGS=-O3 -MMD -MP
LDFLAGS=
LIBS=

APPSRCS=main.c crc.c staticHashTable.c

all:app

app:$(APPSRCS:.c=.o)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

-include $(APPSRCS:.c=.d)

clean:
	rm -f app *.o *.d