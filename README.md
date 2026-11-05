<div align="center">

<img src="./clusterforge-banner.svg" alt="ClusterForge — Distributed work. Resilient execution." width="100%">

# ClusterForge

**A fault-tolerant distributed computing system, built for the terminal.**

Split a job. Execute across workers. Recover unfinished tasks when a worker goes offline.

![Stage: System design](https://img.shields.io/badge/stage-system_design-fbbf24?style=flat-square&labelColor=111827)
![Language: C++17/20](https://img.shields.io/badge/language-C%2B%2B17%2F20-67e8f9?style=flat-square&labelColor=111827)
![Architecture: Coordinator–Worker](https://img.shields.io/badge/architecture-Coordinator--Worker-a7f3d0?style=flat-square&labelColor=111827)
![Interface: CLI](https://img.shields.io/badge/interface-CLI-c4b5fd?style=flat-square&labelColor=111827)

[Overview](#overview) · [Architecture](#architecture) · [CLI walkthrough](#cli-walkthrough) · [Failure recovery](#failure-recovery) · [Development](#development)

</div>

---

## Overview

**A worker may fail. The computation should continue.**

ClusterForge is a C++17/20 distributed computing project designed to coordinate computational work across multiple machines. A central Coordinator splits jobs into independent tasks, schedules them on available Workers, and combines their results.

Its core focus is **worker-failure recovery**: detect an unresponsive worker through heartbeat monitoring, return its unfinished tasks to the queue, and assign them to a healthy worker. Completed work should be preserved instead of restarting the entire job.

> **Project stage — system design.** This README describes the intended system. Commands, output, and directory layouts are illustrative; a runnable implementation and verified installation procedure are not included in this documentation package.

## Designed capabilities

| Capability | What it enables |
| :--- | :--- |
| **Distributed execution** | Process one job across multiple worker nodes. |
| **Parallel processing** | Run independent tasks concurrently. |
| **Task scheduling** | Match queued work to available workers. |
| **Worker registration** | Track workers that join the cluster. |
| **Heartbeat monitoring** | Maintain a view of worker availability. |
| **Failure recovery** | Reassign unfinished work after a worker timeout. |
| **Result aggregation** | Collect task outputs and assemble the final result. |
| **Terminal workflow** | Submit jobs, check status, view results, and manage or cancel jobs through a CLI. |

## Architecture

<img src="./clusterforge-architecture.svg" alt="The Client CLI submits jobs to the Coordinator. Its scheduler dispatches tasks to three workers. Workers return results and heartbeats. The Coordinator aggregates results and requeues unfinished tasks after worker timeouts." width="100%">

### Responsibilities

| Component | Responsibility |
| :--- | :--- |
| **Client CLI** | Submit jobs, check job and cluster status, view results, and manage or cancel jobs. |
| **Job Manager** | Divide jobs into tasks and track overall progress. |
| **Task Scheduler & Queue** | Hold pending tasks and dispatch them to available workers. |
| **Worker Registry** | Track registered workers and their availability. |
| **Failure Detector** | Watch heartbeat deadlines and identify unresponsive workers. |
| **Worker Executor** | Execute assigned tasks, return results, and handle task failures and retries under the recovery policy. |
| **Result Manager** | Collect task results and produce the final job output. |

### From submission to result

1. **Submit** — the Client sends a job to the Coordinator.
2. **Partition** — the Job Manager creates smaller, independent tasks.
3. **Schedule** — the Scheduler assigns queued tasks to available workers.
4. **Execute** — workers process tasks in parallel and send heartbeats.
5. **Collect** — the Coordinator records completed task results.
6. **Aggregate** — the Result Manager combines outputs once the required tasks complete.

If a worker becomes unavailable during execution, its unfinished work returns to the scheduling flow. See [Failure recovery](#failure-recovery).

## CLI walkthrough

The proposed CLI keeps the cluster workflow small: start a Coordinator, start workers, submit a job, inspect progress, retrieve results, and cancel jobs.

**Illustrative commands, aligned with the planned CMake targets.** Binary names, output paths, and CLI options are proposed. Run long-lived processes in separate terminals from the project root once the implementation is available.

```bash
# Configure and build (Linux / Ubuntu with a C++17/20 toolchain and CMake)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel

# Terminal 1 · Start the coordinator
./build/bin/coordinator

# Terminals 2–4 · Start one worker per terminal
./build/bin/worker --id worker1
./build/bin/worker --id worker2
./build/bin/worker --id worker3

# Client terminal · Submit work and inspect the cluster
./build/bin/client submit --input data.txt
./build/bin/client status

# Inspect, retrieve, or cancel a submitted job
./build/bin/client status --job job-001
./build/bin/client results --job job-001
./build/bin/client cancel --job job-001
```

Coordinator addresses, ports, input formats, and environment setup will be documented with the implementation.

<details open>
<summary><strong>Example cluster status</strong></summary>

Illustrative output; these counts do not represent a live cluster.

```text
CLUSTERFORGE / CLUSTER STATUS
─────────────────────────────────────
Coordinator    RUNNING

WORKER         STATE
worker1        ONLINE
worker2        ONLINE
worker3        ONLINE

TASKS          COUNT
Pending            2
Running            3
Completed         15
Failed             0
─────────────────────────────────────
```

</details>

## Failure recovery

Consider a job with three tasks. Each worker receives one task, but `worker2` goes offline before returning its result.

<img src="./clusterforge-recovery.svg" alt="Task 2 recovery: worker2 stops responding, the Coordinator detects a heartbeat timeout, Task 2 returns to the queue, and worker3 retries it. Completed Tasks 1 and 3 are retained." width="100%">

| Step | Coordinator action | Intended outcome |
| :--- | :--- | :--- |
| **Detect** | Observe that `worker2` has missed its heartbeat deadline. | Mark the worker unavailable. |
| **Identify** | Find tasks assigned to it without accepted results. | Recover only unfinished work. |
| **Requeue** | Return those tasks to the pending queue. | Make them eligible for scheduling again. |
| **Reassign** | Dispatch `task2` when a healthy worker is available. | Resume progress on the job. |
| **Complete** | Accept the recovered task result and aggregate outputs. | Finish the job without restarting completed tasks. |

<details>
<summary><strong>Example recovery log</strong></summary>

```text
[coordinator] worker2 heartbeat timeout
[coordinator] worker2 marked unavailable
[coordinator] task2 returned to pending queue
[scheduler]   task2 assigned to worker3
[worker3]     task2 completed
[coordinator] all task results collected; aggregating job output
```

This is an illustrative sequence, not captured runtime output.

</details>

### Recovery boundaries

Heartbeat timeouts indicate suspected unavailability: a slow worker or network interruption can look like a failure. A timed-out worker may still finish its original task after reassignment.

The implementation therefore needs explicit decisions about **task identity, duplicate and late results, retry limits, and safe task re-execution**. Exactly-once execution is not established by this design. Workloads should use independent tasks that can be retried safely.

The fault-tolerance scope here is **worker failure**. Coordinator failover and durable recovery after a Coordinator restart are not specified. If no healthy worker is available, recovered tasks must wait for capacity.

## Tech stack

> **C++ • CMake • TCP/IP • Linux • POSIX Sockets • Multithreading • JSON • SSH • Git/GitHub • GoogleTest • GDB • Valgrind**

| Area | Technology | Purpose |
| :--- | :--- | :--- |
| Core implementation | **C++17/20** | Coordinator, Workers, and Client CLI. |
| Build system | **CMake** | Configure and build the C++ components. |
| Platform | **Linux / Ubuntu** | Development and cluster execution environment. |
| Networking | **TCP/IP · POSIX Sockets** | Connect the Coordinator, Workers, and Client across a LAN. |
| Application protocol | **Custom protocol over TCP** | Define registration, jobs, tasks, heartbeats, results, and control messages. |
| Serialization | **JSON / lightweight structured format** | Encode application messages; finalize the format during protocol design. |
| Concurrency | **C++ threads · `std::thread`** | Concurrent communication, monitoring, and task execution. |
| Synchronization | **`std::mutex` · `std::condition_variable` · atomics** | Coordinate shared state and thread activity. |
| Cluster setup | **SSH · Bash** | Access remote workers and script setup and deployment. |
| Unit testing | **GoogleTest** | Validate individual components. |
| Debugging | **GDB · Valgrind** | Debug execution and detect memory errors. |
| System diagnostics | **`top` · `htop` · `ps` · `ss` · `ping`** | Inspect processes, resources, sockets, and network reachability. |
| Version control | **Git + GitHub** | Track changes and collaborate. |

The custom TCP protocol will define message framing, schemas, and failure handling. Worker execution and Coordinator recovery must share a consistent retry policy.

## Development

### Proposed repository layout

```text
ClusterForge/
├── CMakeLists.txt            # Build configuration and executable targets
├── include/clusterforge/     # Shared C++ headers and interfaces
├── src/
│   ├── coordinator/
│   │   ├── main.cpp          # Coordinator entry point
│   │   ├── job_manager.cpp   # Job submission and lifecycle
│   │   ├── scheduler.cpp     # Worker selection and task assignment
│   │   ├── task_queue.cpp    # Pending-task management
│   │   ├── worker_manager.cpp # Registration and worker availability
│   │   ├── failure_detector.cpp # Heartbeat deadlines and recovery
│   │   └── result_manager.cpp # Result collection and aggregation
│   ├── worker/
│   │   ├── main.cpp          # Worker entry point and registration
│   │   ├── executor.cpp      # Task execution and failure handling
│   │   └── heartbeat.cpp     # Periodic liveness reporting
│   ├── client/
│   │   └── main.cpp          # Submit, status, results, and cancel commands
│   └── common/
│       ├── protocol.cpp      # Message framing and serialization
│       └── tcp_socket.cpp    # POSIX socket communication
├── tests/                    # GoogleTest suites and integration scenarios
├── scripts/                  # Bash setup and deployment scripts
├── ./                   # README visuals
└── README.md
```

This is the target layout; the current package contains the README and its SVG ..

### Implementation milestones

- [ ] **Define the contract** — define TCP message framing, serialization, message schemas, task states, and supported input format.
- [ ] **Complete one task end to end** — submit, schedule, execute, and collect a result with one worker.
- [ ] **Distribute a job** — register multiple workers, execute in parallel, and aggregate task results.
- [ ] **Recover interrupted work** — add heartbeats, timeout detection, reassignment, and result deduplication.
- [ ] **Make the system observable** — expose useful cluster status and task lifecycle logs.
- [ ] **Make the demo reproducible** — document the CMake build and SSH/LAN setup, add Bash deployment scripts, and validate failure scenarios.

### Validation targets

| Scenario | Expected behavior to verify |
| :--- | :--- |
| Multiple healthy workers | Distributed output matches a known reference result. |
| Worker stops during a task | Unfinished work is reassigned; completed work is retained. |
| Delayed result after timeout | A duplicate or stale result cannot corrupt aggregation. |
| No available workers | Pending work remains queued until capacity becomes available. |
| A task repeatedly fails | A bounded retry policy produces an inspectable terminal state. |

### Contributing

Useful early contributions include protocol design, scheduling policies, reproducible failure scenarios, and focused tests. Open an issue with the problem, proposed behavior, and validation approach before making a substantial architectural change.

For bug reports, include the workload, worker count, reproduction steps, and relevant Coordinator and Worker logs. Contribution commands and checks will be added once the implementation is available.

## Scope

ClusterForge is a distributed computing learning and engineering project focused on scheduling, communication, worker monitoring, recovery, and aggregation. The CLI keeps development centered on these mechanisms.

A web dashboard, enterprise cloud orchestration, complex authentication, and production-scale operations are outside the initial scope.

**License:** to be selected. Add a `LICENSE` file before publishing licensing or reuse claims.

---

<div align="center">

**Distribute the work. Detect failures. Recover the work. Complete the job.**

[Back to top](#clusterforge)

</div>

