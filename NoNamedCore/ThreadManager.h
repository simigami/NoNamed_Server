#pragma once

#include <thread>
#include <functional> // for callback

// Thread Manager

class ThreadManager
{
public:
    ThreadManager();
    ~ThreadManager();
    
    void        Launch(function<void(void)> callback);
    void        Join();
    
    static void         InitTLS(); // Singleton Init Function
    static void         DestroyTLS();
    
private:
    Mutex                       _lock;
    vector<thread>      _threads;
};
