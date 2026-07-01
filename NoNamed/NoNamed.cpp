#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include "Memory.h"
#include "ThreadManager.h"

// static CoreGlobal GCoreGlobal;

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

SLIST_HEADER* GHeader;

int main()
{
    GHeader = new SLIST_HEADER();
    ASSERT_CRASH((uint64)GHeader % 16 == 0, "GHeader is not 16 byte aligned!");
    ::InitializeSListHead(GHeader);
    
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