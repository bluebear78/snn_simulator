#ifndef SYNAPSE_H
#define SYNAPSE_H
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

class Neuron;

// based on the model of an memrister synapse
class Synapse {
private:
	Neuron* _pre;
	Neuron* _post;
	bool _excitatory;
	bool _fixed;
	int _min;
	int _max;
	int _ini_min;
	int _ini_max;
	int _weight;
	int _factor;

	double _t_spike_pre;
	double _t_spike_post;

	double _mem_pos;
	double _mem_neg;
public:
	//get set
	int Weight();
	bool Excitatory();
	bool Fixed();
	Neuron* PreNeuron();
	Neuron* PostNeuron();


	//이전의 뉴런, 이후 뉴런, 흥분정도, ?? , weight값
	Synapse(Neuron*, Neuron*, bool, bool, int);

	Synapse(Neuron*, Neuron*, bool, bool, int, int, int, int);

	//spike time을 계산
	void SetPreSpikeT(double);
	void SetPostSpikeT(double);


	//axon에 연결된 neuron의 membrane에 sitimul을 적용한다.
	void SendSpike();

	//STDP ********important***********
	void Learn(int);
	//STDP ********important***********

	//weight print
	void Print();
	//weight를 파일에 기록
	void Print(FILE*);
};

#endif

