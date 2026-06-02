#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{
    // Write Lock in Same Thread
    const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
    if (LThreadId == lockThreadId)
    {
        ++_writeCount;
        return;
    }
    
    // Max Time, then CRASH
    const int64 beginTick = ::GetTickCount64();
    
    // Ensure Nobody Currently Reads
    // Ensure Nobody Currently Writes => Can lead starvation...
    const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
    while (true)
    {
        for (uint32 spinCount = 0; spinCount < MAX_SPIN_CNT; ++spinCount)
        {
            uint32 expected = EMPTY_FLAG;
            if (_lockFlag.compare_exchange_strong(OUT expected, desired))
            {
                // Cuurently Optain Write Lock in Recursive Way
                ++_writeCount;
                return;
            }
        }
        
        if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_CHECK)
        {
            CRASH("WRITE LOCK TIMEOUT");
        }
        
        this_thread::yield();
    }
}

void Lock::WriteUnLock()
{
    // Invalid All Read Locks
    if (_lockFlag & READ_COUNT_MASK)
    {
        // Read Thread Currently Exists
        CRASH("CANNOT UNLOCK WRITE LOCK WHEN READ EXISTS!");
    }
    
    const uint32 lockCount = --_writeCount;
    if (lockCount == 0)
    {
        _lockFlag.store(EMPTY_FLAG);
    }
}

void Lock::ReadLock()
{
    // Check only prev Write Lock Thread tries Read Lock
    const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
    if (lockThreadId == LThreadId)
    {
        _lockFlag.fetch_add(1);
        return;
    }
    
    const uint64 beginTick = ::GetTickCount64();
    // Ensure no Write Lock Exsists
    while (true)
    {
        for (int i = 0; i < MAX_SPIN_CNT; ++i)
        {
            uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
            
            // desired is just read count + 1
            if (_lockFlag.compare_exchange_strong(OUT expected, expected+1))
            {
                return;
            }
        }
        
        if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_CHECK)
        {
            CRASH("READ LOCK TIMEOUT");
        }
        
        this_thread::yield();
    }
}

void Lock::ReadUnLock()
{
    if (_lockFlag.fetch_sub(1) & READ_COUNT_MASK == 0)
    {
        CRASH("ERROR LOCK FLAG IS -1, MULTIPLE READ UNLOCK");
    }
}
