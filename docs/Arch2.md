# ClusterForge

ClusterForge is a distributed computing system for running parallel workloads across multiple machines.

It consists of exactly **two applications**:

1. **Coordinator** — central control application that accepts jobs, splits work, schedules tasks, collects results, and manages workers.
2. **Worker** — execution application installed on each compute machine that registers with the Coordinator, receives tasks, executes them, and returns results.

The six supported workloads are modules inside these two applications—not separate software:

- SUM / COUNT
- Prime Calculation
- Parallel Array Processing
- MapReduce Word Count
- Distributed Log Analysis
- Graph Processing

---

## High-Level Architecture

```text
                         ┌─────────────────────┐
                         │     Client / CLI    │
                         │  Submit jobs, view  │
                         │    status/results   │
                         └──────────┬──────────┘
                                    │ TCP/IP
                                    ▼
┌──────────────────────────────────────────────────────────────┐
│                    COORDINATOR APPLICATION                   │
│                                                              │
│  Job Manager → Work-Type Engine → Task Queue → Scheduler     │
│       │               │                           │          │
│       │               ▼                           ▼          │
│       │        Splitter / Aggregator          Worker Manager │
│       │                                          │            │
│       └────────────── Results / Status ─────────┘            │
│                                                              │
│  Heartbeat Monitor + Failure Detector + Recovery Manager      │
└─────────────────────────────┬────────────────────────────────┘
                              │ TCP/IP + Protocol Buffers
                ┌─────────────┼─────────────┐
                ▼             ▼             ▼
       ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
       │   Worker 1  │ │   Worker 2  │ │   Worker N  │
       │ Task Runner │ │ Task Runner │ │ Task Runner │
       └─────────────┘ └─────────────┘ └─────────────┘
```

---

## Core Design Principle

ClusterForge separates generic distributed-system logic from workload-specific logic.

```text
Work Type
    │
    ▼
Workload-specific Splitter
    │
    ▼
Generic Task Queue and Scheduler
    │
    ▼
Workers execute assigned tasks
    │
    ▼
Workload-specific Aggregator
    │
    ▼
Final Job Result
```

The following remain generic for every workload:

- TCP/IP communication
- Worker registration
- Task queueing
- Scheduling
- Heartbeats
- Failure detection
- Retry and reassignment
- Result collection
- Cluster monitoring

Only splitting, execution logic, and aggregation vary by work type.

---

# 1. Coordinator Application

The Coordinator runs on one central machine. It is responsible for controlling the cluster and managing every job from submission to completion.

## Coordinator Modules

```text
Coordinator
├── Network Server
├── Job Management
│   ├── Job Submission
│   ├── Job Validation
│   ├── Job ID Generation
│   ├── Job State Management
│   └── Job Result Delivery
├── Work-Type Engine
│   ├── Workload Registry
│   ├── Splitters
│   └── Aggregators
├── Task Management
│   ├── Task Creation
│   ├── Task Queue
│   ├── Task State Tracking
│   └── Retry Tracking
├── Scheduler
│   ├── Worker Selection
│   ├── Task Assignment
│   ├── Load Awareness
│   └── Reassignment
├── Worker / Cluster Manager
│   ├── Worker Registration
│   ├── Worker Registry
│   ├── Worker Capability Tracking
│   └── Cluster Status
├── Result Manager
│   ├── Partial Result Collection
│   ├── Result Validation
│   └── Final Aggregation
├── Heartbeat Monitor
├── Failure Detector
└── Recovery Manager
    ├── Failed Task Recovery
    ├── Worker Removal
    └── Task Requeueing
```

## Coordinator Responsibilities

