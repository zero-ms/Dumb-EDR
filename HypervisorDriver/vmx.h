#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <wdm.h>

#include "ept.h"
#include "processor.h"
#include "memory.h"
#include "vmcs.h"

/*VMX를 초기화하고, 필요한 스트럭쳐들을 할당하는 기능이다.
* 하나의 논리적 프로세서에 하나의 가상머신이 할당되며, 하나의 가상머신에는
* 반드시 VMXON region과 VMCS region이 필요하다.
* Control Register에서 가상화에 필요한 bit를 켜준 후
* 각각의 논리적 프로세서에 VMXON region을 할당해야 VMM이 가상머신을 관리한다.
* 
*/

/*Ainline.asm 파일에서의 함수를 extern 으로 글로벌하게 선언하여 사용함.*/
extern void inline AsmEnableVmxOperation(void);

/*VMXON region, VMCS region의 피지컬 어드레스를 담아놓은 구조체이다.*/
typedef struct _VIRTUAL_MACHINE_STATE
{
    UINT64 VmxonRegion;        // VMXON region
    UINT64 VmcsRegion;        // VMCS region
    UINT64 Eptp;              // Extended-Page-Table Pointer
    UINT64 VmmStack;          // Stack for VMM in VM-Exit State
    UINT64 MsrBitmap;         // MSR Bitmap Virtual Address
    UINT64 MsrBitmapPhysical; // MSR Bitmap Physical Address
} VIRTUAL_MACHINE_STATE, * PVIRTUAL_MACHINE_STATE;

//
// Globals
//
ULONG ExitReason;
extern VIRTUAL_MACHINE_STATE* g_GuestState;
extern int ProcessorCounts;

BOOLEAN AllocateVmxonRegion(IN VIRTUAL_MACHINE_STATE* GuestState);
BOOLEAN AllocateVmcsRegion(IN VIRTUAL_MACHINE_STATE* GuestState);
BOOLEAN InitializeVmx();
VOID TerminateVmx();

UINT64 VmptrstInstruction();

extern ULONG64 inline GetGdtBase();
extern ULONG64 inline GetIdtBase();
extern void inline AsmEnableVmxOperation();
extern void inline AsmVmxoffAndRestoreState();
extern void inline AsmSaveStateForVmxoff();
extern void AsmVmexitHandler();

BOOLEAN IsVmxSupported();

VOID LaunchVm(int ProcessorID, PEPTP EPTP);
BOOLEAN SetupVmcs(VIRTUAL_MACHINE_STATE* GuestState, PEPTP EPTP);
BOOLEAN LoadVmcs(VIRTUAL_MACHINE_STATE* GuestState);
BOOLEAN ClearVmcsState(VIRTUAL_MACHINE_STATE* GuestState);
VOID VmResumeInstruction();