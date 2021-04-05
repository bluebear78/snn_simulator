#ifndef PATTERN_H
#define PATTERN_H
#define _CRT_SECURE_NO_WARNINGS
#include<list>
class Neuron;
class Network;

//input data∏¶ ¿˙¿Â
class Pattern {
private:
	std::list<Neuron*> _activates;
	char* _name=NULL;
public:
	void AddNeuron(Neuron*);
	void Activate();
	void SetName(char*);
	char* Name();
};

#endif

