#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include <queue>
#include <vector>
#include <cmath>

#define MAX_SIZE 20
using namespace std;

struct Plant{

	char name[MAX_SIZE];
	double minTemp;
	double maxTemp;

	Plant()
	{
		strcpy(name,"");
		minTemp = 0;
		maxTemp = 0;
	}
	
	Plant(const char n[MAX_SIZE],const double& minT,const double& maxT)
	{
		strcpy(name,n);
		minTemp = minT;
		maxTemp = maxT;
	}
	Plant(const Plant& other)
	{
		strcpy(name,other.name);
		minTemp = other.minTemp;
		maxTemp = other.maxTemp;
	}
	Plant& operator=(const Plant& other)
	{
		strcpy(name,other.name);
		minTemp = other.minTemp;
		maxTemp = other.maxTemp;
	}
	bool canThriveAt(const double& temperature) const
	{
		return temperature >= minTemp && temperature <= maxTemp;
	}

	bool hasAverageTemperature(const double& temperature) const
	{
		return abs(((maxTemp - minTemp) / 2) - temperature) <= 2;
	}

};

void parseName(ifstream& in, char *name)
{
	char c;
	char tempName[MAX_SIZE];
	int i = 0; 
	while(c != ',')
	{
		c = in.get();
		if(c!='\n' && c!= ',')
		{
			tempName[i] = c;
			i++;	
		}
	}
	strcpy(name,tempName);
}

ifstream& operator>>(ifstream& in,Plant& plant)
{	
	parseName(in,plant.name);
	in>>plant.minTemp;
	in>>plant.maxTemp;
	return in;
}

// този оператор е използван при тестването
ostream& operator<<(ostream& out,const Plant& plant)
{
	out << "Plant: " << plant.name << "  "
		<< "Minimum t = " << plant.minTemp << "  "
		<< "Maximum t = " << plant.maxTemp << "  " << endl;

	return out;
}

stack<Plant> deserializePlants()
{
	stack<Plant> result;
	ifstream in("plants.txt");
	
	
	while(!in.eof())
	{	
		Plant crr;
		in>>crr;
		result.push(crr);
	}
	
	in.close();
	return result;
}

template<class T>
vector<T> toVector(stack<T> s)
{
	vector<T> result;

	while(!s.empty())
	{
		result.push_back(s.top());
		s.pop();
	}

	return result;
}

stack<Plant> thrivingPlantsToStack(const vector<Plant>& plants, const double& temperature)
{
	stack<Plant> result;

	for(const Plant& plant : plants)
	{
		if(plant.canThriveAt(temperature))
			result.push(plant);
	}

	return result;
} 

queue<stack<Plant>> distributeByTemperature(stack<Plant> plants,stack<double> temperatures)
{
	queue<stack<Plant>> result;
	vector<Plant> allPlants = toVector(plants);
	while(!temperatures.empty())
	{
		stack<Plant> crrPlants = thrivingPlantsToStack(allPlants, temperatures.top());
		result.push(crrPlants);
		temperatures.pop();
	}

	return result;
}

void serializePlantsAtAverageTemperature(stack<Plant> plants, const double& temperature)
{
	ofstream out("botanic.bin",ios::binary);
	
	while(!plants.empty())
	{
		Plant crr = plants.top();
		if(crr.hasAverageTemperature(temperature))
			out.write((char*)&crr,sizeof (Plant));
		plants.pop();
	}

	out.close();
}


