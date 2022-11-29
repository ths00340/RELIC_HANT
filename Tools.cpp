#define _CRT_SECURE_NO_WARNINGS
#include "Tools.h"
#include <stdarg.h>
#include <stdio.h>

void TOOL::Display(char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char mojiretus[256];
	ZeroMemory(mojiretus, sizeof(mojiretus));
	vsprintf(mojiretus, format, ap);
	OutputDebugString(mojiretus);
}