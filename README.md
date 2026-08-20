# uthread-scheduler

A user-space green thread scheduler built from scratch in C++, using `ucontext.h` and POSIX APIs. Implements cooperative round-robin scheduling across N threads with manual stack allocation and context switching — no OS thread involved.

## What it does

- Manual context switching via `getcontext`/`makecontext`/`swapcontext`
- `Thread` struct + `Scheduler` class managing an arbitrary number of green threads
- Round-robin scheduling with finish-tracking, clean exit once all threads complete
- Benchmark harness comparing context-switch latency against native pthreads

## Results

| Metric | Green threads | pthreads |
|---|---|---|
| Avg context-switch latency | ~188 ns (σ ≈ 2 ns) | ~2,100 ns |
| Relative speed | ~11x faster | baseline |

Measured via `std::chrono` batch timing across 1M+ switches per trial, averaged over 70+ trials.

## Methodology notes

- Batch timing used instead of per-call timing — a single switch (~200ns) is too close to clock-call overhead to measure individually.
- Sustained back-to-back runs showed latency climbing from ~188ns to ~250-260ns after ~15-20 consecutive trials, consistent with CPU thermal throttling under sustained load. Reported figures use the stable, non-throttled measurement window.
- pthread comparison uses two `std::thread`s synchronized via `std::mutex` + `std::condition_variable` in a forced ping-pong, timed with identical methodology.

## Build

```
make          # builds uthread, bench_uthread, bench_pthread
./bench_uthread
./bench_pthread
```

## Known limitations

- No preemption — scheduling is fully cooperative, a thread that never yields will hang the scheduler.
- No `delete`/cleanup of allocated `Thread` objects or stacks — acceptable for a short-lived benchmark process, not production-safe.
