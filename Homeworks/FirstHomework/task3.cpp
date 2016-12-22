#include <iostream>
#include <assert.h>
#include <stack>

using namespace std;

unsigned int power10(unsigned int power)
{
	unsigned int result = 1;
	for(int i=0;i<power;i++)
	{
		result*=10;
	}
	return result;
}

//структура, която пази "стойността" на дървото 
//и дали тя е валидна (това е заради условието на задачата: "функцията да връща подходяща стойност, чието значение да бъде успешното й изпълнение")
//една сума ще е невалидна, ако някой от възлите е отрицателен или е повече от 10
struct OptionalValue
{
	unsigned int value;
	bool isValid;
	OptionalValue(unsigned int v, bool i):value(v),isValid(i){}

};

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

	using pendingTraverseStep = pair<int,Node<T>*>;
	
	#define STEP_EXTRACT_ROOT 0
	#define STEP_TRAVERSE_SUBTREE 1

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

	OptionalValue value()
	{
		if(root == nullptr)
			return OptionalValue(0,true);
		
		unsigned int power = 0;
		unsigned int result = 0;
		stack<pendingTraverseStep> operations;
		operations.push(pendingTraverseStep(STEP_TRAVERSE_SUBTREE,root));

		
		while (!operations.empty())
		{

			pendingTraverseStep topOperation = operations.top();
			Node<T>* topNode = topOperation.second;

			operations.pop();

			if(topOperation.first == STEP_EXTRACT_ROOT)
			{	
				if(topNode->data<0 || topNode->data>=10)
				{
					return OptionalValue(-1,false);
				}
				result+=(topNode->data * power10(power));
				power++;
			}
			else{
				if (topNode->right != nullptr)
					operations.push (pendingTraverseStep(STEP_TRAVERSE_SUBTREE,topNode->right));
				operations.push (pendingTraverseStep(STEP_EXTRACT_ROOT,topNode));
				if (topNode->left != nullptr)
					operations.push (pendingTraverseStep(STEP_TRAVERSE_SUBTREE,topNode->left));
			}

		}
		
		return OptionalValue(result,true);
	}


};

//функцията може да връща и OptionalValue, но заради условието връща bool
bool sumOfTrees(BSTree<int>& first,BSTree<int>& second)
{	
	OptionalValue firstValue = first.value();
	OptionalValue secondValue = second.value();
	if(!firstValue.isValid||!secondValue.isValid)
		return false;
	cout<<firstValue.value + secondValue.value<<endl;
	return true;	
}
