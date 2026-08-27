# Fault-Tolerant Distributed Computing System

> A lightweight distributed computing platform that connects multiple physical computers over a local network and makes them work together as a single logical computing cluster.

**Project Type:** Distributed Systems / Cloud Computing  
**Primary Focus:** Distributed Computing, Fault Tolerance, Task Scheduling, Networking  
**Deployment:** College Computer Lab / LAN  
**Interface:** CLI-first  
**Target:** 3–10+ physical computers

---

# 1. Project Overview

The **Fault-Tolerant Distributed Computing System** is a distributed computing platform designed to use multiple ordinary computers connected through a local network as a single logical computing system.

Instead of executing a large computational workload on one computer, the system divides the workload into smaller tasks and distributes those tasks among multiple worker machines.

The system contains:

- A **Master/Controller Node**
- Multiple **Worker Nodes**
- A **Job Scheduler**
- A **Task Manager**
- A **Heartbeat/Health Monitoring System**
- A **Failure Detection Mechanism**
- A **Task Recovery/Rescheduling Mechanism**
- A **Result Aggregation System**

The project intentionally uses a **CLI-first approach** rather than spending significant development time on a large frontend.

The goal is to demonstrate the actual concepts of distributed computing through a working multi-machine system.

---

# 2. Problem Statement

A single computer has limited:

- CPU
- RAM
- Processing capacity
- Availability
- Fault tolerance

Suppose a computational task contains 1,000,000 independent operations.

A traditional approach would be:

```text
Large Job
    |
    v
One Computer
    |
    v
Process Everything
```

This creates several problems:

1. Processing can take a long time.
2. Available computing resources on other machines remain unused.
3. If the computer fails, the entire job may fail.
4. The system cannot easily scale by adding additional computers.

The proposed system addresses these problems by distributing computational tasks across multiple machines.

---

# 3. Proposed Solution

The system creates a logical cluster from multiple physical computers.

```text
                    ┌──────────────────────┐
                    │      MASTER NODE     │
                    │                      │
                    │  Job Manager         │
                    │  Scheduler           │
                    │  Node Manager        │
                    │  Failure Detector    │
                    └──────────┬───────────┘
                               │
                         Local Network
                               │
             ┌─────────────────┼─────────────────┐
             │                 │                 │
             ▼                 ▼                 ▼
       ┌───────────┐     ┌───────────┐     ┌───────────┐
       │  Worker 1 │     │  Worker 2 │     │  Worker 3 │
       │           │     │           │     │           │
       │ Executor  │     │ Executor  │     │ Executor  │
       │ Monitor   │     │ Monitor   │     │ Monitor   │
       └───────────┘     └───────────┘     └───────────┘
```

The user submits a job to the Master.

The Master:

1. Receives the job.
2. Splits it into tasks.
3. Checks available workers.
4. Schedules tasks.
5. Sends tasks to workers.
6. Monitors execution.
7. Detects failed workers.
8. Reschedules unfinished tasks.
9. Collects results.
10. Produces the final result.

---

# 4. Project Objectives

## Primary Objectives

### Objective 1 — Distributed Execution

Execute a single large workload across multiple physical computers.

### Objective 2 — Resource Utilization

Use CPU resources from multiple machines instead of relying on one computer.

### Objective 3 — Task Scheduling

Automatically decide which worker should receive each task.

### Objective 4 — Fault Detection

Detect when a worker becomes unavailable.

### Objective 5 — Fault Tolerance

Recover unfinished work after worker failure.

### Objective 6 — Scalability

Allow additional worker machines to join the cluster.

### Objective 7 — Performance Analysis

Measure how execution time changes as the number of worker nodes increases.

---

# 5. Core Architecture

The system follows a **Master–Worker distributed architecture**.

```text
                         USER
                          |
                          v
                  ┌───────────────┐
                  │  CLI Client   │
                  └───────┬───────┘
                          |
                          v
                  ┌───────────────┐
                  │ Master Node   │
                  ├───────────────┤
                  │ Job Manager   │
                  │ Scheduler     │
                  │ Node Manager  │
                  │ Failure       │
                  │ Detector      │
                  └───────┬───────┘
                          |
             ┌────────────┼────────────┐
             |            |            |
             v            v            v
        ┌────────┐   ┌────────┐   ┌────────┐
        │Worker 1│   │Worker 2│   │Worker 3│
        ├────────┤   ├────────┤   ├────────┤
        │Executor│   │Executor│   │Executor│
        │Monitor │   │Monitor │   │Monitor │
        └────────┘   └────────┘   └────────┘
```

