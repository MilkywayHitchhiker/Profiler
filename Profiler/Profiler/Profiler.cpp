#include"stdafx.h"
#include"Profiler.h"

/////////////////////////////////////////////////////////////////////////////////////
//프로파일 셋팅부
/////////////////////////////////////////////////////////////////////////////////////
bool ProfileStructher::Set_Profile (char *name, __int64 SetTime)
{
	int cnt;
	for ( cnt = 0; cnt < Max; cnt++ )
	{
		//배열을 돌면서 false를 만났다면 해당 프로파일이 존재하지 않으므로 새로 셋팅해 주어야 된다.
		if ( false == profile_Array[cnt].flag )
		{
			break;
		}
		//배열을 돌면서 같은 이름의 프로파일를 만나게 되면은 해당 프로파일의 시작시간이 0으로 초기화 되어있는지 확인. 
		else if ( 0 == strcmp (profile_Array[cnt].Name, name) )
		{
			//해당 프로파일의 beginflag가 true라면 종료되지 않은 프로파일 이므로 false를 리턴
			if ( true == profile_Array[cnt].Beginflag )
			{
				return false;
			}
			break;
		}
	}

	//배열을 오버하여 저장하지 못한것은 false로 리턴.
	if ( cnt >= Max )
	{
		return false;
	}
	else
	{
		profile_Array[cnt].flag = true;
		strcpy_s (profile_Array[cnt].Name,sizeof(profile_Array[cnt].Name), name);
		profile_Array[cnt].Start_Time = SetTime;
		profile_Array[cnt].Beginflag = true;
		return true;
	}

}

/////////////////////////////////////////////////////////////////////////////////////
//프로파일 종료부
////////////////////////////////////////////////////////////////////////////////////
bool ProfileStructher::End_Profile (char *name, __int64 EndTime)
{
	int cnt;
	__int64 Time;


	for ( cnt = 0; cnt < Max; cnt++ )
	{
		//배열의 flag가 false라면 해당 프로파일이 존재하지 않는것이므로 false를 리턴한다.
		if ( false == profile_Array[cnt].flag )
		{
			return false;
		}
		//배열의 해당 프로파일이 존재한다면 Beginflag로 프로파일의 시작여부를 확인.
		else if ( 0 == strcmp (profile_Array[cnt].Name, name) )
		{
			//Beginflag가 true라면 해당 프로파일이 시작되어있으므로 종료처리 false라면 시작되지 않은 프로파일이므로 false를 리턴한다.
			if ( true == profile_Array[cnt].Beginflag )
			{
				profile_Array[cnt].Beginflag = false;
				profile_Array[cnt].CallCNT++;
				Time = EndTime - profile_Array[cnt].Start_Time;

				//최대 시간값보다 클 경우 최대 시간값에 저장하고 현재 최대 시간값은 실질 최대 시간값에 저장한다.
				if ( Time > profile_Array[cnt].Max_Time[0] )
				{
					profile_Array[cnt].Max_Time[1] = profile_Array[cnt].Max_Time[0];
					profile_Array[cnt].Max_Time[0] = Time;
				}
				//최대 시간값보다 작고 실질 최대 시간값보다 큰경우 실질 최대 시간값은 TotalTime에 포함하고 실질 최대 시간값에 저장한다.
				else if ( Time > profile_Array[cnt].Max_Time[1] )
				{
					//처음 한번은 초기화로 인한 변수가 들어가 있으므로 버린다.
					if ( profile_Array[cnt].Max_Time[1] != 0 )
					{
						profile_Array[cnt].TotalTime += profile_Array[cnt].Max_Time[1];
					}
					profile_Array[cnt].Max_Time[1] = Time;
				}

				if ( Time < profile_Array[cnt].Min_Time[0] )
				{
					profile_Array[cnt].Min_Time[1] = profile_Array[cnt].Min_Time[0];
					profile_Array[cnt].Min_Time[0] = Time;
				}

				else if ( Time < profile_Array[cnt].Min_Time[1] )
				{
					//초기화는 저장안함.
					if ( profile_Array[cnt].Min_Time[1] != 0x7fffffffffffffff )
					{
						profile_Array[cnt].TotalTime += profile_Array[cnt].Min_Time[1];
					}
					profile_Array[cnt].Min_Time[1] = Time;
				}


				//현재 시간이 최대값보다 작고 최소값보다 클 경우 토탈타임에 합침.
				if ( Time > profile_Array[cnt].Min_Time[1] && Time < profile_Array[cnt].Max_Time[1] )
				{
					profile_Array[cnt].TotalTime += Time;
				}

				return true;

			}
			return false;

		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
//프로파일 출력부
///////////////////////////////////////////////////////////////////////////////////
void ProfileStructher::Print_Profile (void)
{
	int cnt;
	FILE *fp;
	double Average;
	__int64 i_Average;
	double MinTime;
	double MaxTime;


	fopen_s (&fp,"Profile.txt","w+t");


	fprintf_s (fp, "l %-17s l %-11s   l %-11s   l %-11s   l %-8s l\n", "Name", "Average", "MinTime", "MaxTime", "TotalCaLL");
	fprintf_s (fp,"ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");

	for ( cnt = 0; cnt < Max; cnt++ )
	{
		if ( profile_Array[cnt].CallCNT >= 1 )
		{
			profile_Array[cnt].TotalTime += profile_Array[cnt].Min_Time[1];
			profile_Array[cnt].TotalTime += profile_Array[cnt].Max_Time[1];

			i_Average = profile_Array[cnt].TotalTime / profile_Array[cnt].CallCNT;
			Average = (double) i_Average / MicroSecond;
			MinTime = (double) profile_Array[cnt].Min_Time[1] / MicroSecond;
			MaxTime = (double) profile_Array[cnt].Max_Time[1] / MicroSecond;

			fprintf_s (fp, "l %-17s l %-11.4fms l %-11.4fms l %-11.4fms l %8lld  l\n", profile_Array[cnt].Name, Average, MinTime, MaxTime, profile_Array[cnt].CallCNT);
		}
	}
	fclose (fp);
}
void ProfileStructher::ClearProfile (void)		//모든 프로파일을 초기화 시켜버린다.
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
}

ProfileStructher Profile;

void Profile_Begin (char *name)
{
	LARGE_INTEGER StartTime;
	QueryPerformanceCounter (&StartTime);
	if ( false == Profile.Set_Profile (name, StartTime.QuadPart) )
	{
		//throw false;
	}
	return;
}

void Profile_End (char *name)
{
	LARGE_INTEGER EndTime;
	QueryPerformanceCounter (&EndTime);
	if ( false == Profile.End_Profile (name, EndTime.QuadPart) )
	{
	//	throw false;
	}
	return;
}

void Profile_Print (void)
{

	if ( GetAsyncKeyState (0x50) & 0x8001 )
	{
		Profile.Print_Profile ();
	}
	if ( GetAsyncKeyState (0x4f) & 0x8001 )
	{
		Profile.ClearProfile ();
	}
	return;
}
