# 🔐 eBPF-LSM Playground

This project is a collection of **eBPF LSM (Linux Security Module)** programs to explore and demonstrate how different Linux security hooks can be used to **monitor** and **control** kernel-level behavior.

## Project Structure

```
eBPF-LSM-Playground/
├── common/               # Shared headers (vmlinux.h, shared.h)
├── sections/             # eBPF LSM programs, grouped by hook type
│   ├── bpf/              # Hooks for BPF syscall
│   ├── cap/              # Capability checks
│   ├── exec/             # Executable-related hooks
│   ├── fs/               # Filesystem event hooks
│   ├── ipc/              # IPC (semaphore, message queue) hooks
│   ├── network/          # Networking LSM hooks
│   ├── perf/             # perf_event_open monitoring
├── user/                 # User space loader (loader.c)
├── loader                # Compiled loader binary
├── Makefile              # Build all sections + loader
```

---

## Prerequisites

* Linux kernel **5.7+** with `CONFIG_BPF_LSM=y`
* `bpftool`, `clang`, `libbpf-dev`
* `libbpf` (static or shared)
* Capable of loading LSM eBPF (see `/sys/kernel/security/lsm`)

---

## Build Instructions

```bash
# Optional: regenerate vmlinux.h if kernel changed
bpftool btf dump file /sys/kernel/btf/vmlinux format c > common/vmlinux.h

# Build all eBPF programs + loader
make
```

---

## Run Loader

```bash
sudo ./loader
```

You should see logs printed via `bpf_printk()` using:

```bash
sudo cat /sys/kernel/debug/tracing/trace_pipe
```

---

## Test Guide (by Section)

### 1. Capability (`cap/cap.bpf.c`)

* Hook: `lsm/capable`
* Detects if a process requests `CAP_SYS_ADMIN`
* Test:

  ```bash
  sudo touch /etc/test.txt
  # or: run any privileged command
  ```

---

### 2. Filesystem (`fs/fs_watch.bpf.c`)

* Hook: `lsm/inode_unlink`
* Logs deleted file names.
* Test:

  ```bash
  sudo rm /etc/test.txt
  ```

---

### 3. IPC (`ipc/ipc.bpf.c`)

* Hook: `lsm/ipc_permission`
* Logs `flag` used in IPC operations.
* Test:

  ```bash
  ipcmk -Q    # Create message queue (requires util-linux)
  ```

---

### 4. BPF Syscall Block (`bpf/bpf_hooks.bpf.c`)

* Hook: `lsm/bpf`
* Blocks all BPF syscalls.
* Test:

  ```bash
  bpftool prog list   # Should fail with permission error
  ```

---

### 5. exec (`exec/exec.bpf.c`)

* Hook: `lsm/bprm_check_security`
* Logs every binary execution.
* Test:

  ```bash
  ls
  cat /etc/os-release
  ```

---

### 6. Network (`network/net.bpf.c`)

* Hook: `lsm/socket_connect`
* Logs outbound connections.
* Test:

  ```bash
  curl google.com
  ```

---

### 7. Perf Event (`perf/perf.bpf.c`)

* Hook: `lsm/perf_event_open`
* Blocks `perf_event_open` if PID > 1000.
* Test:

  ```bash
  perf stat -p <your-pid>
  ```

---

## 📝 Notes

* If you move the project to another machine or kernel:

  ```bash
  bpftool btf dump file /sys/kernel/btf/vmlinux format c > common/vmlinux.h
  ```

* Use `sudo dmesg` or `trace_pipe` to debug eBPF program behavior.

---

## 🧑‍💻 Author

* **Thong Nguyen** – [GitHub](https://github.com/ThongNguyen182003)

---

## 📄 License

This project is licensed under the GPLv2 – see the [LICENSE](LICENSE) file for details.
