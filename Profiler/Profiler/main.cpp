// Profiler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Profiler.h"
#include <Windows.h>

int main()
{
	while ( 1 )
	{
		PROFILE_BEGIN ("1ms");
		Sleep (1);
		PROFILE_END ("1ms");

		PROFILE_BEGIN ("10ms");
		Sleep (10);
		PROFILE_END ("10ms");

		PROFILE_BEGIN ("100ms");
		Sleep (100);
		PROFILE_END ("100ms");

		PROFILE_PRINT;

	}

    return 0;
}

