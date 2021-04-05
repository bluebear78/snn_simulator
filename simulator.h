#ifndef SIMULATOR_H
#define SIMULATOR_H
#define _CRT_SECURE_NO_WARNINGS
#include"network.h"

class Simulator {
private:
	double _t;
	
	//�ùķ��̼� �����ϴ� �ð�
	double _t_end;

	//���� ���ķ��̼��� �����ϰ��ִ� step
	double _t_step;

	Network* _network;
public:
	Simulator(Network*);
	void SetEndTime(double);
	void SetStepSize(double);
	void Run();
};

#endif

