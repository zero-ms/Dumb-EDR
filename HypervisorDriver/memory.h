#pragma once

#include <ntddk.h>
#include <wdm.h>

UINT64 VirtualToPhysicalAddress(void* va);
UINT64 PhysicalToVirtualAddress(UINT64 pa);