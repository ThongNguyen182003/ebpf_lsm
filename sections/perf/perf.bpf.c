#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

SEC("lsm/perf_event_open")
int BPF_PROG(block_perf_pid, struct perf_event_attr *attr, int pid, int cpu, int group_fd, unsigned long flags)
{
    if (pid > 1000) {
        bpf_printk("Blocked perf for PID > 1000");
        return -1;
    }
    return 0;
}

