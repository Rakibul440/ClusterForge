# ClusterForge — Fault-Tolerant Distributed Computing Cluster

## 1. Project Overview

**ClusterForge** is a fault-tolerant distributed computing system that allows multiple computers to work together as a single computing cluster.

The system consists of **two separate software applications**:

1. **ClusterForge Coordinator**
2. **ClusterForge Worker**

The Coordinator manages the entire cluster, while Workers execute computational tasks assigned by the Coordinator.

The computers communicate over a **TCP/IP network**, using either **Ethernet/LAN or Wi-Fi**.

---

# 2. Core Architecture

```text
                         ┌──────────────────────────┐
                         │          USER            │
                         │                          │
                         │  Submit / Monitor Jobs   │
                         └────────────┬─────────────┘
                                      │
                                      ▼
                    ┌─────────────────────────────────┐
                    │     CLUSTERFORGE COORDINATOR   │
                    │            SOFTWARE            │
                    │                                 │
                    │  ┌───────────────────────────┐  │
                    │  │       Job Manager          │  │
                    │  └─────────────┬─────────────┘  │
                    │                │                │
                    │  ┌─────────────▼─────────────┐  │
                    │  │       Task Manager         │  │
                    │  │   Split Job → Tasks         │  │
                    │  └─────────────┬─────────────┘  │
                    │                │                │
                    │  ┌─────────────▼─────────────┐  │
                    │  │       Task Queue            │  │
                    │  └─────────────┬─────────────┘  │
                    │                │                │
                    │  ┌─────────────▼─────────────┐  │
                    │  │        Scheduler            │  │
                    │  └─────────────┬─────────────┘  │
                    │                │                │
                    │  ┌─────────────▼─────────────┐  │
                    │  │      Worker Registry       │  │
                    │  └─────────────┬─────────────┘  │
                    │                │                │
                    │  ┌─────────────▼─────────────┐  │
                    │  │ Heartbeat & Failure        │  │
                    │  │ Detection                  │  │
                    │  └─────────────┬─────────────┘  │
                    │                │                │
                    │  ┌─────────────▼─────────────┐  │
                    │  │ Task Recovery & Result     │  │
                    │  │ Aggregation                │  │
                    │  └───────────────────────────┘  │
                    └───────────────┬─────────────────┘
                                    │
                              TCP/IP Network
                           LAN / Ethernet / Wi-Fi
                                    │
                 ┌──────────────────┼──────────────────┐
                 │                  │                  │
                 ▼                  ▼                  ▼
        ┌────────────────┐ ┌────────────────┐ ┌────────────────┐
        │    WORKER 1    │ │    WORKER 2    │ │    WORKER 3    │
        │ Worker Software│ │ Worker Software│ │ Worker Software│
        │                │ │                │ │                │
        │ ┌────────────┐ │ │ ┌────────────┐ │ │ ┌────────────┐ │
        │ │ Registration│ │ │ │ Registration│ │ │ │ Registration│ │
        │ └────────────┘ │ │ └────────────┘ │ │ └────────────┘ │
        │                │ │                │ │                │
        │ ┌────────────┐ │ │ ┌────────────┐ │ │ ┌────────────┐ │
        │ │  Heartbeat │ │ │ │  Heartbeat │ │ │ │  Heartbeat │ │
        │ └────────────┘ │ │ └────────────┘ │ │ └────────────┘ │
        │                │ │                │ │                │
        │ ┌────────────┐ │ │ ┌────────────┐ │ │ ┌────────────┐ │
        │ │Task Executor│ │ │ │Task Executor│ │ │ │Task Executor│ │
        │ └──────┬─────┘ │ │ └──────┬─────┘ │ │ └──────┬─────┘ │
        │        │       │ │        │       │ │        │       │
        │     Compute    │ │     Compute    │ │     Compute    │
        └────────────────┘ └────────────────┘ └────────────────┘
```

---

# 3. Two Software Components

## A. ClusterForge Coordinator

The **Coordinator** is installed on **one central computer**.

Its responsibility is to control the complete cluster.

### Main responsibilities

