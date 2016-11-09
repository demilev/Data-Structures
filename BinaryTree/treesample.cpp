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
	void schemePrint(Node<T>*)const;
	static Node<T>* copyTree (const Node<T>*);
	void insertBOT (Node<T>*&,const T&);
	Node<T>* insertedBOT (Node<T>*, const T& );
	void deleteElement (Node<T>*&, const T&);
	T minelement (Node<T>*) const;
	vector<T> level(Node<T>*,int)const;
	static Node<T>* deleted(Node<T>*,const T&);
	bool isBOT(Node<T>*)const;
public:
	BTree();
	BTree(Node<T>*);
	BTree (const BTree<T> &);
	BTree<T>& operator = (const BTree<T> &);
	BTree<T>& add (const T& , const char *);
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
	T& getElement(const char *) const;
	void serialize (ostream&)const;
	void deserialize (istream&);
	vector<T> listLeaves() const;
	~BTree();
	string findTrace(const T&) const;
	BTree<T>& insertBOT (const T&);
	BTree<T> insertedBOT (const T&);
	void deleteElement (const T&);
	T minelement ()const;
	vector<T> level(int)const;
	BTree<T> deletedBot(const T&)const;
	bool isEmpty()const;
	bool isBOT()const;

};
template<class T>
bool BTree<T>::isBOT(Node<T>* subTreeRoot)const{
	if(subTreeRoot==NULL)
		return true;
	if(subTreeRoot->left==NULL&&subTreeRoot->right==NULL)
		return true;
	if(subTreeRoot->left==NULL)
		return subTreeRoot->data < subTreeRoot->right->data&&
			   isBOT(subTreeRoot->right);
	if(subTreeRoot->right==NULL)
		return subTreeRoot->data >= subTreeRoot->left->data&&
	   		   isBOT(subTreeRoot->left);
	
	return subTreeRoot->data < subTreeRoot->right->data&&
		   subTreeRoot->data >= subTreeRoot->left->data&&
		   isBOT(subTreeRoot->left)&&
		   isBOT(subTreeRoot->right);

}
template<class T>
bool BTree<T>::isBOT()const{
	return isBOT(root);
}
template<class T>
bool BTree<T>::isEmpty()const{
	return root==NULL;
}
template<class T>
BTree<T>::BTree(Node<T>* subTreeRoot){
    root=copyTree(subTreeRoot);
}
template<class T>
Node<T>* BTree<T>::deleted(Node<T>* subTreeRoot,const T& x){
	if(subTreeRoot==NULL)
		return NULL;
	if(subTreeRoot->data==x&&
	   subTreeRoot->left==NULL&&
	   subTreeRoot->right==NULL)
	{
		return NULL;
	}
	if (x < subTreeRoot->data)
	{
		return new Node<T> (subTreeRoot->data,
							deleted(subTreeRoot->left,x),
							copyTree(subTreeRoot->right));
	}


	if (x > subTreeRoot->data)
	{

		return new Node<T> (subTreeRoot->data,
							copyTree(subTreeRoot->left),
							deleted(subTreeRoot->right,x));
	}
	if (subTreeRoot->right == NULL)
	{
		return subTreeRoot->left;
	}

	if (subTreeRoot->left == NULL)
	{
		return subTreeRoot->right;
	}
	if(subTreeRoot->data==x&&
	   subTreeRoot->left!=NULL&&
	   subTreeRoot->right!=NULL
		)
	{
		BTree<T> temp(subTreeRoot->right);
		T minel = temp.minelement(subTreeRoot->right);
		return new Node<T> (minel,
						copyTree(subTreeRoot->left),
						deleted(subTreeRoot->right,minel));
	}
}
template<class T>
BTree<T> BTree<T>::deletedBot(const T& x)const{
	BTree<T> result(deleted(root,x));
	//result.root=deleted(root,x);
	return result;
}
template<class T>
vector<T> BTree<T>::level (Node<T>* subTreeRoot,int k)const{
	if(subTreeRoot==NULL){
		vector<T> result;
		return result;
	}
	if(k==1){
		vector<T> result;
		result.push_back(subTreeRoot->data);
		return result;
	}
	vector<T> leftResult=level(subTreeRoot->left,k-1);
	vector<T> rightResult=level(subTreeRoot->right,k-1);
	for(int i=0;i<rightResult.size();i++)
		leftResult.push_back(rightResult[i]);
	return leftResult;

}
template<class T>
vector<T> BTree<T>::level (int k)const{
	return level(root,k);
}
template<class T>
void BTree<T>::deleteElement (Node<T> *&subTreeRoot, const T&x)
{
	//триене от празно дърво
	if (subTreeRoot==NULL)
		return;

	//триене от листо
	if (subTreeRoot->data == x &&
		subTreeRoot->left == NULL &&
		subTreeRoot->right == NULL)

	{
		delete subTreeRoot;
		subTreeRoot = NULL;
		return;
	}

	//триене от лявото поддърво
	if (x < subTreeRoot->data)
	{
		deleteElement (subTreeRoot->left,x);
		return;
	}

	//триене от дясното поддърво
	if (x > subTreeRoot->data)
	{
		deleteElement (subTreeRoot->right,x);
		return;
	}

	//вече сме сигурни, че трием корена!
	//освен това сме сигурни, че корена има ПОНЕ ЕДИН
	//наследник

	//триене на корен само с 1 наследник
	if (subTreeRoot->right == NULL)
	{
		Node<T> *tmp = subTreeRoot;
		subTreeRoot = subTreeRoot->left;
		delete tmp;
		return;
	}

	//триене на корен само с 1 наследник
	//този случй може да не се разглежда
	if (subTreeRoot->left == NULL)
	{
		Node<T> *tmp = subTreeRoot;
		subTreeRoot = subTreeRoot->right;
		delete tmp;
		return;
	}


	//триене на корена
	//вече сме сигурни, че корена има точно два наследника
	T minel = minelement(subTreeRoot->right);
	subTreeRoot->data = minel;
	deleteElement (subTreeRoot->right, minel);


}

