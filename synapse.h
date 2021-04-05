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


	//������ ����, ���� ����, �������, ?? , weight��
	Synapse(Neuron*, Neuron*, bool, bool, int);

	Synapse(Neuron*, Neuron*, bool, bool, int, int, int, int);

	//spike time�� ���
	void SetPreSpikeT(double);
	void SetPostSpikeT(double);


	//axon�� ����� neuron�� membrane�� sitimul�� �����Ѵ�.
	void SendSpike();

	//STDP ********important***********
	void Learn(int);
	//STDP ********important***********

	//weight print
	void Print();
	//weight�� ���Ͽ� ���
	void Print(FILE*);
};

#endif