| Module | Responsibilities |
|---|---|
| Network Server | Listens for client and Worker TCP connections; receives and sends Protocol Buffer messages. |
| Job Management | Accepts jobs, validates input, creates job IDs, tracks job lifecycle, and returns results. |
| Work-Type Engine | Selects workload logic based on the requested work type. |
| Splitter | Divides one job into small independent tasks. |
| Task Queue | Stores waiting tasks until Workers are available. |
| Scheduler | Assigns queued tasks to suitable available Workers. |
| Worker Manager | Registers Workers, tracks availability, capability, and connection state. |
| Result Manager | Receives partial task results and combines them into one final result. |
| Heartbeat Monitor | Checks that Workers are alive and responsive. |
| Failure Detector | Marks disconnected or unresponsive Workers as failed. |
| Recovery Manager | Requeues unfinished tasks from failed Workers and assigns them elsewhere. |
| Cluster Status | Reports connected Workers, idle/busy state, job progress, and failures. |

## Coordinator Job States

```text
SUBMITTED
   │
   ▼
VALIDATED
   │
   ▼
SPLIT
   │
   ▼
QUEUED
   │
   ▼
RUNNING
   │
   ├── task failure → RETRYING
   │
   ▼
AGGREGATING
   │
   ▼
COMPLETED
```

Possible terminal states:

```text
COMPLETED
FAILED
CANCELLED
```

---

# 2. Worker Application

The Worker runs on every machine that contributes computing power. A Worker does not decide how a job is split; it receives one task at a time from the Coordinator and executes it.

## Worker Modules

```text
Worker
├── Network Client
│   ├── Coordinator Connection
│   ├── Registration Sender
│   ├── Task Receiver
│   └── Result Sender
├── Worker Registration
│   ├── Worker ID
│   ├── Host Information
│   ├── CPU / Memory Information
│   └── Supported Work Types
├── Task Executor
│   ├── Task Decoder
│   ├── Workload Execution Engine
│   ├── Cancellation Support
│   └── Execution Status Reporter
├── Resource Monitor
│   ├── CPU Usage
│   ├── Memory Usage
│   └── Available Capacity
├── Heartbeat Service
├── Local Task State
│   ├── Current Task
│   ├── Task Progress
│   └── Temporary Results
└── Error Handler
    ├── Execution Failure Reporting
    └── Connection Retry
```

## Worker Responsibilities

| Module | Responsibilities |
|---|---|
| Network Client | Maintains a TCP/IP connection with the Coordinator. |
| Registration | Announces the Worker’s identity, address, capacity, and supported workloads. |
| Task Receiver | Waits for task assignments from the Coordinator. |
| Task Executor | Runs the assigned workload task. |
| Resource Monitor | Reports current resource usage and availability. |
| Heartbeat Service | Periodically proves that the Worker is alive. |
| Result Sender | Sends task result, status, and errors to the Coordinator. |
| Local Task State | Tracks the currently running task for safe reporting and recovery. |
| Error Handler | Reports failed execution and reconnects when possible. |

---

# TCP/IP Communication

All communication occurs over TCP/IP. Protocol Buffers define compact, typed messages exchanged between the Coordinator, Workers, and optionally a client CLI.

```text
Client ───── TCP/IP ─────► Coordinator
Worker ───── TCP/IP ─────► Coordinator
Coordinator ─ TCP/IP ────► Worker
```

## Message Types

```text
Client → Coordinator
├── SubmitJobRequest
├── GetJobStatusRequest
├── GetJobResultRequest
└── GetClusterStatusRequest

Worker → Coordinator
├── RegisterWorkerRequest
├── Heartbeat
├── TaskResult
├── TaskFailure
├── ResourceUpdate
└── DisconnectNotice

Coordinator → Worker
├── RegisterWorkerResponse
├── AssignTask
├── CancelTask
├── HeartbeatAcknowledgement
└── ShutdownNotice
```

## Example Protocol Buffer Concepts

```proto
message JobRequest {
  string work_type = 1;
  bytes input_data = 2;
  uint32 chunk_size = 3;
}

message TaskAssignment {
  string job_id = 1;
  string task_id = 2;
  string work_type = 3;
  bytes task_input = 4;
}

message TaskResult {
  string job_id = 1;
  string task_id = 2;
  bool success = 3;
  bytes result_data = 4;
  string error_message = 5;
}

message Heartbeat {
  string worker_id = 1;
  uint32 cpu_usage_percent = 2;
  uint32 memory_usage_percent = 3;
  bool is_busy = 4;
}
```