template<class T>
void BTree<T>::deleteElement (const T&x)
{
	deleteElement (root,x);
}

template<class T>
T BTree<T>::minelement (Node<T> *subTreeRoot) const
{
	assert (subTreeRoot != NULL);
	Node<T> *current = subTreeRoot;

	while (current->left != NULL)
	{
		current = current->left;
	}

	return current->data;

}


template<class T>
T BTree<T>::minelement () const
{
	return minelement (root);

}

template<class T>
BTree<T> BTree<T>::insertedBOT (const T& x)
{
	BTree<T> result;
	result.root = insertedBOT (root,x);

	return result;
}


template<class T>
Node<T>* BTree<T>::insertedBOT (Node<T>*subTreeRoot, const T& x)
{
	if (subTreeRoot == NULL)
	{
		return new Node<T> (x,NULL,NULL);
	}

	if (x > subTreeRoot->data)
	{
		return new Node<T> (subTreeRoot->data,
							copyTree(subTreeRoot->left),
							insertedBOT(subTreeRoot->right,x));

	}

	return new Node<T> (subTreeRoot->data,
						insertedBOT(subTreeRoot->left,x),
						copyTree(subTreeRoot->right));

}


template<class T>
void BTree<T>::insertBOT (Node<T>* &subTreeRoot,const T& x)
{

	if (subTreeRoot == NULL)
	{
		subTreeRoot = new Node<T> (x,NULL,NULL);
		return;
	}

	if (x <= subTreeRoot->data)
	{
		insertBOT (subTreeRoot->left,x);
	} else {
		insertBOT (subTreeRoot->right,x);
	}

}


template<class T>
BTree<T>& BTree<T>::insertBOT (const T& x)
{
	insertBOT (root,x);
	return *this;
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

	t.insertBOT(10).insertBOT(23).insertBOT(18).insertBOT(4).insertBOT(32).insertBOT(29).insertBOT(7).insertBOT(17).insertBOT(40).insertBOT(2).add(11,"LLL");


	//t.prettyPrint();
	t.dottyPrint(cerr);
	BTree<int> t1=t.deletedBot(23);
	t1.prettyPrint();
	t.simplePrint();
	cout<<t.isBOT()<<endl;
	system("pause");
	return 0;
}
