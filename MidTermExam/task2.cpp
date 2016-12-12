#include <iostream>
#include <assert.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
using namespace std;

template <class T>
using mapFn = T (*)(const T&);

template <class T>
struct Node
{
	Node<T> *left, *right;
	T data;

	Node (const T& d,Node<T> *l, Node<T> *r):left(l),right(r),data(d){
		makeID();
	}
	Node ():left(NULL),right(NULL){
		makeID();
	}
	int getID() const
	{
		return id;
	}

private:
	int id;

	void makeID ()
	{
		static int maxID = 0;
		maxID++;
    	id = maxID;

	}
};

template <class T>
class BTree
{
private:
	Node<T> *root;
    T& getElement(const char *trace, Node<T>*) const;
	bool add (const T& data, const char *trace, Node<T>*&);

	void deleteAll (Node<T> *subTreeRoot);

	static Node<T>* copyTree (const Node<T>*);

public:
	BTree();
	BTree(Node<T>*);
	BTree (const BTree<T> &);
	BTree<T>& operator = (const BTree<T> &);
	BTree<T>& add (const T& , const char *);
    T& getElement(const char *) const;

	~BTree();


};

template<class T>
BTree<T>::BTree(Node<T>* subTreeRoot){
    root=copyTree(subTreeRoot);
}


template<class T>
BTree<T>& BTree<T>::operator = (const BTree<T> &other)
{
	if (this == &other)
		return *this;

	deleteAll (root);
	root = copyTree (other.root);

	return *this;
}


template<class T>
Node<T>* BTree<T>::copyTree (const Node<T> *subTreeRoot)
{
	if (subTreeRoot == NULL)
		return NULL;

	return new Node<T> (subTreeRoot->data,
		                copyTree(subTreeRoot->left),
		                copyTree(subTreeRoot->right));
}

template<class T>
BTree<T>::BTree (const BTree<T> &other)
{
	root = copyTree (other.root);
}

template<class T>
void BTree<T>::deleteAll (Node<T> *subTreeRoot)
{
	if (subTreeRoot == NULL)
		return;

	deleteAll (subTreeRoot->left);
	deleteAll (subTreeRoot->right);
	delete subTreeRoot;
}

template <class T>
BTree<T>::~BTree()
{
	deleteAll (root);

}

template <class T>
BTree<T>& BTree<T>::add(const T& x, const char *trace)
{
   add (x,trace,root);
   return *this;
}
template <class T>
T& BTree<T>::getElement(const char *trace) const
{
	return getElement(trace, root);
}


template <class T>
T& BTree<T>::getElement(const char *trace, Node<T>* subTreeRoot) const
{
	if (subTreeRoot == NULL)
	{
		assert(false);
	}
	if (strlen(trace) == 0){
		return subTreeRoot->data;
	}

	assert(strlen(trace)>0);

	if (trace[0] == 'L')
		return getElement(trace + 1, subTreeRoot->left);

	assert(trace[0] == 'R');
	return getElement(trace + 1, subTreeRoot->right);

}
template <class T>
bool BTree<T>::add(const T& x, const char *trace, Node<T>* &subTreeRoot)
{
	if (subTreeRoot == NULL)
	{
		assert (strlen(trace) == 0);
		subTreeRoot = new Node<T> (x,NULL,NULL);
		return true;
	}

	assert (strlen(trace)>0);

	if (trace[0]=='L')
		return add (x,trace+1,subTreeRoot->left);

	assert (trace[0]=='R');
	return add (x,trace+1,subTreeRoot->right);

	return true;

}

template <class T>
BTree<T>::BTree ():root(NULL){}



bool opositeLists(BTree<list<int>> tree,const char* trace1,const char* trace2)
{
    list<int> firstList = tree.getElement(trace1);
    firstList.reverse();
    return firstList ==(tree.getElement(trace2));
}
int main ()
{
    //todo
    //test (easy)
	return 0;
}