---

# 6. Components

## 6.1 Master Node

The Master is the central coordinator.

Responsibilities:

- Accept jobs
- Maintain worker registry
- Monitor workers
- Create tasks
- Schedule tasks
- Track task state
- Detect failures
- Reschedule failed tasks
- Aggregate results

The Master does not perform the heavy computation itself.

Its primary role is **coordination**.

---

# 6.2 Worker Node

A Worker is an ordinary computer connected to the same network.

Each worker runs a lightweight worker service.

Example:

```text
Worker ID: worker-03
IP: 192.168.1.23
CPU: 8 cores
RAM: 16 GB
Status: READY
```

Responsibilities:

- Register with Master
- Send heartbeat
- Receive tasks
- Execute tasks
- Return results
- Report task status

---

# 6.3 Scheduler

The scheduler determines where tasks should execute.

Initial implementation:

### Round Robin

```text
Task 1 → Worker 1
Task 2 → Worker 2
Task 3 → Worker 3
Task 4 → Worker 1
Task 5 → Worker 2
```

Later implementation:

### Resource-Aware Scheduling

Example:

```text
Worker 1 → CPU 90%
Worker 2 → CPU 25%
Worker 3 → CPU 45%
```

A new task would preferably be assigned to Worker 2.

This allows the project to demonstrate different scheduling strategies.

---

# 6.4 Job Manager

The Job Manager handles the lifecycle of submitted jobs.

Example:

```text
Job #1001

QUEUED
   ↓
SCHEDULED
   ↓
RUNNING
   ↓
COMPLETED
```

Possible states:

```text
QUEUED
RUNNING
COMPLETED
FAILED
RECOVERING
CANCELLED
```

---

# 6.5 Task Manager

A large job is divided into smaller tasks.

Example:

```text
Job #1001
     |
     +---- Task 1
     +---- Task 2
     +---- Task 3
     +---- Task 4
```

The Master tracks every task individually.

Example:

```text
Task 1 → Worker 1 → COMPLETED
Task 2 → Worker 2 → COMPLETED
Task 3 → Worker 3 → FAILED
Task 4 → Worker 1 → COMPLETED
```

Task 3 can then be rescheduled.

---

# 6.6 Heartbeat System

Every worker periodically sends a heartbeat to the Master.

```text
Worker 1 → HEARTBEAT
Worker 2 → HEARTBEAT
Worker 3 → HEARTBEAT
```

The Master maintains:

```text
Worker 1 → ONLINE
Worker 2 → ONLINE
Worker 3 → ONLINE
```

If a worker stops responding:

```text
No heartbeat
      ↓
Timeout
      ↓
Failure suspected
      ↓
Worker marked OFFLINE
```

---

# 6.7 Failure Detector

The failure detector monitors worker availability.

A simple mechanism:

```text
last_heartbeat = current_time

IF
current_time - last_heartbeat > timeout

THEN
worker = FAILED
```

For example:

```text
Heartbeat interval: 2 seconds
Failure timeout: 6 seconds
```

If no heartbeat is received for 6 seconds, the worker can be considered unavailable.

The exact values should be experimentally evaluated rather than treated as universal defaults.

---

# 6.8 Task Recovery

When a worker fails, the Master checks which tasks were running on that worker.

Example:

```text
Worker 2

Task 10 → COMPLETED
Task 11 → RUNNING
Task 12 → RUNNING
```

Worker 2 fails.

The Master determines:

```text
Task 10 → Do nothing
Task 11 → Reschedule
Task 12 → Reschedule
```

Then:

```text
Task 11 → Worker 1
Task 12 → Worker 3
```

This is the core fault-tolerance mechanism.

---

# 6.9 Result Aggregator

After workers complete their tasks, the Master collects the results.

```text
Worker 1 → Result 1
Worker 2 → Result 2
Worker 3 → Result 3
                 |
                 v
          Result Aggregator
                 |
                 v
            Final Result
```

---

# 7. End-to-End Workflow

