#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "irp.h"
#include "ioctl.h"
#include "vmx.h"
#include "ept.h"

/*����̹��� �ε�� ��, ��ε� �� ��, IRP ��û�� ó���ϴ� �ݹ��Լ����� ����Ǿ��ִ�.*/
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
VOID DriverUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS DriverCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverUnsupported(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

/*�ش� �Լ����� ����Ǵ� �ڵ� ������ �̸��� ����. �����ص� ��.*/
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DriverUnload)
#pragma alloc_text(PAGE, DriverCreate)
#pragma alloc_text(PAGE, DriverRead)
#pragma alloc_text(PAGE, DriverWrite)
#pragma alloc_text(PAGE, DriverClose)
#pragma alloc_text(PAGE, DriverUnsupported)