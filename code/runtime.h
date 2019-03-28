#ifndef __COMMON_RUNTIME_H_
#define __COMMON_RUNTIME_H_

typedef void(__stdcall *FnAtExitCallback)(void);

void __stdcall runtime_atexit(FnAtExitCallback fnAtExitCallback);

// ��� ������� ������ �������� �������� ��� ��� ���������� ������.
void __stdcall runtime_shutdown();

#endif // __COMMON_ATEXIT_H_
