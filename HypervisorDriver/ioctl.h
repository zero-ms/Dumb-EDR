#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "irp.h"
#include "common.h"

/*유저모드 프로그램과 디바이스 드라이버 사이의 데이터를 주고받는 함수이다.
* Ioctl: Input/Output Control
*/
NTSTATUS DriverIoctlDispatcher(PDEVICE_OBJECT DeviceObject, PIRP Irp);

#pragma alloc_text(PAGE, DriverIoctlDispatcher)