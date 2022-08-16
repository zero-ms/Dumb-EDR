#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "HypervisorDriver.h"
#include "ioctl.h"
#include "common.h"

/*IRP major function(ó�� ������ irp Ÿ�Ե�)�� �ʱ�ȭ �ϰų�,
* IRP������ ����Ѵ�.
*/

VOID InitIRPMjFunctions(PDRIVER_OBJECT DriverObject);
VOID PrintIrpInfo(PIRP Irp);