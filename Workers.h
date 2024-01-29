//
// Created by henri on 29.01.2024.
//

#ifndef EVENT_LOOP_THREADING_WORKERS_H
#define EVENT_LOOP_THREADING_WORKERS_H
# include <functional>
# include <iostream>
# include <mutex>
# include <thread>
# include <vector>

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
    Workers(int num)
        : num_threads(num)
    {}
    void start() {

    }

private:
    std::vector<thread> threads;
    int num_threads;
};


#endif //EVENT_LOOP_THREADING_WORKERS_H
