// Profiler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Profiler.h"
#include <Windows.h>
#include<process.h>

unsigned int WINAPI WorkerThread (LPVOID lParam);
bool flag = false;

int main()
{
	HANDLE Thread[3];
	for ( int Cnt = 0; Cnt < 3; Cnt++ )
	{
		Thread[Cnt] = ( HANDLE )_beginthreadex (NULL, 0, WorkerThread, NULL, true, NULL);
	}
	

	while ( 1 )
	{
		PROFILE_KEYPROC;

		//q
		if ( GetAsyncKeyState (0x51) & 0x8001 )
		{
			flag = true;

			WaitForMultipleObjects (3, Thread, TRUE, INFINITE);
			break;
		}


	}

    return 0;
}

unsigned int WINAPI WorkerThread (LPVOID lParam)
{
	int Cnt=0;
	while ( 1 )
	{

		PROFILE_BEGIN (L"One");
		for ( int Num = 0; Num < 100; Num++ )
		{
			Cnt += Num;
		}
		PROFILE_END (L"One");

		PROFILE_BEGIN (L"Two");
		for ( int Num = 0; Num < 1000; Num++ )
		{
			Cnt += Num;
		}
		PROFILE_END (L"Two");

		PROFILE_BEGIN (L"Three");
		for ( int Num = 0; Num < 10000; Num++ )
		{
			Cnt += Num;
		}
		PROFILE_END (L"Three");


		if ( flag == true )
		{
			return 0;
		}
	}
}