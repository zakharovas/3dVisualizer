//
// Created by User on 19.05.2016.
//

#include "ThreadPool.h"

const std::size_t ThreadPool::kWorkersNumber_ = 2;

void ThreadPool::WorkerRunner_() {
    while (true) {
        std::shared_ptr<std::packaged_task<Color()>> t;
        if (tasks_.Pop(t) == false) {
            return;
        }
        (*t)();
    }
}


ThreadPool::ThreadPool() {
    for (size_t i = 0; i < kWorkersNumber_; i++) {
        workers_.push_back(std::thread(std::bind(&ThreadPool::WorkerRunner_, this)));
    }
}

std::future<Color> ThreadPool::Submit(const std::function<Color()> &func) {
    std::shared_ptr<std::packaged_task<Color()>> task_to_submit(new std::packaged_task<Color()>(func));
    tasks_.Push(task_to_submit);
    return task_to_submit->get_future();
}


ThreadPool::~ThreadPool() {
    tasks_.shutdown();
    for (auto b = workers_.begin(), e = workers_.end(); b != e; ++b) {
        b->join();
    }
}
