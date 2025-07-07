#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

#define CAP_SYS_ADMIN 21

char LICENSE[] SEC("license") = "GPL";

SEC("lsm/capable")
int BPF_PROG(detect_sysadmin,
             const struct cred *cred,
             struct user_namespace *ns,
             int cap,
             unsigned int opts)
{
    if (cap == CAP_SYS_ADMIN) {
        bpf_printk("CAP_SYS_ADMIN requested\n");
    }
    return 0;
}

