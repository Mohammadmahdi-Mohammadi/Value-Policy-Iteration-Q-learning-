/*
Mohammad mehdi Mohammadi - 964416
Mohammad Andalibi - 964408
2020-spring

Project is about Reinforcement learning addresses the question of how
an autonomous agent that senses and acts in its environment can learn
to choose optimal actions to achieve its goals.
And include Value Iteration , Policy Iteration , Q learning .

*/
#include <windows.h>
#include <stack>
#include<iostream>
#include<vector>
#include<fstream>
#include<utility>
#include <queue>
#include <time.h>
#include <algorithm>
#include <math.h>
using namespace std;
struct Gameboard
{
	friend class State;
	int** Data; // data bar hasbe sotuni shomare ghozari shode / 1-khali 0-divar
	float** Q_data; // data bara Q learning 
	int Size; 
	int S;
	int numb;//size data Q learning
	vector<int> G; //baraye zakhireye goal ha
	float* Value;
	int * action ;
	int** policy;
	bool _change;
	int u;
	Gameboard() {
		this->Size = 1;
		this->Q_data = new float*[1];
		Q_data[0] = new float[1];
		this->Data = new int*[1];
		this->Data[0] = new int[4];
		this->action = new int[1];
		_change = true;

		this->policy = new int*[1];
	//	int * p_val = new int[1];

		S = 1;
		Value = NULL;
		//     Initial registration
	}
	Gameboard(int n)
	{
		this->Size = n;
		this->action = new int[this->Size];
		int Y, X;
		Value = new float[n];
		Data = new int*[n];
		Q_data = new float*[4*n];

		policy = new int*[n];
		for (X = 0; X < 4*n; X++)
			Q_data[X] = new float[4];

		for (X = 0; X < n; X++)
		{
			action[X] = -1;
			Data[X] = new int[4];
			policy[X] = new int[4];
		}

		_change = true;
		//     Initial registration
		for (X = 0; X < n; X++)
		{
			Value[X] = 0;
			Q_data[X][0] = 0;
			Q_data[X][1] = 0;

			Q_data[X][2] = 0;
			Q_data[X][3] = 0;


			for (Y = 0; Y < 4; Y++)
			{
				policy[X][Y] = -1;
				Data[X][Y] = -1;

			}
		}
		S = -1;
	}
	Gameboard(const Gameboard& temp)
	{
		int   Y, X;
		this->Size = temp.Size;
		Data = new int*[temp.Size];
		for (X = 0; X < temp.Size; X++)
		{
			Data[X] = new int[4];
		}
		//     Initial registration
		for (X = 0; X < temp.Size; X++)
			for (Y = 0; Y < 4; Y++)
				Data[X][Y] = temp.Data[X][Y];
		S = temp.S;
		G = temp.G;

	}
	void Read(string Address);
	float measerment(float * , int, int, int,int);
	void ValueIteration(float);
	void PolicyIteration(float);
	void Q_learning(float);
	void player();
	int reward(int);
	int random(int);
};
class State {
	friend struct  Gameboard;
	friend class  Game;

private:
	int NumberOfCell;
	int S;
};


int Gameboard::random(int n)// ye action random baraye state N mide
{
	int temp_number;
	bool boolturn = true;
	temp_number = rand() % 4;
	while (this->Data[n][temp_number] != 1 && boolturn) {
		boolturn = false;
		for (int i = 0; i < 4; i++) {
			if(this->Data[n][i]==1)
				boolturn = true;
			
		}
		temp_number = rand() % 4;
	}

	return temp_number;
}
void Gameboard::Read(string Address)
{
	int num1, num2;
	fstream file;
	file.open(Address);
	if (!file) {
		cout << "file khondeh nashoedeh!!";
		return;
	}
	file >> num1;
	file >> num2;
	this->u = num2;
	int tmp;
	tmp = num1 * num2;
	this->Size = tmp;
	int  Y = 0, X = 0;
	this->Data = new int*[this->Size];
	for (X = 0; X < this->Size; X++)
	{
		Data[X] = new int[4];
	}

	int temp;
	for (X = 0; X < this->Size; X++) {
		for (Y = 0; Y < 4; Y++) {
			file >> temp;
			this->Data[X][Y] = temp;

		}
	}
	int o; 
	file >> o;
	S = o - 1;
	int number, m;
	file >> number;
	for (int i = 0; i < number; i++) {
		file >> m;
		this->G.push_back(m-1);
	}


}

