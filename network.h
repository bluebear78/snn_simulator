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

	//���� 
	std::list<Neuron*> _individualNeurons;

	std::list<NeuronGroup*> _groupNeurons;

	//���� 
	std::list<Neuron*> _allNeurons;
	//Ȱ��ȭ�� ���� 
	std::list<Neuron*> _allExcitatoryNeurons;
	//��Ȱ��ȭ�� ����
	std::list<Neuron*> _allInhibitoryNeurons;


	std::list<Synapse*> _synapses;

	std::list<Synapse*> _activeInputSyns;
	std::list<Synapse*> _activeOutputSyns;

	//�н��� �����͸� ����
	std::list<Pattern*> _patterns;
	std::list<Pattern*>::iterator iter;
	bool _iter_started;
public:
	Network();
	~Network();

	//������ ���� �̸��� ������ �����ϴ��� üũ
	bool CheckExistence(char*);

	//������ �����ϰ� excitatory�� ���ο����� Ȱ��/��Ȱ�� ���� ����Ʈ�� �־���
	void AddNeuron(char*, bool);

	//���������� �����ϰ� excitatory�� ���ο����� Ȱ��/��Ȱ�� ���� ����Ʈ�� �־���
	void AddNeuronGroup(char*, int, bool);

	//synapse�� �߰�
	//synapse�� �߰� (neuron <-> neuron)
	void AddSynapse(Neuron*, Neuron*, bool, bool, int);
	//synapse�� �߰� (neuron <-> neuron group)
	void AddSynapse(Neuron*, NeuronGroup*, bool, bool, int);
	//synapse�� �߰� (group neuron <-> neuron)
	void AddSynapse(NeuronGroup*, Neuron*, bool, bool, int);
	//synapse�� �߰� (group neuron <-> group neuron)
	void AddSynapse(NeuronGroup*, NeuronGroup*, bool, bool, int);

	void AddSynapse(char*, char*, bool, bool, int);
	void AddSynapse(Neuron*, Neuron*, bool, bool, int, int, int, int);
	void AddSynapse(Neuron*, NeuronGroup*, bool, bool, int, int, int, int);
	void AddSynapse(NeuronGroup*, Neuron*, bool, bool, int, int, int, int);
	void AddSynapse(NeuronGroup*, NeuronGroup*, bool, bool, int, int, int, int);
	void AddSynapse(char*, char*, bool, bool, int, int, int, int);

	//neuron�� ��, synapse�� ��, Ȱ��,��Ȱ�� synapse�� �� ���
	void PrintSize();

	//��� ���� update
	void UpdateNeurons(double, double);
	//Ȱ��ȭ�� ������ update
	void UpdateExcitatoryNeurons(double, double);
	//��Ȱ��ȭ�� ������ update
	void UpdateInhibitoryNeurons(double, double);
	//spike�� �߻��ϰ� synapse�� update
	void UpdateSynapses();

	//Ư�� ���� ã��
	Neuron* SearchForNeuron(char*);

	//??
	Neuron* SearchForNeuron(char*, char*);

	//all neruon Ŭ����
	void ClearInputs();
	void AddPattern(Pattern*);
	Pattern* FirstPattern();
	Pattern* NextPattern();


	//��� print
	void Print();
};

#endif
