#include"network.h"
#include"neuron.h"
#include"synapse.h"
#include"pattern.h"
#include<stdio.h>
#include<iostream>
#include<assert.h>
#include<string.h>

using namespace std;

Network::Network() {
	_iter_started = false;
}

Network::~Network() {
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		delete (*iter);
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		delete (*iter);
	for (list<Pattern*>::iterator iter = _patterns.begin(); iter != _patterns.end(); iter++)
		delete (*iter);
}

//뉴런이 같은 이름의 뉴런이 존재하는지 체크
bool Network::CheckExistence(char* name) {
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), name) == 0) return true;
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), name) == 0) return true;
	return false;
}

//뉴런을 생성하고 excitatory의 여부에따라 활성/비활성 뉴런 리스트에 넣어줌
void Network::AddNeuron(char* name, bool excitatory) {
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		assert(strcmp((*iter)->Name(), name) != 0);
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		assert(strcmp((*iter)->Name(), name) != 0);

	Neuron* neuron = new Neuron(name);
	_individualNeurons.push_back(neuron);
	_allNeurons.push_back(neuron);
	if (excitatory == true) _allExcitatoryNeurons.push_back(neuron);
	else _allInhibitoryNeurons.push_back(neuron);
}

//뉴런집합을 생성하고 excitatory의 여부에따라 활성/비활성 뉴런 리스트에 넣어줌
void Network::AddNeuronGroup(char* name, int num, bool excitatory) {
	assert(num > 1);
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		assert(strcmp((*iter)->Name(), name) != 0);
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		assert(strcmp((*iter)->Name(), name) != 0);

	NeuronGroup* neuronGroup = new NeuronGroup(name, num);
	_groupNeurons.push_back(neuronGroup);
	for (Neuron* neuron = neuronGroup->First(); neuron != NULL; neuron = neuronGroup->Next())
		_allNeurons.push_back(neuron);
	if (excitatory == true) for (Neuron* neuron = neuronGroup->First(); neuron != NULL; neuron = neuronGroup->Next()) _allExcitatoryNeurons.push_back(neuron);
	else for (Neuron* neuron = neuronGroup->First(); neuron != NULL; neuron = neuronGroup->Next()) _allInhibitoryNeurons.push_back(neuron);
}

//synapse를 추가 (neuron <-> neuron)
void Network::AddSynapse(Neuron* pre, Neuron* post, bool exitatory, bool fixed, int value) {
	Synapse* synapse = new Synapse(pre, post, exitatory, fixed, value);
	_synapses.push_back(synapse);
	pre->AddPostSyn(synapse);
	post->AddPreSyn(synapse);
}

//synapse를 추가 (neuron <-> neuron group)
void Network::AddSynapse(Neuron* pre, NeuronGroup* post, bool exitatory, bool fixed, int value) {
	for (Neuron* neuron = post->First(); neuron != NULL; neuron = post->Next())
		AddSynapse(pre, neuron, exitatory, fixed, value);
}

//synapse를 추가 (group neuron <-> neuron)
void Network::AddSynapse(NeuronGroup* pre, Neuron* post, bool exitatory, bool fixed, int value) {
	for (Neuron* neuron = pre->First(); neuron != NULL; neuron = pre->Next())
		AddSynapse(neuron, post, exitatory, fixed, value);
}

//synapse를 추가 (group neuron <-> group neuron)
void Network::AddSynapse(NeuronGroup* pre, NeuronGroup* post, bool exitatory, bool fixed, int value) {
	for (Neuron* neuron = post->First(); neuron != NULL; neuron = post->Next())
		AddSynapse(pre, neuron, exitatory, fixed, value);
}

void Network::AddSynapse(char* pre, char* post, bool exitatory, bool fixed, int value) {
	Neuron* preNeuron = NULL, * postNeuron = NULL;
	NeuronGroup* preNeuronGroup = NULL, * postNeuronGroup = NULL;
	int preIndex = 0;
	int postIndex = 0;

	// find the presynaptic part
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), pre) == 0) {
			preIndex = 1;
			preNeuron = (*iter);
		}
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), pre) == 0) {
			preIndex = 2;
			preNeuronGroup = (*iter);
		}

	// find the postsynaptic part
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), post) == 0) {
			postIndex = 1;
			postNeuron = (*iter);
		}
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), post) == 0) {
			postIndex = 2;
			postNeuronGroup = (*iter);
		}

	// add synapses
	if (preIndex == 1) {
		if (postIndex == 1) {
			AddSynapse(preNeuron, postNeuron, exitatory, fixed, value);
		}
		else if (postIndex == 2) {
			AddSynapse(preNeuron, postNeuronGroup, exitatory, fixed, value);
		}
		else assert(0);
	}
	else if (preIndex == 2) {
		if (postIndex == 1) {
			AddSynapse(preNeuronGroup, postNeuron, exitatory, fixed, value);
		}
		else if (postIndex == 2) {
			AddSynapse(preNeuronGroup, postNeuronGroup, exitatory, fixed, value);
		}
		else assert(0);
	}
	else assert(0);
}