float Gameboard::measerment(float * temp_val , int n_size, int i, int z,int turn)//baraye mohasebe tavabe
{
	bool tmp = false;

	if (turn == 0) { //  Value Iteration - sometime for policy Iteration
		if (count(this->G.begin(), this->G.end(), i))//  goal
			return 10000;
		float* temp = new float[4];
		float* T = new float[4];

		/*if (this->Data[i][0] == 1 && this->Data[i][1] == 1 && this->Data[i][2] == 1 && this->Data[i][3] == 1)
		{
			if (count(this->G.begin(), this->G.end(), i - 1)) {
				T[0] = 0.7;
				T[1] = 0.1;
				T[2] = 0.1;
				T[3] = 0.1;

			}
			if (count(this->G.begin(), this->G.end(), i + 1)) {
				T[0] = 0.1;
				T[1] = 0.1;
				T[2] = 0.7;
				T[3] = 0.1;
			}
			if (count(this->G.begin(), this->G.end(), i + u)) {
				T[0] = 0.1;
				T[1] = 0.7;
				T[2] = 0.1;
				T[3] = 0.1;
			}
			if (count(this->G.begin(), this->G.end(), i - u)) {
				T[0] = 0.1;
				T[1] = 0.1;
				T[2] = 0.1;
				T[3] = 0.7;
			}
		}
		if (this->Data[i][0] == 1 && this->Data[i][1] == 1 && this->Data[i][2] == 1 && this->Data[i][3] == 1)*/
		for (int k = 0; k < 4; k++)
			temp[k] = 0;
		for (int j = 0; j < 4; j++) {
			if (this->Data[i][j] == 1) {

				if (j == 0) { // N
					temp[j] = reward(i - 1) + 0.9 * temp_val[i - 1];
					if (temp[0]>0)
						tmp = true;
					/*if (count(this->G.begin(), this->G.end(), i - 1)) {
						temp[j] = temp[j] * 0.8;
					}
					else
						temp[j] = temp[j] * 0.1;*/
				}
				if (j == 1) { // E
					temp[j] = reward(i + u) + 0.9 * temp_val[i + u];
					if (temp[1]>0)
						tmp = true;
					/*if (count(this->G.begin(), this->G.end(), i + u)) {
						temp[j] = temp[j] * 0.8;
					}
					else
						temp[j] = temp[j] * 0.1;*/


				}
				if (j == 2) { //  S
					temp[j] = reward(i + 1) + 0.9 * temp_val[i + 1];
					if (temp[2]>0)
						tmp = true;
					/*if (count(this->G.begin(), this->G.end(), i + 1)) {
						temp[j] = temp[j] * 0.8;
					}
					else
						temp[j] = temp[j] * 0.1;*/
				}
				if (j == 3) {// W

					temp[j] = reward(i - u) + 0.9 * temp_val[i - u];
					if (temp[3]>0)
						tmp = true;
					/*if (count(this->G.begin(), this->G.end(), i - u)) {
						temp[j] = temp[j] * 0.8;
					}
					else
						temp[j] = temp[j] * 0.1;*/
				}
			}
		}
		float _max = 0;
		
		for (int f = 0; f < 4; f++)
			if (_max < temp[f]) {
				_max = temp[f];
			}
		for (int f = 0; f < 4; f++) {
			if (temp[f] != _max)
				policy[i][f] = -1;
		
			if (_max == temp[f] && tmp)
				policy[i][f] = 1;
		}

		//	_max = temp[0] + temp[1] + temp[2] + temp[3];
		return _max;
	}
	

	if (turn == 1) { // for policy Iteration
		if (count(this->G.begin(), this->G.end(), i))
			return 10000;
		float* temp = new float[4];
		for (int k = 0; k < 4; k++)
			temp[k] = -1;
		if (this->policy[i][0] != -1) { // N
			temp[0]= reward(i - 1) + 0.9 * this->Value[i - 1];
			if(temp[0]>0)
				tmp = true;
		}
		if (this->policy[i][1] != -1) { // E
			temp[1]= reward(i +u) + 0.9 * this->Value[i +u];
			if (temp[1]>0)
				tmp = true;

			}
		if (this->policy[i][2] != -1) {// S
			temp[2]= reward(i + 1) + 0.9 * this->Value[i + 1];
			if (temp[2]>0)
				tmp = true;

				}
		if (this->policy[i][3] != -1) {// W
			temp[3]= reward(i - u) + 0.9 * this->Value[i - u];
			if (temp[3]>0)
				tmp = true;

			}
		float _max = 0;

		for (int f = 0; f < 4; f++)
			if (_max <= temp[f]) {
				_max = temp[f];
			}
		for (int f = 0; f < 4; f++) {
			if (policy[i][f] != _max)
				policy[i][f] = -1;
			if (_max == temp[f] && tmp) 
				policy[i][f] = 1;
		}

		return _max;// give us maximum action
	}
}

