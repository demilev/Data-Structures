#include <iostream>
#include <stack>
using namespace std;

#define SIZE 4

bool labyrinth[SIZE][SIZE]={ 0, 0, 0, 0,
							 1, 1, 1, 0,
							 0, 1, 1, 0,
							 0, 0, 0, 0 };

struct LabField{
	int x;
	int y;
	LabField(int x, int y)
	{
		this->x=x;
		this->y=y;
	}
	bool operator== (const LabField& other)
	{
		return x == other.x &&
			   y == other.y;
	}
};

bool canStepOn(int x, int y)
{
	return x >= 0 &&
		   x < SIZE &&
		   y >= 0 &&
		   y < SIZE &&
		   labyrinth[x][y] == 0;
}
bool recDownstairs(int sx, int sy, int ex, int ey)
{	
	if(sx == sy && ex == ey)
		return true;
	if(canStepOn(sx+1,sy) && recDownstairs(sx+1,sy,ex,ey))
		return true;
	if(canStepOn(sx,sy+1) && recDownstairs(sx,sy+1,ex,ey))
		return true;
	return false;

}
bool downstairs(int sx, int sy, int ex, int ey)
{
	LabField end(ex,ey);
	LabField start(sx,sy);
	stack<LabField> fields;
	fields.push(start);

	while(!fields.empty())
	{
		LabField crr = fields.top();
		fields.pop();
		if(crr == end)
			return true;

		if(canStepOn(crr.x, crr.y+1))
			fields.push(LabField(crr.x, crr.y+1));
		if(canStepOn(crr.x+1, crr.y))
			fields.push(LabField(crr.x+1, crr.y));
	}
	return false;
}

int main(){

	cout<<downstairs(2,0,2,3);
}