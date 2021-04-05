#ifndef SIMULATOR_H
#define SIMULATOR_H
#define _CRT_SECURE_NO_WARNINGS
#include"network.h"

class Simulator {
private:
	double _t;
	
	//시뮬레이션 종료하는 시간
	double _t_end;

	//지금 스뮬레이션이 동작하고있는 step
	double _t_step;

	Network* _network;
public:
	Simulator(Network*);
	void SetEndTime(double);
	void SetStepSize(double);
	void Run();
};

#endif

