#include <iostream>
#include "tree.cpp"
#include <queue>

using namespace std;
using Iterator = BinTree<int>::HierarchicalIterator;

int sumNodes(const BinTree<int>& t)
{	
	if(t.empty())
		return 0;

	int result = 0;
	
	//опашка от двойки родител - наследник
	//изразени чрез итератора
	queue<pair<Iterator,Iterator>> nodes;
	Iterator root = t.rootIter();
	result += (*root);


	if(!root.goLeft().empty())
		nodes.push(pair<Iterator,Iterator>(root,root.goLeft()));
	if(!root.goRight().empty())
		nodes.push(pair<Iterator,Iterator>(root,root.goRight()));
	
	while(!nodes.empty())
	{
		pair<Iterator,Iterator> crr = nodes.front();
		nodes.pop();

		bool isBiggerThanLeft = false;
		bool isBiggerThanRight = false;

		if(!crr.second.goLeft().empty())
		{
			nodes.push(pair<Iterator,Iterator>(crr.second,crr.second.goLeft()));
			if(*(crr.second) > *(crr.second.goLeft()))
				isBiggerThanLeft = true;
		}
		else
			isBiggerThanLeft = true;
		
		if(!crr.second.goRight().empty())
		{
			nodes.push(pair<Iterator,Iterator>(crr.second,crr.second.goRight()));
			if(*(crr.second) > *(crr.second.goRight()))
				isBiggerThanRight = true;
		}
		else
			isBiggerThanRight = true;
		
		if(isBiggerThanLeft && isBiggerThanRight && *(crr.first) > *(crr.second))
		{
			result += *(crr.second);
		}
	}

	return result;
}


int main()
{
	BinTree<int> t;
	t.add(20,"").add(12,"L").add(10,"R").add(10,"LR").add(11,"LL").add(13,"RL").add(-13,"RR").add(-12,"RRR");

	cout<<sumNodes(t)<<endl;
}