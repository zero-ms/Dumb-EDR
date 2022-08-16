#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "irp.h"
#include "ioctl.h"
#include "vmx.h"
#include "ept.h"

/*드라이버가 로드될 때, 언로드 될 때, IRP 요청을 처리하는 콜백함수들이 선언되어있다.*/
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
VOID DriverUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS DriverCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverUnsupported(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

/*해당 함수들이 저장되는 코드 섹션의 이름을 지정. 무시해도 됨.*/
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DriverUnload)
#pragma alloc_text(PAGE, DriverCreate)
#pragma alloc_text(PAGE, DriverRead)
#pragma alloc_text(PAGE, DriverWrite)
#pragma alloc_text(PAGE, DriverClose)
#pragma alloc_text(PAGE, DriverUnsupported)