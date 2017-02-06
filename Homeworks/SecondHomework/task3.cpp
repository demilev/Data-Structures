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
	
	bool isRootBiggerThanLeft = false;
	bool isRootBiggerThanRight = false;

	//приемам, че коренът се добавя само ако е по - голям 
	//от наследниците си, защото няма родител, а листата -
	//само ако са по - малки от родителя си, защото нямат
	//наследници
	
	if(!root.goLeft().empty())
	{
		nodes.push(pair<Iterator,Iterator>(root,root.goLeft()));
		isRootBiggerThanLeft = *root > *(root.goLeft()) ? true : false;
	}
	else
		isRootBiggerThanLeft = true;
	
	if(!root.goRight().empty())
	{
		nodes.push(pair<Iterator,Iterator>(root,root.goRight()));
		isRootBiggerThanRight = *root > *(root.goRight()) ? true : false;
	}
	else
		isRootBiggerThanRight = true;
	
	
	result += (isRootBiggerThanRight && isRootBiggerThanLeft) ? *root : 0;
	
	while(!nodes.empty())
	{
		pair<Iterator,Iterator> crr = nodes.front();
		nodes.pop();

		bool isBiggerThanLeft = false;
		bool isBiggerThanRight = false;

		if(!crr.second.goLeft().empty())
		{
			nodes.push(pair<Iterator,Iterator>(crr.second,crr.second.goLeft()));
			isBiggerThanLeft = *(crr.second) > *(crr.second.goLeft()) ? true : false;
		}
		else
			isBiggerThanLeft = true;
		
		if(!crr.second.goRight().empty())
		{
			nodes.push(pair<Iterator,Iterator>(crr.second,crr.second.goRight()));
			isBiggerThanRight = *(crr.second) > *(crr.second.goRight()) ? true : false;
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

