#pragma once

#include <ntddk.h>
#include <wdm.h>
#include <wdf.h>

#include "msr.h"

/*���� �޸� �ּ� -> ���� �޸� �ּ� Ȥ�� �� �ݴ�*/

UINT64 VirtualToPhysicalAddress(void* va);
UINT64 PhysicalToVirtualAddress(UINT64 pa);