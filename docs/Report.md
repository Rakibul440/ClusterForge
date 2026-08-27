# ClusterForge: A Fault-Tolerant Distributed Computing Cluster

## 1. Project Overview

**ClusterForge** is a distributed computing system where multiple computers work together to complete a large computational task.

The system has one **Coordinator** and multiple **Worker Nodes**.

The Coordinator divides a job into smaller tasks and distributes them among available workers. If a worker fails, the Coordinator detects the failure and sends the unfinished task to another worker.

### Main Focus

- Distributed Systems
- Parallel Computing
- Networking
- Task Scheduling
- Fault Tolerance

---

# 2. Problem Statement

If a large computation is running on a single computer and that computer fails, the entire computation may stop.

ClusterForge solves this by distributing the work across multiple machines and recovering unfinished tasks when a worker fails.

---

# 3. Proposed Architecture

```text
                  +-------------+
                  |   Client    |
                  +------+------+
                         |
                    Submit Job
                         |
                         v
                +------------------+
                |   COORDINATOR    |
                |------------------|
                | Task Scheduler   |
                | Task Queue       |
                | Worker Manager   |
                | Failure Detector |
                | Result Manager   |
                +---+---+---+------+
                    |   |   |
             -------+   |   +-------
             |           |          |
             v           v          v
        +---------+ +---------+ +---------+
        | Worker 1| | Worker 2| | Worker 3|
        |---------| |---------| |---------|
        | Execute | | Execute | | Execute |
        | Tasks   | | Tasks   | | Tasks   |
        +---------+ +---------+ +---------+
             |           |          |
             +-----------+----------+
                         |
                         v
                  Final Result
```

---

# 4. How It Works

### Step 1 — Job Submission

The Client sends a computational job to the Coordinator.

### Step 2 — Task Division

The Coordinator divides the job into smaller tasks.

```text
Job
 |
 +-- Task 1
 +-- Task 2
 +-- Task 3
 +-- Task 4
```

### Step 3 — Task Distribution

The Coordinator assigns tasks to available workers.

```text
Worker 1 → Task 1
Worker 2 → Task 2
Worker 3 → Task 3
Worker 1 → Task 4
```

### Step 4 — Parallel Execution

Multiple workers execute their tasks at the same time.

### Step 5 — Result Collection

Workers send their results back to the Coordinator.

The Coordinator combines the results and returns the final output.

---

# 5. Fault Tolerance

Workers send regular **heartbeat messages** to the Coordinator.

```text
Worker 1 ---- Heartbeat ----> Coordinator
Worker 2 ---- Heartbeat ----> Coordinator
Worker 3 ---- Heartbeat ----> Coordinator
```

If a worker stops sending heartbeats:

```text
Worker 2
   |
   X
Failed
   |
   v
Coordinator detects failure
   |
   v
Recover unfinished task
   |
   v
Assign to another worker
```

Therefore, the whole job does not need to restart.

---

# 6. Main Components

| Component | Responsibility |
|---|---|
| Client | Submits jobs |
| Coordinator | Controls the cluster |
| Task Scheduler | Assigns tasks |
| Worker Nodes | Execute tasks |
| Failure Detector | Detects failed workers |
| Result Manager | Collects results |

---

# 7. Technology Stack

| Area | Technology |
|---|---|
| Programming | Python |
| Communication | gRPC / HTTP |
| Serialization | Protocol Buffers / JSON |
| Concurrency | Multiprocessing / AsyncIO |
| Containerization | Docker |
| OS | Linux |
| Version Control | Git / GitHub |
| Testing | Pytest |

---

# 8. Key Features

- Distributed task execution
- Parallel processing
- Multiple worker nodes
- Automatic task scheduling
- Worker heartbeat monitoring
- Failure detection
- Task reassignment
- Result aggregation
- Basic cluster monitoring

---

# 9. Example

Suppose we have 4 tasks:

```text
Task 1 → Worker 1
Task 2 → Worker 2
Task 3 → Worker 3
Task 4 → Worker 4
```

If **Worker 2 fails**:

```text
Task 2 → Worker 2 ❌

Coordinator detects failure

Task 2 → Worker 3 ✓
```

The remaining computation continues without restarting the complete job.

---

# 10. Expected Outcome

ClusterForge will demonstrate a working distributed computing environment where:

**Multiple machines → share computation → execute tasks in parallel → detect failures → recover tasks → produce final result.**

