//
// Created by henri on 29.01.2024.
//
#include "Workers.h"

using namespace std;

void Workers::start() {
    threads.reserve(num_threads);
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(&Workers::work, this);
    }
}

void Workers::post( const std::function<void()>& task) {
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        tasks.emplace_back(task);
        cv.notify_one();
    }
}

void Workers::post_timeout(const std::function<void()>& task, int delay) {
    std::unique_lock<std::mutex> lock(tasks_mutex);
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    tasks.emplace_back(task);
    cv.notify_one();
}

void Workers::join() {
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        stop_threads = true;
        cv.notify_all();
    }
    for (auto& thread : threads) thread.join();
}

bool Workers::stop() {
    return (stop_threads && tasks.empty());
}

void Workers::work() {
    while(!stop()) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(tasks_mutex);
            cv.wait(lock, [this] {
                return !tasks.empty() || stop_threads;
            });
            if (stop()) break;
            if (!tasks.empty()) {
                task = *tasks.begin(); // Copy task for later use
                tasks.pop_front(); // Remove task from list
            }
        }
        if (task)
            task();
    }
}


int main() {
    Workers worker_threads(2);
    Workers event_loop(1);
    worker_threads.start(); // Create 4 internal threads
    event_loop.start(); // Create 1 internal thread
    worker_threads.post([] {
        // Task A
        cout << "Goddag fra Task A \n" << endl;
    });
    worker_threads.post([] {
        // Task B
        // Might run in parallel with task A
        cout << "Goddag fra Task B \n" << endl;
    });
    event_loop.post([] {
        // Task C
        // Might run in parallel with task A and B
        cout << "Goddag fra Task C \n" << endl;
    });
    event_loop.post([] {
        // Task D
        // Will run after task C
        // Might run in parallel with task A and B
        cout << "Goddag fra Task D \n" << endl;
    });
    worker_threads.join(); // Calls join() on the worker threads
    event_loop.join(); // Calls join() on the event thread


    return 0;
}

