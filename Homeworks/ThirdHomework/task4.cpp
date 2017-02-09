#include "graph.cpp"
#include <unordered_set>

using std::unordered_set;

template <class T> 
list<list<T>> allPaths(const T& start, const T& end, const Graph<T> graph)
{	
	unordered_set<T> visited;
	list<list<T>> result;
	list<list<T>> paths;

	list<T> first;
	first.push_back(start);
	paths.push_back(first);
	visited.insert(start);

	int maxPathLength = graph.getVertices().size();
	for(int i = 0; i <= maxPathLength; i++)
	{
		int currentSize = paths.size();
		for(int j = 0; j < currentSize; j++)
		{
			list<T> crr = paths.front();
			paths.pop_front();
			T last = crr.back();
			if(last == end)
				result.push_back(crr);
			else
			{
				for(T neighbour : graph.getNeighbours(last))
				{	
					if(visited.count(neighbour) == 0)
					{	
						visited.insert(neighbour);
						crr.push_back(neighbour);
						paths.push_back(crr);
						crr.pop_back();
					}
				}
			}	
		}
	}
	return result;
}

template <class T>
bool contains(const list<T>& path, const T& elem)
{
	for(const T& x : path)
	{
		if (x == elem)
			return true;
	}

	return false;
}

template <class T>
bool isSubSet(const list<T>& firstPath, const list<T>& secondPath)
{
	for(const T& elem : firstPath)
	{
		if(!contains(secondPath,elem))
			return false;
	}
	return true;
}

template <class T>
bool hasPathWithVerticesOnlyFrom(const list<T>& path, const list<list<T>>& allPaths)
{
	for(const list<T>& crrPath : allPaths)
	{
		if(isSubSet(crrPath,path))
			return true;
	}
	return false;
}

template <class T>
list<T> findPath(const T& start, const T& end, const Graph<T>& graph)
{
	list<list<T>> allPathsFromFirstToSecond = allPaths(start, end, graph);
	list<list<T>> allPathsFromSecondToFirst = allPaths(end, start, graph);

	for(const list<T>& path : allPathsFromFirstToSecond)
	{
		if(hasPathWithVerticesOnlyFrom(path,allPathsFromSecondToFirst))
			return path;
	}

	return list<T>();
}
