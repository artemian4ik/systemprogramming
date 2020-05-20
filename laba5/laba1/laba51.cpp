#include <stdio.h>
#include <Windows.h>

int main()
{
	HANDLE mutex = CreateMutexA(NULL, FALSE, "MyMutex");

	if (WaitForSingleObject(mutex, 0) == WAIT_OBJECT_0)
	{
		printf("Program started! Press any key to stop!");
		getchar();
		ReleaseMutex(mutex);
	}
	else
	{
		printf("Can't start program!");
		getchar();
	}

	CloseHandle(mutex);
	return 0;
}