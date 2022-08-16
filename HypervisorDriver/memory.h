#pragma once

#include <ntddk.h>
#include <wdm.h>
#include <wdf.h>

#include "msr.h"

/*가상 메모리 주소 -> 물리 메모리 주소 혹은 그 반대*/

UINT64 VirtualToPhysicalAddress(void* va);
UINT64 PhysicalToVirtualAddress(UINT64 pa);