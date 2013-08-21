#include<time.h>

using namespace std;

void test_time(){
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> [test_time] >>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

        tzset();
        time_t time_now = time(NULL);
	cout << "[test_time] time(NULL) = " << time_now << endl;

        char year[10];
        char month[10];
        char day[10];
        strftime(year, sizeof(year), "%Y", localtime(&time_now));
        strftime(month, sizeof(month), "%m", localtime(&time_now));
        strftime(day, sizeof(day), "%d", localtime(&time_now));
        string _today_str = string(year) + "-" + string(month) + "-" + string(day);
	cout << "[test_time] today is : " << _today_str << endl;

        cout << endl;

        struct tm *tm_local,*tm_gmt;
        tm_gmt = gmtime(&time_now);
	cout << "[test_time] tm_gmt = gmtime(&time(NULL))" << endl;
	cout << "[test_time] tm_gmt->tm_year = " << tm_gmt->tm_year <<		"\t\tyears since 1900 (from 0)        " << endl;
	cout << "[test_time] tm_gmt->tm_mon = " << tm_gmt->tm_mon <<		"\t\tmonth of the year (from 0)       " << endl;
	cout << "[test_time] tm_gmt->tm_mday = " << tm_gmt->tm_mday <<		"\t\tday of the month (from 1)        " << endl;
	cout << "[test_time] tm_gmt->tm_hour = " << tm_gmt->tm_hour <<		"\t\thour of the day (from 0)         " << endl;
	cout << "[test_time] tm_gmt->tm_min = " << tm_gmt->tm_min <<		"\t\tminutes after the hour (from 0)  " << endl;
	cout << "[test_time] tm_gmt->tm_sec = " << tm_gmt->tm_sec <<		"\t\tseconds after the minute (from 0)" << endl;
	cout << "[test_time] tm_gmt->tm_wday = " << tm_gmt->tm_wday <<		"\t\tdays since Sunday (from 0)       " << endl;
	cout << "[test_time] tm_gmt->tm_yday = " << tm_gmt->tm_yday <<		"\t\tday of the year (from 0)         " << endl;
	cout << "[test_time] tm_gmt->tm_isdst = " << tm_gmt->tm_isdst <<	"\t\tDaylight Saving Time flag        " << endl;
        cout << endl;

        tm_local = localtime(&time_now);
	cout << "[test_time] tm_local = localtime(&time(NULL))" << endl;
	cout << "[test_time] tm_local->tm_year = " << tm_local->tm_year <<	"\t\tyears since 1900 (from 0)        " << endl;
	cout << "[test_time] tm_local->tm_mon = " << tm_local->tm_mon <<	"\t\tmonth of the year (from 0)       " << endl;
	cout << "[test_time] tm_local->tm_mday = " << tm_local->tm_mday <<	"\t\tday of the month (from 1)        " << endl;
	cout << "[test_time] tm_local->tm_hour = " << tm_local->tm_hour <<	"\t\thour of the day (from 0)         " << endl;
	cout << "[test_time] tm_local->tm_min = " << tm_local->tm_min <<	"\t\tminutes after the hour (from 0)  " << endl;
	cout << "[test_time] tm_local->tm_sec = " << tm_local->tm_sec <<	"\t\tseconds after the minute (from 0)" << endl;
	cout << "[test_time] tm_local->tm_wday = " << tm_local->tm_wday <<	"\t\tdays since Sunday (from 0)       " << endl;
	cout << "[test_time] tm_local->tm_yday = " << tm_local->tm_yday <<	"\t\tday of the year (from 0)         " << endl;
	cout << "[test_time] tm_local->tm_isdst = " << tm_local->tm_isdst <<	"\t\tDaylight Saving Time flag        " << endl;
        cout << endl;
                                                                         
        tm_local->tm_hour = 0;
        tm_local->tm_min = 0;
        tm_local->tm_sec = 0;

        time_t time_zero = timelocal(tm_local);
	cout << "[test_time] time(ZERO) = " << time_zero << " ( " << (time_now-time_zero)/3600 << "h" << (time_now-time_zero)%3600/60 << "m ago )" << endl;
        cout << endl;

/*

  struct   timeb   tp;  

  ftime(&tp);  

  Hour=p->tm_hour;  

  Min=p->tm_min;  

  Sec=p->tm_sec;  

  MSEL=int(tp.millitm); //MillionSecond  



	time_t t = time(NULL);
	struct tm *local;
	local = localtime(&t);
	printf("Local time is %s ",asctime(local));
	cout << local->tm_year << endl;
	cout << local->tm_mon << endl;
	cout << local->tm_mday << endl;
        ostringstream date;
	date << local->tm_year << "-" << local->tm_mon << "-" << local->tm_mday;
	cout << date.str() << endl;

	long _today_gmt = mktime(local);
	long _today_local = timelocal(gmt);

	//long last_day = (long)( (double)(1268582399842.0) / (1000) );

	cout << "check today_gmt=" << _today_gmt << "(" << (t-_today_gmt)/3600 << ") today_local=" << _today_local << "(" << (t-_today_local)/3600 << ")" << endl;




	cout << mktime(gmt) << endl;
	cout << timegm(gmt) << endl;
	cout << timelocal(gmt) << endl;
	cout << mktime(local) << endl;
	cout << timegm(local) << endl;
	cout << timelocal(local) << endl;

*/
}