void Gameboard::ValueIteration(float N)
{/*
 Value iteration is a method of computing an optimal 
 MDP policy and its value. Value iteration starts at 
 the "end" and then works backward, refining an estimate
 of either Q* or V*. There is really no end, so it uses
 an arbitrary end point
 
 
 */
	float _max;
	int n = this->S;
	float _value = 0;
	float z = 10;
	for (int k = 0; k < this->Size; k++) {
		this->Value[k] = 0;

	}

	float * temp_val = new float[this->Size];
	while (z >= N) { // Convergence V in Value Iteration
		z = 0;
		float n;
		for (int i = 0; i < this->Size; i++) 
			temp_val[i] = this->Value[i];
		for (int i = 0; i < this->Size; i++) { //for each state 
			this->Value[i] = measerment(temp_val,this->Size, i, i,0);
			n = abs(temp_val[i] - this->Value[i]); // distance
			if (n > z) 
				z = n;
			
		}
	}
	
}

void Gameboard::PolicyIteration(float N)
{

	/*
	In Policy Iteration - You randomly select a policy
	and find value function corresponding to it , then 
	find a new (improved) policy based on the previous 
	value function, and so on this will lead to optimal policy
	*/
	for (int k = 0; k < this->Size; k++) {
		this->Value[k] = 0;
	}
	for (int i = 0; i < this->Size; i++) {  // randomly select a policy
		this->action[i] = random(i);

		this->policy[i][this->action[i]] = 1;

	}

	bool change = true;
	int z = N;
	float * temp_val = new float[this->Size];
	while (change) {//	Convergence policy iteration

		while (z >= N) {
			for (int i = 0; i < this->Size; i++)
			z = 0;
			float n;
			for (int i = 0; i < this->Size; i++)
				temp_val[i] = this->Value[i];
			
			for (int i = 0; i < this->Size; i++) { //for each state
				
				this->Value[i] = measerment(temp_val, this->Size, i, i, 1);
				n = abs(temp_val[i] - this->Value[i]); // distance
				if (n > z)
					z = n;

			}


		}

		int ** temp_policy = new int*[this->Size];
		for (int X = 0; X < this->Size; X++)
		{
			temp_policy[X] = new int[4];
		}

		for (int i = 0; i < this->Size; i++) {
			temp_policy[i][0] = policy[i][0];
			temp_policy[i][1] = policy[i][1];
			temp_policy[i][2] = policy[i][2];
			temp_policy[i][3] = policy[i][3];

		}
		bool a = true;
		for (int i = 0; i < this->Size; i++)
			temp_val[i] = this->Value[i];
		for (int i = 0; i < this->Size && a; i++)
		{
			this->Value[i] = measerment(temp_val, this->Size, i, i, 0);
			if (temp_policy[i][0] != policy[i][0]
				|| temp_policy[i][1] != policy[i][1]
				|| temp_policy[i][2] != policy[i][2]
				|| temp_policy[i][3] != policy[i][3]) {
				a = false;
			}

		}

		change = false;

		for (int i = 0; i < this->Size; i++) {

			if (temp_policy[i][0] != policy[i][0]
				|| temp_policy[i][1] != policy[i][1]
				|| temp_policy[i][2] != policy[i][2]
				|| temp_policy[i][3] != policy[i][3])
			{
				change = true;
				
			}
			if (change)
				break;
		}
	}
}