```text
START
  |
  v
User submits job
  |
  v
Master receives job
  |
  v
Validate job
  |
  v
Split job into tasks
  |
  v
Check available workers
  |
  v
Scheduler selects workers
  |
  v
Send tasks
  |
  v
Workers execute tasks
  |
  +-------------------+
  |                   |
  v                   v
Success             Failure
  |                   |
  v                   v
Return result      Detect failure
                      |
                      v
                 Find unfinished tasks
                      |
                      v
                 Reschedule tasks
                      |
                      v
                 Continue execution
  |
  v
Aggregate results
  |
  v
Return final result
  |
  v
END
```

---

# 8. Distributed Computing Algorithm

## Algorithm

```text
1. Start Master.

2. Start Worker Nodes.

3. Each Worker registers with Master.

4. Master maintains worker state.

5. User submits a Job.

6. Master validates the Job.

7. Master divides Job into Tasks.

8. Scheduler selects available Workers.

9. Master assigns Tasks to Workers.

10. Workers execute Tasks concurrently.

11. Workers periodically send Heartbeats.

12. Workers return Task Results.

13. Master marks completed Tasks.

14. If Worker heartbeat times out:
       mark Worker as FAILED.

15. Find Tasks assigned to failed Worker.

16. Mark unfinished Tasks as RECOVERABLE.

17. Scheduler assigns those Tasks to healthy Workers.

18. Workers execute recovered Tasks.

19. Master waits until all required Tasks complete.

20. Aggregator combines Task Results.

21. Return Final Result.

END
```

---

# 9. Fault-Tolerance Algorithm

```text
FOR each worker:

    monitor heartbeat

    IF heartbeat timeout:

        mark worker OFFLINE

        FOR each task assigned to worker:

            IF task != COMPLETED:

                mark task RECOVERABLE

                place task in scheduler queue

        scheduler assigns task to healthy worker

        continue execution
```

---

# 10. Scheduling Algorithm

## Version 1 — Round Robin

```text
workers = [W1, W2, W3]

Task 1 → W1
Task 2 → W2
Task 3 → W3
Task 4 → W1
Task 5 → W2
```

This is simple and provides a baseline.

## Version 2 — Least Loaded

For every new task:

```text
Select worker with lowest load.
```

Example:

```text
W1 → 85%
W2 → 30%
W3 → 55%

Task → W2
```

## Version 3 — Resource-Aware

Calculate a worker score using:

```text
CPU utilization
RAM utilization
Active task count
Queue length
```

Then select the worker with the best available capacity.

---

# 11. Task Lifecycle

```text
             ┌──────────┐
             │  QUEUED  │
             └────┬─────┘
                  |
                  v
             ┌──────────┐
             │SCHEDULED │
             └────┬─────┘
                  |
                  v
             ┌──────────┐
             │ RUNNING  │
             └────┬─────┘
                  |
             ┌────┴────┐
             |         |
             v         v
        COMPLETED    FAILED
                       |
                       v
                   RECOVERING
                       |
                       v
                   SCHEDULED
```

---

# 12. Communication Model

All machines communicate over the college LAN.

```text
Worker 1 ───────┐
Worker 2 ───────┤
Worker 3 ───────┼──── LAN ──── Master
Worker 4 ───────┤
Worker 5 ───────┘
```

Communication can be implemented using:

- TCP sockets
- HTTP/REST
- gRPC

For a small academic implementation, either **TCP-based communication** or **gRPC** would be a strong choice.

A simple REST implementation is also possible, but the project should clearly demonstrate distributed communication rather than hide everything behind a conventional web API.

---

# 13. Suggested Communication Protocol

Example Worker registration:

```text
REGISTER_WORKER

worker_id
ip_address
port
cpu_cores
memory
```

Master response:

```text
REGISTERED
worker_id
heartbeat_interval
```

Task assignment:

```text
TASK_ASSIGN

job_id
task_id
task_type
task_data
```

Worker response:

```text
TASK_RESULT

job_id
task_id
status
result
execution_time
```

Heartbeat:

```text
HEARTBEAT

worker_id
timestamp
cpu_usage
memory_usage
active_tasks
```

---

# 14. Recommended Workload

The first workload should be something that can be split cleanly.

## Recommended: Distributed Data Processing

Example:

```text
Large CSV
    |
    v
Split into chunks
    |
    +---- Worker 1
    +---- Worker 2
    +---- Worker 3
    +---- Worker 4
    |
    v
Aggregate results
```

Possible tasks:

