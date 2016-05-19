//
// Created by User on 19.05.2016.
//

#ifndef INC_3DVISUALIZER_THREADPOOL_H
#define INC_3DVISUALIZER_THREADPOOL_H

#include <cstdlib>
#include <future>
#include <vector>
#include <iostream>
#include <queue>
#include "Color.h"

template<class T>
class ThreadSafeQueue {
public:
    explicit ThreadSafeQueue();

    void Push(T item);

    bool Pop(T &item);

    void shutdown() {
        queue_mtx_.lock();
        is_working_ = 0;
        is_empty_.notify_all();
        queue_mtx_.unlock();
    }

private:
    std::size_t current_size_;
    std::queue<T> queue_;
    std::mutex queue_mtx_;
    std::condition_variable is_empty_;
    bool is_working_;
};

class ThreadPool {
public:

    ThreadPool();

    ~ThreadPool();

    std::future<Color> Submit(const std::function<Color()> &func);

private:
    ThreadSafeQueue<std::shared_ptr<std::packaged_task<Color()> > > tasks_;
    std::vector<std::thread> workers_;
    static const std::size_t kWorkersNumber_;

    void WorkerRunner_();
};


template<class T>
ThreadSafeQueue<T>::ThreadSafeQueue() {
    current_size_ = 0;
    is_working_ = 1;
}

template<class T>
void ThreadSafeQueue<T>::Push(T item) {
    queue_mtx_.lock();
    if (current_size_ == 0) {
        is_empty_.notify_all();
    }
    queue_.push(item);
    current_size_++;
    queue_mtx_.unlock();
}

template<class T>
bool ThreadSafeQueue<T>::Pop(T &item) {
    std::unique_lock<std::mutex> lck(queue_mtx_);
    while ((current_size_ == 0) && (is_working_ == 1)) {
        is_empty_.wait(lck);
    }
    if (current_size_ > 0) {
        item = std::move(queue_.front());
        queue_.pop();
        current_size_--;
    }
    lck.unlock();
    return is_working_;
}

#endif //INC_3DVISUALIZER_THREADPOOL_H