void Gameboard::Q_learning(float N)
{
	/*
	Q-learning is an off policy reinforcement learning 
	algorithm that seeks to find the best action to take
	given the current state. It's considered off-policy 
	because the q-learning function learns from actions 
	that are outside the current policy, like taking 
	random actions, and therefore a policy isn't needed.
	*/
	float* temp = new float[4];
	int counter = 0;
	for (int k = 0; k < this->Size; k++) {
		for (int kk = 0; kk < 4; kk++) {
			if (count(this->G.begin(), this->G.end(), k)) {
				Q_data[counter][0] = k;
				Q_data[counter][1] = k;
				Q_data[counter][2] = 100;
			}
			if (this->Data[k][kk] == 1 && !(count(this->G.begin(), this->G.end(), k))) {
				Q_data[counter][0] = k;
				if (kk == 0)
					Q_data[counter][1] = k - 1;
				if (kk == 1)
					Q_data[counter][1] = k + u;
				if (kk == 2)
					Q_data[counter][1] = k + 1;
				if (kk == 3)
					Q_data[counter][1] = k - u;
				Q_data[counter][2] = 0;
				counter++;
			}
		}
	}

	this->numb = counter;
	while (1000000 > N) { 

		int num = rand() % (counter + 1);
		if (!count(this->G.begin(), this->G.end(), Q_data[num][0])) {
			if (!count(this->G.begin(), this->G.end(), Q_data[num][1])) {
				float _max = 0;
				for (int i = 0; i < counter; i++) {
					if (Q_data[i][0] == Q_data[num][1]) {
						if (_max <= Q_data[i][2])
							_max = Q_data[i][2];
					}
				}
				Q_data[num][2] = reward(Q_data[num][1]) + 0.5 *_max;
			}

			else {
				Q_data[num][2] = reward(Q_data[num][1]);
			}
		}
		N++;
	}
}

void Gameboard::player() // in function bara S hast.
{
	int n;
	this->Read("input.txt");
	cout << "\n1-Value Iteration\n2-Q learning\n3-Policy Iteration\n";
	cin >> n;
	if (n == 1)
		this->ValueIteration(1);
	if (n == 2)
		this->Q_learning(0);
	if (n == 3) {

		this->PolicyIteration(1);
	}
	if (n == 1 || n == 3) {
		while (!count(this->G.begin(), this->G.end(), S)) {
			cout << "state of player: ----->" << S + 1;
			cout << endl;
			if (this->policy[S][0] != -1)
				S = S - 1;
			if (this->policy[S][1] != -1)
				S = S + u;
			if (this->policy[S][2] != -1)
				S = S + 1;
			if (this->policy[S][3] != -1)
				S = S - u;

		}
		cout << "state of player: ----->" << S + 1;


	}
	if (n == 2) {
		
		int n_temp = -1;
		bool  s_temp = false;
		while (!count(this->G.begin(), this->G.end(), S)) {
			cout << "\nstate of player: -->" << S + 1;

			int _max = 0;
			int index_ = -1;
			int * temp1 = new int[4];
			/*temp1[0] = -1;
			temp1[1] = -1;
			temp1[2] = -1;
			temp1[3] = -1;*/
			int count = 0;
			for (int i = 0; i < this->numb; i++) {
				if (Q_data[i][0] == S) {
					
					if (_max <= Q_data[i][2] && Q_data[i][3] != 1) {
					//	temp1[count] = i;
						_max = Q_data[i][2];
						index_ = i;
						count++;
					}
					
				}
			}
			if (index_ != -1) {
				//index_ = rand() % 4;
				//while(temp1[index_]==-1)
				//	index_ = rand() % 4;
			//	Q_data[index_][3] = 1;
				n_temp = S;
				S = Q_data[index_][1];
			}
		}
		cout << "\nstate of player: ----->" << S + 1;

	}

}

int Gameboard::reward(int n)
{

	if (count(this->G.begin(), this->G.end(), n)) // age goal bashad 100 midahad
		return 100;
	else
		return 0;
}




int main() {
	int n;
	cout << "size of cube(25 or 625) : ";
	cin >> n;
	Gameboard temp(n);
	temp.player();
	cin >> n;
	return 0;
}