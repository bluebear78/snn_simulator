#include"simulator.h"
#include"network.h"
#include"pattern.h"
#include<assert.h>
#include<iostream>

using namespace std;

Simulator::Simulator(Network* network) 
{
	_network = network;
	
	_t = 0.0;
	_t_end = 0.0;
	_t_step = 0.0;
}

//�ùķ��̼� �����ϴ� �ð�
void Simulator::SetEndTime(double endTime) {
	assert(endTime > 0);
	_t_end = endTime;
}

//���� ���ķ��̼��� �����ϰ��ִ� step
void Simulator::SetStepSize(double stepSize) {
	assert(stepSize > 0);
	_t_step = stepSize;
}



void Simulator::Run() {
	//������ ��
	Pattern* pattern;

	//���� Ÿ�̸�
	double t_start = 0;
	int index = 0;
	cout << "finished letter " << index++ << "\n";
	for (pattern = _network->FirstPattern(); pattern != NULL; pattern = _network->NextPattern()) {
		cout << "Input for trainning: " << pattern->Name() << "\n";
		pattern->Activate();
		for (_t = t_start + _t_step; _t < t_start + _t_end + 1e-12; _t += _t_step) {
			//_network->UpdateNeurons(_t_step,_t);
			_network->UpdateExcitatoryNeurons(_t_step, _t);
			_network->UpdateSynapses();
			_network->UpdateInhibitoryNeurons(_t_step, _t);
			_network->UpdateSynapses();
		}

		//cout<<"---------------------------------------------------------------- start to forget ..."<<endl;
		_network->ClearInputs();
		for (; _t < t_start + _t_end + 200 + 1e-12; _t += _t_step) {
			//_network->UpdateNeurons(_t_step,_t);
			_network->UpdateExcitatoryNeurons(_t_step, _t);
			_network->UpdateSynapses();
			_network->UpdateInhibitoryNeurons(_t_step, _t);
			_network->UpdateSynapses();
		}
		t_start = _t;
		//cout<<"---------------------------------------------------------------- end of forgetting ..."<<endl;
		cout << "finished letter " << index++ << endl;
	}
}

