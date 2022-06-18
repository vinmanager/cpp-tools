/******************************************************************************
 Copyright (C) 2022 Vin Manager All rights reserved
 *Author: vin manager
 *Date: 2022-06-13
 *****************************************************************************/

#include <utility>
#include <functional>
#include <iostream>
#include "thread/thread.h"

namespace mngr {

Thread::Thread(std::string name, Action act)
    : name_(name)
    , action_(act) {
}

Thread::Thread(Thread&& thrd) {
    thread_ = std::move(thrd.thread_);
}

Thread::~Thread() {
    // 考虑抛出exception异常的case，因此放在析构函数进行join
    if (thread_.joinable()) {
        thread_.join();
    }
}

bool Thread:: pre_init() {
    return true;
}

bool Thread::run() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    static int count = 0;
    count++;
    printf("Thread::run count:%d\n", count);
    return true;
}

bool Thread::process() {
    while (1) {
        if (b_run_ && b_suspend_) {
            std::unique_lock<std::mutex> tlock(mutex_);
            condition_variable_.wait(tlock);
            b_suspend_ = false;
        }
        if (!run()) {
            break;
        }
    }
    stop();
    return false;
}

int Thread::resume() {
    if (!b_suspend_) {
        printf("this thread is not suspend, do not resume\n");
        return 0;
    }
    condition_variable_.notify_one();
    return 0;
}

int Thread::suspend() {
    b_suspend_ = true;
    return 0;
}

bool Thread::stop() {
    return true;
}

bool Thread::start() {
    if (b_run_) {
        printf("%s thread has already started\n", name_.c_str());
        return true;
    }
    thread_ = std::thread(std::bind(&Thread::process, this));
    b_run_ = true;

    return true;
}

int Thread::join() {
    thread_.join();
    return 0;
}

int Thread::detach() {
    thread_.detach();
    return 0;
}

bool Thread::joinable() {
    return thread_.joinable();
}

std::thread::id Thread::get_id() {
    std::cout << "this_thread::get_id: " << std::this_thread::get_id()
              << ", thread.get_id: " << std::this_thread::get_id()
              << std::endl;
    return thread_.get_id();
}

bool Thread::is_running() {
    return b_run_;
}

}  // namespace mngr
