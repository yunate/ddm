#ifndef IPC_IPC_PENDING_TASK_H_
#define IPC_IPC_PENDING_TASK_H_ 1

#include <queue>

#include "ipc_callback.h"

namespace ipc {

// Contains data about a pending task. Stored in TaskQueue and DelayedTaskQueue
// for use by classes that queue and execute tasks.
struct PendingTask {
  PendingTask(const Closure& task);
  ~PendingTask();

  // Used to support sorting.
  bool operator<(const PendingTask& other) const;

  void Run();

  // The task to run.
  Closure task;

  // Secondary sort key for run time.
  int32_t sequence_num;
};

// Wrapper around std::queue specialized for PendingTask which adds a Swap
// helper method.
class TaskQueue : public std::queue<PendingTask> {
 public:
  void Swap(TaskQueue* queue);
};

// PendingTasks are sorted by their |delayed_run_time| property.
typedef std::priority_queue<PendingTask> DelayedTaskQueue;

}  // namespace ipc

#endif  // IPC_IPC_PENDING_TASK_H_
