#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <wdm.h>

#include "ept.h"
#include "processor.h"
#include "memory.h"
#include "vmcs.h"

/*VMX�� �ʱ�ȭ�ϰ�, �ʿ��� ��Ʈ���ĵ��� �Ҵ��ϴ� ����̴�.
* �ϳ��� ���� ���μ����� �ϳ��� ����ӽ��� �Ҵ�Ǹ�, �ϳ��� ����ӽſ���
* �ݵ�� VMXON region�� VMCS region�� �ʿ��ϴ�.
* Control Register���� ����ȭ�� �ʿ��� bit�� ���� ��
* ������ ���� ���μ����� VMXON region�� �Ҵ��ؾ� VMM�� ����ӽ��� �����Ѵ�.
* 
*/

/*Ainline.asm ���Ͽ����� �Լ��� extern ���� �۷ι��ϰ� �����Ͽ� �����.*/
extern void inline AsmEnableVmxOperation(void);

/*VMXON region, VMCS region�� ������ ��巹���� ��Ƴ��� ����ü�̴�.*/
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