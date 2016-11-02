#include <iostream>
#include <assert.h>
#include <string.h>
#include <algorithm> 
#include <vector>
#include <string>

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
	void simplePrint (Node<T> *subTreeRoot) const;
	bool add (const T& data, const char *trace, Node<T>*&);
	T& getElement(const char *trace, Node<T>*) const;
	void deleteAll (Node<T> *subTreeRoot);
	bool member (const T&,Node<T> *subTreeRoot) const;
	void map (mapFn<T>,Node<T> *subTreeRoot);
	int searchCount(bool(*)(const T&), Node<T>*);
	int height(Node<T>*) const;
	int countLeaves(Node<T>*) const;
	T& maxLeaf(Node<T>*) const;
	void dottyPrint (Node<T> *subTreeRoot,ostream& out) const;
	void serialize (Node<T>*, ostream&) const;
	Node<T>* parseTree (istream &in);
	void listLeaves(Node<T>*,vector<T>&) const;
	void findTrace(const T&,Node<T>*,string&) const;
	void prettyPrint(Node<T>*,int)const;
	void findElem(int,Node<T>*,T*&);
	void schemePrint(Node<T>*)const;
public:
	BTree();
	BTree<T>& add (const T& data, const char *trace);
	void simplePrint () const;
	void dottyPrint (ostream&);
	void prettyPrint()const;
	void schemePrint()const;
	bool member (const T&) const;
	int searchCount(bool(*)(const T&));
	void map (mapFn<T>);
	int height() const;
	int countLeaves() const;
	T& maxLeaf() const;
	T& getElement(const char *trace) const;
	void serialize (ostream&)const;
	void deserialize (istream&);
	vector<T> listLeaves() const;
	~BTree();
	string findTrace(const T&) const;
	T& operator[](int);
};
template<class T>
void BTree<T>::schemePrint (Node<T> *subTreeRoot) const
{
	if (subTreeRoot == NULL)
	{
		cout << "()";
		return;
	}

	cout <<"("<<subTreeRoot->data << " ";

	schemePrint (subTreeRoot->left);
	schemePrint (subTreeRoot->right);
	cout<<")";
}

template<class T>
void BTree<T>::schemePrint () const
{
	schemePrint (root);
	cout << endl;
}
template<class T>
void BTree<T>::findElem(int i,Node<T>*subTreeRoot,T*& elem){
	if(i==0){
		assert(subTreeRoot!=NULL);
		elem=&subTreeRoot->data;
		return;
	}
	if(subTreeRoot->left!=NULL){
		findElem(i-1,subTreeRoot->left,elem);
	}
	if(subTreeRoot->right!=NULL)
		findElem(i-1,subTreeRoot->right,elem);
	return;
}
template<class T>
T& BTree<T>::operator[](int i){
	T* result;
	findElem(i,root,result);
	return *result;
}
template<class T>
void BTree<T>::prettyPrint()const{
	prettyPrint(root,1);
}
template<class T>
void BTree<T>::prettyPrint(Node<T>* subTreeRoot,int currentHeight)const{
	if(subTreeRoot==NULL)
		return;
	if(subTreeRoot->left==NULL&&subTreeRoot->right==NULL){
		for(int i=0;i<3*currentHeight-3;i++)
			cout<<" ";
		cout<<subTreeRoot->data<<endl;
		return;
	}
	prettyPrint(subTreeRoot->right,currentHeight+1);
	for(int i=0;i<3*currentHeight-3;i++)
			cout<<" ";
	cout<<subTreeRoot->data<<endl;
	prettyPrint(subTreeRoot->left,currentHeight+1);

}
template<class T>
string BTree<T>::findTrace(const T& x)const{
	if(!member(x))
		return "_";
	string result="";
	findTrace(x,root,result);
	return result;
}
template<class T>
void BTree<T>::findTrace(const T& x,Node<T>* subTreeRoot,string& result)const{
	if(subTreeRoot->data==x)
		return;
	if(member(x,subTreeRoot->left)){
		result.push_back('L');
		findTrace(x,subTreeRoot->left,result);
		return;
	}
	if(member(x,subTreeRoot->right)){
		result.push_back('R');
		findTrace(x,subTreeRoot->right,result);
		return;
	}
}
template<class T>
vector<T> BTree<T>::listLeaves()const{
	vector<T> result;
	listLeaves(root,result);
	return result;
}
template<class T>
void BTree<T>::listLeaves(Node<T>* subTreeRoot,vector<T>& result)const{
	if(subTreeRoot==NULL)
		return;
	if(subTreeRoot->left==NULL&&subTreeRoot->right==NULL){
		result.push_back(subTreeRoot->data);
		return;
	}
	listLeaves(subTreeRoot->left,result);
	listLeaves(subTreeRoot->right,result);

}
template<class T>
void BTree<T>::serialize (Node<T> *subTreeRoot, ostream &out) const
{
	if (subTreeRoot == NULL)
	{
		out << "null ";
		return;
	}

	out << subTreeRoot->data << " ";

	serialize (subTreeRoot->left,out);
	serialize (subTreeRoot->right,out);

}

template<class T>
void BTree<T>::serialize (ostream &out) const
{
	serialize (root,out);
	cout << endl;
}



template<class T>
void BTree<T>::dottyPrint (ostream &out)
{
	out << "digraph G {" << endl;
	dottyPrint (root,out);
	out << "}" << endl;
}



