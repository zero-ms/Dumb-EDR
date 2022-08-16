#include "common.h"

VOID PrintChars(PCHAR  BufferAddress, size_t CountChars)
{
    PAGED_CODE();

    if (CountChars)
    {
        while (CountChars--)
        {
            if (*BufferAddress > 31 && *BufferAddress != 127)
            {
                KdPrint(("%c", *BufferAddress));
            }
            else
            {
                KdPrint(("."));
            }
            BufferAddress++;
        }
        KdPrint(("\n"));
    }
    return;
}