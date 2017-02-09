

#include "idgenerator.cpp"

#include <map>
#include <unordered_set>
#include <list>
#include <vector>
#include <ostream>
#include <stdexcept>
#include <iostream>

using std::cout;
using std::map;
using std::unordered_set;
using std::list;
using std::vector;
using std::ostream;
using std::endl;
using std::runtime_error;

template <class T>
class Graph
{
	private:
		struct Vertex
		{
			T data;
			unordered_set<size_t> adjacentIDs;
			Vertex() {}
			Vertex(const T& data) : data(data) {}
		};
		IDGenerator idGenerator;
		size_t graphId;
		map<T, size_t> ids;
		map<size_t, Vertex> vertexes;
	public:
		void addVertex(const T& vertexData);
		void addEdge(const T& firstVertexData, const T& secondVertexData);
		void deleteVertex(const T& vertexData);
		void deleteEdge(const T& firstVertexData, const T& secondVertexData);
		void dottyPrint(ostream& out) const;
		list<T> getAdjacent(const T& vertexData) const;
		list<T> getVertexes() const;
		bool containsVertex(const T& vertexData) const;
		bool containsEdge(const T& firstVertexData, const T& secondVertexData) const;
};

template <class T>
void Graph<T>::addVertex(const T& vertexData)
{
	if (containsVertex(vertexData))
		throw runtime_error("Trying to add already existing vertex!");
	size_t id = idGenerator.getID();
	ids[vertexData] = id;
	vertexes[id] = Vertex(vertexData);
}

template <class T>
void Graph<T>::deleteVertex(const T& vertexData)
{
	if (!containsVertex(vertexData))
		throw runtime_error("Trying to delete non-existing vertex!");
	size_t id = ids.erase(vertexData);
	vertexes.erase(id);
	idGenerator.free(id);
	for (auto& entry : vertexes)
	{
		entry.second.adjacentIDs.erase(id);
	}
}

template <class T>
void Graph<T>::addEdge(const T& firstVertexData, const T& secondVertexData)
{
	if (!containsVertex(firstVertexData) || !containsVertex(secondVertexData))
		throw runtime_error("Trying to add an edge with a non-existing vertex!");
	size_t firstID = ids.at(firstVertexData);
	size_t secondID = ids.at(secondVertexData);
	vertexes.at(firstID).adjacentIDs.insert(secondID);
}

template <class T>
void Graph<T>::deleteEdge(const T& firstVertexData, const T& secondVertexData)
{
	if (!containsVertex(firstVertexData) || !containsVertex(secondVertexData))
		throw runtime_error("Trying to delete an edge with a non-existing vertex!");
	size_t firstID = ids.at(firstVertexData);
	size_t secondID = ids.at(secondVertexData);
	vertexes.at(firstID).adjacentIDs.erase(secondID);
}

template <class T>
void Graph<T>::dottyPrint(ostream& out) const
{
	out << "digraph G\n{";
	for (const auto& entry : ids)
		out << entry.second << "[label=\"" << entry.first << "\"];" << endl;
	for (const auto& entry : vertexes)
		for (const size_t& adjID : entry.second.adjacentIDs)
			out << entry.first << " -> " << adjID << ";" << endl;
	out << "}" << endl;
}

template <class T>
list<T> Graph<T>::getAdjacent(const T& vertexData) const
{
	list<T> adjacent;
	for (size_t id : vertexes.at(ids.at(vertexData)).adjacentIDs)
		adjacent.push_back(vertexes.at(id).data);
	return adjacent;
}

template <class T>
list<T> Graph<T>::getVertexes() const
{
	list<T> vertexesData;
	for (const auto& entry : vertexes)
		vertexesData.push_back(entry.second.data);
	return vertexesData;
}

template <class T>
bool Graph<T>::containsVertex(const T& vertexData) const
{
	return ids.find(vertexData) != ids.end();
}

template <class T>
bool Graph<T>::containsEdge(const T& firstVertexData, const T& secondVertexData) const
{
		size_t firstID = ids.at(firstVertexData);
		size_t secondID = ids.at(secondVertexData);
		return vertexes.at(firstID).adjacentIDs.count(secondID) > 0;
}


template <class T>
list<list<T>> allPaths(const T& start, const T& end, const Graph<T>& graph, int k)
{	
	list<list<T>> result;
	list<list<T>> paths;
	list<T> first;
	first.push_back(start);
	paths.push_back(first);
	for(int i = 0; i <= k; i++)
	{
		int currentSize = paths.size();
		for(int j = 0; j < currentSize; j++)
		{
			list<T> crr = paths.front();
			paths.pop_front();
			if(crr.back() == end)
				result.push_back(crr);
			T last = crr.back();
			for(T neighbour : graph.getAdjacent(last))
			{	
				crr.push_back(neighbour);
				paths.push_back(crr);
				crr.pop_back();
			}
		}
	}
	return result;
}

template <class T>
void printList(const list<T>& l)
{
	for(T elem : l)
		cout<< elem << " ";
}
template <class T>
void printListOfLists(const list<list<T>> listOfLists)
{
	for(const list<T> l : listOfLists)
	{
		printList(l);
		cout << endl;
	}

}
int main()
{
	Graph<int> g;
	g.addVertex(1);
	g.addVertex(2);
	g.addVertex(3);
	g.addVertex(4);
	g.addVertex(5);
	g.addEdge(1,2);
	g.addEdge(1,5);
	g.addEdge(5,2);
	g.addEdge(1,4);
	g.addEdge(2,3);
	g.addEdge(4,3);
	printListOfLists(allPaths(1,3,g,3));
}

