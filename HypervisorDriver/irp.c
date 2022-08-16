#include "irp.h"

/*����̹����� ó�� ������ IRP Ÿ���� �ʱ�ȭ�ϴ� ��ƾ�̴�.*/
VOID InitIRPMjFunctions(PDRIVER_OBJECT DriverObject) 
{
	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) 
	{
		DriverObject->MajorFunction[i] = DriverUnsupported;		/*����̹����� ó���� �� �ִ� IRP ������ �ʱ�ȭ.*/
	}

	DbgPrint("[*] Setting Device major functions.\n");
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverClose;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverCreate;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverIoctlDispatcher;
	DriverObject->MajorFunction[IRP_MJ_READ] = DriverRead;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = DriverWrite;
}

VOID PrintIrpInfo(PIRP Irp)
{
    PIO_STACK_LOCATION  IrpStack;
    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    PAGED_CODE();

    DbgPrint("\tIrp->AssociatedIrp.SystemBuffer = 0x%p\n",
        Irp->AssociatedIrp.SystemBuffer);
    DbgPrint("\tIrp->UserBuffer = 0x%p\n", Irp->UserBuffer);
    DbgPrint("\tIrpStack->Parameters.DeviceIoControl.Type3InputBuffer = 0x%p\n",
        IrpStack->Parameters.DeviceIoControl.Type3InputBuffer);
    DbgPrint("\tIrpStack->Parameters.DeviceIoControl.InputBufferLength = %d\n",
        IrpStack->Parameters.DeviceIoControl.InputBufferLength);
    DbgPrint("\tIrpStack->Parameters.DeviceIoControl.OutputBufferLength = %d\n",
        IrpStack->Parameters.DeviceIoControl.OutputBufferLength);
}