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
//
// Workers klassen skal ha en intern variabel? som er en liste med threads
// konstruktøren skal bestemme hvor mange threads som skal brukes
// metoden start() skal lage thread-ene i worker_threads? på en eller annen måte skal man kunnne kjøre dette
// metoden event_loop skal gjøre noe tilsvarende?
// Metoden post(function<void> f) skal ta inn en funksjon og poste den til thread-listen. denne skal da utføres av en
// worker thread
//
//
class Workers {

public:
    explicit Workers(int num)
        : num_threads(num)
    {}

    ~Workers() {
        std::cout<<"destructor"<<std::endl;
        stop();
    };
    void start() {
        threads.reserve(num_threads);
        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(&Workers::work, this);
        }
    }

    void post(const std::function<void()>& task) {
        {
            std::unique_lock<std::mutex> lock(tasks_mutex);
            tasks.emplace_back(task);
        }
        cv.notify_one();
    }

    void post_timeout(const std::function<void()>& task, int delay) {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        tasks.emplace_back(task);
        cv.notify_one();
    }

    void join() {
        for (auto &thread : threads) if (thread.joinable())thread.join();
    }


private:
    std::vector<std::thread> threads;
    std::list<std::function<void()>> tasks;
    int num_threads;
    std::mutex tasks_mutex;
    std::condition_variable cv;
    bool stopThreads = false;

    void stop() {
        stopThreads = true;
        cv.notify_all();
        join();
    }

    void work() {
        while(true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(tasks_mutex);
                cv.wait(lock, [this] {
                    return !tasks.empty() || stopThreads;
                });
                if (stopThreads) break;
                if (!tasks.empty()) {
                    task = *tasks.begin(); // Copy task for later use
                    tasks.pop_front(); // Remove task from list
                }
            }
            if (task)
                task();
        }
    }
};


#endif //EVENT_LOOP_THREADING_WORKERS_H
