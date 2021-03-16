#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;
#define REPORT_HEAD_LENGTH 64

class statistic
{
public:
	statistic(string delay_chart,string jitter_chart,string reporter) :rrate( 0.0),counted_packages (1)
	{
		if((delay_chart.empty()) &&  (jitter_chart.empty()) && (reporter.empty()))
		{
			cout<<"Invalida path name"<<endl;	
		} else {
		//Create Report file to record
		statistic_report.open(reporter,ios::out|ios::app);
		if(!statistic_report.is_open())
			cout<<"Open Reporter file failure"<< endl;
		else {
			statistic_report.write("###############################################################\n",REPORT_HEAD_LENGTH);
			statistic_report.write("##################  Performance Reporter  #####################\n",REPORT_HEAD_LENGTH);
			statistic_report.write("###############################################################\n",REPORT_HEAD_LENGTH);
		}
		
		period_jitter.open(jitter_chart,ios::out|ios::app);
		if(!period_jitter.is_open())
			cout<<"Open period_jitter file failure"<<endl;
		else
			period_jitter.write("########################      Sender Jitter        ############\n",REPORT_HEAD_LENGTH);


		time_delay.open(delay_chart,ios::out|ios::app);
		if(!time_delay.is_open())
			cout<<"Open time_delay file failure"<<endl;
		else
			time_delay.write("###################      Transmit Delay       #################\n",REPORT_HEAD_LENGTH);
		}
	}

	~statistic()
	{
		cout<<"Statistic functions end "<<endl;
	}
	
	statistic(const statistic &file) = delete;
	statistic& operator=(const statistic &file) = delete; 
	
	void Save_All()
	{
		statistic_report.close();
		period_jitter.close();
		time_delay.close();
	}

	void Set_Test_Packages_Size(int test_package_size)
	{
		if((test_package_size < 0) ||(test_package_size > numeric_limits<int>::max()))
		{
			cout<<"Set_Test_Packages_Size : input data is out of range"<<endl;
		} else {
			package_size = test_package_size;
			statistic_report.write("Test Package Size     :\n",24);
			statistic_report<<test_package_size<<" Bytes"<<"\n";
		}
	}
	void Set_Test_Batch_Nums(int batch_num)
	{
		if((batch_num < 0) ||(batch_num > numeric_limits<int>::max()) )
		{
			cout<<"Set_Test_Batch_Nums : input data is out of range"<<endl;
		} else {
			test_batch_nums = batch_num;
			statistic_report.write("Test Batch Nums     :\n",22);
			statistic_report<<batch_num<<" Loops"<<"\n";
		}
	}
	void Time_Delay(uint64_t sender,uint64_t receiver)
	{
		if(sender >= receiver)
		{
			cout<<"Time_Delay : input data is out of range (too small to get time gap)"<<endl;
			return;
		}
		uint64_t delt = receiver - sender;
		time_delay<<delt/1000<<"\n";
		
		if(counted_packages%2 == 0) {
			cout<<"counted_packages "<<counted_packages<<endl;
			Period_Jitter(sender_last,sender);
		} else {
			sender_last = sender;
		}
		counted_packages += 1;
	}

	void Loss_Rate()
	{
		double recv_total = recv_end_package -recv_start_package;
		double send_total = send_end_package - send_start_package;
		double lrate;
		
		cout<<"send_start_package  :"<<send_start_package<<endl;
		cout<<"send_end_package  :"<<send_end_package<<endl;
		cout<<"recv_start_package  :"<<recv_start_package<<endl;
		cout<<"recv_end_package  :"<<recv_end_package<<endl;
		
		if((recv_end_package < recv_start_package) ||(send_end_package < send_start_package))
		{
			cout<<"Loss_Rate : input data is out of range"<<endl;
			return;
		}
		
		if(recv_total == send_total)
		{
			//loss rate is zero
			rrate = 1.0;
		} else {
			rrate = recv_total/send_total;
		}
		
		lrate = 1 - rrate;
		statistic_report.write("Loss Rate     :\n",16);
		statistic_report<<setw(4)<<setprecision(4)<<lrate*100<<" %"<<"\n";
	}

