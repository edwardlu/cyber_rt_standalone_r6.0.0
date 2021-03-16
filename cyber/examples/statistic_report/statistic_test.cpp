#include <iostream>
#include "statistic.hpp"

int main()
{
	statistic sta("delay.txt","jitter.txt","report.txt");
	sta.Set_Test_Packages_Size(102400);
	sta.Set_Test_Batch_Nums(10000);

	sta.Set_Start_TimePoint();
	sta.Align_Start_Package_Nums(1000,0);
	sta.Align_End_Package_Nums(11000,10000); //test 0% loss rate
	sta.Loss_Rate();

	sta.Align_Start_Package_Nums(1000,0); //test 10% loss rate
	sta.Align_End_Package_Nums(11000,9000);
	sta.Loss_Rate();

	sta.Align_Start_Package_Nums(1000,0); //test 20% loss rate
	sta.Align_End_Package_Nums(11000,8000);
        sta.Loss_Rate();

	for(int i = 0;i < 100;i++)
		sta.Time_Delay(100+i,200);
	return 0;
}
