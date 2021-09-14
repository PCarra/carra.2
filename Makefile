# Patrick Carra
# make is used to compile binary
# make clean is used to remove all binaries and object files
#
# T1: T1.c
# 	gcc -o T1 T1.c

target1 = runsim
target2 = testsim
objs = testsim.o runsim.o
deps = $(patsubst %.o,%.h,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.h)

CC = gcc
CFLAGS = -Wall

all: $(target1) $(target2)

$(target1): $(objs)
	$(CC) $(CFLAGS) -o $@ $^

$(target2): $(objs)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(DEPFLAGS)
clean:
	rm -f $(target1) $(objs) $(deps)
	rm -f $(target2) $(objs) $(deps)
