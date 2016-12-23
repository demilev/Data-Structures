#include <iostream>
#include <assert.h>
#include <stack>

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

	stack<size_t> value()
	{
		stack<size_t> result;

		if(root == nullptr)
			return result;


		stack<pendingTraverseStep> operations;
		operations.push(pendingTraverseStep(STEP_TRAVERSE_SUBTREE,root));


		while (!operations.empty())
		{

			pendingTraverseStep topOperation = operations.top();
			Node<T>* topNode = topOperation.second;

			operations.pop();

			if(topOperation.first == STEP_EXTRACT_ROOT)
			{
				assert(topNode->data>=0 && topNode->data<10);
				result.push(topNode->data);

			}
			else{
				if (topNode->right != nullptr)
					operations.push (pendingTraverseStep(STEP_TRAVERSE_SUBTREE,topNode->right));
				operations.push (pendingTraverseStep(STEP_EXTRACT_ROOT,topNode));
				if (topNode->left != nullptr)
					operations.push (pendingTraverseStep(STEP_TRAVERSE_SUBTREE,topNode->left));
			}

		}

		return result;
	}


};



stack<size_t> reverseStack(stack<size_t>& digitStack)
{
	stack<size_t> result;

	while(!digitStack.empty())
	{
		result.push(digitStack.top());
		digitStack.pop();
	}

	return result;
}

stack<size_t> sumOfDigitStacks(stack<size_t>& first,stack<size_t>& second)
{
	stack<size_t> result;
	stack<size_t> firstBackwards = reverseStack(first);
	stack<size_t> secondBackwards = reverseStack(second);

	size_t left = 0;

	while(!firstBackwards.empty() && !secondBackwards.empty())
	{
		size_t current = firstBackwards.top() + secondBackwards.top() + left;
		firstBackwards.pop();
		secondBackwards.pop();
		result.push(current%10);
		left = current/10;
	}

	if(firstBackwards.empty())
	{
		while(!secondBackwards.empty())
		{
			size_t current = secondBackwards.top() + left;
			secondBackwards.pop();
			result.push(current%10);
			left = current/10;
		}
	}
	else
	{
		while(!firstBackwards.empty())
		{
			size_t current = firstBackwards.top() + left;
			firstBackwards.pop();
			result.push(current%10);
			left = current/10;
		}
	}
	if(left!=0)
	{
		result.push(left);
	}
	return result;
}

bool sumOfTrees(BSTree<size_t>& first,BSTree<size_t>& second)
{
	stack<size_t> firstValue = first.value();
	stack<size_t> secondValue = second.value();

	stack<size_t> result = sumOfDigitStacks(firstValue,secondValue);

	while(!result.empty())
	{
		cout<<result.top();
		result.pop();
	}

	return true;
}

