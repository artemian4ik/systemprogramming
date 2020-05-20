#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>

#define ARRAY_MAX 50000000

CRITICAL_SECTION section;
int* array;

DWORD WINAPI thread_function_min(LPVOID use_critical_section);
DWORD WINAPI thread_function_max(LPVOID use_critical_section);
DWORD WINAPI thread_function_avg(LPVOID use_critical_section);
void generate_array(int* array);
int random_int(int min, int max);

int main()
{
	HANDLE* threads;
	clock_t start;
	float elapsed_time;

	srand(time(NULL));
	InitializeCriticalSection(&section);

	array = new int[ARRAY_MAX];
	generate_array(array);

	start = clock();
	threads = new HANDLE[3];
	threads[0] = CreateThread(NULL, 0, thread_function_min, (LPVOID)TRUE, NULL, NULL);
	threads[1] = CreateThread(NULL, 0, thread_function_avg, (LPVOID)TRUE, NULL, NULL);
	threads[2] = CreateThread(NULL, 0, thread_function_max, (LPVOID)TRUE, NULL, NULL);
	WaitForMultipleObjects(3, threads, TRUE, INFINITE);
	for (int i = 0; i < 3; i++)
		CloseHandle(threads[i]);

	elapsed_time = ((float)(clock() - start)) / CLK_TCK;
	printf("With critical section it took %f seconds\n\n", elapsed_time);

	start = clock();
	threads = new HANDLE[3];
	threads[0] = CreateThread(NULL, 0, thread_function_min, (LPVOID)FALSE, NULL, NULL);
	threads[1] = CreateThread(NULL, 0, thread_function_avg, (LPVOID)FALSE, NULL, NULL);
	threads[2] = CreateThread(NULL, 0, thread_function_max, (LPVOID)FALSE, NULL, NULL);
	WaitForMultipleObjects(3, threads, TRUE, INFINITE);
	for (int i = 0; i < 3; i++)
		CloseHandle(threads[i]);

	elapsed_time = ((float)(clock() - start)) / CLK_TCK;
	printf("Without critical section it took %f seconds\n\n", elapsed_time);

	DeleteCriticalSection(&section);
}

DWORD WINAPI thread_function_min(LPVOID use_critical_section)
{
	if ((bool)use_critical_section)
	{
		while (!TryEnterCriticalSection(&section))
		{
			//Nothing. Waiting until critical section free.
		}
	}

	int min = array[0];
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		if (min > array[i])
			min = array[i];
	}
	printf("Min: %i\n", min);

	if ((bool)use_critical_section)
	{
		LeaveCriticalSection(&section);
	}

	return 0;
}

DWORD WINAPI thread_function_max(LPVOID use_critical_section)
{
	if ((bool)use_critical_section)
	{
		while (!TryEnterCriticalSection(&section))
		{
			//Nothing. Waiting until critical section free.
		}
	}

	int max = array[0];
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		if (max < array[i])
			max = array[i];
	}
	printf("Max: %i\n", max);

	if ((bool)use_critical_section)
	{
		LeaveCriticalSection(&section);
	}

	return 0;
}

DWORD WINAPI thread_function_avg(LPVOID use_critical_section)
{
	if ((bool)use_critical_section)
	{
		while (!TryEnterCriticalSection(&section))
		{
			//Nothing. Waiting until critical section free.
		}
	}

	float avg = 0;
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		avg += array[i];
	}
	printf("Avg: %f\n", avg / ARRAY_MAX);

	if ((bool)use_critical_section)
	{
		LeaveCriticalSection(&section);
	}

	return 0;
}

void generate_array(int* array)
{
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		array[i] = random_int(0, 500);
	}
}

int random_int(int min, int max)
{
	return min + rand() % (max + 1 - min);
}