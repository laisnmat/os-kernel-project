# PSYS — OS Kernel Project

> A minimal 32-bit x86 operating system kernel built in C and assembly, developed as part of the OS project at Ensimag. Runs on QEMU.

![img.png](assets/img.png)

---

## Requirements

- **GCC** (x86 cross-compilation toolchain)
- **Make**
- **QEMU** (`qemu-system-i386`)
- **GDB** (optional, for debugging)

---

## Build & Run

```sh
# Build kernel and user space
make

# Build and launch in QEMU
make run

# Clean build artifacts
make clean
```

QEMU launches the kernel directly (`-kernel kernel/kernel.bin`) with 256 MB of RAM on a q35 machine.

For GDB debugging:

```sh
bash bin/scripts/debug.sh
```

---

## Architecture

The system is split into two spaces that communicate exclusively through **system calls** (software interrupt):

```
┌──────────────────────────────────┐
│         User space (ring 3)      │  user/
│   Shell, user processes, IPC     │
├──────────────────────────────────┤
│         Kernel space (ring 0)    │  kernel/
│   Scheduler, process mgmt,       │
│   message queues, drivers        │
├──────────────────────────────────┤
│         Shared code              │  shared/
│   printf, string, malloc…        │
└──────────────────────────────────┘
```

The kernel is loaded at **1 MB**, user code at **16 MB**.

---

## Kernel features

### Process management

Up to **512 concurrent processes** (`NBPROC`), with priorities from 1 to 256.

| Syscall | Description |
|---|---|
| `start(func, ssize, prio, name, arg)` | Create and launch a new process |
| `exit(retval)` | Terminate the calling process |
| `kill(pid)` | Force-terminate a process |
| `killall(name)` | Kill all processes with a given name |
| `waitpid(pid, retvalp)` | Wait for a child process to finish |
| `getpid()` | Get the current process PID |
| `getprio(pid)` | Get a process's priority |
| `chprio(pid, newprio)` | Change a process's priority |
| `wait_clock(clock)` | Sleep until a given clock tick |

Process states: `RUNNING`, `READY`, `SLEEPING`, `WAIT_CHILD`, `BLOCKED_MSG`, `BLOCKED_SEM`, `BLOCKED_IO`, `ZOMBIE`.

### Scheduler

Priority-based preemptive scheduler triggered by the clock interrupt. Context switches are implemented in assembly (`ctx_sw.S`), saving and restoring the 5 general-purpose registers (EBX, ESP, EBP, ESI, EDI).

### Message queues (IPC)

Up to **10 message queues** (`MAX_NB_QUEUE`), with blocking send/receive semantics.

| Syscall | Description |
|---|---|
| `pcreate(count)` | Create a queue with given capacity |
| `pdelete(fid)` | Destroy a queue, unblock waiting processes |
| `psend(fid, msg)` | Send a message (blocks if queue is full) |
| `preceive(fid, &msg)` | Receive a message (blocks if queue is empty) |
| `preset(fid)` | Reset a queue, unblock all waiting processes |
| `pcount(fid, &count)` | Query queue occupancy |

### Drivers

- **Display** — direct VGA text mode memory writes, scrolling cursor
- **Keyboard** — IRQ-based scancode handler using the Linux keyboard driver, with history support
- **Serial / GDB** — remote debugging over serial port

---

## Shell commands

The user-space shell (`user/shell.c`) launches at boot and supports:

| Command | Description |
|---|---|
| `help` | Show available commands |
| `banner` | Display the team banner |
| `ps` | List running processes |
| `history` | Show command history |
| `kill <pid>` | Kill a process by PID |
| `killall <name>` | Kill all processes by name |
| `sleep <n>` | Sleep for n clock ticks |
| `set` | Change system parameters |
| `time` | Display current clock value |
| `clear` | Clear the screen |
| `test` | Run professor's test suite |
| `test_phase5` | Run phase 5 tests |
| `sh` | Launch a sub-shell |
| `exit` / `quit` | Quit the shell |

---

## Project structure

```
kernel/         Kernel source (C + assembly)
│ ├── process.c/h     Process management & lifecycle
│ ├── scheduler.c/h   Preemptive priority scheduler
│ ├── messages.c/h    Message queue IPC
│ ├── syscalls.c      System call dispatcher
│ ├── clock.c/h       Clock & timer interrupts
│ ├── keyboard.c/h    Keyboard driver
│ ├── display.c/h     VGA text display driver
│ ├── mem.c/h         Kernel heap allocator
│ ├── ctx_sw.S        Assembly context switch
│ ├── crt0.S          Kernel boot entry point
│ └── debugger.c/h    Register dump & GDB serial support
user/           User-space code
│ ├── shell.c/h       Interactive shell
│ ├── start.c/h       User-space entry point
│ ├── syscalls.S      Syscall wrappers (assembly)
│ └── lib/            Utility processes (kill, sleep, killall…)
shared/         Code shared between kernel and user
│ ├── printf.c        Formatted output
│ ├── string.c/h      String utilities
│ ├── queue.h         Priority queue macros
│ └── malloc.c.h      Generic heap allocator
```

---

## Memory map

See the `CARTE_MEMOIRE` file at the project root for a detailed layout of kernel, user, and stack memory regions.
