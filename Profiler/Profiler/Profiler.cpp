#include"stdafx.h"
#include"Profiler.h"

/////////////////////////////////////////////////////////////////////////////////////
//�������� ���ú�
/////////////////////////////////////////////////////////////////////////////////////
bool ProfileStructher::Set_Profile (char *name, __int64 SetTime)
{
	int cnt;
	for ( cnt = 0; cnt < Max; cnt++ )
	{
		//�迭�� ���鼭 false�� �����ٸ� �ش� ���������� �������� �����Ƿ� ���� ������ �־�� �ȴ�.
		if ( false == profile_Array[cnt].flag )
		{
			break;
		}
		//�迭�� ���鼭 ���� �̸��� �������ϸ� ������ �Ǹ��� �ش� ���������� ���۽ð��� 0���� �ʱ�ȭ �Ǿ��ִ��� Ȯ��. 
		else if ( 0 == strcmp (profile_Array[cnt].Name, name) )
		{
			//�ش� ���������� beginflag�� true��� ������� ���� �������� �̹Ƿ� false�� ����
			if ( true == profile_Array[cnt].Beginflag )
			{
				return false;
			}
			break;
		}
	}

	//�迭�� �����Ͽ� �������� ���Ѱ��� false�� ����.
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
//�������� �����
////////////////////////////////////////////////////////////////////////////////////
bool ProfileStructher::End_Profile (char *name, __int64 EndTime)
{
	int cnt;
	__int64 Time;


	for ( cnt = 0; cnt < Max; cnt++ )
	{
		//�迭�� flag�� false��� �ش� ���������� �������� �ʴ°��̹Ƿ� false�� �����Ѵ�.
		if ( false == profile_Array[cnt].flag )
		{
			return false;
		}
		//�迭�� �ش� ���������� �����Ѵٸ� Beginflag�� ���������� ���ۿ��θ� Ȯ��.
		else if ( 0 == strcmp (profile_Array[cnt].Name, name) )
		{
			//Beginflag�� true��� �ش� ���������� ���۵Ǿ������Ƿ� ����ó�� false��� ���۵��� ���� ���������̹Ƿ� false�� �����Ѵ�.
			if ( true == profile_Array[cnt].Beginflag )
			{
				profile_Array[cnt].Beginflag = false;
				profile_Array[cnt].CallCNT++;
				Time = EndTime - profile_Array[cnt].Start_Time;

				//�ִ� �ð������� Ŭ ��� �ִ� �ð����� �����ϰ� ���� �ִ� �ð����� ���� �ִ� �ð����� �����Ѵ�.
				if ( Time > profile_Array[cnt].Max_Time[0] )
				{
					profile_Array[cnt].Max_Time[1] = profile_Array[cnt].Max_Time[0];
					profile_Array[cnt].Max_Time[0] = Time;
				}
				//�ִ� �ð������� �۰� ���� �ִ� �ð������� ū��� ���� �ִ� �ð����� TotalTime�� �����ϰ� ���� �ִ� �ð����� �����Ѵ�.
				else if ( Time > profile_Array[cnt].Max_Time[1] )
				{
					//ó�� �ѹ��� �ʱ�ȭ�� ���� ������ �� �����Ƿ� ������.
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
					//�ʱ�ȭ�� �������.
					if ( profile_Array[cnt].Min_Time[1] != 0x7fffffffffffffff )
					{
						profile_Array[cnt].TotalTime += profile_Array[cnt].Min_Time[1];
					}
					profile_Array[cnt].Min_Time[1] = Time;
				}


				//���� �ð��� �ִ밪���� �۰� �ּҰ����� Ŭ ��� ��ŻŸ�ӿ� ��ħ.
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
//�������� ��º�
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
	fprintf_s (fp,"�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");

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
void ProfileStructher::ClearProfile (void)		//��� ���������� �ʱ�ȭ ���ѹ�����.
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
