#include <stdio.h>
#include <bpf/libbpf.h>
#include <signal.h>
#include <unistd.h>

#include "../sections/ipc/ipc.bpf.skel.h"
#include "../sections/perf/perf.bpf.skel.h"
#include "../sections/network/net.bpf.skel.h"
#include "../sections/fs/fs_watch.bpf.skel.h"
#include "../sections/cap/cap.bpf.skel.h"
#include "../sections/exec/exec.bpf.skel.h"
#include "../sections/bpf/bpf_hooks.bpf.skel.h"

static volatile sig_atomic_t stop;

void handle_signal(int signo) {
    stop = 1;
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("Loading all eBPF LSM programs...\n");

    struct ipc_bpf *ipc = ipc_bpf__open_and_load();
    if (!ipc || ipc_bpf__attach(ipc)) {
        fprintf(stderr, "Failed to load/attach IPC\n");
        return 1;
    }

    struct perf_bpf *perf = perf_bpf__open_and_load();
    if (!perf || perf_bpf__attach(perf)) {
        fprintf(stderr, "Failed to load/attach PERF\n");
        return 1;
    }

    struct net_bpf *net = net_bpf__open_and_load();
    if (!net || net_bpf__attach(net)) {
        fprintf(stderr, "Failed to load/attach NETWORK\n");
        return 1;
    }

    struct fs_watch_bpf *fs = fs_watch_bpf__open_and_load();
    if (!fs || fs_watch_bpf__attach(fs)) {
        fprintf(stderr, "Failed to load/attach FS\n");
        return 1;
    }

    struct cap_bpf *cap = cap_bpf__open_and_load();
    if (!cap || cap_bpf__attach(cap)) {
        fprintf(stderr, "Failed to load/attach CAP\n");
        return 1;
    }

    struct exec_bpf *exec = exec_bpf__open_and_load();
    if (!exec || exec_bpf__attach(exec)) {
        fprintf(stderr, "Failed to load/attach EXEC\n");
        return 1;
    }

    struct bpf_hooks_bpf *bpf = bpf_hooks_bpf__open_and_load();
    if (!bpf || bpf_hooks_bpf__attach(bpf)) {
        fprintf(stderr, "Failed to load/attach BPF syscall\n");
        return 1;
    }

    printf("All eBPF programs loaded. Press Ctrl+C to exit.\n");

    while (!stop)
        sleep(1);

    ipc_bpf__destroy(ipc);
    perf_bpf__destroy(perf);
    net_bpf__destroy(net);
    fs_watch_bpf__destroy(fs);
    cap_bpf__destroy(cap);
    exec_bpf__destroy(exec);
    bpf_hooks_bpf__destroy(bpf);

    return 0;
}

