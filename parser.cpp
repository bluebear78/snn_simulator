#include"parser.h"
#include"pattern.h"
#include"neuron.h"
#include"network.h"
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<assert.h>
#include<string.h>

using namespace std;

Parser::Parser(Network* network) {
	_network = network;
}

//input 읽어오기
void Parser::Parse(char* filename)
{
	char linestring[8192];
	char** token = new char* [64];
	FILE* fp = fopen(filename, "r");
	assert(fp != NULL);
	assert(fp != 0);

	int i;
	while (fgets(linestring, 8191, fp) != NULL) {
		if (strlen(linestring) <= 1) continue;
		if (linestring[0] == '#') continue;

		token[0] = strtok(linestring, " \t\n");//tab
		assert(token[0] != NULL);


		//26letters.txt
		//ex)neuron(token[0]) inhiS(token[1]) inhibitory(token[2])
		//신경망 구성
		if (strcmp(token[0], "neuron") == 0) {
			// neuron name
			token[1] = strtok(NULL, " \t\n");
			assert(token[1] != NULL);
			token[2] = strtok(NULL, " \t\n");
			assert(token[2] != NULL);

			//membrane potenial 에 sitimul(excitatory,inhibitory) 쌓는다
			ParseNeuron(token[1], token[2]);
		}
		//26letters.txt
		//neurongroup input 1024 excitatory
		//input output neuron set
		else if (strcmp(token[0], "neurongroup") == 0) {
			// neurongroup name number
			token[1] = strtok(NULL, " \t\n");
			assert(token[1] != NULL);
			token[2] = strtok(NULL, " \t\n");
			assert(token[2] != NULL);
			token[3] = strtok(NULL, " \t\n");
			assert(token[3] != NULL);

			ParseNeuronGroup(token[1], atoi(token[2]), token[3]);
		}

		//26letters.txt 
		//ex1)synapse input  output excitatory learn 0 7 0 6
		//ex2)synapse output inhiS   excitatory fixed 3000
		else if (strcmp(token[0], "synapse") == 0) {
			// synapse preNeuron postNeuron excitatory/inhibitory fixed/learn (values)
			token[1] = strtok(NULL, " \t\n");
			assert(token[1] != NULL);
			token[2] = strtok(NULL, " \t\n");
			assert(token[2] != NULL);
			token[3] = strtok(NULL, " \t\n");
			assert(token[2] != NULL);
			token[4] = strtok(NULL, " \t\n");
			assert(token[2] != NULL);

			if (strcmp(token[4], "fixed") == 0) {
				token[5] = strtok(NULL, " \t\n");
				assert(token[5] != NULL);

				//ex2)synapse output inhiS   excitatory fixed 3000
				ParseSynapse(token[1], token[2], token[3], token[4], atoi(token[5]));
			}
			else if (strcmp(token[4], "learn") == 0) {
				token[5] = strtok(NULL, " \t\n");
				assert(token[5] != NULL);
				token[6] = strtok(NULL, " \t\n");
				assert(token[6] != NULL);
				token[7] = strtok(NULL, " \t\n");
				assert(token[7] != NULL);
				token[8] = strtok(NULL, " \t\n");
				assert(token[8] != NULL);

				//ex1)synapse input  output excitatory learn 0 7 0 6
				ParseSynapse(token[1], token[2], token[3], token[4], atoi(token[5]), atoi(token[6]), atoi(token[7]), atoi(token[8]));
			}
			else assert(0);
		}
		//inputs.txt && 26letters.txt
		//activate input 176 177 178 207 ...
		//학습할 데이터
		else if (strcmp(token[0], "activate") == 0) {
			Neuron* neuron;
			Pattern* pattern = new Pattern();
			_network->AddPattern(pattern);
			token[1] = strtok(NULL, " \t\n");
			assert(token[1] != NULL);
			pattern->SetName(token[1]);
			token[2] = strtok(NULL, " \t\n");
			assert(token[2] != NULL);
			neuron = _network->SearchForNeuron(token[2]);
			if (neuron == NULL) {
				token[3] = strtok(NULL, " \t\n");
				assert(token[3] != NULL);
				do {
					neuron = _network->SearchForNeuron(token[2], token[3]);
					pattern->AddNeuron(neuron);
					token[3] = strtok(NULL, " \t\n");
				} while (token[3] != NULL);
			}
			else pattern->AddNeuron(neuron);
		}
		else if (strcmp(token[0], "end") == 0) {
			break;
		}
		else {
			cout << token[0] << endl;
			assert(0);
		}

		//    while(1){
		//      token = strtok(NULL," \t\n");
		//    }
	}

	fclose(fp);
}

//membrane potenial 에 sitimul 쌓는 부분
//neuron name1 : inhiS e_i : inhibitory or excitatory
//excitatory : true  inhitory : false
void Parser::ParseNeuron(char* name, char* e_i) {
	assert(_network->CheckExistence(name) == false);
	assert((strcmp(e_i, "excitatory") == 0) || (strcmp(e_i, "inhibitory") == 0));
	bool excitatory;
	if (strcmp(e_i, "excitatory") == 0) excitatory = true;
	else excitatory = false;
	_network->AddNeuron(name, excitatory);
}


//neurongroup, input 1024 excitatory
void Parser::ParseNeuronGroup(char* name, int num, char* e_i) {
	assert(_network->CheckExistence(name) == false);
	assert(num > 1);
	assert((strcmp(e_i, "excitatory") == 0) || (strcmp(e_i, "inhibitory") == 0));

	bool excitatory;
	if (strcmp(e_i, "excitatory") == 0) excitatory = true;
	else excitatory = false;
	_network->AddNeuronGroup(name, num, excitatory);
}



//synapse, output inhiS   excitatory fixed 3000
void Parser::ParseSynapse(char* from, char* to, char* e_i, char* fixed_learn, int value) {
	assert(_network->CheckExistence(from) == true);
	assert(_network->CheckExistence(to) == true);
	assert((strcmp(e_i, "excitatory") == 0) || (strcmp(e_i, "inhibitory") == 0));
	assert(strcmp(fixed_learn, "fixed") == 0);
	assert(value > -1e-18);

	bool excitatory, fixed;
	if (strcmp(e_i, "excitatory") == 0) excitatory = true;
	else excitatory = false;
	fixed = true;
	_network->AddSynapse(from, to, excitatory, fixed, value);
}


//ex)synapse input  output excitatory learn 0 7 0 6
void Parser::ParseSynapse(char* from, char* to, char* e_i, char* fixed_learn, int min, int max, int ini_min, int ini_max) {
	assert(_network->CheckExistence(from) == true);
	assert(_network->CheckExistence(to) == true);
	assert((strcmp(e_i, "excitatory") == 0) || (strcmp(e_i, "inhibitory") == 0));
	assert(strcmp(fixed_learn, "learn") == 0);
	assert(min > -1e-18);
	assert(max > min);
	assert(ini_min >= min - 1e-18);
	assert(ini_max <= max + 1e-18);

	bool exitatory, fixed;
	if (strcmp(e_i, "excitatory") == 0) exitatory = true;
	else exitatory = false;
	fixed = false;
	_network->AddSynapse(from, to, exitatory, fixed, min, max, ini_min, ini_max);
}

