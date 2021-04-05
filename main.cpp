#define _CRT_SECURE_NO_WARNINGS
#include<ctime>
#include"neuron.h"
#include"synapse.h"
#include"parser.h"
#include"network.h"
#include"simulator.h"


int main(int argc, char* argv[]) {
	srand((unsigned int)time(0));

	//network ��
	Network network;

	Parser parser(&network);

	//inputs.txt �б�
	parser.Parse(argv[1]);

	  network.Print();
	  network.PrintSize();

	Simulator simulator(&network);
	
	//�����ϴ� time set
	simulator.SetEndTime(10000);
	simulator.SetStepSize(2);
	simulator.Run();

	network.Print();

	return 0;
}
