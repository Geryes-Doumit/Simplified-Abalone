# Compilateur utilisé
CC= gcc

# Source C qui va fournir l'exécutable (ou les exécutables)
# MAINSRCS= main.c
MAINSRCS= $(wildcard main*.c)

# Ci-dessous, tous les fichiers *.c à l'exception du ou des précédents, sont 
# des librairies avec entêtes *.h
LIBSRCS= $(filter-out $(MAINSRCS), $(wildcard *.c))


CPPFLAGS= -DDEBUG
CFLAGS= -Wall `pkg-config --cflags gtk+-3.0`    # options propres au compilateur C
LDFLAGS=
LDLIBS= -lm `pkg-config --libs gtk+-3.0`        # librairies à intégrer à l'édition de liens

	
EXECS = $(patsubst %.c,%,$(MAINSRCS))
MAINOBJS=$(patsubst %.c,%.o,$(MAINSRCS))
LIBHS=$(LIBSRCS:.c=.h)
LIBOBJS=$(LIBSRCS:.c=.o)

# Règles

.PHONY: clean
	
all: $(EXECS)

$(MAINOBJS) : %.o :%.c $(LIBHS) $(COMMONHS) $(MAINHS)
	$(CC) -c -o $@ $(CFLAGS) $(CPPFLAGS) $<

$(LIBOBJS) : %.o :%.c %.h $(COMMONHS)
	$(CC) -c -o $@ $(CFLAGS) $(CPPFLAGS) $<
	
$(EXECS) : % : %.o $(LIBOBJS)
	$(CC) $(LDFLAGS) $^  -o abalone $(LDLIBS)

clean: 
	rm -f abalone *.o

tests:
	@make; ./abalone -tests

docs:
	doxygen doxygen_config_file