// the following five functions are for learning synapses
void Network::AddSynapse(Neuron* pre, Neuron* post, bool exitatory, bool fixed, int min, int max, int ini_min, int ini_max) {
	Synapse* synapse = new Synapse(pre, post, exitatory, fixed, min, max, ini_min, ini_max);
	_synapses.push_back(synapse);
	pre->AddPostSyn(synapse);
	post->AddPreSyn(synapse);
}
void Network::AddSynapse(Neuron* pre, NeuronGroup* post, bool exitatory, bool fixed, int min, int max, int ini_min, int ini_max) {
	for (Neuron* neuron = post->First(); neuron != NULL; neuron = post->Next())
		AddSynapse(pre, neuron, exitatory, fixed, min, max, ini_min, ini_max);
}
void Network::AddSynapse(NeuronGroup* pre, Neuron* post, bool exitatory, bool fixed, int min, int max, int ini_min, int ini_max) {
	for (Neuron* neuron = pre->First(); neuron != NULL; neuron = pre->Next())
		AddSynapse(neuron, post, exitatory, fixed, min, max, ini_min, ini_max);
}
void Network::AddSynapse(NeuronGroup* pre, NeuronGroup* post, bool exitatory, bool fixed, int min, int max, int ini_min, int ini_max) {
	for (Neuron* neuron = post->First(); neuron != NULL; neuron = post->Next())
		AddSynapse(pre, neuron, exitatory, fixed, min, max, ini_min, ini_max);
}
void Network::AddSynapse(char* pre, char* post, bool exitatory, bool fixed, int min, int max, int ini_min, int ini_max) {
	Neuron* preNeuron=nullptr, * postNeuron = nullptr;
	NeuronGroup* preNeuronGroup = nullptr, * postNeuronGroup = nullptr;
	int preIndex = 0;
	int postIndex = 0;

	// find the presynaptic part
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), pre) == 0) {
			preIndex = 1;
			preNeuron = (*iter);
			break;
		}
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), pre) == 0) {
			preIndex = 2;
			preNeuronGroup = (*iter);
			break;
		}

	// find the postsynaptic part
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), post) == 0) {
			postIndex = 1;
			postNeuron = (*iter);
			break;
		}
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), post) == 0) {
			postIndex = 2;
			postNeuronGroup = (*iter);
			break;
		}

	// add synapses
	if (preIndex == 1) {
		if (postIndex == 1) {
			AddSynapse(preNeuron, postNeuron, exitatory, fixed, min, max, ini_min, ini_max);
		}
		else if (postIndex == 2) {
			AddSynapse(preNeuron, postNeuronGroup, exitatory, fixed, min, max, ini_min, ini_max);
		}
		else assert(0);
	}
	else if (preIndex == 2) {
		if (postIndex == 1) {
			AddSynapse(preNeuronGroup, postNeuron, exitatory, fixed, min, max, ini_min, ini_max);
		}
		else if (postIndex == 2) {
			AddSynapse(preNeuronGroup, postNeuronGroup, exitatory, fixed, min, max, ini_min, ini_max);
		}
		else assert(0);
	}
	else assert(0);
}

//neuron의 수, synapse의 수, 활성,비활성 synapse의 수 출력
void Network::PrintSize() {
	cout << "Total Number of neurons: " << _allNeurons.size() << endl;
	cout << "Total Number of synapses: " << _synapses.size() << endl;
	int numExitatory = 0;
	int numLearning = 0;
	for (list<Synapse*>::iterator iter = _synapses.begin(); iter != _synapses.end(); iter++) {
		if ((*iter)->Excitatory() == true) numExitatory++;
		if ((*iter)->Fixed() == false) numLearning++;
	}

	cout << "\tNumber of exitatory synapses: " << numExitatory << endl;
	cout << "\tNumber of learning synapses: " << numLearning << endl;
}

