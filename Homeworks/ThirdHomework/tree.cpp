#include <iostream>
#include <assert.h>
#include <cstring>

using namespace std;

// имплементацията на двоично дърво,
// използвана в задачa 5
template <class T>
class BinTree
{

private:

	struct Node
	{
		Node *left, *right;
		T data;
		Node (const T& d,Node *l, Node *r);
		Node ();
	};
public:

	class HierarchicalIterator
	{

	public:
		HierarchicalIterator (Node *root);
		T operator * () const;
		HierarchicalIterator goLeft ();
		HierarchicalIterator goRight ();
		bool empty();
	private:
		Node *currentSubtree;
	};


private:
	Node *root;
	bool add (const T& data, const char *trace, Node*&);
	void deleteAll (Node *subTreeRoot);
	static Node* copyTree (const Node *subTreeRoot);
	

public:
	BinTree();
	BinTree (const BinTree<T> &other);

	
	BinTree<T>& operator = (const BinTree<T> &other);

	BinTree<T>& add (const T& data, const char *trace);
	bool empty() const;
	HierarchicalIterator rootIter() const;

	~BinTree();

};

template<class T>
BinTree<T>::HierarchicalIterator::HierarchicalIterator (Node *root):currentSubtree(root)
{}

template<class T>
T BinTree<T>::HierarchicalIterator::operator * () const
{
	assert (currentSubtree != nullptr);
	return currentSubtree->data;
}

template<class T>
typename BinTree<T>::HierarchicalIterator BinTree<T>::HierarchicalIterator::goLeft ()
{
	assert (currentSubtree != nullptr);
	return HierarchicalIterator(currentSubtree->left);
}
template<class T>
typename BinTree<T>::HierarchicalIterator BinTree<T>::HierarchicalIterator::goRight ()
{
	assert (currentSubtree != nullptr);
	return HierarchicalIterator(currentSubtree->right);
}
template<class T>
bool BinTree<T>::HierarchicalIterator::empty()
{
	return currentSubtree == nullptr;
}

template <class T>
BinTree<T>::Node::Node (const T& d,BinTree<T>::Node *l, BinTree<T>::Node *r)
  :left(l),right(r),data(d)
{
	
}
template <class T>
BinTree<T>::Node::Node ():left(nullptr),right(nullptr)
{
	
}

template<class T>
BinTree<T>& BinTree<T>::operator = (const BinTree<T> &other)
{
	if (this == &other)
		return *this;

	deleteAll (root);
	root = copyTree (other.root);

	return *this;
}


template<class T>
typename BinTree<T>::Node* BinTree<T>::copyTree (const BinTree<T>::Node *subTreeRoot)
{
	if (subTreeRoot == nullptr)
		return nullptr;

	return new BinTree<T>::Node (subTreeRoot->data,
		                copyTree(subTreeRoot->left),
		                copyTree(subTreeRoot->right));
}

template<class T>
BinTree<T>::BinTree (const BinTree<T> &other)
{
	root = copyTree (other.root);
}


template<class T>
void BinTree<T>::deleteAll (BinTree<T>::Node *subTreeRoot)
{
	if (subTreeRoot == nullptr)
		return;

	deleteAll (subTreeRoot->left);
	deleteAll (subTreeRoot->right);
	delete subTreeRoot;
}

template <class T>
BinTree<T>::~BinTree()
{
	deleteAll (root);
	root = nullptr;

}


template <class T>
BinTree<T>& BinTree<T>::add(const T& x, const char *trace)
{ 
   add (x,trace,root);
   return *this;
}

template <class T>
bool BinTree<T>::empty() const
{ 
   return root == nullptr;
}

template <class T>
bool BinTree<T>::add(const T& x, const char *trace, BinTree<T>::Node* &subTreeRoot)
{
	if (subTreeRoot == nullptr)
	{
		assert (strlen(trace) == 0);
		subTreeRoot = new BinTree<T>::Node (x,nullptr,nullptr);
		return true;
	}

	assert (strlen(trace)>0);

	if (trace[0]=='L')
		return add (x,trace+1,subTreeRoot->left);

	assert (trace[0]=='R');
	return add (x,trace+1,subTreeRoot->right);

}

template <class T>
BinTree<T>::BinTree ():root(nullptr){}

template <class T>
typename BinTree<T>::HierarchicalIterator BinTree<T>::rootIter() const
{	
	Node* copyOfRoot = root;
	return typename BinTree<T>::HierarchicalIterator(copyOfRoot);
}


