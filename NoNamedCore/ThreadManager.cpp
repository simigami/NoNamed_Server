#include "pch.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

// Thread Manager

ThreadManager::ThreadManager()
{
    InitTLS();
}

ThreadManager::~ThreadManager()
{
    DestroyTLS();
}

void ThreadManager::Launch(function<void(void)> callback)
{
    LockGuard guard(_lock);
    
    // use std function to make callback for every general purposes
    _threads.emplace_back([=] ()
        {
            InitTLS();
            callback();
            DestroyTLS();
        }
    );
}

void ThreadManager::Join()
{
    for (thread& t : _threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    _threads.clear();
}

void ThreadManager::InitTLS()
{
    static Atomic<uint32> SLThreadId = 1;
    LThreadId = SLThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}
