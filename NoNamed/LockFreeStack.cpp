#include "pch.h"
#include "LockFreeStack.h"

// Custom SList LV 1
// void InitalizeHead(SListHeader* header)
// {
//     header->next = nullptr;
// }
//
// void PushEntryList(SListHeader* header, SListEntry* entry)
// {
//     entry->next = header->next;
//     header->next = entry;
// }
//
// SListEntry* PopEntryList(SListHeader* header)
// {
//     SListEntry* first = header->next;
//     if (first == nullptr)
//     {
//         return nullptr;
//     }
//     else
//     {
//         header->next = first->next;
//         return first;
//     }
// }

// Custom SList LV 2
// void InitalizeHead(SListHeader* header)
// {
//     header->next = nullptr;
// }
//
// void PushEntryList(SListHeader* header, SListEntry* entry)
// {
//     // CAS
//     entry->next = header->next;
//     while (::_InterlockedCompareExchange64((volatile int64*)&header->next, 
//         (int64)entry, 
//         (int64)entry->next)  == 0)
//     {
//
//     }
// }
//
// SListEntry* PopEntryList(SListHeader* header)
// {
//     SListEntry* expected = header->next;
//     
//     // expected->next can be null if other thread erases expected node
//     // but assume no null in LV2
//     while (expected && ::_InterlockedCompareExchange64(
//         (int64*)&header->next, 
//         (int64)expected->next, 
//         (int64)expected) == 0)
//     {
//         // ABA Problem can be occured
//     }
//     
//     return expected;
// }

// LV 3
void InitalizeHead(SListHeader* header)
{
    header->alignment = 0;
    header->region = 0;
    header->next = nullptr;
}

void PushEntryList(SListHeader* header, SListEntry* entry)
{
    SListHeader expected = {};
    SListHeader desired = {};
    
    // 16 byte allign
    desired.HeaderX64.next = (((uint64)entry) >> 4);
    
    while (true)
    {
        expected = *header;
        
        entry->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
        desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
        desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;
        
        if (::_InterlockedCompareExchange128(
            (int64*)header, 
            desired.region, 
            desired.alignment, 
            (int64*)&expected) == 1
            )
        {
            break;
        }
    }
}

SListEntry* PopEntryList(SListHeader* header)
{
    SListHeader expected = {};
    SListHeader desired = {};
    
    SListEntry* entry = nullptr;
    while (true)
    {
        expected = *header;
        
        entry = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
        if (entry == nullptr) break;
        
        desired.HeaderX64.next = ((uint64)entry->next) >> 4;
        desired.HeaderX64.depth = expected.HeaderX64.depth - 1;
        desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;
        
        if (::_InterlockedCompareExchange128(
            (int64*)header,
            desired.region, 
            desired.alignment,
            (int64*)&expected) == 1
            )
        {
            break;
        }
    }
    
    return entry;
}