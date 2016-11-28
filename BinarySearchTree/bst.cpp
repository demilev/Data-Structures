#include <iostream>
#include <queue>
#include <assert.h>
#include <vector>
using namespace std;

template<typename T>
struct Node{
    T data;
    Node<T>* left;
    Node<T>* right;
    Node(T d,Node<T>* l,Node<T>* r): data(d),left(l),right(r){}
};
template<typename T>
class BSTree{

    Node<T>* root;
    void add(const T& elem, Node<T>*& subTreeRoot){
        if(subTreeRoot==NULL){
            subTreeRoot=new Node<T>(elem,NULL,NULL);
            return;
        }
        if(elem>subTreeRoot->data){
            add(elem,subTreeRoot->right);
            return;
        }
        add(elem,subTreeRoot->left);
    }
    void deleteAll(Node<T>* subTreeRoot){
        if(subTreeRoot==NULL)
            return;
        Node<T>* left=subTreeRoot->left;
        Node<T>* right=subTreeRoot->right;
        delete subTreeRoot;
        deleteAll(left);
        deleteAll(right);
    }
    void print(Node<T>* subTreeRoot){
        if(subTreeRoot==NULL)
            return;
        print(subTreeRoot->left);
        cout<<subTreeRoot->data<<" ";
        print(subTreeRoot->right);
    }
    void deleteElem(Node<T>*& subTreeRoot,const T& elem){
        if(subTreeRoot==NULL)
            return;
        if(subTreeRoot->data==elem&&
           subTreeRoot->left==NULL&&
           subTreeRoot->right==NULL){

            delete subTreeRoot;
            subTreeRoot=NULL;
            return;
        }
        if(subTreeRoot->data==elem&&subTreeRoot->left==NULL){
            subTreeRoot->data=minElem(subTreeRoot->right);
            deleteElem(subTreeRoot->right,subTreeRoot->data);
            return;
        }
        if(subTreeRoot->data==elem&&subTreeRoot->right==NULL){
            subTreeRoot->data=maxElem(subTreeRoot->left);
            deleteElem(subTreeRoot->left,subTreeRoot->data);
            return;
        }
        if(subTreeRoot->data==elem&&
           subTreeRoot->left!=NULL&&
           subTreeRoot->right!=NULL){
            subTreeRoot->data=maxElem(subTreeRoot->left);
            deleteElem(subTreeRoot->left,subTreeRoot->data);
            return;
        }
        if(elem>subTreeRoot->data){
            deleteElem(subTreeRoot->right,elem);
            return;
        }
        else{
            deleteElem(subTreeRoot->left,elem);
            return;
        }
    }
    T minElem(Node<T>* subTreeRoot){
        if(subTreeRoot==NULL)
            assert(false);
        Node<T>* crr=subTreeRoot;
        while(crr->left!=NULL){
            crr=crr->left;
        }
        return crr->data;
    }
    T maxElem(Node<T>* subTreeRoot){
        if(subTreeRoot==NULL)
            assert(false);
        Node<T>* crr=subTreeRoot;
        while(crr->right!=NULL){
            crr=crr->right;
        }
        return crr->data;
    }
    bool member(Node<T>* subTreeRoot,const T& element){
    	if(subTreeRoot==NULL)
    		return false;
    	return subTreeRoot->data==element||
    		   member(subTreeRoot->left,element)||
    		   member(subTreeRoot->right,element);
    }
    void allPaths(Node<T>* subTreeRoot,vector<T>& crr,vector<vector<T>>& result){
    	crr.push_back(subTreeRoot->data);
    	if(subTreeRoot->left==NULL&&subTreeRoot->right==NULL){
    		result.push_back(crr);
    		return;
    	}
    	vector<T> leftCopy=crr;
    	vector<T> rightCopy=crr;
    	if(subTreeRoot->left!=NULL)
    		allPaths(subTreeRoot->left,leftCopy,result);
    	if(subTreeRoot->right!=NULL)
    		allPaths(subTreeRoot->right,rightCopy,result);
    }

public:
    BSTree(){
        root=NULL;

    }
    ~BSTree(){
        deleteAll(root);
        root=NULL;
    }
    BSTree<T>& add(const T& elem){
        add(elem,root);
        return *this;
    }

    void print(){
        print(root);
        cout<<endl;
    }
    void bfsPrint(){
        if(root==NULL)
            return;
        queue<Node<T>*> nodeHolder;
        nodeHolder.push(root);
        while(!nodeHolder.empty()){
            Node<T>* top=nodeHolder.front();
            nodeHolder.pop();
            cout<<top->data<<" ";
            if(top->left!=NULL)
                nodeHolder.push(top->left);
            if(top->right!=NULL)
                nodeHolder.push(top->right);
        }
        cout<<endl;
    }
    void deleteElem(const T& element){
        deleteElem(root,element);
    }
    bool member(const T& element){
    	return member(root,element);
    }
    vector<vector<T>> allPaths(){
    	vector<vector<T>> result;
    	vector<T> crr;
    	allPaths(root,crr,result);
    	return result;
    }

    vector<vector<T>> allLevels()
    {
        vector<vector<T>> result;
        queue<Node<T>*> crr;
        crr.push(root);
        while(!crr.empty())
        {
            vector<T> crrData;
            int size=crr.size();
            for(int i=0;i<size;i++)
            {
                Node<T>* crrNode=crr.front();
                crr.pop();
                crrData.push_back(crrNode->data);
                if(crrNode->left!=NULL)
                    crr.push(crrNode->left);
                if(crrNode->right!=NULL)
                    crr.push(crrNode->right);
            }
            result.push_back(crrData);
        }
        return result;
    }
};
int main(){

    BSTree<int> t;
    t.add(5);
    t.add(3);
    t.add(4);
    t.add(20);
    t.add(17);
    t.add(18);
    //t.bfsPrint();
   // t.deleteElem(5);
    //t.bfsPrint();
    vector<vector<int>> levels=t.allLevels();
    	for(int i=0;i<levels.size();i++){
    	for(int k=0;k<levels[i].size();k++)
    		cout<<levels[i][k]<<" ";
    	cout<<endl;
    }

}
