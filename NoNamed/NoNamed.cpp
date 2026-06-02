#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"

#include <future>
#include "CoreGlobal.h"
#include "CoreMacro.h"
#include "ThreadManager.h"

CoreGlobal Core;

class TestLock
{
    USE_LOCK;
    
public:
    int32 TestRead()
    {
        READ_LOCK;
        
        if (_queue.empty())
        {
            return -1;
        }
        
        return _queue.front();
    }
    
    void TestPush()
    {
        WRITE_LOCK;
        
        _queue.push(rand() % 1000);
    }    
    
    void TestPop()
    {
        WRITE_LOCK;
        
        if (!_queue.empty())
        {
            _queue.pop();
        }
    }
    
private:
    queue<int32> _queue;
};

TestLock test;

void ThreadWrite()
{
    while (true)
    {
        test.TestPush();
        this_thread::sleep_for(1ms);
        test.TestPop();
    }
}

void ThreadRead()
{
    while (true)
    {
        int32 value = test.TestRead();
        cout << "VALUE IS : " << value << endl;
        this_thread::sleep_for(1ms);
    }
}

int main()
{
    for (int32 i = 0; i < 3; ++i)
    {
        GThreadManager->Launch(ThreadWrite);
    }
    
    for (int32 i = 0; i < 2; ++i)
    {
        GThreadManager->Launch(ThreadRead);
    }
    
    GThreadManager->Join();
}