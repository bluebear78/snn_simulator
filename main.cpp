#define _CRT_SECURE_NO_WARNINGS
#include<ctime>
#include"neuron.h"
#include"synapse.h"
#include"parser.h"
#include"network.h"
#include"simulator.h"


int main(int argc, char* argv[]) {
	srand((unsigned int)time(0));

	//network 셋
	Network network;

	Parser parser(&network);

	//inputs.txt 읽기
	parser.Parse(argv[1]);

	  network.Print();
	  network.PrintSize();

	Simulator simulator(&network);
	
	//종료하는 time set
	simulator.SetEndTime(10000);
	simulator.SetStepSize(2);
	simulator.Run();

	network.Print();

	return 0;
}
