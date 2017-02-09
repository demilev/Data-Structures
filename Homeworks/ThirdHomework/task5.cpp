#include "tree.cpp"
#include <queue>
#include <list>

using std::queue;
using std::list;
using Iterator = BinTree<int>::HierarchicalIterator;

void allPaths(Iterator subTreeRootIterator, list<int> crr, list<list<int>>& result)
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

list<list<int>> allLevels(const BinTree<int>& btree)
{
	list<list<int>> result;
    queue<Iterator> crr;    
    crr.push(btree.rootIter());
    while(!crr.empty())
    {
    	list<int> crrData;
		int size = crr.size();
	    for(int i=0; i<size ;i++)
        {
            Iterator crrNode=crr.front();
            crr.pop();
			crrData.push_back(*crrNode);
		    if(!crrNode.goLeft().empty())
	            crr.push(crrNode.goLeft());
            if(!crrNode.goRight().empty())
                crr.push(crrNode.goRight());
        }
        result.push_back(crrData);
    }
    return result;
}

void trimZeros(list<int>& path)
{
	while(path.front() == 0)
		path.pop_front();
}

bool hasLevelEqualToPath(list<list<int>>& levels, const list<int>& path)
{
	for(list<int>& level : levels)
	{	
		trimZeros(level);
		if(level == path)
			return true;
	}
	return false;
}
bool existsPathEqualToLevel(const BinTree<int>& btree)
{
	list<list<int>> paths;
	allPaths(btree.rootIter(),list<int>(),paths);

	list<list<int>> levels = allLevels(btree);

	for(list<int>& path : paths)
	{
		trimZeros(path);
		if(hasLevelEqualToPath(levels,path))
			return true;
	}

	return false;
}
