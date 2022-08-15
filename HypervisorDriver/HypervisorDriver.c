#include "HypervisorDriver.h"

/*드라이버가 로드될 때 호출되는 루틴이다. 일반적인 프로그램에서의 main함수와 비슷하다.*/
NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);	/*드라이버에서는 사용하지 않는 파라미터는 에러로 간주하므로, 사용하지 않는다고 명시한다.*/

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject = NULL;
	UNICODE_STRING DriverName, DosDeviceName;

	DbgPrint("[*] Hypervisor driver loaded!\n");

	/*디바이스 드라이버의 이름을 할당함.*/
	RtlInitUnicodeString(&DriverName, L"\\Device\\DumbHypervisor");
	RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\DumbHypervisorDevice");

	/*디바이스 드라이버를 등록한 후, 등록이 성공적으로 이루어졌다면, 심볼릭 링크(바로가기라고 보면 됨)을 등록한다.*/
	/*DosDeviceName과 심볼릭 링킹을 하는 이유는 하위 호환성을 위한 것이다. 유저모드 프로그램에서는 여전히 디바이스를 다룰 때 DosDeviceName을 사용한다.*/
	status = IoCreateDevice(DriverObject, 0, &DriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);

	if (status == STATUS_SUCCESS) {
		InitIRPMjFunctions(DriverObject);

		DriverObject->DriverUnload = DriverUnload;
		/*비트 마스킹을 통해서 디바이스 드라이버의 Flags 값을 수정한다.*/
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

/*드라이버가 언로드 될 때 호출되는 루틴이다.*/
VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING DosDeviceName;

	DbgPrint("[*] Hypervisor driver unloaded!\n");

	RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\DumbHypervisor");

	/*로드 루틴에서 등록된 심볼릭 링크와 디바이스 드라이버를 삭제한다.*/
	IoDeleteSymbolicLink(&DosDeviceName);
	IoDeleteDevice(DriverObject->DeviceObject);
}

/*드라이버에 지원되지 않는 타입의 IRP가 들어올 경우 이를 처리하는 루틴이다.*/
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

	AsmEnableVmxOperation();	/*CR4 영역의 VMXE를 활성화 시켜 VMX Operation을 수행한다.*/
	DbgPrint("[*] VMX Operation Enabled Successfully!\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

/*IRP에서 Read 타입이 들어올 때 이를 처리하는 루틴이다.*/
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