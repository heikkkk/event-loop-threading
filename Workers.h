//
// Created by henri on 29.01.2024.
//
#ifndef EVENT_LOOP_THREADING_WORKERS_H
#define EVENT_LOOP_THREADING_WORKERS_H
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include "chrono"

class Workers {

public:
    explicit Workers(int num)
        : num_threads(num)
    {}



    void start();

    void post( const std::function<void()>& task);

    void post_timeout(const std::function<void()>& task, int delay);

    void join();


private:
    std::vector<std::thread> threads;
    std::list<std::function<void()>> tasks;
    int num_threads;
    std::mutex tasks_mutex;
    std::condition_variable cv;
    bool stop_threads = false;

    bool stop();

    void work();
};


#endif //EVENT_LOOP_THREADING_WORKERS_H
