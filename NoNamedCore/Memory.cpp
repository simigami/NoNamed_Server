#include "pch.h"
#include "Memory.h"

#include "MemoryPool.h"

MemoryManager::MemoryManager()
{
    int32 size = 0;
    int32 tableIndex = 0;
    
    for (int32 size = 32; size <= 1024; size+=32)
    {
        MemoryPool* pool = new MemoryPool(size);
        _pools.push_back(pool);
        
        while (tableIndex <= size)
        {
            _pooltable[tableIndex] = pool;
            tableIndex++;
        }
    }
    
    for (; size <= 2048; size+=128)
    {
        MemoryPool* pool = new MemoryPool(size);
        _pools.push_back(pool);
        
        while (tableIndex <= size)
        {
            _pooltable[tableIndex] = pool;
            tableIndex++;
        }
    }
    
    for (; size <= 4096; size+=256)
    {
        MemoryPool* pool = new MemoryPool(size);
        _pools.push_back(pool);
        
        while (tableIndex <= size)
        {
            _pooltable[tableIndex] = pool;
            tableIndex++;
        }
    }
}

MemoryManager::~MemoryManager()
{
    for (MemoryPool* pool : _pools)
    {
        delete(pool);
    }
    
    _pools.clear();
}

void* MemoryManager::Allocate(int32 size) const
{
    MemoryHeader* header = nullptr;
    
    const int32 allocSize = size + sizeof(MemoryHeader);
    
    if (allocSize > MAX_ALLOC_SIZE)
    {
        // default new
        header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
    }
    else
    {
        // get from pool
        header = _pooltable[allocSize]->Pop();
    }
    
    return MemoryHeader::AttachHeader(header, size);
}

void MemoryManager::Release(void* ptr) const
{
    MemoryHeader* header = MemoryHeader::DetachHeader(ptr);
    
    const int32 allocSize = header->allocSize;
    ASSERT_CRASH(allocSize > 0);
    
    if (allocSize > MAX_ALLOC_SIZE)
    {
        ::_aligned_free(header);
    }
    else
    {
        // return to pool
        _pooltable[allocSize]->Push(header);
    }
}