- Accept jobs from users
- Divide jobs into smaller tasks
- Maintain task queue
- Discover/register Workers
- Maintain Worker status
- Schedule tasks
- Send tasks to Workers
- Receive results
- Monitor Worker heartbeats
- Detect Worker failures
- Recover unfinished tasks
- Reassign failed tasks
- Aggregate final results
- Display cluster status

```text
Coordinator
│
├── Job Manager
├── Task Manager
├── Task Queue
├── Scheduler
├── Worker Registry
├── Heartbeat Manager
├── Failure Detector
├── Task Recovery Manager
└── Result Aggregator
```

---

# 4. ClusterForge Worker

The **Worker** is installed on every computer that participates in the cluster.

A Worker does not manage the cluster.

It mainly:

- Connects to Coordinator
- Registers itself
- Reports CPU/RAM/resources
- Sends heartbeat
- Receives tasks
- Executes tasks
- Sends results
- Reports task status

```text
Worker
│
├── Coordinator Connection
├── Registration
├── Heartbeat
├── Resource Monitor
├── Task Receiver
├── Task Executor
└── Result Sender
```

---

# 5. How the Connection Works

ClusterForge uses **TCP/IP communication**.

The physical network can be:

```text
Ethernet
   OR
Wi-Fi
```

The software does not need to care whether the connection is wired or wireless.

### Example

```text
Coordinator
IP: 192.168.1.10
Port: 5000

        │
        │ TCP
        │
        ├────────────── Worker 1
        │              192.168.1.11
        │
        ├────────────── Worker 2
        │              192.168.1.12
        │
        └────────────── Worker 3
                       192.168.1.13
```

All machines must be connected to the same local network.

---

# 6. Worker Registration

When a Worker starts:

```text
Worker starts
     │
     ▼
Connect to Coordinator
     │
     ▼
Send Registration
     │
     ├── Worker ID
     ├── IP Address
     ├── CPU information
     ├── RAM information
     └── Available resources
     │
     ▼
Coordinator registers Worker
     │
     ▼
Worker = READY
```

Example:

```text
Worker ID : W-002
IP        : 192.168.1.12
CPU       : 8 cores
RAM       : 16 GB
Status    : READY
```

---

# 7. Job Execution Flow

The complete execution process:

```text
                    USER
                     │
                     │ Submit Job
                     ▼
              ┌───────────────┐
              │  Coordinator  │
              └───────┬───────┘
                      │
                      ▼
                Job Manager
                      │
                      ▼
                Split Job
                      │
              ┌───────┼────────┐
              ▼       ▼        ▼
           Task 1   Task 2   Task 3
              │       │        │
              └───────┼────────┘
                      ▼
                  Task Queue
                      │
                      ▼
                  Scheduler
                      │
          ┌───────────┼───────────┐
          ▼           ▼           ▼
       Worker 1    Worker 2    Worker 3
          │           │           │
       Task 1      Task 2      Task 3
          │           │           │
          ▼           ▼           ▼
       Execute      Execute     Execute
          │           │           │
          └───────────┼───────────┘
                      ▼
                  Results
                      │
                      ▼
               Result Aggregator
                      │
                      ▼
                     USER
```

---

# 8. Scheduling

The Coordinator decides **which Worker receives which task**.

For example:

```text
Task Queue

Task 1
Task 2
Task 3
Task 4
Task 5
Task 6

             Scheduler
                 │
       ┌─────────┼─────────┐
       ▼         ▼         ▼
    Worker 1  Worker 2  Worker 3
       │         │         │
    Task 1    Task 2    Task 3
    Task 4    Task 5    Task 6
```

Scheduling can later consider:

- CPU availability
- RAM availability
- Worker status
- Number of running tasks
- Task requirements

---

# 9. Heartbeat System

Workers continuously send heartbeat messages to the Coordinator.

```text
Worker                     Coordinator
   │                            │
   │──── HEARTBEAT ────────────►│
   │                            │
   │──── HEARTBEAT ────────────►│
   │                            │
   │──── HEARTBEAT ────────────►│
   │                            │
   │          Worker crashes     X
   │                            │
   │       No heartbeat         │
   │                            │
   │                            ▼
   │                     Failure Detector
   │                            │
   │                            ▼
   │                    Worker = FAILED
```

