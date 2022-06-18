/******************************************************************************
 Copyright (C) 2022 Vin Manager All rights reserved
 *Author: vin manager
 *Date: 2022-06-13
 *****************************************************************************/

#pragma once
#include <atomic>
#include <condition_variable>  // NOLINT
#include <mutex>               // NOLINT
#include <queue>
#include <utility>

namespace mngr {

template <typename T>
class Queue {
 public:
  /**
  * @brief Construct a new Queue object
  *
  */
  Queue() {}
  ~Queue() {}

  /**
   * @brief enqueue T object to queue
   *
   * @param t
   */
  void enqueue(const T& t) {
    {
      std::lock_guard<std::mutex> lck(mutex_);
      queue_.emplace(t);
    }
    condition_variable_.notify_one();
  }

  /**
   * @brief dequeue t from queue
   *
   * @param timeout wait time to dequeue t
   * @return T
   */
  T dequeue(int32_t timeout = 0) {
    std::unique_lock<std::mutex> lck(mutex_);
    if (timeout > 0) {
      condition_variable_.wait_for(lck, std::chrono::milliseconds(timeout));
    }
    if (empty()) {
      condition_variable_.wait(lck, [this] { return !empty(); });
    }
    auto t = std::move(queue_.front());
    queue_.pop();
    return t;
  }

  int dequeue(T* t) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return -1;
    }
    t = std::move(queue_.front());
    queue_.pop();
    return 0;
  }

  /**
   * @brief get front object
   *
   * @return T&
   */
  T& front() { return queue_.front(); }
  /**
   * @brief queue size
   *
   * @return int
   */
  int size() { return queue_.size(); }
  /**
   * @brief check queue empty
   *
   * @return true : empty
   * @return false : not empty
   */
  bool empty() { return queue_.empty(); }

 protected:
  std::mutex mutex_;
  std::condition_variable condition_variable_;
  std::queue<T> queue_;
};

}  // namespace mngr
