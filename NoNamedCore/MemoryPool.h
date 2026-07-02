#pragma once

enum
{
    SLIST_ALIGNMENT = 16
};

// Individual Memory Cell 

// Header
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
    MemoryHeader(int32 size) : allocSize(size) { }
    
    static void* AttachHeader(MemoryHeader* header, int32 size)
    {
        new(header)MemoryHeader(size);
        return reinterpret_cast<void*>(++header); // first data pointer
    }
    
    static MemoryHeader* DetachHeader(void* ptr)
    {
        MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
        return header;
    }
    
    int32 allocSize;
    
    // TODO -> 실제 데이터 내용
};

// Pool
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
    MemoryPool(int32 allocSize);
    ~MemoryPool();
    
    void Push(MemoryHeader* ptr);
    MemoryHeader* Pop();
    
private:
    SLIST_HEADER _header;
    int32 _allocSize = 0;
    atomic<int32> _useCount = 0;
    atomic<int32> _reserveCount = 0;
    
    // TODO: Queue Needs Additional memory space to cache, 
    // TODO: so use SLIST_ENTRY
    // USE_LOCK;
    // queue<MemoryHeader*> _queue; 
};
