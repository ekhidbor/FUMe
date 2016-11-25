CC      = ccache-swig cc -Qunused-arguments -fcolor-diagnostics
CXX     = ccache-swig c++ -Qunused-arguments -fcolor-diagnostics
RM      ?= rm

.SUFFIXES:

all: loopback

loopback: loopback.c
	$(CC) -I../include -o loopback loopback.c -L../ -lfume

clean:
	$(RM) -f loopback