- Word counting
- Number analysis
- CSV aggregation
- Prime number calculation
- Matrix operations
- Log analysis
- Image processing

For the first working version, **word count or CSV aggregation** is enough.

Once the infrastructure works, more computationally expensive workloads can be added.

---

# 15. Example Demonstration

Suppose the system has four workers.

```text
worker-01   ONLINE
worker-02   ONLINE
worker-03   ONLINE
worker-04   ONLINE
```

Submit:

```text
job submit dataset.csv
```

Master:

```text
Job ID: 1001

Created Tasks:

Task 1 → worker-01
Task 2 → worker-02
Task 3 → worker-03
Task 4 → worker-04
```

All workers process simultaneously.

Now intentionally shut down worker-03.

Master detects:

```text
worker-03 → OFFLINE

Heartbeat timeout detected.
```

Then:

```text
Task 3 → RECOVERABLE

Rescheduling Task 3...

Task 3 → worker-01
```

Final:

```text
Job 1001

Task 1 → COMPLETED
Task 2 → COMPLETED
Task 3 → RECOVERED
Task 4 → COMPLETED

FINAL RESULT → SUCCESS
```

This is the primary project demonstration.

---

# 16. Performance Evaluation

The project should not only demonstrate that distributed computing works.

It should measure whether it provides a benefit.

## Experiment 1 — Node Scaling

Run the same workload using:

```text
1 node
2 nodes
4 nodes
6 nodes
8 nodes
```

Record:

```text
Execution Time
Speedup
Efficiency
Throughput
```

Speedup:

```text
Speedup = T1 / Tp
```

where:

- T1 = execution time using one node
- Tp = execution time using p nodes

Efficiency:

```text
Efficiency = Speedup / Number of Nodes
```

This gives the project an experimental/research component.

---

# 17. Experiment 2 — Fault Recovery

Measure:

```text
Failure Time
    ↓
Failure Detection Time
    ↓
Task Rescheduling Time
    ↓
Recovery Time
    ↓
Final Completion
```

Example results:

```text
Failure Detection: 5.8 sec
Task Recovery:     1.2 sec
Job Completion:    SUCCESS
```

These values should be measured from your implementation rather than invented beforehand.

---

# 18. Experiment 3 — Scheduling Algorithms

Compare:

```text
Round Robin
        vs
Least Loaded
        vs
Resource Aware
```

Measure:

- Average execution time
- Worker utilization
- Task distribution
- Completion time

This can become a strong part of the project report.

---

# 19. Technology Stack

## Recommended Stack

### Programming Language

**Python**

Why:

- Fast development
- Excellent networking libraries
- Easy system monitoring
- Easy multiprocessing
- Easy data processing
- Large ecosystem
- Suitable for a time-limited academic project

### Communication

Choose one:

**gRPC** — recommended for a structured distributed system.

or

**TCP sockets** — excellent if the goal is to demonstrate networking fundamentals directly.

### Concurrency

Python:

```text
multiprocessing
concurrent.futures
threading
```

depending on the workload.

### Monitoring

Possible libraries:

```text
psutil
```

for:

- CPU
- RAM
- disk
- network
- process information

### Storage

Start simple:

```text
SQLite
```

or JSON/local files for early development.

The distributed system itself should not depend on a complex database.

### Operating System

**Linux / Ubuntu**

Recommended because:

- Easy SSH access
- Strong networking tools
- Good process management
- Good system monitoring
- Natural environment for server-style workloads

The project can also run on Windows, but Linux is preferable for the lab deployment.

### Version Control

**Git + GitHub**

---

# 20. Software Architecture

A possible repository structure:

```text
distributed-computing-system/
│
├── master/
│   ├── scheduler/
│   ├── job_manager/
│   ├── node_manager/
│   ├── failure_detector/
│   └── aggregator/
│
├── worker/
│   ├── executor/
│   ├── heartbeat/
│   └── resource_monitor/
│
├── common/
│   ├── protocol/
│   ├── models/
│   └── utilities/
│
├── workloads/
│   ├── wordcount/
│   ├── csv_processing/
│   └── benchmark/
│
├── tests/
│
├── docs/
│
├── requirements.txt
└── README.md
```

This structure can change during implementation. The architecture is more important than preserving this exact directory layout.

---

# 21. Hardware Requirements

You do not need expensive hardware.

