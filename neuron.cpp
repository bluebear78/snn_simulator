#include"def.h"
#include"neuron.h"
#include"synapse.h"
#include<stdlib.h>
#include<iostream>
#include<assert.h>
#include<string.h>
#include<stdio.h>
#include<math.h>

using namespace std;

Neuron::Neuron(char* name) {
	_name = new char[strlen(name) + 2];
	strcpy(_name, name);
	_v_mem = VREST;
	_t = -1e12;
	_withInput = false;
	
	_all_inputs = 0;
}

Neuron::~Neuron() {
	delete[] _name;
}

// set input to true
void Neuron::SetInput() {
	_withInput = true;
}

// set input to false
void Neuron::ClearInput() {
	_withInput = false;
}

char* Neuron::Name() {
	return _name;
}

//axon의 list
void Neuron::AddPostSyn(Synapse* postSyn) {
	_outputSyns.push_back(postSyn);
}

//dendrite의 list
void Neuron::AddPreSyn(Synapse* preSyn) {
	_inputSyns.push_back(preSyn);
}


//membrane potenital을 계산해서 spike를 발생시킴
void Neuron::Update(list<Synapse*>* inputSyns, list<Synapse*>* outputSyns, double t_step, double t) {
	//  if(_withInput == true) _v_mem += 16;//rand()%40;
	if (_withInput == true) _v_mem += rand() % 120;
	_v_mem += _all_inputs;
	_v_mem -= VSTEP;

	if (_v_mem < VREST) _v_mem = VREST;
	//if(strcmp(_name,"inhi")==0) cout<<t<<'\t'<<_v_mem<<'\t'<<_all_inputs<<endl;
	_all_inputs = 0;

	//membrane potenital이 튀는 경우
	if (_v_mem > VTHRE) {

		//if(_inputSyns.begin() != _inputSyns.end()) 
		//cout<<_name<<"\t@ "<<t<<endl;

		//membrane 초기화
		_v_mem = VREST;
		list<Synapse*>::iterator iter;

		//연결된거에 spike를 set함
		//pre spike와 post spike는 STDP에 이용할때 사용함
		for (iter = _inputSyns.begin(); iter != _inputSyns.end(); iter++) {
			(*iter)->SetPostSpikeT(t);
			inputSyns->push_back(*iter);
		}
		for (iter = _outputSyns.begin(); iter != _outputSyns.end(); iter++) {
			(*iter)->SetPreSpikeT(t);
			outputSyns->push_back(*iter);
		}
	}
}

//spike를 받는거
void Neuron::ReceiveSpike(int weight) {
	_all_inputs += weight;
}

//연결된 dendrite 정보를 알려준다.
void Neuron::PrintInputSyns() {
	int counter = 0;
	int counter2 = 0;
	FILE* fp = fopen(_name, "w");
	for (list<Synapse*>::iterator iter = _inputSyns.begin(); iter != _inputSyns.end(); iter++) {
		(*iter)->Print(fp);
		fprintf(fp, "\t"); // cout<<'\t';
		counter++;
		if (counter == FIXEL) {
			counter = 0;
			counter2++;
			fprintf(fp, "\n"); // cout<<endl;
		}
		if (counter2 == FIXEL) break;
	}
	fclose(fp);
}

int Neuron::Active() {
	if (_withInput == true) return 1;
	else return 0;
}


///////////////////////////////////////////////////////////////////////////
//
NeuronGroup::NeuronGroup(char* name, int num) {
	_firstCalled = false;
	_name = new char[strlen(name) + 2];
	strcpy(_name, name);

	char* neuronName = new char[1024];
	for (int i = 0; i < num; i++) {
		sprintf(neuronName, "%s_%d\0", name, i);
		Neuron* neuron = new Neuron(neuronName);
		_neurons.push_back(neuron);
	}
}

NeuronGroup::~NeuronGroup() {
	delete[] _name;
}

Neuron* NeuronGroup::First() {
	assert(_firstCalled == false);
	_firstCalled = true;
	_iter = _neurons.begin();
	if (_iter != _neurons.end()) return (*_iter);
	else return NULL;
}

Neuron* NeuronGroup::Next() {
	assert(_firstCalled == true);
	if (_iter == _neurons.end()) return NULL;
	_iter++;
	if (_iter != _neurons.end()) return (*_iter);
	else {
		_firstCalled = false;
		return NULL;
	}
}

void NeuronGroup::UnlockFirst() {
	_firstCalled = false;
}

char* NeuronGroup::Name() {
	return _name;
}

