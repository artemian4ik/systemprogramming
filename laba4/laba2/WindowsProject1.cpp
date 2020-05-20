#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <locale.h>
using namespace std;

#define ARRAY_MAX 5

CRITICAL_SECTION criticalSection;
HANDLE* threads;
int tlsIndex;


void startThreds(int count);
DWORD WINAPI threadAction(LPVOID arr);
void FindDivider(int* mainArr, int* resultArr);
int GreatestCommonFactor(int num);
int ArraySum(int* arr);
void ShowArray(int* arr);



int main()
{
	//srand(time(0));
	int countThreds;
	cout << "Input count of threads :" << endl;
	cout << "Number of threds = ";
	cin >> countThreds;
	threads = new HANDLE[countThreds];
	InitializeCriticalSection(&criticalSection);
	startThreds(countThreds);
	WaitForMultipleObjects(countThreds, threads, TRUE, INFINITE);
	DeleteCriticalSection(&criticalSection);
	system("pause");
	return 0;
}


int GreatestCommonFactor(int num)
{
	int j = num / 2;
	for (int i = j; i >= 2; i--)
	{
		if (num % i == 0)
			return i;
	}
}

int ArraySum(int* arr)
{
	int result = 0;
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		result += arr[i];
	}
	return result;
}

void ShowArray(int* arr)
{
	for (int i = 0; i < ARRAY_MAX; i++) {
		printf("%i ", arr[i]);
	}
}


void startThreds(int count)
{
	InitializeCriticalSection(&criticalSection);
	int** arrays = new int*[count];
	tlsIndex = TlsAlloc();
	for (int i = 0; i < count; i++)
	{
		arrays[i] = new int[ARRAY_MAX];
		for (int j = 0; j < ARRAY_MAX; j++)
			arrays[i][j] = rand() % 90 + 10;

		threads[i] = CreateThread(NULL, 0, threadAction, arrays[i], NULL, NULL);
	}
}

DWORD WINAPI threadAction(LPVOID param)
{
	int sum = 0;

	EnterCriticalSection(&criticalSection);
	printf("ID thred: %u\n", GetCurrentThreadId());

	int* array = (int*)param;
	TlsSetValue(tlsIndex, (LPVOID)(new int[ARRAY_MAX]));
	printf("Array : ");
	ShowArray(array);
	FindDivider(array, (int*)TlsGetValue(tlsIndex));
	printf("\nArray dividers :\n");

	ShowArray((int*)TlsGetValue(tlsIndex));
	sum = ArraySum((int*)TlsGetValue(tlsIndex));
	printf("\nSumma = %d\n\n", sum);

	LeaveCriticalSection(&criticalSection);
	return 0;
}

void FindDivider(int* inArr, int* outArr)
{
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		outArr[i] = GreatestCommonFactor(inArr[i]);
	}
}
