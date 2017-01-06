#include <iostream>
#include "tree.cpp"
#include <string>
#include <vector>

using namespace std;
using Iterator = BinTree<char>::HierarchicalIterator;
void allPaths(Iterator subTreeRootIterator,string crr,vector<string>& result)
{
    crr.push_back(*subTreeRootIterator);
 
    if(subTreeRootIterator.goLeft().empty() &&
       subTreeRootIterator.goRight().empty())
    {
   		result.push_back(crr);
   		return;
   	}
  
   	if(!subTreeRootIterator.goLeft().empty())
   		allPaths(subTreeRootIterator.goLeft(),crr,result);
   	if(!subTreeRootIterator.goRight().empty())
   		allPaths(subTreeRootIterator.goRight(),crr,result);
}

bool readWord(const BinTree<char>& t, char const* w)
{
	string path;
	vector<string> paths;
	
	allPaths(t.rootIter(),path,paths);
	
	string word(w);
	for(string path : paths)
		if(path.find(word) != string::npos)
			return true;

	return false;	
}

int main()
{
	BinTree<char> t;
	t.add('a',"").add('b',"L").add('c',"R").add('a',"LR").add('e',"LL").add('a',"RL").add('e',"RR").add('c',"RRR");
	cout<<readWord(t,"ceca");
}