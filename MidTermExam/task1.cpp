#include <iostream>
#include <queue>
#include <assert.h>
#include <vector>
#include <cstring>
#include <string>
using namespace std;

string toString(vector<string> v)
{
    string result;
    for(int i=0;i<v.size();i++)
        result+=v[i];
    return result;
}

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
        if(elem.compare(subTreeRoot->data)>0){
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

    bool member(Node<T>* subTreeRoot,const T& element){
    	if(subTreeRoot==NULL)
    		return false;
    	return subTreeRoot->data.compare(element)==0||
    		   member(subTreeRoot->left,element)||
    		   member(subTreeRoot->right,element);
    }
    vector<T> level(Node<T>* subTreeRoot,int k){
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

    bool member(const T& element){
    	return member(root,element);
    }
    vector<T> level(int k){
        return level(root,k);
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
    bool sameSentencesAtEachLevel()
    {
        vector<vector<string>> levels = allLevels();
        string rootSentence=toString(levels[0]);
        for(int i=1;i<levels.size();i++)
        {
            if(rootSentence.compare(toString(levels[i])) != 0)
                return false;
        }
        return true;
    }
};

int main(){

    BSTree<string> t;
    t.add("histudents");
    t.add("hi");
    t.add("students");

    vector<vector<string>> a=t.allLevels();
    for(int i=0;i<a.size();i++)
    {
        cout<<toString(a[i])<<endl;
    }

    cout<<t.sameSentencesAtEachLevel()<<endl;

    return 0;


}
