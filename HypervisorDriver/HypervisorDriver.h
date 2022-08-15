#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "irp.h"

/*Assembly Function*/
extern void inline AsmEnableVmxOperation(void);

/*Driver routine, IRP handling routine.*/
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
VOID DriverUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS DriverCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverUnsupported(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DriverUnload)
#pragma alloc_text(PAGE, DriverCreate)
#pragma alloc_text(PAGE, DriverRead)
#pragma alloc_text(PAGE, DriverWrite)
#pragma alloc_text(PAGE, DriverClose)
#pragma alloc_text(PAGE, DriverUnsupported)