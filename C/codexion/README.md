*This project has been created as part of the 42 curriculum by bgomes-b.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic **dining philosophers**
problem. A group of **coders** sit in a circular co-working hub around a shared
Quantum Compiler. There are as many **USB dongles** on the table as there are coders,
one placed between each pair of neighbors. To **compile**, a coder must hold both of
their neighboring dongles (left and right) at the same time; once compiling is done,
both dongles are put back on the table, and the coder moves on to **debugging**, then
**refactoring**, before attempting to compile again.

Every coder must start a new compile within `time_to_burnout` milliseconds of the start
of their previous compile (or of the simulation, if they haven't compiled yet) — otherwise
they **burn out** and the whole simulation stops. The simulation also stops successfully
once every coder has compiled at least `number_of_compiles_required` times.

The goal of the project is to implement this simulation with real POSIX threads and
mutexes: each coder is a thread, each dongle's state is protected by its own mutex, and
a dedicated **monitor thread** watches for burnout in real time (detected within 10ms of
the actual event) without relying on any global variables.

## Instructions

### Compilation

```sh
make        # builds the "codexion" binary
make clean  # removes object files
make fclean # removes object files and the binary
make re     # rebuilds from scratch
```

Compiled with `-Wall -Wextra -Werror -pthread` using `cc`.

### Usage

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
	   time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- `number_of_coders`: number of coders (and dongles).
- `time_to_burnout`, `time_to_compile`, `time_to_debug`, `time_to_refactor`,
  `dongle_cooldown`: all in milliseconds.
- `number_of_compiles_required`: simulation stops successfully once every coder
  reaches this many compiles.
- `scheduler`: `fifo` or `edf`.

All arguments are mandatory and validated: negative numbers, non-integer values, and
any scheduler other than `fifo`/`edf` are rejected.

Example (feasible parameters — runs to completion without burnout):

```sh
./codexion 4 800 200 200 200 3 50 fifo
```

> For the simulation to be feasible (no coder forced to burn out regardless of
> scheduling), `time_to_burnout` must comfortably exceed
> `time_to_compile + time_to_debug + time_to_refactor`, plus some slack for dongle
> contention and cooldown.

## Blocking cases handled

- **Mutual exclusion on dongles**: each dongle has its own `pthread_mutex_t` and
  `pthread_cond_t`. A coder only holds a dongle between a successful `acquire_dongle`
  and its matching `release_dongle` — the dongle's `is_taken` flag is only ever
  read/written while its mutex is held.

- **Deadlock prevention (Coffman's circular-wait condition)**: compiling requires
  holding two dongles at once (hold-and-wait is inherent to the problem), so deadlock
  is prevented by breaking **circular wait** instead: every coder always acquires the
  dongle with the *lower id* first, then the one with the higher id (`pick_order`),
  regardless of which one is physically their "left" or "right" dongle. This creates a
  single global resource ordering, which makes a circular wait — and therefore a
  deadlock — impossible. This also correctly handles the single-coder edge case (where
  the "left" and "right" dongle are the same object): the coder blocks on its own
  already-held dongle and burns out cleanly instead of hanging, since compiling with
  only one dongle is physically impossible.

- **Starvation prevention / fair arbitration**: each dongle keeps a small priority
  queue (a custom binary heap, see `heap.c`/`heap_utils.c`) of pending requests. Under
  `fifo`, requests are served in arrival order, ties (identical arrival time) broken
  by the lower `coder_id`. Under `edf`, the request with the earliest burnout deadline
  (`last_compile_start + time_to_burnout`) is served first; on a deadline tie, the
  higher `coder_id` is preferred, which is a deterministic, easy-to-verify-by-code-
  inspection tie-breaker for the (rare, timestamp-precision-dependent) case of two
  equal deadlines — this happens naturally for every coder's very first request, since
  all coders start with the same deadline (`start_time + time_to_burnout`) before any
  of them has compiled. Since each dongle is only ever contested by its two fixed
  neighboring coders, a coder who loses a round has strictly higher priority on the
  next one under `edf`, preventing indefinite starvation when parameters are feasible.

- **Dongle cooldown**: releasing a dongle does not make it immediately available again.
  `release_dongle` sets `free_at = now + dongle_cooldown`; `acquire_dongle` only grants
  the dongle once `now >= free_at`. Waiting coders re-check this via
  `pthread_cond_timedwait` instead of busy-waiting.

- **Precise burnout detection**: a separate `monitor_thread` polls every 500µs
  (comfortably inside the 10ms tolerance required) and compares
  `now - coder->last_compile_start` against `time_to_burnout` for every coder. As soon
  as a coder misses its deadline, the monitor prints the burnout log, sets a shared
  flag, and broadcasts every dongle's condition variable so that any coder thread stuck
  waiting for a dongle wakes up and exits instead of hanging forever.

- **Log serialization**: every state-change message goes through `print_status`, which
  is the only function allowed to write to stdout, guarded by a dedicated
  `print_mutex`. This mutex is never held at the same time as a dongle mutex in the
  reverse order anywhere in the code, which avoids introducing a lock-ordering
  deadlock between logging and dongle acquisition.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle**: protects `is_taken`, `free_at`, and the dongle's
  waiting-request heap. Held only for the short critical sections that read/modify
  this state — never across a `usleep`/`ft_usleep`.

- **`pthread_cond_t` per dongle**: coders waiting for a dongle block on
  `pthread_cond_timedwait` instead of spinning. They are woken either by
  `release_dongle`'s `pthread_cond_broadcast` (normal turnover) or by the monitor's
  broadcast on burnout/completion (so no thread is left waiting on a dead simulation).
  The timed component lets a waiting coder re-check the cooldown expiry (`free_at`)
  even when nobody actively signals at that exact moment.

- **`state_mutex`**: protects data shared between coder threads and the monitor thread
  that isn't tied to a specific dongle — `burnout_flag`, `completion_flag`,
  `compiles_done`, and `last_compile_start`. This is what makes `simulation_over()` and
  the monitor's deadline checks race-free.

- **`print_mutex`**: serializes all `printf` calls so two state-change lines can never
  interleave.

Race-condition example this prevents: without `state_mutex`, a coder could update
`last_compile_start` (marking the beginning of a new compile) at the same instant the
monitor thread reads it to check for burnout, on ordinary hardware this could tear a
64-bit read/write into two 32-bit halves — with the mutex, the monitor and the coder can
never observe a half-written value. Verified under AddressSanitizer, UndefinedBehaviorSanitizer,
and ThreadSanitizer with no reported races or memory errors across multiple runs
(`fifo`/`edf`, 1 to 5 coders, feasible and intentionally infeasible timing parameters).

## Resources

- [POSIX Threads Programming (LLNL tutorial)](https://hpc-tutorials.llnl.gov/posix/)
- `man pthread_mutex_lock`, `man pthread_cond_timedwait`, `man gettimeofday`
- E. G. Coffman et al., *"System Deadlocks"*, ACM Computing Surveys, 1971 — the four
  conditions used to reason about deadlock prevention in this project.
- E. Dijkstra's dining philosophers problem, as background for the general
  resource-sharing/deadlock scenario this project is modeled after.
- C. L. Liu & J. Layland, *"Scheduling Algorithms for Multiprogramming in a Hard-Real-Time
  Environment"*, 1973 — background on Earliest Deadline First (EDF) scheduling.

### AI usage disclosure

AI (Claude Code) was used during this project for:
- **Debugging a segmentation fault**: reproducing the crash under AddressSanitizer,
  identifying the root cause (`pthread_create` was passed the whole `t_data*` instead
  of the per-coder `t_coder*`, so `coder_routine` dereferenced garbage memory), and
  confirming the fix by re-running the sanitizer build.
- **Design and implementation help** for the dongle acquisition/release logic, the
  custom FIFO/EDF priority heap, the burnout-detection monitor thread, and the memory
  cleanup routines.
- **Verification**: running the resulting code under AddressSanitizer,
  UndefinedBehaviorSanitizer, and ThreadSanitizer, and manually testing edge cases
  (single coder, feasible vs. infeasible timing parameters, both schedulers) to confirm
  correctness before accepting the changes.

All AI-assisted code was read, tested, and is understood well enough to be explained and
modified live during evaluation/defense.
