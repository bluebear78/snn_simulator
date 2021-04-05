#ifndef NETWORK_H
#define NETWORK_H
#define _CRT_SECURE_NO_WARNINGS

#include<list>
class Neuron;
class Synapse;
class NeuronGroup;
class Pattern;

class Network {
private:

	//뉴런 
	std::list<Neuron*> _individualNeurons;

	std::list<NeuronGroup*> _groupNeurons;

	//뉴런 
	std::list<Neuron*> _allNeurons;
	//활성화된 뉴런 
	std::list<Neuron*> _allExcitatoryNeurons;
	//비활성화된 뉴런
	std::list<Neuron*> _allInhibitoryNeurons;


	std::list<Synapse*> _synapses;

	std::list<Synapse*> _activeInputSyns;
	std::list<Synapse*> _activeOutputSyns;

	//학습할 데이터를 쌓음
	std::list<Pattern*> _patterns;
	std::list<Pattern*>::iterator iter;
	bool _iter_started;
public:
	Network();
	~Network();

	//뉴런이 같은 이름의 뉴런이 존재하는지 체크
	bool CheckExistence(char*);

	//뉴런을 생성하고 excitatory의 여부에따라 활성/비활성 뉴런 리스트에 넣어줌
	void AddNeuron(char*, bool);

	//뉴런집합을 생성하고 excitatory의 여부에따라 활성/비활성 뉴런 리스트에 넣어줌
	void AddNeuronGroup(char*, int, bool);

	//synapse를 추가
	//synapse를 추가 (neuron <-> neuron)
	void AddSynapse(Neuron*, Neuron*, bool, bool, int);
	//synapse를 추가 (neuron <-> neuron group)
	void AddSynapse(Neuron*, NeuronGroup*, bool, bool, int);
	//synapse를 추가 (group neuron <-> neuron)
	void AddSynapse(NeuronGroup*, Neuron*, bool, bool, int);
	//synapse를 추가 (group neuron <-> group neuron)
	void AddSynapse(NeuronGroup*, NeuronGroup*, bool, bool, int);

	void AddSynapse(char*, char*, bool, bool, int);
	void AddSynapse(Neuron*, Neuron*, bool, bool, int, int, int, int);
	void AddSynapse(Neuron*, NeuronGroup*, bool, bool, int, int, int, int);
	void AddSynapse(NeuronGroup*, Neuron*, bool, bool, int, int, int, int);
	void AddSynapse(NeuronGroup*, NeuronGroup*, bool, bool, int, int, int, int);
	void AddSynapse(char*, char*, bool, bool, int, int, int, int);

	//neuron의 수, synapse의 수, 활성,비활성 synapse의 수 출력
	void PrintSize();

	//모든 뉴런 update
	void UpdateNeurons(double, double);
	//활성화된 뉴런만 update
	void UpdateExcitatoryNeurons(double, double);
	//비활성화된 뉴런만 update
	void UpdateInhibitoryNeurons(double, double);
	//spike가 발생하고 synapse를 update
	void UpdateSynapses();

	//특정 뉴런 찾기
	Neuron* SearchForNeuron(char*);

	//??
	Neuron* SearchForNeuron(char*, char*);

	//all neruon 클리어
	void ClearInputs();
	void AddPattern(Pattern*);
	Pattern* FirstPattern();
	Pattern* NextPattern();


	//결과 print
	void Print();
};

#endif
