#include "pch.h"
#include "Allocator.h"

#include "Memory.h"

// Base Allocator
void* BaseAllocator::Alloc(int32 size)
{
    return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
    ::free(ptr);
}

// Stomp Allocator
void* StompAllocator::Alloc(int32 size)
{
    const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    
    // To preserve overflow memory offset goes to end to start
    const int64 offset = pageCount * PAGE_SIZE - size;
    
    void* base =  ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return static_cast<void*>(static_cast<uint8*>(base) + offset);
}

void StompAllocator::Release(void* ptr)
{
    const int64 address = reinterpret_cast<int64>(ptr);
    
    // This need gradnuality of OS Page system align with 0x10000
    const int64 base = address - (address % PAGE_SIZE);
    ::VirtualFree(reinterpret_cast<void*>(base), 0, MEM_RELEASE);
}

void* PoolAllocator::Alloc(int32 size)
{
    return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
    GMemory->Release(ptr);
}

// STL Allocator
