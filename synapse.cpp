#include"synapse.h"
#include"neuron.h"
#include<math.h>
#include<stdlib.h>
#include<iostream>
#include<assert.h>

using namespace std;


//synapse�� ����
//������ ����, ���� ����, �������, ?? , weight��
Synapse::Synapse(Neuron* pre, Neuron* post, bool excitatory, bool fixed, int value) {
	_pre = pre;
	_post = post;
	_excitatory = excitatory;
	_fixed = fixed;
	_min = -1;
	_max = -1;
	_ini_min = -1;
	_ini_max = -1;
	_weight = value;

	_t_spike_pre = -1e12;
	_t_spike_post = -1e12;

	_mem_pos = 0;
	_mem_neg = 0;

	_factor = 0;
}

//weight�� ����,���� �����ϴ°�
Synapse::Synapse(Neuron* pre, Neuron* post, bool excitatory, bool fixed, int min, int max, int ini_min, int ini_max) {
	_pre = pre;
	_post = post;
	_excitatory = excitatory;
	_fixed = fixed;
	_min = min;
	_max = max;
	_ini_min = ini_min;
	_ini_max = ini_max;

	//wight�ʱⰪ
	_weight = (int)(((double)(rand() % 10000)) / 10000 * (ini_max - ini_min) + ini_min);

	_t_spike_pre = -1e12;
	_t_spike_post = -1e12;

	_mem_pos = 0;
	_mem_neg = 0;

	_factor = 0;
}


//spike time�� ���
void Synapse::SetPreSpikeT(double t) {
	_mem_pos = _mem_pos * exp((_t_spike_pre - t) / 80) + 1.5;
	_mem_neg = _mem_neg * exp((_t_spike_pre - t) / 20) - 3;

	_t_spike_pre = t;
}
void Synapse::SetPostSpikeT(double t) {
	_t_spike_post = t;
}

//axon�� ����� neuron�� membrane�� sitimul�� �����Ѵ�.
void Synapse::SendSpike() {
	if (_excitatory == true) _post->ReceiveSpike(_weight);
	else _post->ReceiveSpike(-_weight);
}

//STDP
void Synapse::Learn(int index) {
	if (_fixed == true) 
		return;

	double temp;

	//spike time�� post�� pre����  ũ�� �� �����Ͱ� ������ �ִ°��̴�.
	//�н� ����
	if (_t_spike_post > _t_spike_pre) {

		//0.5>�ø��� �뵵
		temp = _mem_pos + _mem_neg + 0.5;
		_mem_pos = 0;
		_mem_neg = 0;

		if (_t_spike_pre < 0) temp = -1;
		_t_spike_pre = -1e12;

		//weight�� temp ����
		_weight += (int)temp;
		if (_weight > _max) _weight = _max;
		if (_weight < _min) _weight = _min;
		_t_spike_pre = -1e12;

		/*
#if 0
		temp2 = 3.5 * (1 - (_t_spike_post - _t_spike_pre) / 100);
		if (temp2 < 0) temp = -1;
		else temp = (int)(temp2);
#else
		temp = _mem_pos + _mem_neg + 0.5;
		_mem_pos = 0;
		_mem_neg = 0;
		if (_t_spike_pre < 0) temp = -1;
		_t_spike_pre = -1e12;
#endif

#if 0
		if (temp >= 1) _weight += 1;
		else if (temp <= 1) _weight -= 1;
		else {
			if (temp < 0) {
				_weight -= 1;
				temp -= 1;
			}

		}
#endif

#if 1
		//    int weight = _weight;
		//    int weight_ori = _weight;
		_weight += (int)temp;
		//    weight += temp;
		//if(fabs(temp-floor(temp))>1e-6)cout<<"difference = "<<temp-floor(temp)<<endl;
		//    if(_weight != weight) cout<<"weight_ori = "<<weight_ori<<"\t_weight = "<<_weight<<"\tweight = "<<weight<<"\ttemp = "<<temp<<endl;
#endif

#if 0
		_weight += (ceil(temp) - temp) < (temp - floor(temp)) ? (int)ceil(temp) : (int)floor(temp);
		if (_t_spike_pre < 0) temp = -1;
		_t_spike_pre = -1e12;
#endif

#if 0
		if (temp > 0) temp = (temp * (_max * 2 - _min - _weight)) / (2 * (_max - _min));
		else temp = (temp * (_max + _weight - _min * 2)) / (2 * (_max - _min));
		//    if(temp > 0) temp = temp*(_max-temp)/(_max-_min);
		//    else temp = temp*(temp-_min)/(_max-_min);
		_weight += (int)floor(temp) + (rand() % 10000 < (temp - floor(temp)) * 10000) ? 1 : 0;
#endif
		if (_weight > _max) _weight = _max;
		if (_weight < _min) _weight = _min;
		_t_spike_pre = -1e12;
	*/
	
		//�ϵ��� ���۵��Ѱ��, pre�� post���� Ŭ �� ������ ó���ؾ���
		if(_t_spike_post < _t_spike_pre){
			temp = (int)(1.5*(1-(_t_spike_pre - _t_spike_post)/100));
		if(temp < 0) temp = 0;
			_weight -= temp;
		if(_weight < _min) _weight = _min;
			_t_spike_post = -1e12;
}

	}
}

int Synapse::Weight() {
	return _weight;
}

Neuron* Synapse::PreNeuron() {
	return _pre;
}

Neuron* Synapse::PostNeuron() {
	return _post;
}

bool Synapse::Excitatory() {
	return _excitatory;
}

bool Synapse::Fixed() {
	return _fixed;
}

void Synapse::Print() {
	//  cout<<"("<<_t_spike_pre<<","<<_t_spike_post<<")";
	cout << _weight;
}

void Synapse::Print(FILE* fp) {
	fprintf(fp, "%d", _weight);
}
