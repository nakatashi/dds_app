CC=gcc
CFLAGS=-O3 -Wall
LDFLAGS=-lrp_dds -lm

all: swing_app config_app checkval.out

test: main.out

swing_app: swing.out

config_app: config.out

checkval_app : checkval.out

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

#gcc -o config.out config.c -lrp_dds -lm