template<class T>
void BTree<T>::dottyPrint (Node<T> *subTreeRoot,ostream& out) const
{
	if (subTreeRoot == NULL)
		return;

	out << subTreeRoot->getID() 
	    << "[label=\"" 
	    << subTreeRoot->data 
	    << "\"];" << endl;

	if (subTreeRoot->left != NULL)
		out << subTreeRoot->getID() 
	        <<"->"
	        << subTreeRoot->left->getID()
	        << endl;

	if (subTreeRoot->right != NULL)
		out << subTreeRoot->getID() 
	        <<"->"
	        << subTreeRoot->right->getID()
	        << endl;

	dottyPrint (subTreeRoot->left,out);
	dottyPrint (subTreeRoot->right,out);
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

template<class T>
T& BTree<T>::maxLeaf(Node<T>* subTreeRoot) const{
	if (subTreeRoot->right == NULL&&subTreeRoot->left != NULL){
		return maxLeaf(subTreeRoot->left);
	}
	if (subTreeRoot->right != NULL&&subTreeRoot->left == NULL){
		return maxLeaf(subTreeRoot->right);
	}
	if (subTreeRoot->right == NULL&&subTreeRoot->left == NULL){
		return subTreeRoot->data;
	}
	if (maxLeaf(subTreeRoot->left) >= maxLeaf(subTreeRoot->right)){
		return maxLeaf(subTreeRoot->left);
	}
	else{
		return maxLeaf(subTreeRoot->right);
	}
		
}
template<class T>
T& BTree<T>::maxLeaf() const{
	assert(root != NULL);
	return maxLeaf(root);
}

template<class T>
int BTree<T>::countLeaves() const{
	return countLeaves(root);
}

template<class T>
int BTree<T>::countLeaves(Node<T>* subTreeRoot) const{
	if (subTreeRoot == NULL)
		return 0;
	if (subTreeRoot->right == NULL&&subTreeRoot->left == NULL)
		return 1;
	return countLeaves(subTreeRoot->right)+countLeaves(subTreeRoot->left);
}

template<class T>
int BTree<T>::height() const{
	return height(root);
}

template<class T>
int BTree<T>::height(Node<T>* subTreeRoot) const{
	if (subTreeRoot == NULL)
		return 0;
	return 1 + max(height(subTreeRoot->left), height(subTreeRoot->right));
}

template<class T>
int BTree<T>::searchCount(bool(*pred)(const T&)){
	return searchCount(pred, root);
}
template<class T>
int BTree<T>::searchCount(bool(*pred)(const T&),Node<T> *subTreeRoot){
	if (subTreeRoot == NULL)
		return 0;
	
	if (pred(subTreeRoot->data))
		return 1 + searchCount(pred, subTreeRoot->right) + searchCount(pred, subTreeRoot->left);
	else
		return searchCount(pred, subTreeRoot->right) + searchCount(pred, subTreeRoot->left);
}
template<class T>
void BTree<T>::map (mapFn<T> f,Node<T> *subTreeRoot)
{
	if (subTreeRoot == NULL)
		return;

	subTreeRoot->data = f (subTreeRoot->data);
	map (f,subTreeRoot->left);
	map (f,subTreeRoot->right);
}

template<class T>
void BTree<T>::map (mapFn<T>f)
{
	map (f,root);
}


template<class T>
bool BTree<T>::member (const T& x) const
{
	return member (x,root);
}


template<class T>
bool BTree<T>::member (const T& x,Node<T> *subTreeRoot) const
{
	if (subTreeRoot == NULL)
		return false;


	return subTreeRoot->data == x ||
	       member (x,subTreeRoot->left) ||
	       member (x,subTreeRoot->right);


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

template <class T>
void BTree<T>::simplePrint() const
{
	simplePrint (root);
	cout << endl;
}

template <class T>
void BTree<T>::simplePrint(Node<T> *subTreeRoot) const
{
	if (subTreeRoot == NULL)
		return;

	cout << subTreeRoot->data << " ";
	simplePrint (subTreeRoot->left);
	simplePrint (subTreeRoot->right);
}

void removeWhite (istream &in)
{
	while (in.peek() <= 32)
		in.get();
}

template <class T>
Node<T>* BTree<T>::parseTree (istream &in)
{
	
	removeWhite (in);

	if (in.peek() == 'n')
	{
		string dummy;
		in >> dummy;
		assert (dummy == "null");
		return NULL;
	}

	T data;
	in >> data;

	return new Node<T> (data,
		 				parseTree(in),
		 				parseTree(in));


}


template <class T>
void BTree<T>::deserialize (istream &in)
{
	deleteAll(root);

	root = parseTree (in);


}

void testMember ()
{
	BTree<int> t;

	t.add(10,"").add(12,"L").add(14,"R").add(15,"LR");	

	assert (t.member(12) == true);
	assert (t.member(18) == false);
	assert (t.member(15) == true);
}


int plusOne (const int& i)
{
	return i + 1;
}
bool count(const int& x){
	return true;
}
bool isEven(const int& x){
	return x%2==0;
}
int main () //some testing
{
	
	testMember ();

	BTree<int> t;

	t.add(10, "").add(12, "L").add(14, "R").add(15, "LR").add(23, "LRL").add(24, "RR").add(27, "RRR").add(28, "RRRR").add(50,"LRR").add(50,"LL").add(2000,"RRRL");
	t.simplePrint();
	
	t.map (plusOne);

	t.simplePrint ();
	cout << endl;
	cout << t.getElement("");
	cout << endl;
	cout << t.getElement("L");
	cout << endl;
	cout << t.getElement("RRRR");
	cout<<endl;
	vector<int> leaves=t.listLeaves();
	for(int i=0;i<leaves.size();i++)
		cout<<leaves[i]<<" ";
	cout<<endl;
	cout<<t.findTrace(28)<<endl;
	t.prettyPrint();
	t.dottyPrint(cerr);
	cout<<endl;
	t.schemePrint();
	system("pause");
	return 0;
}