This allows ClusterForge to detect unavailable Workers.

---

# 10. Fault Tolerance and Task Recovery

Fault tolerance is one of the main features of ClusterForge.

Example:

```text
Task 1 ──► Worker 1 ──► COMPLETED
Task 2 ──► Worker 2 ──► RUNNING
Task 3 ──► Worker 3 ──► COMPLETED
                         │
                         X
                    Worker 2 crashes
```

Coordinator detects the failure:

```text
Worker 2 Failure
       │
       ▼
Heartbeat Timeout
       │
       ▼
Mark Worker 2 as FAILED
       │
       ▼
Find unfinished Task 2
       │
       ▼
Return Task 2 to Queue
       │
       ▼
Scheduler selects Worker 1
       │
       ▼
Worker 1 executes Task 2
       │
       ▼
Result received
```

Therefore, a Worker failure does not necessarily cause the entire job to fail.

---

# 11. Detailed Communication Architecture

```text
                         CLUSTERFORGE
                              │
             ┌────────────────┴────────────────┐
             │                                 │
             ▼                                 ▼
   ┌───────────────────┐             ┌───────────────────┐
   │    COORDINATOR    │             │      WORKER       │
   │      SOFTWARE     │             │     SOFTWARE      │
   ├───────────────────┤             ├───────────────────┤
   │                   │             │                   │
   │ Job Manager       │             │ Registration      │
   │ Task Manager      │             │ Connection        │
   │ Scheduler         │             │ Heartbeat         │
   │ Task Queue        │◄───────────►│ Task Receiver     │
   │ Worker Registry   │    TCP      │ Task Executor     │
   │ Heartbeat Manager │    IP       │ Resource Monitor  │
   │ Failure Detector  │             │ Result Sender     │
   │ Recovery Manager  │             │                   │
   │ Result Aggregator │             │                   │
   │                   │             │                   │
   └─────────┬─────────┘             └───────────────────┘
             │
             │
       TCP/IP Network
       LAN / Ethernet
            or
            Wi-Fi
             │
      ┌──────┴─────────┬──────────────┐
      │                │              │
      ▼                ▼              ▼
 Worker 1          Worker 2       Worker 3
```

---

# 12. Message Types

The Coordinator and Workers will exchange structured messages.

Examples:

```text
REGISTER_WORKER
HEARTBEAT
TASK_ASSIGN
TASK_ACK
TASK_RESULT
TASK_FAILED
WORKER_STATUS
TASK_CANCEL
```

Example task assignment:

```text
Coordinator
     │
     │ TASK_ASSIGN
     │
     │ Task ID: T-102
     │ Type: PRIME_CALCULATION
     │ Input: 1-1000000
     ▼
Worker
```

Result:

```text
Worker
   │
   │ TASK_RESULT
   │
   │ Task ID: T-102
   │ Result: ...
   ▼
Coordinator
```

---

# 13. Cluster Status

The Coordinator maintains a registry of Workers.

Example:

```text
             CLUSTER STATUS

Worker     IP              CPU     RAM      STATUS
------------------------------------------------------
W-001      192.168.1.11   8 Core   16 GB    READY
W-002      192.168.1.12   4 Core    8 GB    BUSY
W-003      192.168.1.13   8 Core   16 GB    READY
W-004      192.168.1.14   4 Core    8 GB    FAILED
```

Possible states:

```text
STARTING
READY
BUSY
FAILED
OFFLINE
```

---

# 14. Technology Stack

| Component | Technology |
|---|---|
| Programming Language | **C++** |
| Communication | TCP/IP |
| Concurrency | C++ Threads |
| Synchronization | Mutex / Condition Variable / Atomic |
| Serialization | Protocol Buffers |
| Build System | CMake |
| Operating System | Linux / Ubuntu |
| Testing | GoogleTest |
| Version Control | Git / GitHub |
| Containerization | Docker |

---

# 15. Software Deployment

### Central Computer

```text
Central PC
│
└── ClusterForge Coordinator
```

### Other Computers

