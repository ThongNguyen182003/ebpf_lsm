
BPFTOOL ?= bpftool
CLANG ?= clang
CC ?= gcc

BPF_CFLAGS = -g -O2 -Wall -target bpf -D__TARGET_ARCH_x86
USER_CFLAGS = -g -O2 -Wall

INCLUDES = -Icommon

# Find all .bpf.c files
BPF_SRCS := $(shell find sections -name '*.bpf.c')
BPF_OBJS := $(BPF_SRCS:.bpf.c=.bpf.o)
SKELETONS := $(BPF_OBJS:.o=.skel.h)

all: loader

# Rule to compile .bpf.o from .bpf.c
%.bpf.o: %.bpf.c
	$(CLANG) $(BPF_CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to generate .skel.h from .bpf.o
%.bpf.skel.h: %.bpf.o
	$(BPFTOOL) gen skeleton $< > $@

# Build user-space loader with all skeleton headers
loader: user/loader.c $(SKELETONS)
	$(CC) $(USER_CFLAGS) -I. -o $@ $< $(shell pkg-config --cflags --libs libbpf)

clean:
	rm -f $(BPF_OBJS) $(SKELETONS) loader

