#include <iostream>
#include <assert.h>
using namespace std;

template <typename T>
struct Node
{
	T data;
	Node<T>* next;
	Node<T>* previous;
	Node():next(NULL),previous(NULL){}
	Node(T d,Node<T> *p,Node<T> *n):data(d),next(n),previous(p){}
};

template <typename T>
class DoublyLinkedList{
	Node<T> *start;
	Node<T> *end;
	void copy(const DoublyLinkedList<T>&);
	void deleteList();
public:
	DoublyLinkedList();
	DoublyLinkedList(const DoublyLinkedList<T>&);
	~DoublyLinkedList();
	DoublyLinkedList& operator=(const DoublyLinkedList<T>&);
	bool empty();
	void toEnd(const T&);
	void pop();
	void insertAfter(int, const T&);
	void insertBefore(int, const T&);
	void insertElem(int, const T&);
	void deleteAfter(int, T&);
	void deleteBefore(int, T&);
	void deleteElem(int, T&);
	void print() const;
	int length() const;
	void concat(const DoublyLinkedList<T>&);
	void map(T(*)(T&));
	bool member(const T&)const;
	DoublyLinkedList<T>* filter(bool(*)(const T&))const;
};


template<typename T>
void DoublyLinkedList<T>::copy(const DoublyLinkedList<T>& other){
	Node<T>* crr=other.start;
	int index=0;
	while(crr!=NULL){
		toEnd(crr->data);
		crr=crr->next;
	}
}
template<typename T>
void DoublyLinkedList<T>::deleteList(){
	Node<T>* crr=start;
	while(crr!=NULL){
		Node<T>* tmp=crr;
		crr=crr->next;
		delete tmp;
	}
}
template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(){
	start=NULL;
	end=NULL;
}
template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& other){
	start=NULL;
	end=NULL;	
	copy(other);
}
template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList(){
	deleteList();
}
template<typename T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(const DoublyLinkedList<T>& other){
	if(this!=other){
		deleteList();
		copy(other);
	}
	return this;
}
template<typename T>
bool DoublyLinkedList<T>::empty(){
	return start==NULL&&end==NULL;
}
template<typename T>
void DoublyLinkedList<T>::toEnd(const T& elem){
	if(empty()){
		start=new Node<T>(elem,NULL,NULL);
		end=start;
	}
	else{
		end->next=new Node<T>(elem,end,NULL);
		end=end->next;
	}
}
template<typename T>
void DoublyLinkedList<T>::pop(){
	if(empty())
		return;
	if(start==end){
		delete start;
		start=end=NULL;
		return;
	}
	Node<T> *crr=end;
	end=end->previous;
	delete crr;
}
template<typename T>
void DoublyLinkedList<T>::insertElem(int index,const T& element){
	int listSize=length();
	assert(index>=0&&index<=listSize);
	if(index==0){
		if(listSize==0){
			start=new Node<T>(element,NULL,NULL);
			end=start;
			return;
		}
		Node<T> *newNode=new Node<T>(element,NULL,start);
		start->previous=newNode;
		start=newNode;
		return;
	}
	if(index==listSize){
		toEnd(element);
		return;
	}
	int counter=0;
	Node<T> *crr=start;
	while(counter<index){
		crr=crr->next;
		counter++;
	}
	Node<T> *previous=crr->previous;
	previous->next=new Node<T>(element,previous,crr);
	crr->previous=previous->next;
}
template<typename T>
void DoublyLinkedList<T>::insertBefore(int index,const T& element){
	insertElem(index-1,element);
}
template<typename T>
void DoublyLinkedList<T>::insertAfter(int index,const T& element){
	insertElem(index+1,element);
}
template<typename T>
int DoublyLinkedList<T>::length() const{
	int result=0;
	Node<T> *crr=start;
	while(crr!=NULL){
		result++;
		crr=crr->next;
	}
	return result;
}
template<typename T>
void DoublyLinkedList<T>::deleteElem(int index,T& element){
	int listSize=length();
	assert(index>=0&&index<listSize);
	if(index==0){
        Node<T> *tmp=start;
        element=start->data;
        start=start->next;
        if(listSize==1)
            end=NULL;
        delete tmp;
        return;
	}
	if(index==listSize-1){
        pop();
        return;
	}
	int counter=0;
	Node<T> *crr=start;
	while(counter<index){
		crr=crr->next;
		counter++;
	}
	element=crr->data;
	crr->previous->next=crr->next;
	crr->next->previous=crr->previous;
	delete crr;
}
template<typename T>
void DoublyLinkedList<T>::deleteBefore(int index,T& element){
	deleteElem(index-1,element);
}
template<typename T>
void DoublyLinkedList<T>::deleteAfter(int index,T& element){
	deleteElem(index+1,element);
}
template<typename T>
void DoublyLinkedList<T>::print() const{
	Node<T> *crr=start;
	while(crr!=NULL){
		cout<<crr->data<<" ";
		crr=crr->next;
	}
	cout<<endl;
}
template<typename T>
void DoublyLinkedList<T>::concat(const DoublyLinkedList<T>& other){
	Node<T> *crr=other.start;
	while(crr!=NULL){
		toEnd(crr->data);
		crr=crr->next;
	}
}
template<typename T>
void DoublyLinkedList<T>::map(T(*f)(T&)){
	Node<T>* crr=start;
	while(crr!=NULL){
		crr->data=f(crr->data);
		crr=crr->next;
	}
}
template<typename T>
bool DoublyLinkedList<T>::member(const T& element)const{
	Node<T>* crr=start;
	while(crr!=NULL){
		if(crr->data==element)
			return true;
		crr=crr->next;
	}
	return false;
}
template<typename T>
DoublyLinkedList<T>* DoublyLinkedList<T>::filter(bool (*pred)(const T&))const{
	DoublyLinkedList<T> result;
	Node<T>* crr=start;
	while(crr!=NULL){
		if(pred(crr->data))
			result.toEnd(crr->data);
		crr=crr->next;
	}
	return new DoublyLinkedList<T>(result);
}

