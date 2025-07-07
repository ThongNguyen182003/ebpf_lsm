#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

SEC("lsm/inode_unlink")
int BPF_PROG(log_delete_etc, struct inode *dir, struct dentry *dentry)
{
    char name[64];
    bpf_core_read_str(name, sizeof(name), dentry->d_name.name);
    bpf_printk("Deleting file: %s", name);
    return 0;
}

