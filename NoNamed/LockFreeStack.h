#pragma once

// Custom SList LV 1
// struct SListEntry
// {
//     SListEntry* next;
// };
//
// struct SListHeader
// {
//     SListEntry* next = nullptr;
// };
//
// void InitalizeHead(SListHeader* header);
// void PushEntryList(SListHeader* header, SListEntry* entry);
// SListEntry* PopEntryList(SListHeader* header);

// Custom SList LV 2
// struct SListEntry
// {
//     SListEntry* next;
// };
//
// struct SListHeader
// {
//     SListEntry* next = nullptr;
// };
//
// void InitalizeHead(SListHeader* header);
// void PushEntryList(SListHeader* header, SListEntry* entry);
// SListEntry* PopEntryList(SListHeader* header);

// LV 3
DECLSPEC_ALIGN(16) // 16 byte align
struct SListEntry
{
    SListEntry* next;
};

DECLSPEC_ALIGN(16) // 16 byte align
struct SListHeader
{
    SListHeader()
    {
        alignment = 0;
        region = 0;
    }
    
    union 
    {
        struct
        {
            uint64 alignment;
            uint64 region;
        } DUMMYSTRUCTNAME;
        
        struct
        {
            uint64 depth : 16;
            uint64 sequence : 48;
            uint64 reserved : 4;
            uint64 next : 60;
        } HeaderX64;
    };
    
    SListEntry* next = nullptr;
};

void InitalizeHead(SListHeader* header);
void PushEntryList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntryList(SListHeader* header);