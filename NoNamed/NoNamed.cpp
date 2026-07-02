#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include "Memory.h"
#include "ThreadManager.h"

static CoreGlobal GCoreGlobal;

// DECLSPEC_ALIGN(16)
// class Data // : public SListEntry
// {
// public:
//     Data() : _entry(), _hp(rand() % 1000), _mp(rand() % 1000)
//     {
//     }
//
//     SLIST_ENTRY _entry;
//     
//     int64 _hp;
//     int64 _mp;
// };

class Knight
{
public:
    int32 _hp = rand() % 1000;
};

class Monster
{
public:
    int64 _id = 0;
};

SLIST_HEADER* GHeader;

int main()
{
    GHeader = new SLIST_HEADER();
    ASSERT_CRASH((uint64)GHeader % 16 == 0, "GHeader is not 16 byte aligned!");
    ::InitializeSListHead(GHeader);
    
    Knight* knights[100];
    for (int i = 0; i < 100; i++)
    {
        knights[i] = ObjectPool<Knight>::Pop();
    }
    
    for (int i = 0; i < 100; i++)
    {
        ObjectPool<Knight>::Push(knights[i]);
        knights[i] = nullptr;
    }
    
    shared_ptr<Knight> sptr = ObjectPool<Knight>::MakeShared();
    shared_ptr<Knight> mptr = MakeShared<Knight>();
    
    for (int i = 0; i < 15; i++)
    {
        GThreadManager->Launch([]()
        {
            Knight* knigt = xnew<Knight>();
            
            cout << "Knight Hp "<< knigt->_hp << endl;
            
            this_thread::sleep_for(10ms);
            
            xdelete(knigt);
        });
    }
    
    GThreadManager->Join();
}