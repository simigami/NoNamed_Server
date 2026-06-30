#pragma once
#include "Allocator.h"
#include "CoreMacro.h"

class MemoryPool;

class MemoryManager
{
    // 32B per slot ~ 1024B
    // 128B per slot ~2048B
    // 256B per slot ~ 4096B
    enum
    {
        POOL_COUNT = (1024/32) + (1024/128) + (2048/256),
        MAX_ALLOC_SIZE = 4096,
    };
    
public:
    MemoryManager();
    ~MemoryManager();
    
    void* Allocate(int32 size) const;
    void Release(void* ptr) const;
    
private:
    vector<MemoryPool*> _pools;
    
    // Custom Memory Pool Table
    MemoryPool* _pooltable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
    Type* memory = static_cast<Type*>(Xalloc(sizeof(Type)));
    
    // placement new
    new(memory) Type(std::forward<Args>(args)...);
    return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
    obj->~Type();
    Xrelease(obj);
}