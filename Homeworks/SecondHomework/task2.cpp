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
		// първо се прави тази проверка, защото считам, че при  
		// разбиването едната част може да е празна	
		if(abs(positiveSumOfPrevious - abs (negativeSumOfRemaining)) < eps)
			return true;
		
		positiveSumOfPrevious+=sumOfPositive(*it);
		negativeSumOfRemaining-=sumOfNegative(*it);
	}

	return false;
}

