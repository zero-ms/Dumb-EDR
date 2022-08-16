#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "memory.h"

/*하이퍼바이저의 핵심 EPT(Extended Page Table)을 관리한다.
* 실제 가상메모리 페이지 처럼 EPT 또한 4단계 페이징을 통해
* 게스트의 물리 메모리 주소와 호스트의 물리 메모리 주소를 매칭한다.
* 4단계 페이징에 사용되는 테이블은 PML4, PDPT, PD, PT 이다.
*/


#define POOLTAG 0x48564653	/*non paged pool에서 사용되는 풀 태그이다. 별 다른 의미는 없고, 말 그대로 식별하기 위한 태그이다.*/

typedef union _EPTP
{
    ULONG64 All;
    struct
    {
        UINT64 MemoryType : 3;            // bit 2:0 (0 = Uncacheable (UC) - 6 = Write - back(WB))
        UINT64 PageWalkLength : 3;        // bit 5:3 (This value is 1 less than the EPT page-walk length)
        UINT64 DirtyAndAceessEnabled : 1; // bit 6  (Setting this control to 1 enables accessed and dirty flags for EPT)
        UINT64 Reserved1 : 5;             // bit 11:7
        UINT64 PML4Address : 36;
        UINT64 Reserved2 : 16;
    } Fields;
} EPTP, * PEPTP;

//
// See Table 28-1.
//
typedef union _EPT_PML4E
{
    ULONG64 All;
    struct
    {
        UINT64 Read : 1;               // bit 0
        UINT64 Write : 1;              // bit 1
        UINT64 Execute : 1;            // bit 2
        UINT64 Reserved1 : 5;          // bit 7:3 (Must be Zero)
        UINT64 Accessed : 1;           // bit 8
        UINT64 Ignored1 : 1;           // bit 9
        UINT64 ExecuteForUserMode : 1; // bit 10
        UINT64 Ignored2 : 1;           // bit 11
        UINT64 PhysicalAddress : 36;   // bit (N-1):12 or Page-Frame-Number
        UINT64 Reserved2 : 4;          // bit 51:N
        UINT64 Ignored3 : 12;          // bit 63:52
    } Fields;
} EPT_PML4E, * PEPT_PML4E;

//
// See Table 28-3
//
typedef union _EPT_PDPTE
{
    ULONG64 All;
    struct
    {
        UINT64 Read : 1;               // bit 0
        UINT64 Write : 1;              // bit 1
        UINT64 Execute : 1;            // bit 2
        UINT64 Reserved1 : 5;          // bit 7:3 (Must be Zero)
        UINT64 Accessed : 1;           // bit 8
        UINT64 Ignored1 : 1;           // bit 9
        UINT64 ExecuteForUserMode : 1; // bit 10
        UINT64 Ignored2 : 1;           // bit 11
        UINT64 PhysicalAddress : 36;   // bit (N-1):12 or Page-Frame-Number
        UINT64 Reserved2 : 4;          // bit 51:N
        UINT64 Ignored3 : 12;          // bit 63:52
    } Fields;
} EPT_PDPTE, * PEPT_PDPTE;

//
// See Table 28-5
//
typedef union _EPT_PDE
{
    ULONG64 All;
    struct
    {
        UINT64 Read : 1;               // bit 0
        UINT64 Write : 1;              // bit 1
        UINT64 Execute : 1;            // bit 2
        UINT64 Reserved1 : 5;          // bit 7:3 (Must be Zero)
        UINT64 Accessed : 1;           // bit 8
        UINT64 Ignored1 : 1;           // bit 9
        UINT64 ExecuteForUserMode : 1; // bit 10
        UINT64 Ignored2 : 1;           // bit 11
        UINT64 PhysicalAddress : 36;   // bit (N-1):12 or Page-Frame-Number
        UINT64 Reserved2 : 4;          // bit 51:N
        UINT64 Ignored3 : 12;          // bit 63:52
    } Fields;
} EPT_PDE, * PEPT_PDE;

//
// See Table 28-6
//
typedef union _EPT_PTE
{
    ULONG64 All;
    struct
    {
        UINT64 Read : 1;               // bit 0
        UINT64 Write : 1;              // bit 1
        UINT64 Execute : 1;            // bit 2
        UINT64 EPTMemoryType : 3;      // bit 5:3 (EPT Memory type)
        UINT64 IgnorePAT : 1;          // bit 6
        UINT64 Ignored1 : 1;           // bit 7
        UINT64 AccessedFlag : 1;       // bit 8
        UINT64 DirtyFlag : 1;          // bit 9
        UINT64 ExecuteForUserMode : 1; // bit 10
        UINT64 Ignored2 : 1;           // bit 11
        UINT64 PhysicalAddress : 36;   // bit (N-1):12 or Page-Frame-Number
        UINT64 Reserved : 4;           // bit 51:N
        UINT64 Ignored3 : 11;          // bit 62:52
        UINT64 SuppressVE : 1;         // bit 63
    } Fields;
} EPT_PTE, * PEPT_PTE;

enum INVEPT_TYPE
{
    SINGLE_CONTEXT = 0x00000001,
    ALL_CONTEXTS = 0x00000002,
};

typedef struct INVEPT_DESC
{
    EPTP   Eptp;
    UINT64 Reserved;
} INVEPT_DESC, * PINVEPT_DESC;

extern UINT64 g_VirtualGuestMemoryAddress;

//
// EPT Functions
//
PEPTP InitializeEptp();
unsigned char InveptAllContexts(void);
unsigned char InveptSingleContext(EPTP ept_pointer);

//
// Assembly functions
//
extern unsigned char inline AsmPerformInvept(_In_ unsigned long Type, _In_ void* Descriptor);