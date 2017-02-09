#include <list>
#include <assert.h>

using std::list;

// имплементацията на ориентиран граф,
// използвана в задачи 1,2,3,4
template <class T>
class Graph{

	private:
		list<list<T>> vertices;
		void addIfNotExistingVertex(const T&);
	public:
		void addVertex(const T&);
		void addEdge(const T&, const T&);
		list<T> getNeighbours(const T&) const;
		list<T> getVertices() const;
		bool containsVertex(const T&) const;
		bool containsEdge(const T&, const T&) const;
};

template <class T>
void Graph<T>::addIfNotExistingVertex(const T& vertex)
{
	if(!containsVertex(vertex))
	{
		list<T> newVertex;
		newVertex.push_back(vertex);
		vertices.push_back(newVertex);
	}
}

template <class T>
void Graph<T>::addVertex(const T& vertex)
{
	addIfNotExistingVertex(vertex);
}


template <class T>
void Graph<T>::addEdge(const T& firstVertex, const T& secondVertex)
{
	if(containsEdge(firstVertex,secondVertex))
		return;

	addIfNotExistingVertex(firstVertex);
	addIfNotExistingVertex(secondVertex);

	for(list<T>& vertex : vertices)
	{
		if(vertex.front() == firstVertex)
		{
			vertex.push_back(secondVertex);
		}
	}
}


template <class T>
list<T> Graph<T>::getNeighbours(const T& vertex) const
{
	if(containsVertex(vertex))
	{
		for(list<T> adjacentList : vertices)
		{
			if (adjacentList.front() == vertex)
			{
				adjacentList.pop_front();
				return adjacentList;
			}
		}
	}
	
	return list<T>();
}


template <class T>
list<T> Graph<T>::getVertices() const
{
	list<T> result;
	for(const list<T>& vertex : vertices)
	{
		result.push_back(vertex.front());
	}
	return result;
}


template <class T>
bool Graph<T>::containsVertex(const T& vertex) const
{
	for(const list<T>& adjacentList : vertices)
	{
		if(adjacentList.front() == vertex)
			return true;
	}
	return false;
}


template <class T>
bool Graph<T>::containsEdge(const T& firstVertex, const T& secondVertex) const
{
	list<T> firstVertexNeighbours = getNeighbours(firstVertex);
	for(const T& neighbour : firstVertexNeighbours)
	{
		if(neighbour == secondVertex)
			return true;
	}
	return false;
}
