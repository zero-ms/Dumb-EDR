#pragma once

#include <ntddk.h>
#include <wdm.h>

#include "msr.h"
#include "common.h"

BOOLEAN IsVmxSupported();
void RunOnEachLogicalProcessor(void* (*FunctionPtr)());
int MathPower(int Base, int Exponent);