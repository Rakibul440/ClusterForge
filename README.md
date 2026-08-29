# ClusterForge

### A Fault-Tolerant Distributed Computing System

ClusterForge is a **CLI-based distributed computing system** that uses multiple computers or worker nodes to process computational tasks together.

The system follows a **Coordinator–Worker architecture**. A Coordinator manages jobs and distributes smaller tasks among available Worker Nodes. Workers execute these tasks in parallel and return their results to the Coordinator.

The key feature of ClusterForge is **fault tolerance**. If a worker fails during execution, the Coordinator detects the failure and reassigns its unfinished tasks to another available worker instead of restarting the entire job.

> **Distribute the work. Detect failures. Recover the work. Complete the job.**

---

## 🎯 Project Goal

The main goal of ClusterForge is to build and understand a practical distributed computing system while demonstrating important distributed-system concepts.

The project focuses on:

* Distributed task execution
* Parallel processing
* Task scheduling
* Network communication
* Worker monitoring
* Failure detection
* Task recovery
* Result aggregation

ClusterForge is intentionally designed as a **CLI application** so that development can focus on the core distributed-system functionality rather than a web frontend.

---

## 🏗️ Architecture

```text
                         +----------------+
                         |     Client     |
                         |      CLI       |
                         +-------+--------+
                                 |
                              Submit Job
                                 |
                                 v
                       +-------------------+
                       |    Coordinator    |
                       |-------------------|
                       |   Job Manager     |
                       |   Task Scheduler  |
                       |   Task Queue      |
                       |   Worker Registry |
                       |   Failure Detector|
                       |   Result Manager  |
                       +---------+---------+
                                 |
              +------------------+------------------+
              |                  |                  |
              v                  v                  v
        +-----------+      +-----------+      +-----------+
        |  Worker 1 |      |  Worker 2 |      |  Worker 3 |
        |-----------|      |-----------|      |-----------|
        | Executor  |      | Executor  |      | Executor  |
        | Heartbeat |      | Heartbeat |      | Heartbeat |
        +-----------+      +-----------+      +-----------+
              |                  |                  |
              +------------------+------------------+
                                 |
                                 v
                       +-------------------+
                       |  Result Manager   |
                       |   Final Result    |
                       +-------------------+
```

---

## ⚙️ How It Works

### 1. Submit a Job

The user submits a computational job through the CLI.

```text
Client → Coordinator
```

### 2. Split the Job

The Coordinator divides the job into smaller independent tasks.

```text
Large Job
   |
   +-- Task 1
   +-- Task 2
   +-- Task 3
   +-- Task 4
```

### 3. Distribute Tasks

The Coordinator assigns tasks to available workers.

```text
Task 1 → Worker 1
Task 2 → Worker 2
Task 3 → Worker 3
Task 4 → Worker 1
```

### 4. Execute in Parallel

Workers process their assigned tasks simultaneously.

### 5. Collect Results

Workers send completed results back to the Coordinator.

```text
Worker 1 ──┐
Worker 2 ──┼──> Coordinator → Final Result
Worker 3 ──┘
```

### 6. Handle Worker Failure

Workers periodically send heartbeat messages.

```text
Worker 1 ── HEARTBEAT ──> Coordinator
Worker 2 ── HEARTBEAT ──> Coordinator
Worker 3 ── HEARTBEAT ──> Coordinator
```

If a worker stops responding:

```text
Worker 2
   |
   X
Failure
   |
   v
Coordinator detects failure
   |
   v
Find unfinished tasks
   |
   v
Reassign task
   |
   v
Healthy Worker
```

The entire job does **not** need to restart.

---

## 🧩 Core Components

| Component            | Responsibility                   |
| -------------------- | -------------------------------- |
| **Client**           | Submits jobs through the CLI     |
| **Coordinator**      | Controls and manages the cluster |
| **Task Scheduler**   | Assigns tasks to workers         |
| **Task Queue**       | Stores pending tasks             |
| **Worker**           | Executes assigned tasks          |
| **Heartbeat System** | Reports worker health            |
| **Failure Detector** | Detects failed workers           |
| **Result Manager**   | Collects and combines results    |

---

## 🔥 Key Features

* **Distributed Computing** — Process one job using multiple worker nodes.
* **Parallel Execution** — Independent tasks can run simultaneously.
* **Task Scheduling** — Coordinator distributes pending tasks to workers.
* **Worker Registration** — Workers register themselves with the Coordinator.
* **Heartbeat Monitoring** — Workers periodically report that they are alive.
* **Failure Detection** — Coordinator detects unavailable workers.
* **Task Recovery** — Unfinished tasks from failed workers are reassigned.
* **Result Aggregation** — Coordinator collects worker results and produces the final result.
* **CLI Interface** — Control and monitor the cluster from the terminal.

---

## 🛠️ Technology Stack

