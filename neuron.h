#ifndef NEURON_H
#define NEURON_H
#define _CRT_SECURE_NO_WARNINGS
#include<list>
class Synapse;

class Neuron {
private:
	//뉴런 이름
	char* _name;
	//membrane potential input volage level
	int _v_mem;
	//input voltage level
	int _all_inputs;
	double _t;
	//axon의 list
	std::list<Synapse*> _outputSyns;

	//dendrite의 list
	std::list<Synapse*> _inputSyns;

	//input이 있는지 없는지 확인
	bool _withInput;
public:
	Neuron(char*);
	~Neuron();
	char* Name();


	// set input to true
	void SetInput();

	// set input to false
	void ClearInput();

	void AddPostSyn(Synapse*);
	void AddPreSyn(Synapse*);
	void Update(std::list<Synapse*>*, std::list<Synapse*>*, double, double);
	void ReceiveSpike(int);
	void PrintInputSyns();
	int Active();
};

//뉴런 한번에 만드는 놈
class NeuronGroup {
private:
	char* _name;
	std::list<Neuron*> _neurons;
	std::list<Neuron*>::iterator _iter;
	bool _firstCalled;
public:
	NeuronGroup(char*, int);
	~NeuronGroup();
	char* Name();
	Neuron* First();
	Neuron* Next();
	void UnlockFirst();
};

#endif