	void Throughtput()
	{
		statistic_report.write("Throughtput     :\n",18);
		uint64_t duration = (end_time_point - start_time_point)/1000000000UL;
		if(duration==0)
		{
			cout<<"Throughtput : delay is out of range (too small to get time gap)"<<endl;
		} else {
			uint64_t throughtput  = static_cast<uint64_t>(package_size)*static_cast<uint64_t>(test_batch_nums); //Bytes
			cout<<"duration : "<<duration<<" Seconds"<<endl;
			cout<<"total : "<<throughtput<<" Bytes"<<endl;
		
			statistic_report<<throughtput/1024/1024/duration<<" MBytes per Seconds"<<"\n";
		}
	}

	void Set_Start_TimePoint()
	{
		statistic_report.write("Test Start Time Point      :\n",29);
		start_time_point = Now();
		statistic_report<<Time_ToString(Now()).c_str()<<"\n";
	}

	void Set_End_TimePoint()
	{
		statistic_report.write("Test End Time Point      :\n",27);
		end_time_point = Now();
		statistic_report<<Time_ToString(Now()).c_str()<<"\n";
	}

	void Align_Start_Package_Nums(uint64_t sender_start,uint64_t recv_start)
	{
		if(((sender_start < 0)||(sender_start > numeric_limits<uint64_t>::max())) &&
			((recv_start < 0)||(recv_start > numeric_limits<uint64_t>::max())))
		{
			cout<<"Align_Start_Package_Nums : input data is out of range"<<endl;
		} else {
			send_start_package = sender_start;
			recv_start_package = recv_start;
		}
	}

	void Align_End_Package_Nums(uint64_t sender_end,uint64_t recv_end)
	{

		if(((sender_end < 0)||(sender_end > numeric_limits<uint64_t>::max())) &&
			((recv_end < 0)||(recv_end > numeric_limits<uint64_t>::max())))
		{
			cout<<"Align_End_Package_Nums : input data is out of range"<<endl;
		} else {
			send_end_package = sender_end;
			recv_end_package = recv_end;
		}
	}

	
	uint64_t To_Nanosecond(uint64_t nano_secs) 
	{
		if((nano_secs < 0) ||(nano_secs > numeric_limits<uint64_t>::max()))
		{
			cout<<"To_Nanosecond : input data is out of range"<<endl;
			return  -1;
		} else {
			return nano_secs;
		}
	}
	
	uint64_t To_Microsecond(uint64_t nano_secs)
	{
		if((nano_secs < 0) ||(nano_secs > numeric_limits<uint64_t>::max()))
		{
			cout<<"To_Microsecond : input data is out of range"<<endl;
			return  -1;
		} else {
			return static_cast<uint64_t>(nano_secs / 1000.0);
		}
	}
	
private:
	void Period_Jitter(uint64_t last,uint64_t current)
	{
		uint64_t delt;
		if(last >= current)	
		{
			cout<<"Period_Jitter : delay is out of range (too small to get time gap)"<<endl;
			return;
		}
		delt = current - last;
		period_jitter<<delt/1000<<"\n";
	}
	
	uint64_t Now() {
		auto now = std::chrono::high_resolution_clock::now();
		auto nano_time_point =
			std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
		auto epoch = nano_time_point.time_since_epoch();
		uint64_t now_nano = 
			std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
	
		return now_nano;
	}

	std::string Time_ToString(uint64_t nanoseconds_) const
	{
		auto nano = std::chrono::nanoseconds(nanoseconds_);
		std::chrono::system_clock::time_point tp(nano);
		auto time = std::chrono::system_clock::to_time_t(tp);
		struct tm stm;
		auto ret = localtime_r(&time, &stm);
		if (ret == nullptr) {
			return std::to_string(static_cast<double>(nanoseconds_) / 1000000000.0);
		}

		std::stringstream ss;
#if __GNUC__ >= 5
		ss << std::put_time(ret, "%F %T");
		ss << "." << std::setw(9) << std::setfill('0') << nanoseconds_ % 1000000000UL;
#else
		char date_time[128];
		strftime(date_time, sizeof(date_time), "%F %T", ret);
		ss << std::string(date_time) << "." << std::setw(9) << std::setfill('0')
			<< nanoseconds_ % 1000000000UL;
#endif
		return ss.str();
	}

	std::fstream statistic_report;
	std::fstream time_delay; //to chart 
	std::fstream period_jitter; //to chart

	double rrate; //recv rate

	int test_batch_nums;
	uint64_t sender_last;
	int package_size;
	int counted_packages;
	
	uint64_t start_time_point;
	uint64_t end_time_point;
	
	//uint64_t package_loops;
	uint64_t recv_start_package;
	uint64_t recv_end_package;
	uint64_t send_start_package;
	uint64_t send_end_package;
};

