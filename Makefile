CFLAGS=-ggdb3
OBJS=ts2l.o
BIN=ts2l

${BIN}: ${OBJS}
	cc -o $@ $^ -ggdb3

clean:
	rm -f ${OBJS} ${BIN}
