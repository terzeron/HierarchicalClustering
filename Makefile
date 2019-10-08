.SUFFIXES:
.SUFFIXES: .cc .h .o

CC=g++
GCCMAKEDEP=/usr/X11R6/bin/gccmakedep

EXES=hcluster test
OBJS=common.o cluster.o cluster_table.o keyword_table.o levenshtein.o util.o
SRCS=$(OBJS:.o=.cc)

CFLAGS+=-m64 -Wall -O2 -g -c -std=c++11
LDFLAGS+=-m64 -Wall -g

.cc.o:
	@echo '+------------------------------------------+'
	@echo '| Compile $<                                '
	@echo '+------------------------------------------+'
	$(CC) -c $< $(CFLAGS)

all: $(EXES) retags

retags:
	etags *.h *.cc

hcluster: hcluster.o $(OBJS)
	@echo '+------------------------------------------+'
	@echo '| HCLUSTER                                 |'
	@echo '+------------------------------------------+'
	$(CC) -o $@ $^ $(LDFLAGS)

test: test.o $(OBJS)
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
