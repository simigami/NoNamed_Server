#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include "LockFreeStack.h"
#include "ThreadManager.h"

static CoreGlobal GCoreGlobal;

DECLSPEC_ALIGN(16)
class Data // : public SListEntry
{
public:
    Data() : _entry(), _hp(rand() % 1000), _mp(rand() % 1000)
    {
    }

    SListEntry _entry;
    
    int64 _hp;
    int64 _mp;
};

SListHeader* GHeader;

int main()
{
    GHeader = new SListHeader();
    ASSERT_CRASH((uint64)GHeader % 16 == 0, "GHeader is not 16 byte aligned!");
    InitalizeHead(GHeader);
    
    for (int i = 0; i < 15; i++)
    {
        GThreadManager->Launch([]()
        {
            while (true)
            {
                Data* data = new Data();
                ASSERT_CRASH((uint64)data % 16 == 0, "Data is not 16 byte aligned!");
                
                PushEntryList(GHeader, (SListEntry*)data);
                this_thread::sleep_for(chrono::milliseconds(10));
            }
        });
    }
    
    for (int i = 0; i < 2; i++)
    {
        GThreadManager->Launch([]()
        {
            while (true)
            {
                Data* pop = nullptr;
                pop = (Data*)PopEntryList(GHeader);
                
                if (pop)
                {
                    cout << pop->_hp << " " << pop->_mp << endl;
                    delete(pop);
                }
                else
                {
                    cout << "NULL" << endl;
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
            }
        });
    }
    
    GThreadManager->Join();
}