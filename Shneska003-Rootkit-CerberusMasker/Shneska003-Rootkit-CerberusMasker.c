// Shneska003-Rootkit-CerberusMasker.cpp
// Written for educational purpouses

#include <stdlib.h>
#include <Windows.h>
#include <Psapi.h>
#include <WinBase.h>

#define true 1
#define false 0

void FindProcs();
int locFrstDirDiv(wchar_t * ptr);

HWND taskmgr;
DWORD taskmgrpid;
DWORD prcs[1024] = { 0 };
wchar_t * finame;
wchar_t * procname;

int main()
{
	finame = (wchar_t*)malloc(256);
	procname = (wchar_t*)malloc(256);
	while (true) 
	{
		FindProcs();
	}
	
    return 0;
}

void FindProcs()
{
	DWORD retprcs = 0;
	EnumProcesses(prcs, 1024 * sizeof(DWORD), &retprcs);

	for (int i = 0; i < retprcs; i++)
	{
		DWORD prcID = 0;
		__asm
		{
			PUSH EAX
			PUSH EBX
			MOV EAX, [i]
			MOV EBX, 4
			MUL EBX
			LEA EBX, prcs
			ADD EBX, EAX
			MOV EAX, [EBX]
			MOV [prcID], EAX
			POP EBX
			POP EAX
		}
		HANDLE prc = OpenProcess(PROCESS_QUERY_INFORMATION, false, prcID);
		if (prc != NULL)
		{
			memset(finame, '\0', 256);
			GetProcessImageFileName(prc, finame, 256);
			int pthstr = locFrstDirDiv(finame);
			int relpthname = lstrlenW(finame) - pthstr;
			//wchar_t * procname = (wchar_t*)malloc(relpthname);
			for (int i = 0; i < relpthname+1; i++)
				*(procname + i * sizeof(char)) = *(finame + pthstr + i * sizeof(char));
			if (!lstrcmpiW(procname, L"taskmgr.exe"))
			{
				taskmgrpid = prcID;
				i = retprcs;
			}

		}
	}
	if (taskmgrpid == NULL)
		return;
	//TODO
	
}

int locFrstDirDiv(wchar_t * ptr)
{
	int len = lstrlenW(ptr);
	for (int i = len - 1; i != 0; i--)
	{
		char t = *(ptr + i);
		if (t == '\\')
			return i + 1;
	}
}
