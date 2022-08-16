#include "HypervisorDriver.h"

NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	/*����̹������� ������� �ʴ� �Ķ���ʹ� ������ �����ϹǷ�, ������� �ʴ´ٰ� ����Ѵ�.*/
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject = NULL;
	UNICODE_STRING DriverName, DosDeviceName;

	DbgPrint("[*] Hypervisor driver loaded!\n");

	/*����̽� ����̹��� �̸��� �Ҵ���.*/
	RtlInitUnicodeString(&DriverName, L"\\Device\\DumbHypervisor");
	RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\DumbHypervisorDevice");

	/*����̽� ����̹��� ����� ��, ����� ���������� �̷�����ٸ�, �ɺ��� ��ũ(�ٷΰ����� ���� ��)�� ����Ѵ�.*/
	/*DosDeviceName�� �ɺ��� ��ŷ�� �ϴ� ������ ���� ȣȯ���� ���� ���̴�. ������� ���α׷������� ������ ����̽��� �ٷ� �� DosDeviceName�� ����Ѵ�.*/
	status = IoCreateDevice(DriverObject, 0, &DriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);

	if (status == STATUS_SUCCESS) {
		InitIRPMjFunctions(DriverObject);

		DriverObject->DriverUnload = DriverUnload;
		/*��Ʈ ����ŷ�� ���ؼ� ����̽� ����̹��� Flags ���� �����Ѵ�.*/
		DeviceObject->Flags |= IO_TYPE_DEVICE;
		DeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);
		IoCreateSymbolicLink(&DosDeviceName, &DriverName);
	}
	else
	{
		DbgPrint("[*] There were some errors in creating device.\n");
	}

	__try
	{
		//
		// Initiating EPTP and VMX
		//
		PEPTP EPTP = InitializeEptp();

		InitializeVmx();

		for (size_t i = 0; i < (100 * PAGE_SIZE) - 1; i++)
		{
			void* TempAsm = "\xF4";
			memcpy(g_VirtualGuestMemoryAddress + i, TempAsm, 1);
		}

		//
		// Launching VM for Test (in the 0th virtual processor)
		//
		int ProcessorID = 0;

		LaunchVm(ProcessorID, EPTP);
	}
	__except (GetExceptionCode())
	{
	}

	return status;
}

VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING DosDeviceName;

	DbgPrint("[*] Hypervisor driver unloaded!\n");

	RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\DumbHypervisor");

	/*�ε� ��ƾ���� ��ϵ� �ɺ��� ��ũ�� ����̽� ����̹��� �����Ѵ�.*/
	IoDeleteSymbolicLink(&DosDeviceName);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverUnsupported(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrint("[*] This function is not supported.\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DriverCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrint("[*] DriverCreate Called !");

	VmptrstInstruction();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DriverRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrint("[*] Not implemented yet.\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DriverWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrint("[*] Not implemented yet.\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DriverClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrint("[*] DrvClose Called !");

	/*VMX�� �����Ѵ�.*/
	TerminateVmx();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}