## Minimum

```text
Master: 1 PC
Workers: 2 PCs
```

Total:

```text
3 machines
```

## Recommended

```text
Master: 1
Workers: 4–8
```

## Network

All machines should be connected to the same LAN.

Example:

```text
192.168.1.10 → Master
192.168.1.11 → Worker 1
192.168.1.12 → Worker 2
192.168.1.13 → Worker 3
```

---

# 22. Why Physical Lab Machines?

Using physical machines makes the project more demonstrable.

Instead of simulating:

```text
Node 1
Node 2
Node 3
```

you can actually have:

```text
PC 1
PC 2
PC 3
```

connected through the lab network.

This allows you to demonstrate:

- Real network communication
- Real CPU utilization
- Real resource contention
- Real node failures
- Real recovery
- Real scalability

---

# 23. Why Not a Large Frontend?

The project intentionally follows a **CLI-first architecture**.

The main objective is to demonstrate:

```text
Distributed Computing
Networking
Scheduling
Fault Tolerance
Parallel Execution
```

not frontend development.

Example:

```text
$ cluster status

MASTER       ONLINE

WORKERS
worker-01    ONLINE    CPU: 31%
worker-02    ONLINE    CPU: 48%
worker-03    ONLINE    CPU: 22%
worker-04    OFFLINE
```

Job submission:

```text
$ job submit dataset.csv
```

Status:

```text
$ job status 1001

Task 1    worker-01    COMPLETED
Task 2    worker-02    COMPLETED
Task 3    worker-03    RECOVERING
Task 4    worker-04    COMPLETED
```

This is sufficient for a strong technical demonstration.

---

# 24. Key Features

## Core Features

- Multi-node cluster
- Master-worker architecture
- Worker registration
- Worker discovery
- Heartbeat monitoring
- CPU/RAM monitoring
- Job submission
- Task splitting
- Task scheduling
- Parallel execution
- Result aggregation
- Failure detection
- Task rescheduling
- Fault recovery
- Cluster status
- Job status
- Performance benchmarking

## Advanced Features

- Resource-aware scheduling
- Priority-based jobs
- Dynamic worker registration
- Worker graceful shutdown
- Retry limits
- Task timeout
- Job cancellation
- Multiple workload types
- Docker-based deployment
- Authentication between nodes

Advanced features should only be added after the core system is stable.

---

# 25. Security Considerations

The initial system operates inside a controlled college LAN.

Nevertheless, basic security should be considered.

Possible protections:

- Worker authentication
- Shared secret/token
- Message validation
- Input validation
- Restricted ports
- TLS for communication
- Task validation
- Maximum task size
- Execution timeout

Security should not become the primary scope of this project.

---

# 26. Failure Scenarios

The project should test several failures.

### Scenario 1 — Worker crashes

```text
Worker → CRASH
        ↓
Heartbeat timeout
        ↓
Failure detected
        ↓
Tasks recovered
```

### Scenario 2 — Network interruption

```text
Worker
   X
Master
```

Master eventually detects the missing heartbeat.

### Scenario 3 — Worker becomes overloaded

```text
CPU → 100%
```

Scheduler avoids sending unnecessary additional tasks.

### Scenario 4 — Task failure

```text
Task → FAILED
```

System retries or reschedules the task depending on configured policy.

---

# 27. What Makes This a Distributed System?

This is an important question for the viva.

The project is not simply a program running on multiple computers.

It demonstrates several defining distributed-system concepts:

### Multiple independent machines

Each worker has its own:

- CPU
- RAM
- operating system
- process state

### Network communication

Machines coordinate through the network.

### No shared physical memory

Workers communicate through messages rather than shared RAM.

### Distributed execution

Different machines execute different parts of the same workload.

### Coordination

The Master coordinates independent nodes.

### Failure handling

Machines can fail independently.

### Partial failure

One worker can fail while the rest of the system continues.

### Scalability

More worker nodes can increase available computing resources.

These are core distributed-system concepts.

---

# 28. Cloud Computing Relationship

Although this is not intended to become a complete commercial cloud platform, it demonstrates several cloud-computing principles.

```text
Physical Machines
       ↓
Resource Pool
       ↓
Workload Scheduling
       ↓
Compute Allocation
       ↓
Parallel Execution
       ↓
Fault Recovery
```

Concepts demonstrated:

- Resource pooling
- On-demand computation
- Scalability
- Availability
- Load balancing
- Fault tolerance
- Distributed resource management

The project can therefore be presented as a **small-scale private distributed computing environment**.

---

# 29. What This Project Is NOT

To keep the project achievable, it is explicitly not:

- A Kubernetes replacement
- A complete AWS replacement
- A complete OpenStack replacement
- A distributed database
- A distributed filesystem
- A full container orchestration platform
- A complete enterprise cloud
- A distributed ML training framework

The project focuses on:

> **Distributed task execution + scheduling + fault tolerance.**

This scope is intentional.

---

# 30. Development Roadmap

## Phase 1 — Basic Worker

Build a Worker that:

```text
Starts
 ↓
Listens
 ↓
Receives task
 ↓
Executes task
 ↓
Returns result
```

Estimated: **1–2 days**

---

## Phase 2 — Master

Implement:

```text
Master
 ↓
Worker registry
 ↓
Task assignment
```

Estimated: **1–2 days**

---

## Phase 3 — Real LAN Communication

Connect:

```text
Master
   |
 LAN
   |
Workers
```

Test using 2–3 machines.

Estimated: **1–2 days**

---

## Phase 4 — Job Splitting

Implement:

```text
Job
 ↓
Task 1
Task 2
Task 3
Task 4
```

Estimated: **1–2 days**

---

## Phase 5 — Scheduling

Implement:

```text
Round Robin
```

Then optionally:

```text
Least Loaded
```

Estimated: **1–2 days**

---

## Phase 6 — Heartbeat

Implement:

```text
Worker → Heartbeat → Master
```

Estimated: **1 day**

---

## Phase 7 — Fault Detection

Implement:

```text
Heartbeat timeout
      ↓
Worker failed
```

Estimated: **1 day**

---

## Phase 8 — Recovery

Implement:

```text
Failed Worker
      ↓
Unfinished Tasks
      ↓
Scheduler Queue
      ↓
Healthy Worker
```

Estimated: **1–2 days**

---

## Phase 9 — Benchmarking

Test:

```text
1 node
2 nodes
4 nodes
6 nodes
```

Measure performance.

Estimated: **1–2 days**

---

## Phase 10 — Documentation + Demo

Prepare:

- Architecture diagram
- Algorithms
- Performance graphs
- Failure demonstration
- Viva explanation
- README
- Project report

Estimated: **2–3 days**

---

# 31. Estimated Total Time

### Minimum working system

Approximately:

**7–10 days**

This gives you:

- Master
- Workers
- LAN communication
- Job execution
- Basic scheduling
- Heartbeat
- Failure detection
- Basic recovery

### Strong academic version

Approximately:

**12–18 days**

Adds:

- Better scheduling
- Resource monitoring
- Multiple workloads
- Benchmarks
- Performance analysis
- Improved recovery
- Documentation
- Testing

### Polished version

Approximately:

**3–4 weeks**

Adds:

- Resource-aware scheduler
- Priority queues
- Better retry policies
- Docker deployment
- Security
- Extensive benchmarks
- Better CLI
- More comprehensive testing

For your situation, the **12–18 day version is the sweet spot**.

Do not spend 4 weeks building a fancy frontend.

---

# 32. Recommended Priority

If time becomes limited, implement features in this order:

```text
1. Worker
        ↓
2. Master
        ↓
3. Network communication
        ↓
4. Job execution
        ↓
5. Task splitting
        ↓
6. Scheduling
        ↓
7. Heartbeat
        ↓
8. Failure detection
        ↓
9. Task recovery
        ↓
10. Benchmarking
```

Everything after that is optional.

---

# 33. Final Demonstration

The final presentation should show something like this.

### Initial cluster

```text
MASTER
  ● ONLINE

WORKERS
  worker-01 ● ONLINE
  worker-02 ● ONLINE
  worker-03 ● ONLINE
  worker-04 ● ONLINE
```

### Submit workload

```text
$ job submit large_dataset.csv
```

Master:

```text
Job: 1001

Task 1 → worker-01
Task 2 → worker-02
Task 3 → worker-03
Task 4 → worker-04
```

### Processing

```text
worker-01 → RUNNING
worker-02 → RUNNING
worker-03 → RUNNING
worker-04 → RUNNING
```

### Kill worker-03

```text
worker-03 → OFFLINE
```

