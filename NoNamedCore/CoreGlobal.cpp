#include "pch.h"

#include "Memory.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemory = nullptr;

CoreGlobal::CoreGlobal()
{
    GThreadManager = new ThreadManager();
    GMemory = new MemoryManager();
}

CoreGlobal::~CoreGlobal()
{
    delete(GThreadManager);
    GThreadManager = nullptr;
    delete(GMemory);
    GMemory = nullptr;
}