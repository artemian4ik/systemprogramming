#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <locale.h>
using namespace std;

#define PACK 1
#define UNPACK 2
#define EXIT_APP 0


LPCSTR unpackCommand = "7z.exe e ";
LPCSTR packCommand = "7z.exe a -tzip ";

void UnpackFiles(LPSTR unpackFile, LPSTR resultFile);
void PackFiles(LPSTR unpackFile, LPSTR resultFile);
void ShowError();

int main()
{
	int menu;
	while (true)
	{
		cout << "[" << PACK << "] Pack files" << endl;
		cout << "[" << UNPACK << "] Unpack files" << endl;
		cout << "[" << EXIT_APP << "] Exit" << endl;
		cin >> menu;
		switch (menu)
		{
		case UNPACK:
		{
			LPSTR unpackFile = new CHAR[MAX_PATH];
			LPSTR resultFile = new CHAR[MAX_PATH];

			printf("Input full path to zip\n");
			scanf("%s", unpackFile);
			printf("Input full path to directory\n");
			scanf("%s", resultFile);
			UnpackFiles(unpackFile, resultFile);
			break;
		}
		case PACK:
		{
			LPSTR packFile = new CHAR[MAX_PATH];
			LPSTR resultFile = new CHAR[MAX_PATH];

			printf("Input full path to File/Directory\n");
			scanf("%s", packFile);
			printf("Input full path to archive (.zip)\n");
			scanf("%s", resultFile);
			PackFiles(packFile, resultFile);
			break;
		}
		case EXIT_APP: {
			return 0;
		}

		default: printf("Invalid input!\n"); break;
		}
		system("pause");
		system("cls");
	}
}

void UnpackFiles(LPSTR unpackFile, LPSTR resultFile)
{
	LPSTR commandLine = new CHAR[MAX_PATH];
	ZeroMemory(commandLine, MAX_PATH);

	strncpy(commandLine, unpackCommand, MAX_PATH - strlen(commandLine));
	strncat(commandLine, unpackFile, MAX_PATH - strlen(commandLine));

	strncat(commandLine, " -o", MAX_PATH - strlen(commandLine));
	strncat(commandLine, resultFile, MAX_PATH - strlen(commandLine));

	strncat(commandLine, " -y", MAX_PATH - strlen(commandLine));

	HANDLE hReadPipe;
	HANDLE hWritePipe;

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0);
	if (hReadPipe == INVALID_HANDLE_VALUE || hWritePipe == INVALID_HANDLE_VALUE)
		exit(1);

	if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0))
		exit(1);

	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(si);
	si.hStdError = hWritePipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessA(NULL, commandLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		ShowError();
	}
	else
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		DWORD readed = 0;
		LPSTR result = new CHAR[1024];
		ZeroMemory(result, 1024);
		OVERLAPPED overlapped;
		while (ReadFile(hReadPipe, result, 1024, &readed, &overlapped))
		{
			printf("%s", result);
		}
	}

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
void PackFiles(LPSTR packFile, LPSTR resultFile)
{

	LPSTR commandLine = new CHAR[MAX_PATH];
	ZeroMemory(commandLine, MAX_PATH);
	strncpy(commandLine, packCommand, MAX_PATH - strlen(commandLine));
	strncat(commandLine, resultFile, MAX_PATH - strlen(commandLine));

	strncat(commandLine, " ", MAX_PATH - strlen(commandLine));
	strncat(commandLine, packFile, MAX_PATH - strlen(commandLine));

	HANDLE hReadPipe;
	HANDLE hWritePipe;

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0);
	if (hReadPipe == INVALID_HANDLE_VALUE || hWritePipe == INVALID_HANDLE_VALUE)
		exit(1);

	if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0))
		exit(1);

	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(si);
	si.hStdError = hWritePipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));


	if (!CreateProcessA(NULL, commandLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
		ShowError();
	}
	else
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		DWORD readed = 0;
		LPSTR result = new CHAR[1024];
		ZeroMemory(result, 1024);
		OVERLAPPED overlapped;
		while (ReadFile(hReadPipe, result, 1024, &readed, &overlapped))
		{
			printf("%s", result);
		}
	}
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void ShowError()
{
	LPVOID msg;
	DWORD e_code = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		e_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&msg,
		0,
		NULL
	);
	wprintf(L"\nERROR : %s\n", (char*)msg);
}