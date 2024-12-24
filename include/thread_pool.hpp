#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector> 
using namespace std; 


class ThreadPool {
public: 
    ThreadPool(size_t num_threads = thread::hardware_concurrency()); 
    ~ThreadPool(); 
    void enqueue(function<void()>); 

private: 
    vector<thread> threads; 
    queue<function<void()>> tasks; 
    mutex queue_mutex; 
    condition_variable cv; 
    bool stop = false; 
};



#endif