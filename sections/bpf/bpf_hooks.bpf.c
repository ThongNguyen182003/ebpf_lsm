#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

SEC("lsm/bpf")
int BPF_PROG(block_bpf_syscall, int cmd, union bpf_attr *attr, unsigned int size)
{
    bpf_printk("Blocked bpf syscall with cmd=%d", cmd);
    return -1;
}

