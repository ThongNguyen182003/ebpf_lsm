#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

SEC("lsm/bprm_check_security")
int BPF_PROG(block_exec_tmp, struct linux_binprm *bprm)
{
    const char prefix[] = "/tmp";
    char filename[256];

    bpf_core_read_str(filename, sizeof(filename), bprm->filename);

    if (__builtin_memcmp(filename, prefix, sizeof(prefix) - 1) == 0) {
        bpf_printk("Blocked execution from /tmp: %s", filename);
        return -1;
    }

    return 0;
}