| Technology                    | Purpose                          |
| ----------------------------- | -------------------------------- |
| **Python**                    | Core implementation              |
| **gRPC / HTTP**               | Coordinator–Worker communication |
| **Protocol Buffers / JSON**   | Data serialization               |
| **Multiprocessing / AsyncIO** | Concurrent task execution        |
| **Docker**                    | Running multiple nodes           |
| **Linux**                     | Primary development environment  |
| **Git & GitHub**              | Version control                  |
| **Pytest**                    | Testing                          |

> The exact communication mechanism may be finalized during implementation based on project requirements.

---

## 💻 CLI-Based Design

ClusterForge does not require a React or full-stack frontend.

The system will be controlled through command-line tools.

Example:

```bash
# Start the Coordinator
python coordinator.py

# Start workers
python worker.py --id worker1
python worker.py --id worker2
python worker.py --id worker3

# Submit a job
python client.py submit --input data.txt

# View cluster status
python client.py status
```

Example output:

```text
ClusterForge
--------------------------------
Coordinator: RUNNING

Workers:
  worker1    ONLINE
  worker2    ONLINE
  worker3    ONLINE

Tasks:
  Pending:    2
  Running:    3
  Completed:  15
  Failed:     0
```

---

## 🚨 Fault-Tolerance Example

Suppose the cluster has three workers:

```text
Worker 1 → Task 1
Worker 2 → Task 2
Worker 3 → Task 3
```

If Worker 2 suddenly fails:

```text
Worker 1 → Task 1 ✓
Worker 2 → Task 2 ✗
Worker 3 → Task 3 ✓
```

The Coordinator detects the failure:

```text
[Coordinator] Worker 2 heartbeat timeout
[Coordinator] Worker 2 marked as FAILED
[Coordinator] Recovering Task 2
[Coordinator] Task 2 assigned to Worker 3
```

The computation continues:

```text
Worker 3 → Task 2 ✓
```

This demonstrates the main fault-tolerance mechanism of ClusterForge.

---

## 🧠 Distributed Systems Concepts

ClusterForge provides practical implementation of:

* Coordinator–Worker architecture
* Distributed task scheduling
* Parallel computation
* Network communication
* Heartbeat-based failure detection
* Task state management
* Task reassignment
* Fault recovery
* Result aggregation
* Basic workload distribution

---

## 📁 Planned Project Structure

```text
ClusterForge/
│
├── coordinator/
│   ├── coordinator.py
│   ├── scheduler.py
│   ├── task_queue.py
│   ├── worker_manager.py
│   ├── failure_detector.py
│   └── result_manager.py
│
├── worker/
│   ├── worker.py
│   ├── executor.py
│   └── heartbeat.py
│
├── client/
│   └── client.py
│
├── common/
│   ├── models.py
│   └── protocol/
│
├── tests/
│
├── docker/
│
├── requirements.txt
├── README.md
└── LICENSE
```

The structure may evolve as implementation progresses.

---

## 🔄 System Workflow

```text
          Submit Job
              |
              v
        +-------------+
        | Coordinator |
        +------+------+
               |
          Split Tasks
               |
               v
        +-------------+
        | Task Queue  |
        +------+------+
               |
        Assign Tasks
               |
       +-------+-------+
       |       |       |
       v       v       v
    Worker 1 Worker 2 Worker 3
       |       |       |
       +-------+-------+
               |
          Execute Tasks
               |
               v
        Return Results
               |
               v
        +-------------+
        | Coordinator |
        +------+------+
               |
        Aggregate Results
               |
               v
          Final Result
```

---

## 🛡️ Failure Recovery Workflow

```text
Worker Running
      |
      v
Heartbeat Sent
      |
      v
Worker Failure
      |
      v
Heartbeat Timeout
      |
      v
Failure Detected
      |
      v
Find Unfinished Tasks
      |
      v
Return Tasks to Queue
      |
      v
Assign to Healthy Worker
      |
      v
Task Completed
```

---

## 🎓 Academic Focus

ClusterForge is primarily a **Distributed Computing project** with a strong focus on **Fault Tolerance**.

The project is designed to demonstrate how multiple machines can cooperate to solve a computational problem and how a distributed system can recover from individual worker failures.

The project prioritizes the implementation and understanding of the underlying distributed-system mechanisms rather than building a large user interface.

---

## 📌 Project Scope

### Included

* Coordinator
* Multiple Workers
* CLI client
* Task scheduling
* Parallel task execution
* Worker communication
* Heartbeat mechanism
* Failure detection
* Task reassignment
* Result aggregation

### Not the primary focus

* React frontend
* Large web dashboard
* Enterprise-level cloud infrastructure
* Complex authentication
* Full-scale production orchestration

---

## 🚀 Vision

ClusterForge aims to provide a clear and practical demonstration of how a **fault-tolerant distributed computing system** works internally.

The project focuses on one simple principle:

> **A worker may fail, but the computation should continue.**

---

## 👥 Project

**Project Name:** ClusterForge
**Project Type:** Distributed Computing System
**Architecture:** Coordinator–Worker
**Interface:** CLI
**Primary Focus:** Distributed Computing & Fault Tolerance
**Language:** Python
