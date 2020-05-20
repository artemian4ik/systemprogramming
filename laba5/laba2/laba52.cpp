#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>

HANDLE semaphore;

int random_int(int min, int max);
DWORD WINAPI thread_function(LPVOID param);
enum cases { first_param, second_param };

int main()
{
	int max_handles;
	int max_threads;
	srand(time(NULL));

	printf("Input max number of handles\n");
	printf(">>");
	scanf("%i", &max_handles);

	printf("Input max number of threads\n");
	printf(">>");
	scanf("%i", &max_threads);

	HANDLE* threads = new HANDLE[max_threads];
	semaphore = CreateSemaphoreA(NULL, max_handles, max_handles, "MySemaphore");
	if (semaphore == NULL)
		return 1;


	HANDLE file = CreateFileA("result.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);
		return 1;
	}
	CloseHandle(file);

	for (int i = 0; i < max_threads; i++)
	{
		int* params = new int[2];
		params[first_param] = i;
		params[second_param] = random_int(1, 5);

		threads[i] = CreateThread(NULL, 0, thread_function, (LPVOID)params, NULL, NULL);
	}
	WaitForMultipleObjects(max_threads, threads, TRUE, INFINITE);

	return 0;
}

DWORD WINAPI thread_function(LPVOID param)
{
	int* params = (int*)param;
	clock_t start = clock();
	int thread_number = params[first_param];
	LPSTR str = new CHAR[128];

	DWORD result = WaitForSingleObject(semaphore, 500);
	while (result != WAIT_OBJECT_0)
	{
		result = WaitForSingleObject(semaphore, 1000);
		printf("Thread %i waiting for semaphore\n", thread_number);
	}

	printf("Thread %i decrement semaphore. Going to sleep\n", thread_number);

	Sleep(params[1] * 1000);

	HANDLE file = CreateFileA("result.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		ReleaseSemaphore(semaphore, 1, NULL);
		return 0;
	}
	SetFilePointer(file, 0, NULL, FILE_END);

	clock_t finish = clock();
	float time_elapsed = (finish - start) / CLK_TCK;

	sprintf(str, "Thread %i made this in %f seconds\n\0", thread_number, time_elapsed);
	WriteFile(file, str, strlen(str), NULL, NULL);
	CloseHandle(file);

	printf("Thread %i released semaphore.\n", thread_number);
	ReleaseSemaphore(semaphore, 1, NULL);

	return 0;
}

int random_int(int min, int max)
{
	return min + rand() % (max + 1 - min);
}