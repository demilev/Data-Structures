#include <iostream>
#include <assert.h>

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
	void map(Node<T>* subTreeRoot,T (*mapFunction) (const T&))
	{
		if(subTreeRoot == nullptr)
			return;

		subTreeRoot->data = mapFunction(subTreeRoot->data);
		map(subTreeRoot->left,mapFunction);
		map(subTreeRoot->right,mapFunction);
	}

	Node<T>* switchInheritors(Node<T>* subTreeRoot)
	{
		if(subTreeRoot == nullptr)
			return nullptr;

		return new Node<T>(subTreeRoot->data,
						   switchInheritors(subTreeRoot->right),
						   switchInheritors(subTreeRoot->left));
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

	bool empty()
	{
		return root == nullptr;
	}

	void map(T (*mapFunction) (const T&))
	{
		map(root,mapFunction);
	}

	void switchInheritors()
	{
		root = switchInheritors(root);
	}

};


int negative(const int& number)
{
	return (-1)*number;
}

void oppositeTree(BSTree<int>& tree)
{
	tree.map(negative);
	tree.switchInheritors();
}
