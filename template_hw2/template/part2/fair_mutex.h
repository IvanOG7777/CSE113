#pragma once

#include <atomic>
#include <mutex>
using namespace std;

class rw_mutex
{
private:
    std::mutex internal_mutex;
    bool writer_active;
    int total_readers;
    int waiting_writers;

public:
    rw_mutex() {
        writer_active = false;
        total_readers = 0;
        waiting_writers = 0;
    }

    void lock_reader() {
        bool acquired = false;
        // while reader thread isn't acquired
        while (acquired == false) {
            internal_mutex.lock(); // while we wait lock mutex for current reader.
            // check if reader thread can enter;
            if (writer_active == false && waiting_writers == 0) { // can only enter when no active writers and no waiting writers
                total_readers++;
                acquired = true;
            }
            internal_mutex.unlock();
        }
    }

    void unlock_reader() {
        internal_mutex.lock();
        total_readers--;
        internal_mutex.unlock();
    }


    void lock() {
        bool acquired = false;
        internal_mutex.lock();
        waiting_writers++;
        internal_mutex.unlock();
        // while writer thread isn't acquired
        while (acquired == false) {
            internal_mutex.lock();
            // check if no reader threads are active
            if (total_readers == 0 && writer_active == false) {
                acquired = true;
                writer_active = true;
                waiting_writers--;
            }
            internal_mutex.unlock();
        }
    }

    void unlock() {
        internal_mutex.lock();
        writer_active = false;
        internal_mutex.unlock();
    }
};
