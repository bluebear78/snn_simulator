#ifndef PARSER_H
#define PARSER_H
#define _CRT_SECURE_NO_WARNINGS
class Network;

class Parser {
private:
	Network* _network;
public:
	Parser(Network*);

	void Parse(char*);
	//inputs.txt , 26letters.txt ÀÐ±â

	void ParseNeuron(char*, char*);

	void ParseNeuronGroup(char*, int, char*);
	void ParseSynapse(char*, char*, char*, char*, int);
	void ParseSynapse(char*, char*, char*, char*, int, int, int, int);
};

#endif

