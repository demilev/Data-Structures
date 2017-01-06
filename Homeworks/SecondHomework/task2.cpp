#include <iostream>
#include <list>
#include <queue>
#include <cmath>

using namespace std;

double sumOfPositive(queue<double> numbers)
{
	double result = 0;

	while(!numbers.empty())
	{	
		if(numbers.front() > 0)
			result+=numbers.front();
		numbers.pop();
	}

	return result;
}

double sumOfNegative(queue<double> numbers)
{
	double result = 0;

	while(!numbers.empty())
	{	
	
		if(numbers.front() < 0)
			result+=numbers.front();
		numbers.pop();
	}

	return result;
}


bool canBeSplited(list<queue<double>> listOfQueues)
{
	const double eps = 0.0001;
	double positiveSumOfPrevious = 0;
	double negativeSumOfRemaining = 0;

	for(list<queue<double>>::iterator it = listOfQueues.begin(); it != listOfQueues.end(); ++it)
	{
		negativeSumOfRemaining+=sumOfNegative(*it);
	}

	for(list<queue<double>>::iterator it = listOfQueues.begin(); it != listOfQueues.end(); ++it)
	{			
		if(abs(positiveSumOfPrevious - abs (negativeSumOfRemaining)) < eps)
			return true;
		
		positiveSumOfPrevious+=sumOfPositive(*it);
		negativeSumOfRemaining-=sumOfNegative(*it);
	}

	return false;
}


int main()
{	
	queue<double> q;
	q.push(3);
	queue<double> q1;
	q1.push(-4);
	list<queue<double>> listOfQueues;
	listOfQueues.push_back(q);
	listOfQueues.push_back(q1);
	cout<<canBeSplited(listOfQueues);
	return 0;
}