Master:

```text
FAILURE DETECTED

Worker: worker-03

Unfinished tasks:
Task 3

Rescheduling...
```

### Recovery

```text
Task 3 → worker-01
```

### Final result

```text
Job 1001 → COMPLETED

Workers used: 4
Worker failures: 1
Recovered tasks: 1
Final result: SUCCESS
```

This single demonstration proves the most important concepts of the project.

---

# 34. Expected Learning Outcomes

After completing the project, the developer should understand:

- Distributed system architecture
- Master-worker models
- Network communication
- TCP/IP or RPC
- Concurrent execution
- Task scheduling
- Load balancing
- Heartbeat protocols
- Failure detection
- Fault tolerance
- Task recovery
- Resource monitoring
- Scalability
- Performance measurement
- Cloud resource pooling

---

# 35. Resume Description

### Short Version

**Fault-Tolerant Distributed Computing System**  
Built a distributed computing platform that pools multiple physical Linux machines into a logical compute cluster, supporting task scheduling, parallel execution, heartbeat-based failure detection, and automatic task recovery.

### Technical Version

**Fault-Tolerant Distributed Computing System** | Python, Linux, gRPC/TCP, Distributed Systems

- Designed a Master–Worker architecture for distributing computational workloads across multiple physical machines over LAN.
- Implemented task scheduling, parallel execution, worker heartbeat monitoring, failure detection, and automatic task rescheduling.
- Evaluated scalability and fault recovery using multi-node performance benchmarks.

---

# 36. Final Project Definition

The simplest way to describe the project is:

> **We are building a small distributed computing cluster using multiple physical computers. A Master node divides computational workloads into tasks and schedules them across Worker nodes. Workers execute tasks concurrently and return results. The Master monitors worker health using heartbeats and automatically reschedules unfinished tasks when a worker fails.**

That is the complete essence of the project.

---

# 37. Final Architecture

```text
                         ┌─────────────────────┐
                         │        USER         │
                         │      CLI Client     │
                         └──────────┬──────────┘
                                    │
                                    ▼
                    ┌──────────────────────────┐
                    │       MASTER NODE        │
                    │                          │
                    │     Job Manager          │
                    │     Task Manager         │
                    │     Scheduler             │
                    │     Node Manager          │
                    │     Failure Detector     │
                    │     Result Aggregator    │
                    └────────────┬─────────────┘
                                 │
                         LAN / TCP / gRPC
                                 │
              ┌──────────────────┼──────────────────┐
              │                  │                  │
              ▼                  ▼                  ▼
       ┌────────────┐     ┌────────────┐     ┌────────────┐
       │  WORKER 1  │     │  WORKER 2  │     │  WORKER 3  │
       │            │     │            │     │            │
       │ Executor   │     │ Executor   │     │ Executor   │
       │ Heartbeat  │     │ Heartbeat  │     │ Heartbeat  │
       │ Monitoring │     │ Monitoring │     │ Monitoring │
       └────────────┘     └────────────┘     └────────────┘
              │                  │                  │
              └──────────────────┼──────────────────┘
                                 │
                                 ▼
                           FINAL RESULTS
```

---

# 38. The Core Idea in One Diagram

```text
              ONE LARGE COMPUTATIONAL JOB
                           │
                           ▼
                    ┌────────────┐
                    │   MASTER   │
                    └─────┬──────┘
                          SPLIT
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
          ┌───────┐    ┌───────┐    ┌───────┐
          │Worker1│    │Worker2│    │Worker3│
          └───┬───┘    └───┬───┘    └───┬───┘
              │            │            │
              ▼            ▼            ▼
           Result 1     Result 2     Result 3
              │            │            │
              └────────────┼────────────┘
                           ▼
                       AGGREGATE
                           │
                           ▼
                      FINAL RESULT


              IF WORKER 2 FAILS:

                  Worker 2
                     ✗
                     │
                Detect failure
                     │
                     ▼
               Recover Task
                     │
                     ▼
              Worker 1 / 3
                     │
                     ▼
                 Continue
```

---

# 39. Project Philosophy

The project follows one simple principle:

> **Use multiple ordinary computers to create a reliable distributed computing system without requiring expensive hardware or a complex frontend.**

The focus is not on making the project look like a cloud platform.

The focus is on making the **distributed system actually work**.

That is what makes the project technically valuable.