#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

SEC("lsm/ipc_permission")
int BPF_PROG(ipc_permission_check, struct kern_ipc_perm *ipcp, short flag)
{
    bpf_printk("IPC permission check: flag=%d", flag);
    return 0;
}