---

# Worker Registration Flow

A Worker must register before receiving work.

```text
Worker starts
    │
    ▼
Connect to Coordinator TCP address
    │
    ▼
Send RegisterWorkerRequest
    │
    ├── Worker ID
    ├── Hostname / IP
    ├── CPU cores
    ├── Memory information
    └── Supported work types
    │
    ▼
Coordinator validates and stores Worker
    │
    ▼
Coordinator sends RegisterWorkerResponse
    │
    ▼
Worker becomes AVAILABLE
```

The Coordinator maintains a Worker registry:

```text
Worker Registry
├── Worker ID
├── Network address
├── Connection status
├── Last heartbeat time
├── Available / busy state
├── Current task
├── CPU and memory usage
└── Supported workloads
```

---

# Job Submission and Execution Flow

```text
1. Client submits a job to the Coordinator.
2. Coordinator validates the selected work type and input.
3. Coordinator creates a unique Job ID.
4. Work-Type Engine selects the matching splitter.
5. Splitter divides the job into independent tasks.
6. Tasks enter the generic task queue.
7. Scheduler selects available Workers.
8. Coordinator sends each Worker a task assignment.
9. Workers execute their assigned tasks.
10. Workers return partial results.
11. Result Manager validates and stores results.
12. Aggregator combines partial results.
13. Coordinator marks the job as completed.
14. Client retrieves final result and job status.
```

```text
Client
  │ SubmitJob(work_type, input)
  ▼
Coordinator
  │ Validate + create Job ID
  ▼
Work-Type Splitter
  │ Create tasks
  ▼
Task Queue
  │
  ▼
Scheduler ───────────────► Workers
                              │
                              ▼
                       Execute task
                              │
                              ▼
Coordinator Result Manager
  │
  ▼
Work-Type Aggregator
  │
  ▼
Final Result
```

---

# Supported Workloads

These workloads are implemented as workload modules within the same Coordinator and Worker applications.

## 1. SUM / COUNT

Use case: sum a large numeric dataset or count records/items.

```text
Input: [1, 2, 3, ..., N]

Coordinator:
  Split input into chunks.

Workers:
  Calculate partial sum or count.

Aggregator:
  Adds partial sums or counts.
```

```text
Final Sum = partial_sum_1 + partial_sum_2 + ... + partial_sum_N
```

## 2. Prime Calculation

Use case: find prime numbers or count primes within a large numeric range.

```text
Input: 1 to 10,000,000

Coordinator:
  Splits the range into smaller ranges.

Workers:
  Test numbers in their assigned range.

Aggregator:
  Merges prime lists or adds prime counts.
```

## 3. Parallel Array Processing

Use case: transform, filter, sort portions of, or calculate statistics on a large array.

```text
Input: large array

Coordinator:
  Splits the array by index range.

Workers:
  Process assigned elements.

Aggregator:
  Reassembles output in original order or merges statistics.
```

## 4. MapReduce Word Count

Use case: count every word in a large file or collection of text files.

```text
Input: large text dataset

Coordinator:
  Splits files into safe text chunks.

Workers:
  Map each chunk into (word, count) pairs.

Aggregator:
  Reduces identical keys by summing counts.
```

```text
"hello world hello"
      │
      ▼
hello → 2
world → 1
```

## 5. Distributed Log Analysis

Use case: analyze application or server logs for errors, event counts, IP addresses, response times, or patterns.

```text
Coordinator:
  Splits logs by file or time range.

Workers:
  Parse assigned log chunks and generate partial metrics.

Aggregator:
  Combines counts, errors, alerts, and summaries.
```

## 6. Graph Processing

