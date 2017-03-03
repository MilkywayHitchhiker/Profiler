#pragma once
#include"stdafx.h"
#include<Windows.h>

class ProfileStructher
{
#define Max 50		//���⿡���� �� �뵵 �̹Ƿ� class���ο��� ������ �ش�.


private:
	struct Profile
	{
		bool flag;
		char Name[64];
		
		bool Beginflag;		//�������� ���ۿ���
		__int64 Start_Time;	//���۽ð�
		__int64 TotalTime;	//��� �ð��� �հ�
		__int64 Min_Time[2];	//0���� �ּ� �ɸ��ð�. �ʹ� ������. 1���� �� ������
		__int64 Max_Time[2];	//0���� �ִ� �ɸ� �ð�. �ʹ� ū ��. 1���� �� ���� ū��.
		__int64 CallCNT;	//call�� Ƚ��.
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
			profile_Array[cnt].Min_Time[0] = 0x7fffffffffffffff;		//�ּ� �ɸ� �ð��̹Ƿ� �ִ밪���� �о��ش�.
			profile_Array[cnt].Min_Time[1] = 0x7fffffffffffffff;
			profile_Array[cnt].Max_Time[0] = 0;		//�ִ� �ɸ� �ð��̹Ƿ� �ּҰ����� �о��ش�.
			profile_Array[cnt].Max_Time[1] = 0;
			profile_Array[cnt].CallCNT = 0;		//ȣ��Ƚ�� �ʱ�ȭ
		}
		QueryPerformanceFrequency (&SecondFrequency);
		MicroSecond = (double) SecondFrequency.QuadPart / 1000;
	}
	~ProfileStructher (void)
	{

	}
	bool Set_Profile (char *name, __int64 SetTime);	//����ȵ� �������� �� ��� false ����
	bool End_Profile (char *name, __int64 EndTime);	//���ų� ���۵��� ���� �������� �ϰ�� false ����.
	void Print_Profile (void);		//����� �������ϵ��� ���Ϸ� �����.
};

void Profile_Begin (char *name);
void Profile_End (char *name);
void Profile_Print (void);


//�����
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