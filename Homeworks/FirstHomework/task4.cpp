#include <iostream>
#include <stack>
#include <assert.h>

using namespace std;

template <typename T>
class Queue{

	stack<T> head;
	stack<T> tail;

	void transfer()
	{
		while(!tail.empty())
		{
			head.push(tail.top());
			tail.pop();
		}
	}
public:

	Queue():head(),tail(){}

	bool empty()
	{
		return head.empty()&&tail.empty();
	}

	size_t size()
	{
		return head.size()+tail.size();
	}

	T front()
	{
		if(head.empty())
		{
			assert(!tail.empty());
			transfer();
		}
		return head.top();
	}

	void push(const T& element)
	{
		tail.push(element);
	}

	void pop()
	{
		if(head.empty())
		{
			assert(!tail.empty());
			transfer();
		}
		head.pop();
	}
};