Use case: graph traversal, connected components, degree calculations, shortest-path stages, or other graph algorithms.

```text
Coordinator:
  Partitions vertices or edges.

Workers:
  Process assigned graph partitions.

Aggregator:
  Merges graph state or combines algorithm results.
```

Graph workloads may require multiple processing rounds. The Coordinator manages each round, distributes the next tasks, and continues until the algorithm completes.

---

# Scheduling and Task Queue

The task queue stores tasks that are ready to run.

```text
Job Splitter
    │
    ▼
┌──────────────────────────┐
│        TASK QUEUE        │
│  Task A                  │
│  Task B                  │
│  Task C                  │
│  ...                     │
└────────────┬─────────────┘
             │
             ▼
        Scheduler
             │
             ├── Worker 1 available → Task A
             ├── Worker 2 available → Task B
             └── Worker 3 available → Task C
```

A practical first scheduler can use:

- First available Worker
- FIFO task queue
- One task per Worker
- Reassignment after failure

Later improvements can include:

- Load-aware scheduling
- Priority jobs
- Worker capability matching
- Data-locality awareness
- Multiple concurrent task slots per Worker

---

# Heartbeat, Failure Detection, and Recovery

Workers periodically send heartbeats.

```text
Worker ── Heartbeat ──► Coordinator
Worker ── Heartbeat ──► Coordinator
Worker ── Heartbeat ──► Coordinator
```

The Coordinator records the last heartbeat time for each Worker.

```text
if current_time - last_heartbeat > timeout:
    mark worker as FAILED
```

## Failure Recovery Flow

```text
Worker running Task X
        │
        ▼
Worker stops responding
        │
        ▼
Coordinator heartbeat timeout
        │
        ▼
Worker marked FAILED
        │
        ▼
Unfinished Task X returned to Task Queue
        │
        ▼
Scheduler assigns Task X to another available Worker
        │
        ▼
Job continues
```

Task state example:

```text
PENDING → ASSIGNED → RUNNING → COMPLETED
                       │
                       └── FAILED → REQUEUED → ASSIGNED
```

The Coordinator should prevent duplicate final results by accepting the first valid completion for a task and ignoring late duplicate results.

---

# Cluster Status

The Coordinator exposes cluster information for a CLI, dashboard, or API.

```text
Cluster Status
├── Connected Workers
├── Available Workers
├── Busy Workers
├── Failed Workers
├── Active Jobs
├── Queued Tasks
├── Running Tasks
├── Completed Tasks
└── Failed / Retried Tasks
```

Example:

```text
Workers: 5 connected, 3 available, 2 busy
Jobs: 2 active, 14 completed
Tasks: 8 queued, 4 running, 56 completed
Health: Worker-4 heartbeat timeout
```

---

# Deployment and Network Setup

## Coordinator Machine

The Coordinator machine needs:

- Linux/Ubuntu recommended
- Static IP address or stable hostname
- Open TCP port, for example `5000`
- Access from all Worker machines
- Sufficient storage for job metadata and results

Example:

```text
Coordinator address: 192.168.1.10:5000
```

## Worker Machines

Each Worker needs:

- ClusterForge Worker executable
- Network access to the Coordinator
- Configured Coordinator IP address and port
- Permission through firewall rules
- CPU and memory appropriate for expected workloads

Example Worker startup:

```bash
./clusterforge-worker \
  --coordinator-host 192.168.1.10 \
  --coordinator-port 5000 \
  --worker-id worker-01
```

Example Coordinator startup:

```bash
./clusterforge-coordinator --port 5000
```

## Network Requirements

```text
Workers ───────── TCP port 5000 ─────────► Coordinator
```

- Use a private LAN, VPN, or secured cloud network.
- Restrict Coordinator ports with firewall rules.
- Use TLS and authentication when deploying outside a trusted network.
- Give every Worker a stable unique Worker ID.

---

# Suggested Repository Structure

