#include"pattern.h"
#include"neuron.h"
#include"network.h"
#include<list>

using namespace std;

void Pattern::AddNeuron(Neuron* neuron) {
	_activates.push_back(neuron);
}

void Pattern::SetName(char* name) {
	_name = new char[strlen(name) + 2];
	strcpy(_name, name);
}

char* Pattern::Name() {
	return _name;
}

void Pattern::Activate() {
	for (list<Neuron*>::iterator iter = _activates.begin(); iter != _activates.end(); iter++)
		(*iter)->SetInput();
}
 