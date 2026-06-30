#pragma once

// Lock Macro
#define USE_MANY_LOCKS(count)     Lock _locks[count];
#define USE_LOCK                                USE_MANY_LOCKS(1);
#define READ_LOCK_IDX(idx)              ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define READ_LOCK                             READ_LOCK_IDX(0);
#define WRITE_LOCK_IDX(idx)            WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#define WRITE_LOCK                           WRITE_LOCK_IDX(0);

// Memory Macro
#define Xalloc(size)              PoolAllocator::Alloc(size)
#define Xrelease(ptr)           PoolAllocator::Release(ptr)

// Logger [ CRASH ] 
#define CRASH(cause)                                        \
{                                                                              \
    uint32* crash = nullptr;                                   \
    __analysis_assume(crash != nullptr);             \
    *crash = 0xDEADBEEF;                                    \
}                                                                              \


#define ASSERT_CRASH(expr)                         \
{                                                                            \
    if (!(expr))                                                        \
    {                                                                        \
        CRASH("Assert Crash");                             \
        __analysis_assume(expr);                          \
    }                                                                        \
}                                                                            \

#define OUT