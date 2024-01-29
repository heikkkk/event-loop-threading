//
// Created by henri on 29.01.2024.
//

#include "Workers.h"
# include <functional>
# include <iostream>
# include <list>
# include <mutex>
# include <thread>
# include <vector>

int main() {
}

Workers worker_threads(4);
Workers event_loop(1);
worker_threads.start(); // Create 4 internal threads
event_loop.start(); // Create 1 internal thread
worker_threads.post([] {
// Task A
});
worker_threads.post([] {
// Task B
// Might run in parallel with task A
});
event_loop.post([] {
// Task C
// Might run in parallel with task A and B
});
event_loop.post([] {
// Task D
// Will run after task C
// Might run in parallel with task A and B
});
worker_threads.join(); // Calls join() on the worker threads
event_loop.join(); // Calls join() on the event thread