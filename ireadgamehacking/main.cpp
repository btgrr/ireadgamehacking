#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include <cstdint>

#include <stdio.h>

int main()
{
	BOOL success = TRUE;

	DWORD processId = 10432;
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);

	if (process == NULL)
	{
		DWORD error = GetLastError();
		printf("unable to get process handle, error %d\n", (unsigned long)error);
		return 0;
	}

	printf("got process handle\n");

	int valueRead = 0;

	uintptr_t address = 0x00000015597FF704;

	// do your reading and writing here
	BOOL rpmStatus = ReadProcessMemory(process, (LPCVOID)address, &valueRead, sizeof(int), NULL);

	if (rpmStatus == FALSE)
	{
		DWORD error = GetLastError();
		printf("unable to read process memory @ 0x%p, error %d\n", address, (unsigned long)error);
		success = FALSE;
	}

	char varChar[128];
	address = 0x00000015597FF730;
	ReadProcessMemory(process, (LPVOID)address, &varChar, sizeof(varChar), NULL);
	printf("varChar[128]: %s\n", varChar);

	sprintf(varChar, "Bruh Bruh");
	BOOL wpmStatus = WriteProcessMemory(process, (LPVOID)address, &varChar, sizeof(varChar), NULL);

	if (wpmStatus == FALSE)
	{
		DWORD error = GetLastError();
		printf("unable to write process memory @ 0x%p, error %d\n", address, (unsigned long)error);
		success = FALSE;
	}

	int* pointer;
	address = 0x00000015597FF730;
	ReadProcessMemory(process, (LPCVOID)address, &varChar, sizeof(varChar), NULL);
	printf("varChar[128]: %s\n", varChar);

	if (success == TRUE)
	{
		printf("valueRead: %d\n", valueRead);
	}



	CloseHandle(process);

	
}