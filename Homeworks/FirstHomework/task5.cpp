#include <iostream>
#include <queue>

using namespace std;

int getFromSequence(int start, int p)
{	
	queue<int> sequence;
	int crrIndex = 0;
	int result;

	sequence.push(start);
	while(crrIndex < p)
	{
		result = sequence.front();
		sequence.pop();
		sequence.push(result+1);
		sequence.push(result*2);
		crrIndex++;
	}	

	return result;
}
