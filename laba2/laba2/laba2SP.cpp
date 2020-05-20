#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <strsafe.h>
#include <string>

#define BUFSIZE MAX_PATH

using namespace std;
void wrap_error() {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);
	wprintf(L"error %d: %s\n", dw, lpMsgBuf);
}
string SDate(unsigned short day, unsigned short month, unsigned short year) {
	string rs = std::to_string(day) + "th of "s;
	switch (month)
	{
	case 1:
		rs += "January";
		break;
	case 2:
		rs += "February";
		break;
	case 3:
		rs += "March";
		break;
	case 4:
		rs += "April";
		break;
	case 5:
		rs += "May";
		break;
	case 6:
		rs += "June";
		break;
	case 7:
		rs += "July";
		break;
	case 8:
		rs += "August";
		break;
	case 9:
		rs += "September";
		break;
	case 10:
		rs += "October";
		break;
	case 11:
		rs += "November";
		break;
	case 12:
		rs += "December";
		break;
	default:
		rs += "???";
		break;
	}
	rs += " "s + std::to_string(year);
	return rs;
}
string STime(unsigned short hour, unsigned short minute, unsigned short second) {
	return std::string(std::to_string(hour) + ":"s + std::to_string(minute) + ":"s + std::to_string(second));
}
void CurrentDirectory(TCHAR* Path) {
	TCHAR Buffer[BUFSIZE];
	DWORD dwRet;
	dwRet = GetCurrentDirectory(BUFSIZE, Buffer);
	if (dwRet == 0)
	{
		printf("GetCurrentDirectory failed (%d)\n", GetLastError());
		return;
	}
	if (dwRet > BUFSIZE)
	{
		printf("Buffer too small; need %d characters\n", dwRet);
		return;
	}
	for (int i = 0; i < BUFSIZE; ++i) {
		Path[i] = Buffer[i];
	}
}
int fileExists(TCHAR* file)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(file, &FindFileData);
	int found = handle != INVALID_HANDLE_VALUE;
	if (found)
	{
		FindClose(handle);
	}
	return found;
}
#pragma region UserCases
void cs_change_directory() {
	string new_Path;
	cout << "Change directory to: ";
	getline(cin, new_Path);
	getline(cin, new_Path);
	wstring stemp = wstring(new_Path.begin(), new_Path.end());
	LPCWSTR error_switch = stemp.c_str();
	if (!SetCurrentDirectory(error_switch))
	{
		printf("SetCurrentDirectory failed (%d)\n", GetLastError());
	}
}
void cs_list_files(TCHAR Path[BUFSIZE]) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA found_file_data;
	LARGE_INTEGER filesize;
	SYSTEMTIME local_time;
	FILETIME system_time;

	StringCchCat(Path, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(Path, &found_file_data);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		wrap_error();
	}
	else {
		do
		{
			if (found_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				_tprintf(TEXT("  %s   <DIR>\n"), found_file_data.cFileName);
			}
			else
			{
				filesize.LowPart = found_file_data.nFileSizeLow;
				filesize.HighPart = found_file_data.nFileSizeHigh;
				FileTimeToLocalFileTime(&found_file_data.ftCreationTime, &system_time);
				FileTimeToSystemTime(&system_time, &local_time);
				_tprintf(TEXT("  %s   %ld bytes   "), found_file_data.cFileName, filesize.QuadPart);
				cout << SDate(local_time.wDay, local_time.wMonth, local_time.wYear) << " ";
				cout << STime(local_time.wHour, local_time.wMinute, local_time.wSecond) << endl;
			}
		} while (FindNextFile(hFind, &found_file_data) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES)
		{
			wrap_error();
		}
		FindClose(hFind);
	}
}
void cs_copy_file() {
	string new_Path;

	cout << "Copy file (path): ";
	getline(cin, new_Path);
	getline(cin, new_Path);
	wstring stemp1 = wstring(new_Path.begin(), new_Path.end());
	LPCWSTR source = stemp1.c_str();
	cout << " to ";
	getline(cin, new_Path);
	wstring stemp = wstring(new_Path.begin(), new_Path.end());
	LPCWSTR destination = stemp.c_str();
	if (CopyFile(source, destination, 1) == 0) {
		wrap_error();
	}
}
void cs_create_directory() {
	string new_Path;

	cout << "Directory (path): ";
	getline(cin, new_Path);
	getline(cin, new_Path);
	wstring stemp = wstring(new_Path.begin(), new_Path.end());
	LPCWSTR destination = stemp.c_str();
	if (CreateDirectoryW(destination, NULL) == 0) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			cout << "The specified directory already exists." << endl;
		}
		else {
			cout << "One or more intermediate directories do not exist" << endl;
		}
	}
}
void cs_delete_file_or_empty_directory() {
	string new_Path;

	cout << "Path: ";
	getline(cin, new_Path);
	getline(cin, new_Path);
	wstring stemp = wstring(new_Path.begin(), new_Path.end());
	LPCWSTR destination = stemp.c_str();
	DWORD ftyp = GetFileAttributesW(destination);
	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		wrap_error();
	}
	else if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		if (RemoveDirectoryW(destination) == 0) {
			wrap_error();
		}
	}
	else {
		if (DeleteFileW(destination) == 0) {
			if (GetLastError() == ERROR_FILE_NOT_FOUND) {
				cout << "Requested file wasn't found.\n";
			}
			else {
				cout << "Requested file is inaccessible.\n";
			}
		}
	}
}
void cs_extended_file_info() {
	string new_Path;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA found_file_data;
	LARGE_INTEGER filesize;
	SYSTEMTIME local_time;
	FILETIME system_time;

	cout << "File name (path): ";
	getline(cin, new_Path);
	getline(cin, new_Path);
	wstring stemp = wstring(new_Path.begin(), new_Path.end());
	LPCWSTR destination = stemp.c_str();
	hFind = FindFirstFileW(destination, &found_file_data);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		wrap_error();
	}
	else {
		filesize.LowPart = found_file_data.nFileSizeLow;
		filesize.HighPart = found_file_data.nFileSizeHigh;
		_tprintf(TEXT("Name: %s\n Size: %ld bytes\n"), found_file_data.cFileName, filesize.QuadPart);
		FileTimeToLocalFileTime(&found_file_data.ftCreationTime, &system_time);
		FileTimeToSystemTime(&system_time, &local_time);
		cout << "Creation Time: ";
		cout << SDate(local_time.wDay, local_time.wMonth, local_time.wYear) << " ";
		cout << STime(local_time.wHour, local_time.wMinute, local_time.wSecond) << endl;
		FileTimeToLocalFileTime(&found_file_data.ftLastAccessTime, &system_time);
		FileTimeToSystemTime(&system_time, &local_time);
		cout << "Last Access Time: ";
		cout << SDate(local_time.wDay, local_time.wMonth, local_time.wYear) << " ";
		cout << STime(local_time.wHour, local_time.wMinute, local_time.wSecond) << endl;
		FileTimeToLocalFileTime(&found_file_data.ftLastWriteTime, &system_time);
		FileTimeToSystemTime(&system_time, &local_time);
		cout << "Last Write Time: ";
		cout << SDate(local_time.wDay, local_time.wMonth, local_time.wYear) << " ";
		cout << STime(local_time.wHour, local_time.wMinute, local_time.wSecond) << endl;
		cout << "File Attributes: " << found_file_data.dwFileAttributes << endl;
	}
}
#pragma endregion UserCases
int main()
{
#pragma region Variables
	enum cases { exit, change_dir, list_files, copy_file, create_dir, delete_file_or_empty_dir, extended_file_info };
	int mode;
	bool non_program_exit_trigger = true;
	TCHAR Path[BUFSIZE];

#pragma endregion Variables
	do
	{
		CurrentDirectory(Path);
		_tprintf(TEXT("Current directory: %s\n"), Path);
		cout << "Commands:\n1 - change directory;\n2 - list files in current directory;\n3 - copy file;\n";
		cout << "4 - create directory;\n5 - delete file (empty directory);\n6 - extended file info;\n0 - close program;\n";
		cin >> mode;
		switch (mode)
		{
		case exit:
			non_program_exit_trigger = false;
			break;
		case change_dir: {
			cs_change_directory();
			break;
		}
		case list_files: {
			cs_list_files(Path);
			break;
		}
		case copy_file: {
			cs_copy_file();
			break;
		}
		case create_dir: {
			cs_create_directory();
			break;
		}
		case delete_file_or_empty_dir: {
			cs_delete_file_or_empty_directory();
			break;
		}
		case extended_file_info: {
			cs_extended_file_info();
			break;
		}
		default:
			non_program_exit_trigger = false;
			break;
		}
	} while (non_program_exit_trigger);
}