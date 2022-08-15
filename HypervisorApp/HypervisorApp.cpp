#include <iostream>
#include <Windows.h>

void PrintTitle() 
{
	std::cout <<
		"ooooooooo                             oooo                 ooooooooooo ooooooooo  oooooooooo \n"
		"888    88o oooo  oooo  oo ooo oooo    888ooooo             888    88   888    88o 888    888 \n"
		"888    888  888   888   888 888 888   888    888 ooooooooo 888ooo8     888    888 888oooo88 \n"
		"888    888  888   888   888 888 888   888    888           888    oo   888    888 888  88o \n"
		"o888ooo88     888o88 8o o888o888o888o o888ooo88            o888ooo8888 o888ooo88  o888o  88o8 \n\n\n";
}

std::string GetCpuID()
{
	char   SysType[13];
	std::string CpuID;
	_asm
	{
		XOR EAX, EAX
		CPUID
		MOV EAX, EBX
		MOV SysType[0], AL
		MOV SysType[1], AH
		SHR EAX, 16
		MOV SysType[2], AL
		MOV SysType[3], AH
		MOV EAX, EDX
		MOV SysType[4], AL
		MOV SysType[5], AH
		SHR EAX, 16
		MOV SysType[6], AL
		MOV SysType[7], AH
		MOV EAX, ECX
		MOV SysType[8], AL
		MOV SysType[9], AH
		SHR EAX, 16
		MOV SysType[10], AL
		MOV SysType[11], AH
		MOV SysType[12], 00
	}
	CpuID.assign(SysType, 12);
	return CpuID;
}

bool
DetectVmxSupport()
{
	bool VMX = false;
	__asm {
		XOR EAX, EAX
		INC EAX
		CPUID
		BT ECX, 0x5
		JC VMXSupport

		VMXNotSupport:
		JMP NopInstr

		VMXSupport:
		MOV VMX, 0x1

		NopInstr:
		NOP
	}

	return VMX;
}


int main()
{
	PrintTitle();

	std::string cpu_id;
	cpu_id = GetCpuID();

	printf("[*] The CPU Vendor is: %s\n", cpu_id.c_str());

	if (cpu_id == "GenuineIntel") 
	{
		printf("[*] The Processor virtualization technology is VT-x.\n");
	}
	else 
	{
		printf("[*] This program is not designed to run in a non-VT-x environment !\n");
		return 1;
	}

	if (DetectVmxSupport())
	{
		printf("[*] VMX Operation is supported by your processor.\n");
	}
	else
	{
		printf("[*] VMX Operation is not supported by your processor");
		return 1;
	}

	HANDLE hwnd = CreateFileW(L"\\\\.\\DumbHypervisorDevice",
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	getchar();

	return 0;
}