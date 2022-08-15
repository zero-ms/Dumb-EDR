#include "HypervisorDriver.h"

/*����̹��� �ε�� �� ȣ��Ǵ� ��ƾ�̴�. �Ϲ����� ���α׷������� main�Լ��� ����ϴ�.*/
NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);	/*����̹������� ������� �ʴ� �Ķ���ʹ� ������ �����ϹǷ�, ������� �ʴ´ٰ� ����Ѵ�.*/

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

	return status;
}

/*����̹��� ��ε� �� �� ȣ��Ǵ� ��ƾ�̴�.*/
VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING DosDeviceName;

	DbgPrint("[*] Hypervisor driver unloaded!\n");

	RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\DumbHypervisor");

	/*�ε� ��ƾ���� ��ϵ� �ɺ��� ��ũ�� ����̽� ����̹��� �����Ѵ�.*/
	IoDeleteSymbolicLink(&DosDeviceName);
	IoDeleteDevice(DriverObject->DeviceObject);
}

/*����̹��� �������� �ʴ� Ÿ���� IRP�� ���� ��� �̸� ó���ϴ� ��ƾ�̴�.*/
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

	AsmEnableVmxOperation();	/*CR4 ������ VMXE�� Ȱ��ȭ ���� VMX Operation�� �����Ѵ�.*/
	DbgPrint("[*] VMX Operation Enabled Successfully!\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

/*IRP���� Read Ÿ���� ���� �� �̸� ó���ϴ� ��ƾ�̴�.*/
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

	DbgPrint("[*] Not implemented yet.\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}