```text
PC 1 ── ClusterForge Worker
PC 2 ── ClusterForge Worker
PC 3 ── ClusterForge Worker
PC 4 ── ClusterForge Worker
```

The number of Workers is not fixed.

```text
                 Coordinator
                     │
       ┌─────────────┼─────────────┐
       │             │             │
    Worker 1      Worker 2      Worker 3
       │             │             │
    Worker 4      Worker 5      Worker 6
       │
    Worker N
```

---

# 16. Project Structure

A possible repository structure:

```text
ClusterForge/
│
├── coordinator/
│   ├── src/
│   │   ├── JobManager
│   │   ├── TaskManager
│   │   ├── Scheduler
│   │   ├── WorkerRegistry
│   │   ├── HeartbeatManager
│   │   ├── FailureDetector
│   │   ├── TaskRecovery
│   │   ├── ResultAggregator
│   │   └── Network
│   │
│   └── CMakeLists.txt
│
├── worker/
│   ├── src/
│   │   ├── Connection
│   │   ├── Registration
│   │   ├── Heartbeat
│   │   ├── TaskReceiver
│   │   ├── TaskExecutor
│   │   ├── ResourceMonitor
│   │   └── ResultSender
│   │
│   └── CMakeLists.txt
│
├── common/
│   ├── protocol/
│   ├── messages/
│   └── utilities/
│
├── tests/
│
├── docs/
│
└── README.md
```

---

# 17. Complete System Flow

```text
┌──────────────────────────────────────────────────────────────────┐
│                         CLUSTERFORGE                             │
└──────────────────────────────────────────────────────────────────┘

 USER
  │
  │ Submit Job
  ▼
┌──────────────────────┐
│     COORDINATOR      │
│                      │
│  Job Manager         │
│       │              │
│       ▼              │
│  Task Manager        │
│       │              │
│       ▼              │
│  Task Queue          │
│       │              │
│       ▼              │
│  Scheduler           │
│       │              │
│       ▼              │
│  Worker Registry     │◄──── Heartbeats
│       │              │
│       ▼              │
│  Failure Detector    │
│       │              │
│       ▼              │
│  Recovery Manager    │
└─────────┬────────────┘
          │
          │ TCP/IP
          │
 ┌────────┼───────────────┬───────────────┐
 │        │               │               │
 ▼        ▼               ▼               ▼
┌────┐  ┌────┐          ┌────┐          ┌────┐
│ W1 │  │ W2 │          │ W3 │          │ W4 │
└─┬──┘  └─┬──┘          └─┬──┘          └─┬──┘
  │       │               │               │
  ▼       ▼               ▼               ▼
Execute Execute         Execute         Execute
Task     Task           Task            Task
  │       │               │               │
  └───────┴───────────────┴───────────────┘
                    │
                    ▼
              Task Results
                    │
                    ▼
          ┌──────────────────┐
          │ Result Aggregator│
          └────────┬─────────┘
                   │
                   ▼
                  USER
```

---

# 18. Key Features

ClusterForge will provide:

- Distributed task execution
- Multiple Worker nodes
- Centralized coordination
- TCP-based communication
- Task scheduling
- Parallel execution
- Worker registration
- Resource monitoring
- Heartbeat mechanism
- Failure detection
- Automatic task recovery
- Task reassignment
- Result aggregation
- Cluster monitoring
- LAN/Wi-Fi support

---

# 19. Project Goal

The primary goal of ClusterForge is to demonstrate how a **real distributed computing system works internally**.

Instead of simply running programs on multiple computers, ClusterForge will implement the fundamental distributed-system mechanisms:

```text
                  ClusterForge
                       │
        ┌──────────────┼──────────────┐
        ▼              ▼              ▼
   Distributed     Fault           Network
   Execution       Tolerance       Communication
        │              │              │
        ▼              ▼              ▼
   Scheduling     Failure          TCP/IP
   & Parallel     Detection       Messaging
   Processing         │
                      ▼
                   Recovery
```

The final system will allow multiple physical computers to cooperate as a **single fault-tolerant computing cluster**, controlled through the ClusterForge Coordinator.