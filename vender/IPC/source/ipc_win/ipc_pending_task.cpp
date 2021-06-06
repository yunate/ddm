#include "ipc_pending_task.h"

namespace ipc {

PendingTask::PendingTask(const Closure& task)
    : task(task),
      sequence_num(0) {
}

PendingTask::~PendingTask() {
}

bool PendingTask::operator<(const PendingTask& other) const {
  // If the times happen to match, then we use the sequence number to decide.
  // Compare the difference to support integer roll-over.
  return (sequence_num - other.sequence_num) > 0;
}

void PendingTask::Run() {
  if (task) {
    task();
  }
}

void TaskQueue::Swap(TaskQueue* queue) {
  c.swap(queue->c);  // Calls std::deque::swap.
}

}  // namespace base
