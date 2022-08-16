#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>
#include <strsafe.h>
#include "Ioctl.h"

using namespace std;

void PrintTitle() 
{
	cout <<
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

bool TestIoctl(HANDLE Handle)
{
    char  OutputBuffer[1000];
    char  InputBuffer[1000];
    ULONG BytesReturned;
    BOOL  Result;

    //
    // Performing METHOD_BUFFERED
    //
    StringCbCopyA(InputBuffer, sizeof(InputBuffer), "This String is from User Application; using METHOD_BUFFERED");

    printf("\nCalling DeviceIoControl METHOD_BUFFERED:\n");

    memset(OutputBuffer, 0, sizeof(OutputBuffer));

    Result = DeviceIoControl(Handle,
        (DWORD)IOCTL_SIOCTL_METHOD_BUFFERED,
        &InputBuffer,
        (DWORD)strlen(InputBuffer) + 1,
        &OutputBuffer,
        sizeof(OutputBuffer),
        &BytesReturned,
        NULL);

    if (!Result)
    {
        printf("Error in DeviceIoControl : %d", GetLastError());
        return false;
    }
    printf("    OutBuffer (%d): %s\n", BytesReturned, OutputBuffer);

    //
    // Performing METHOD_NIETHER
    //

    printf("\nCalling DeviceIoControl METHOD_NEITHER\n");

    StringCbCopyA(InputBuffer, sizeof(InputBuffer), "This String is from User Application; using METHOD_NEITHER");
    memset(OutputBuffer, 0, sizeof(OutputBuffer));

    Result = DeviceIoControl(Handle,
        (DWORD)IOCTL_SIOCTL_METHOD_NEITHER,
        &InputBuffer,
        (DWORD)strlen(InputBuffer) + 1,
        &OutputBuffer,
        sizeof(OutputBuffer),
        &BytesReturned,
        NULL);

    if (!Result)
    {
        printf("Error in DeviceIoControl : %d\n", GetLastError());
        return false;
    }

    printf("    OutBuffer (%d): %s\n", BytesReturned, OutputBuffer);

    //
    // Performing METHOD_IN_DIRECT
    //

    printf("\nCalling DeviceIoControl METHOD_IN_DIRECT\n");

    StringCbCopyA(InputBuffer, sizeof(InputBuffer), "This String is from User Application; using METHOD_IN_DIRECT");
    StringCbCopyA(OutputBuffer, sizeof(OutputBuffer), "This String is from User Application in OutBuffer; using METHOD_IN_DIRECT");

    Result = DeviceIoControl(Handle,
        (DWORD)IOCTL_SIOCTL_METHOD_IN_DIRECT,
        &InputBuffer,
        (DWORD)strlen(InputBuffer) + 1,
        &OutputBuffer,
        sizeof(OutputBuffer),
        &BytesReturned,
        NULL);

    if (!Result)
    {
        printf("Error in DeviceIoControl : %d", GetLastError());
        return false;
    }

    printf("    Number of bytes transfered from OutBuffer: %d\n",
        BytesReturned);

    //
    // Performing METHOD_OUT_DIRECT
    //

    printf("\nCalling DeviceIoControl METHOD_OUT_DIRECT\n");

    StringCbCopyA(InputBuffer, sizeof(InputBuffer), "This String is from User Application; using METHOD_OUT_DIRECT");

    memset(OutputBuffer, 0, sizeof(OutputBuffer));

    Result = DeviceIoControl(Handle,
        (DWORD)IOCTL_SIOCTL_METHOD_OUT_DIRECT,
        &InputBuffer,
        (DWORD)strlen(InputBuffer) + 1,
        &OutputBuffer,
        sizeof(OutputBuffer),
        &BytesReturned,
        NULL);

    if (!Result)
    {
        printf("Error in DeviceIoControl : %d", GetLastError());
        return false;
    }

    printf("    OutBuffer (%d): %s\n", BytesReturned, OutputBuffer);

    return true;
}

int main()
{
    string CpuID;

    PrintTitle();

    CpuID = GetCpuID();

    printf("[*] The CPU Vendor is : %s\n", CpuID.c_str());

    if (CpuID == "GenuineIntel")
    {
        printf("[*] The Processor virtualization technology is VT-x. \n");
    }
    else
    {
        printf("[*] This program is not designed to run in a non-VT-x environemnt !\n");
        return 1;
    }

    if (DetectVmxSupport())
    {
        printf("[*] VMX Operation is supported by your processor .\n");
    }
    else
    {
        printf("[*] VMX Operation is not supported by your processor .\n");
        return 1;
    }

    HANDLE Handle = CreateFileA("\\\\.\\DumbHypervisorDevice",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,
        NULL, /// lpSecurityAttirbutes
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL |
        FILE_FLAG_OVERLAPPED,
        NULL); /// lpTemplateFile

    if (Handle == INVALID_HANDLE_VALUE)
    {
        DWORD ErrNum = GetLastError();
        printf("[*] CreateFile failed : %d\n", ErrNum);
        return 1;
    }

    //
    // Test IOCTL
    //
    TestIoctl(Handle);

    CloseHandle(Handle);

    getchar();

    return 0;
}