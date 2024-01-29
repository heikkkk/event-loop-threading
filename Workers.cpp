//
// Created by henri on 29.01.2024.
//
#include "Workers.h"

using namespace std;

int main() {
    Workers worker_threads(4);
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

