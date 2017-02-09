#include "graph.cpp"

template <class T>
int numberOfParents(const T& vertex, const Graph<T>& graph)
{
	int result = 0;
	for(const T& graphVertex : graph.getVertices())
	{
		if(graph.containsEdge(graphVertex,vertex))
			result++;
	}
	return result;
}

template <class T>
int countNodes(const Graph<T>& graph)
{	
	int result = 0;
	for(const T& vertex : graph.getVertices())
	{
		if(graph.getNeighbours(vertex).size() < numberOfParents(vertex,graph))
			result++;
	}
	return result;
}


void addLaddersFrom(const int& vertex, const Graph<int>& graph, list<list<int>>& result)
{
	list<list<int>> laddersBuilder;
	list<int> startingLadder;

	startingLadder.push_back(vertex);
	laddersBuilder.push_back(startingLadder);
	
	int maxLadderLength = graph.getVertices().size();
	for(int i = 0; i < maxLadderLength; i++)
	{	
		int crrLadderSize = laddersBuilder.size();
		for(int j = 0; j < crrLadderSize; j++)
		{
			list<int> crrLadder = laddersBuilder.front();
			laddersBuilder.pop_front();
			int crrVertex = crrLadder.back();
			if(crrVertex == 2 * crrLadder.front() || (crrVertex < 0 && 2 * crrVertex == crrLadder.front()))
			{
				result.push_back(crrLadder);
			}
			else
			{
				for(const int& neighbour : graph.getNeighbours(crrVertex))
				{	
					if(neighbour == 1 + crrVertex)
					{	
						crrLadder.push_back(neighbour);
						laddersBuilder.push_back(crrLadder);
						crrLadder.pop_back();
					}
				}
			}
		}
	}
}

list<list<int>> allLadders(const Graph<int>& graph)
{
	list<list<int>> result;
	for(const int& vertex : graph.getVertices())
	{
		addLaddersFrom(vertex,graph,result);
	}
	return result;
}

list<int> shortestLadder(const Graph<int>& graph)
{
	list<int> shortest;
	list<list<int>> ladders = allLadders(graph);
	
	if(!ladders.empty())
	{
		shortest = ladders.front();
		ladders.pop_front();
	
		for(const list<int>& ladder : ladders)
		{
			if(shortest.size() > ladder.size())
				shortest = ladder;
		}
	}

	return shortest;


}
