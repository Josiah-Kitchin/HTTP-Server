
#include "thread_pool.hpp"


ThreadPool::ThreadPool(size_t num_threads) 
{ 
    //Create worker threads 
    for (size_t i = 0; i < num_threads; ++i) 
    { 
        threads.emplace_back([this] 
        { 
            while (true) { 
                function<void()> task;  
                {
                     //Unlock the queue before executing the task so 
                     //other threads can perform enqueue tasks
                    unique_lock<mutex> lock(queue_mutex);
                    //Make the thread wait until there is a task in the queue or stop condition is ste
                    cv.wait(lock, [this] 
                    { 
                        return !tasks.empty() || stop; 
                    });

                    if (stop && tasks.empty())  
                        return;
                    
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() 
{ 
    {
        unique_lock<mutex> lock(queue_mutex);
        stop = true; 
    }
    cv.notify_all();
    for (auto& thread : threads)  
        thread.join(); 
}

void ThreadPool::enqueue(function<void()> task) 
{ 
    {
        unique_lock<mutex> lock(queue_mutex);
        tasks.emplace(std::move(task));
    }
    cv.notify_one();
}
