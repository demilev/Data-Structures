#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;


template <typename T>
struct Node{

	T data;
	Node<T>* left;
	Node<T>* right;

	Node(T d):data(d),left(nullptr),right(nullptr){}
	Node(T d,Node<T>* l,Node<T>* r):data(d),right(r),left(l){}
};


template <typename T>
class BSTree{
	
	Node<T>* root;

	Node<T>* copyTree(const Node<T>* subTreeRoot)
	{
		if(subTreeRoot == nullptr)
			return nullptr;

		return new Node<T>(subTreeRoot->data,
						   copyTree(subTreeRoot->left),
						   copyTree(subTreeRoot->right));
	}

	void deleteTree(Node<T>* subTreeRoot)
	{
		if(subTreeRoot == nullptr)
			return;

		deleteTree(subTreeRoot->left);
		deleteTree(subTreeRoot->right);
		delete subTreeRoot;
	}

	void add(Node<T>*& subTreeRoot,const T& element)
	{
		if(subTreeRoot == nullptr)
		{	
			subTreeRoot = new Node<T>(element,nullptr,nullptr);
			return;
		}

		if(element <= subTreeRoot->data)
			add(subTreeRoot->left,element);
		else
			add(subTreeRoot->right,element);

	}

	void flatten(Node<T>* subTreeRoot,vector<T>& result)
	{
		if(subTreeRoot == nullptr)
			return;

		flatten(subTreeRoot->left,result);
		result.push_back(subTreeRoot->data);
		flatten(subTreeRoot->right,result);
	}
public:

	BSTree()
	{
		root = nullptr;
	}

	BSTree(const BSTree<T>& other)
	{
		root = copyTree(other.root);
	}

	~BSTree()
	{
		deleteTree(root);
		root = nullptr;
	}

	BSTree& operator=(const BSTree<T>& other)
	{
		if(this != &other)
		{
			deleteTree(root);
			root = copyTree(other.root);
		}

		return *this;
	}

	BSTree& add(const T& element)
	{
		add(root,element);
		return *this;
	}

	vector<T> flatten()
	{
		vector<T> result;
		flatten(root,result);
		return result;
	}

	bool empty()
	{
		return root == nullptr;
	}
};

struct Interval{

private:

	int a;
	int b;

public:

	Interval(int a,int b)
	{
		assert(a<=b);
		this->a = a;
		this->b = b;
	}

	bool operator<=(const Interval& other) const
	{
		return ((b - a) <= (other.b - other.a));
	}
	
	int getA()
	{
		return a;
	}

	int getB()
	{
		return b;
	}
};


Interval containingAll(BSTree<Interval>& tree)
{
	assert(!tree.empty());
	
	vector<Interval> intervals = tree.flatten();
	
	int min = intervals[0].getA();
	int max = intervals[0].getB();

	for(Interval interval : intervals)
	{
		if(interval.getA() < min)
			min = interval.getA();
		if(interval.getB() > max)
			max = interval.getB();
	}

	return Interval(min,max);

}
