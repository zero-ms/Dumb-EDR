#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "irp.h"
#include "common.h"

/*������� ���α׷��� ����̽� ����̹� ������ �����͸� �ְ�޴� �Լ��̴�.
* Ioctl: Input/Output Control
*/
NTSTATUS DriverIoctlDispatcher(PDEVICE_OBJECT DeviceObject, PIRP Irp);

#pragma alloc_text(PAGE, DriverIoctlDispatcher)