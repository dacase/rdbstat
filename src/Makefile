#	BIN determines to where the final binary file is copied
#	You will probably want to change this to /usr/bin or something like that
BIN = ../bin
#	PROGS is a list of all the programs.  It is used for "make all"
PROGS = maketrix transpose io abut anova regress dm desc vincent calc critf pof dprime pair validata manstat perm reverse ts series repeat
#	CFLAGS is used as the default set of cc compiler options
CFLAGS = -O
#	LIBES determines the libraries to append to the cc command
#	On newer systems, -lm is common, on older systems, -lS (ask a guru)
LIBES = -lm
#	The UNTESTED programs have not been rigorously tested
UNTESTED = "This program has not been rigorously tested"

.c.o:  $<
	gcc -c $(CFLAGS) -o $@ $<

all:
	make -i $(PROGS)
clean:
	-/bin/rm -f *.o y.tab.*
maketrix: maketrix.o checkio.o number.o
	gcc $(CFLAGS) -o $(BIN)/maketrix maketrix.o checkio.o number.o
transpose: transpose.o checkio.o strings.o number.o
	gcc $(CFLAGS) -o $(BIN)/transpose transpose.o checkio.o strings.o number.o
io: io.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/io io.o checkio.o
abut: abut.o number.o
	gcc $(CFLAGS) -o $(BIN)/abut abut.o number.o
anova: anova.o strings.o number.o pof.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/anova anova.o strings.o number.o pof.o checkio.o $(LIBES)
regress: regress.o strings.o number.o pof.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/regress regress.o strings.o number.o pof.o checkio.o $(LIBES)
corr:
	make regress
	-/bin/ln $(BIN)/regress $(BIN)/corr
dm: dm.y strings.o number.o checkio.o
	yacc dm.y
	/bin/echo "dm: Don't be bothered by shift/reduce conflicts from yacc"
	gcc $(CFLAGS) -o $(BIN)/dm y.tab.c number.o strings.o checkio.o $(LIBES)
desc: desc.o pof.o number.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/desc desc.o number.o pof.o checkio.o $(LIBES)
vincent: vincent.o
	gcc $(CFLAGS) -o $(BIN)/vincent vincent.o $(LIBES)
calc: calc.y
	yacc calc.y
	gcc $(CFLAGS) -o $(BIN)/calc y.tab.c $(LIBES)
critf: CRITF.o pof.o critf.o
	gcc $(CFLAGS) -o $(BIN)/critf CRITF.o critf.o pof.o $(LIBES)
pof: POF.o pof.o
	gcc $(CFLAGS) -o $(BIN)/pof POF.o pof.o $(LIBES)
dprime: dprime.o number.o ptoz.o strings.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/dprime dprime.o number.o ptoz.o strings.o checkio.o $(LIBES)
	/bin/echo "dprime:" $(UNTESTED)
pair: pair.o number.o pof.o strings.o checkio.o scatterplot.o
	gcc $(CFLAGS) -o $(BIN)/pair pair.o number.o pof.o strings.o scatterplot.o checkio.o $(LIBES)
biplot:
	make pair
	-/bin/ln $(BIN)/pair $(BIN)/biplot
validata: validata.o strings.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/validata validata.o strings.o checkio.o -lm
lsd: lsd.o critf.o pof.o
	gcc $(CFLAGS) -o $(BIN)/lsd lsd.o critf.o pof.o -lm
	/bin/echo "lsd:" $(UNTESTED)
datum: datum.o number.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/datum datum.o number.o checkio.o -lm
manstat:
perm: perm.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/perm perm.o checkio.o
reverse: reverse.o strings.o checkio.o
	gcc $(CFLAGS) -o $(BIN)/reverse reverse.o strings.o checkio.o
ts: ts.o number.o cor.o checkio.o pof.o barplot.o
	gcc $(CFLAGS) -o $(BIN)/ts ts.o number.o cor.o pof.o barplot.o checkio.o -lm
series: series.o number.o
	gcc $(CFLAGS) -o $(BIN)/series series.o number.o
repeat: repeat.o checkio.o number.o
	gcc $(CFLAGS) -o $(BIN)/repeat repeat.o checkio.o number.o
