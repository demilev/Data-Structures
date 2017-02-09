#include "graph.cpp"

template <class T>
bool singleChild(const Graph<T>& graph, const T& vertex)
{
	list<T> verices = graph.getVertices();
	for(const T& graphVertex : verices)
	{
		if(graph.containsEdge(graphVertex,vertex) && graph.getNeighbours(graphVertex).size() > 1)
			return false;
	}
	return true;
}
template <class T>
int count(const list<T>& l, const T& elem)
{	
	int result = 0;
	for(const T& x : l)
	{
		if(x == elem)
			result++;
	}
	return result;
}
template <class T>
void addLassosFrom(const T& vertex, const Graph<T>& graph, list<list<T>>& result)
{
	list<list<T>> lassosBuilder;
	list<T> startingLasso;

	startingLasso.push_back(vertex);
	lassosBuilder.push_back(startingLasso);
	
	int maxLassoLength = graph.getVertices().size();
	for(int i = 0; i <= maxLassoLength; i++)
	{	
		int crrLassosSize = lassosBuilder.size();
		for(int j = 0; j < crrLassosSize; j++)
		{
			list<T> crrLasso = lassosBuilder.front();
			lassosBuilder.pop_front();
			T crrVertex = crrLasso.back();
			if(count(crrLasso,crrVertex) == 2)
			{
				result.push_back(crrLasso);
			}
			else
			{
				for(const T& neighbour : graph.getNeighbours(crrVertex))
				{
					crrLasso.push_back(neighbour);
					lassosBuilder.push_back(crrLasso);
					crrLasso.pop_back();
				}
			}
		}
	}
}
template <class T> 
list<list<T>> allLassos(const Graph<T>& graph)
{
	list<list<T>> result;
	for(const T& vertex : graph.getVertices())
	{
		addLassosFrom(vertex,graph,result);
	}
	return result;
}

template <class T>
list<T> longestLasso(const Graph<T>& graph)
{	
	list<T> longest;
	list<list<T>> lassos = allLassos(graph);
	
	if(!lassos.empty())
	{	
		longest = lassos.front();
		lassos.pop_front();
	
		for(const list<T>& lasso : lassos)
		{
			if(longest.size() < lasso.size())
				longest = lasso;
		}
	}
	
	return longest;
}
