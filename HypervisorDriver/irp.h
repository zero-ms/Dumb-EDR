#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "HypervisorDriver.h"
#include "ioctl.h"
#include "logger.h"

VOID InitIRPMjFunctions(PDRIVER_OBJECT DriverObject);
VOID PrintIrpInfo(PIRP Irp);
NTSTATUS DriverIoctlDispatcher(PDEVICE_OBJECT DeviceObject, PIRP Irp);

#pragma alloc_text(PAGE, DriverIoctlDispatcher)