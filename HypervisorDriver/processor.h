#pragma once

#include <ntddk.h>
#include <wdm.h>

BOOLEAN IsVmxSupported();
void RunOnEachLogicalProcessor(void* (*FunctionPtr)());
int MathPower(int Base, int Exponent);