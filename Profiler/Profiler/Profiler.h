#pragma once
#include"stdafx.h"
#include<Windows.h>

class ProfileStructher
{
#define Max 50		//여기에서만 쓸 용도 이므로 class내부에서 정의해 준다.


private:
	struct Profile
	{
		bool flag;
		char Name[64];
		
		bool Beginflag;		//프로파일 시작여부
		__int64 Start_Time;	//시작시간
		__int64 TotalTime;	//모든 시간의 합계
		__int64 Min_Time[2];	//0번은 최소 걸린시간. 너무 작은값. 1번이 그 다음값
		__int64 Max_Time[2];	//0번은 최대 걸린 시간. 너무 큰 값. 1번은 그 다음 큰값.
		__int64 CallCNT;	//call한 횟수.
	};

	Profile profile_Array[Max];
	LARGE_INTEGER SecondFrequency;
	double MicroSecond;

public:
	ProfileStructher(void)
	{
		int cnt;
		for ( cnt = 0; cnt < Max; cnt++ )
		{
			profile_Array[cnt].flag = false;
			profile_Array[cnt].Name[0] = NULL;
			profile_Array[cnt].TotalTime = 0;
			profile_Array[cnt].Min_Time[0] = 0x7fffffffffffffff;		//최소 걸린 시간이므로 최대값으로 밀어준다.
			profile_Array[cnt].Min_Time[1] = 0x7fffffffffffffff;
			profile_Array[cnt].Max_Time[0] = 0;		//최대 걸린 시간이므로 최소값으로 밀어준다.
			profile_Array[cnt].Max_Time[1] = 0;
			profile_Array[cnt].CallCNT = 0;		//호출횟수 초기화
		}
		QueryPerformanceFrequency (&SecondFrequency);
		MicroSecond = (double) SecondFrequency.QuadPart / 1000;
	}
	~ProfileStructher (void)
	{

	}
	bool Set_Profile (char *name, __int64 SetTime);	//종료안된 프로파일 일 경우 false 리턴
	bool End_Profile (char *name, __int64 EndTime);	//없거나 시작되지 않은 프로파일 일경우 false 리턴.
	void Print_Profile (void);		//저장된 프로파일들을 파일로 출력함.
};

void Profile_Begin (char *name);
void Profile_End (char *name);
void Profile_Print (void);


//선언부
#define PROFILE_CHECK

#ifdef PROFILE_CHECK
#define PROFILE_BEGIN(X)	Profile_Begin(X)
#define PROFILE_END(X)		Profile_End(X)
#define PROFILE_PRINT		Profile_Print()

#else
#define PROFILE_BEGIN(X)
#define PROFILE_END(X)
#define PROFILE_PRINT

#endif