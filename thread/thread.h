/******************************************************************************
 Copyright (C) 2022 Vin Manager All rights reserved
 *Author: vin manager
 *Date: 2022-06-13
 *****************************************************************************/

#pragma once
#include <string>
#include <thread>  // NOLINT
#include <mutex>  // NOLINT
#include <condition_variable>  // NOLINT
#include <utility>

namespace mngr {

class Thread {
 public:
  enum class Action {
    kJoin = 0,
    kDetach = 1,
  };

 public:
  /**
  * @brief Construct a new Thread object
  *
  * @param name
  * @param act
  */
  explicit Thread(std::string name = "Thread", Action act = Action::kDetach);
  explicit Thread(Action act);
  Thread(Thread&& thrd);

  template<typename Callable, typename ... Args>
  explicit Thread(Callable&& func, Args&& ... args)
    : thread_(std::forward<Callable>(func), std::forward<Args>(args)...) {
  }

  /**
   * @brief Destroy the Thread object
   *
   */
  virtual ~Thread();

  /**
   * @brief
   *
   * @return true
   * @return false
   */
  bool pre_init();

  /**
   * @brief start thread
   *
   * @return true
   * @return false
   */
  bool start();
  /**
   * @brief stop thread and release resource
   *
   * @return true
   * @return false
   */
  bool stop();

  /**
   * @brief start process really
   *
   * @return true
   * @return false
   */
  virtual bool run();  // should override
  /**
   * @brief resume thread run when thread suspend
   *
   * @return int 0:success, others:fail
   */
  int resume();
  /**
   * @brief suspend thread run when thread run
   *
   * @return int 0:success, others:fail
   */
  int suspend();
  /**
   * @brief thread join
   *
   * @return int
   */
  int join();
  /**
   * @brief thread detach
   *
   * @return int
   */
  int detach();

  /**
   * @brief Get the id object
   *
   * @return std::thread::id
   */
  std::thread::id get_id();
  /**
   * @brief check thread's joinable state
   *
   * @return true
   * @return false
   */
  bool joinable();
  /**
   * @brief check thread is running
   *
   * @return true
   * @return false
   */
  bool is_running();

 protected:
  /**
   * @brief thread process something
   *
   * @return true
   * @return false
   */
  bool process();

 protected:
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable condition_variable_;
    bool b_run_ = false;
    bool b_suspend_ = false;
    std::string name_;
    Action action_ = Action::kDetach;

 protected:
    Thread (const Thread&) = delete;
};

}  // namespace mngr
