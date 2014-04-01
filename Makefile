.SUFFIXES:
.SUFFIXES: .cc .h .o

CC=g++
GCCMAKEDEP=/usr/X11R6/bin/gccmakedep

EXES=hcluster test
OBJS=common.o cluster.o cluster_table.o keyword_table.o levenshtein.o util.o hcluster.o
SRCS=$(OBJS:.o=.cc)

CFLAGS+=-m64 -Wall -O2 -g -c
LDFLAGS+=-m64 -Wall -g

.cc.o:
	@echo '+------------------------------------------+'
	@echo '| Compile $<                                '
	@echo '+------------------------------------------+'
	$(CC) -c $< $(CFLAGS)

all: $(EXES) retags

retags:
	etags *.h *.cc

hcluster: $(OBJS)
	@echo '+------------------------------------------+'
	@echo '| HCLUSTER                                 |'
	@echo '+------------------------------------------+'
	$(CC) -o $@ $^ $(LDFLAGS)

test: $(OBJS)
	@echo '+------------------------------------------+'
	@echo '| test                                     |'
	@echo '+------------------------------------------+'
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	@echo '+------------------------------------------+'
	@echo '| Remove All Object Files                  |'
	@echo '+------------------------------------------+'
	rm -rf $(EXES) *.o *~ core.[0-9][0-9]* core*

depend:
	$(GCCMAKEDEP) -- $(CFLAGS) -- $(SRCS)