//모든 뉴런 update
void Network::UpdateNeurons(double t_step, double t) {
	for (list<Neuron*>::iterator iter = _allNeurons.begin(); iter != _allNeurons.end(); iter++)
		(*iter)->Update(&_activeInputSyns, &_activeOutputSyns, t_step, t);
}
//활성화된 뉴런만 update
void Network::UpdateExcitatoryNeurons(double t_step, double t) {
	for (list<Neuron*>::iterator iter = _allExcitatoryNeurons.begin(); iter != _allExcitatoryNeurons.end(); iter++)
		(*iter)->Update(&_activeInputSyns, &_activeOutputSyns, t_step, t);
}
//비활성화된 뉴런만 update
void Network::UpdateInhibitoryNeurons(double t_step, double t) {
	for (list<Neuron*>::iterator iter = _allInhibitoryNeurons.begin(); iter != _allInhibitoryNeurons.end(); iter++)
		(*iter)->Update(&_activeInputSyns, &_activeOutputSyns, t_step, t);
}

//spike가 발생하고 synapse를 update
void Network::UpdateSynapses() {
	list<Synapse*>::iterator iter;
	for (iter = _activeOutputSyns.begin(); iter != _activeOutputSyns.end(); iter++) (*iter)->SendSpike();
	for (iter = _activeOutputSyns.begin(); iter != _activeOutputSyns.end(); iter++) (*iter)->Learn(0);
	for (iter = _activeInputSyns.begin(); iter != _activeInputSyns.end(); iter++) (*iter)->Learn(1);
	_activeInputSyns.clear();
	_activeOutputSyns.clear();
}

//특정 뉴런 찾기
Neuron* Network::SearchForNeuron(char* name) {
	for (list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), name) == 0) {
			return (*iter);
		}

	return NULL;
}

//??
Neuron* Network::SearchForNeuron(char* name1, char* name2) {
	char* name = new char[strlen(name1) + strlen(name2) + 2];
	sprintf(name, "%s_%s\0", name1, name2);

	list<NeuronGroup*>::iterator iter;
	Neuron* neuron;

	for (iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++)
		if (strcmp((*iter)->Name(), name1) == 0) break;

	assert(iter != _groupNeurons.end());
	for (neuron = (*iter)->First(); neuron != NULL; neuron = (*iter)->Next())
		if (strcmp(name, neuron->Name()) == 0) {
			(*iter)->UnlockFirst();
			delete[] name;
			return neuron;
		}

	assert(neuron != NULL);
}

//all neruon 클리어
void Network::ClearInputs() {
	for (list<Neuron*>::iterator iter = _allNeurons.begin(); iter != _allNeurons.end(); iter++)
		(*iter)->ClearInput();
}


void Network::AddPattern(Pattern* pattern) {
	_patterns.push_back(pattern);
}

Pattern* Network::FirstPattern() {
	iter = _patterns.begin();
	_iter_started = true;
	return (*iter);
}

Pattern* Network::NextPattern() {
	assert(_iter_started == true);
	iter++;
	if (iter != _patterns.end()) return (*iter);
	else {
		_iter_started = false;
		return NULL;
	}
}


//결과 print
void Network::Print() {
	/*
	for(list<Neuron*>::iterator iter = _individualNeurons.begin(); iter != _individualNeurons.end(); iter++){
	  if(strcmp("inhi",(*iter)->Name()) == 0) (*iter)->PrintInputSyns();
	}
	*/

	/*
	  NeuronGroup * output = NULL;
	  for(list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++){
		if(strcmp("output",(*iter)->Name()) == 0) output = *iter;
	  }
	  assert(output != NULL);

	  Neuron * neuron;
	  for(neuron = output->First(); neuron != 0; neuron = output->Next()){
		neuron->PrintInputSyns();
		cout<<endl<<endl;
	  }
	*/

	NeuronGroup* output = NULL;
	for (list<NeuronGroup*>::iterator iter = _groupNeurons.begin(); iter != _groupNeurons.end(); iter++) {
		if (strcmp("output", (*iter)->Name()) == 0) output = *iter;
	}
	assert(output != NULL);

	Neuron* neuron;
	for (neuron = output->First(); neuron != 0; neuron = output->Next()) {
		neuron->PrintInputSyns();
		//    cout<<endl<<endl;
	}
}