```text
clusterforge/
├── README.md
├── CMakeLists.txt
├── proto/
│   ├── clusterforge.proto
│   ├── job.proto
│   ├── task.proto
│   └── worker.proto
├── coordinator/
│   ├── main.cpp
│   ├── network/
│   │   └── coordinator_server.cpp
│   ├── jobs/
│   │   ├── job_manager.cpp
│   │   ├── job_registry.cpp
│   │   └── job_validator.cpp
│   ├── workloads/
│   │   ├── workload_registry.cpp
│   │   ├── sum_count.cpp
│   │   ├── prime.cpp
│   │   ├── array_processing.cpp
│   │   ├── word_count.cpp
│   │   ├── log_analysis.cpp
│   │   └── graph_processing.cpp
│   ├── tasks/
│   │   ├── task_queue.cpp
│   │   ├── task_manager.cpp
│   │   └── scheduler.cpp
│   ├── workers/
│   │   ├── worker_manager.cpp
│   │   ├── heartbeat_monitor.cpp
│   │   └── failure_detector.cpp
│   └── recovery/
│       └── recovery_manager.cpp
├── worker/
│   ├── main.cpp
│   ├── network/
│   │   └── worker_client.cpp
│   ├── execution/
│   │   ├── task_executor.cpp
│   │   └── workload_executor.cpp
│   ├── monitoring/
│   │   ├── heartbeat_service.cpp
│   │   └── resource_monitor.cpp
│   └── state/
│       └── local_task_state.cpp
├── common/
│   ├── models/
│   ├── networking/
│   ├── threading/
│   ├── logging/
│   └── configuration/
├── tests/
│   ├── coordinator/
│   ├── worker/
│   ├── workloads/
│   └── integration/
├── scripts/
│   ├── start_coordinator.sh
│   └── start_worker.sh
└── docker/
    ├── Dockerfile.coordinator
    ├── Dockerfile.worker
    └── docker-compose.yml
```

---

# Technology Stack

| Area | Technology |
|---|---|
| Language | C++ |
| Network communication | TCP/IP sockets |
| Message format | Protocol Buffers |
| Concurrency | `std::thread`, `std::mutex`, `std::condition_variable`, `std::atomic` |
| Build system | CMake |
| Operating system | Linux / Ubuntu |
| Testing | GoogleTest |
| Version control | Git and GitHub |
| Optional deployment | Docker and Docker Compose |

## C++ Concurrency Usage

```text
std::thread
  Run network handlers, heartbeat monitoring, scheduling, and task execution.

std::mutex
  Protect shared job, task, Worker, and result state.

std::condition_variable
  Wake scheduler threads when new tasks enter the queue or Workers become available.

std::atomic
  Store lightweight thread-safe flags, counters, health states, and shutdown signals.
```

---

# Development Order

A practical implementation order is:

```text
1. Shared Protocol Buffer messages
2. Basic Coordinator TCP server
3. Basic Worker TCP client and registration
4. Heartbeat and Worker registry
5. Job submission and job state tracking
6. Task queue and FIFO scheduler
7. SUM / COUNT workload
8. Result collection and aggregation
9. Failure detection and task reassignment
10. Prime and Array Processing workloads
11. Word Count and Log Analysis workloads
12. Graph Processing rounds
13. GoogleTest and integration tests
14. Docker-based deployment
15. Authentication, TLS, persistence, and monitoring improvements
```

---

# Summary

```text
ClusterForge
├── Coordinator Application
│   ├── Controls the cluster
│   ├── Accepts and splits jobs
│   ├── Queues and schedules tasks
│   ├── Collects and aggregates results
│   └── Detects failures and recovers tasks
│
└── Worker Application
    ├── Registers with Coordinator
    ├── Sends heartbeats
    ├── Receives and executes tasks
    ├── Reports resource usage
    └── Returns results or failures
```

ClusterForge therefore remains simple at the application level—two programs—while supporting multiple distributed workloads through reusable scheduling, communication, monitoring, and recovery infrastructure.