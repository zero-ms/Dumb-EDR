#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "HypervisorDriver.h"
#include "ioctl.h"
#include "common.h"

/*IRP major function(처리 가능한 irp 타입들)을 초기화 하거나,
* IRP정보를 출력한다.
*/

VOID InitIRPMjFunctions(PDRIVER_OBJECT DriverObject);
VOID PrintIrpInfo(PIRP Irp);