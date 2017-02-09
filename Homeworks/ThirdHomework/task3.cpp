#include "graph.cpp"

bool isPath(const list<int>& integerSequence, const Graph<int>& graph)
{	
	list<int> sequence = integerSequence;
	
	int crr = sequence.front();
	sequence.pop_front();
	
	for(const int& next : sequence)
	{
		if(!graph.containsEdge(crr,next))
			return false;
		crr = next;
	}

	return true;
}

bool areAllPaths(const Graph<int>& graph, const list<list<int>>& listOfIntegerSequences)
{
	for(const list<int>& integerSequence : listOfIntegerSequences)
	{
		if(!isPath(integerSequence,graph))
			return false;
	}
	return true;
}
