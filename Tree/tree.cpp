#include <iostream>
#include <algorithm>
#include <list>
using namespace std;
template<typename T>
struct Node{
    T data;
    list<Node<T>*> children;
    Node(T d):data(d),children(){}
    Node(T d,list<Node<T>*> c):data(d),children(c){}
    void push(Node<T>* child){
        children.push_back(child);
    }
};
template<typename T>
class Tree{
    Node<T>* root;
    void schemePrint(Node<T>* subTreeRoot)const{
        cout<<"(";
        cout<<subTreeRoot->data<<" ";
        typename list<Node<T>*>::iterator iterator;
        for (iterator = subTreeRoot->children.begin(); iterator != subTreeRoot->children.end();iterator++) {
             schemePrint(*iterator);
        }
        cout<<")";
    }
public:
    Tree(){
        root=NULL;
    }
    void addChild(Node<T>* child){
        if(root==NULL)
            root=child;
        else
            root->push(child);
    }
    Node<T>*& start(){
        if(root!=NULL){
            typename list<Node<T>*>::iterator iterator;
            iterator=root->children.begin();
            return iterator;
        }
        else
            return NULL;

    }
    Node<T>*& end(){
        if(root==NULL)
            return NULL;
        else{
            typename list<Node<T>*>::iterator iterator;
            iterator=root->children.end();
            return iterator;
        }
    }
    T rootData(){
        return root->data;
    }
    void schemePrint()const{
        schemePrint(root);
    }
    template<typename F>
    friend ostream& operator<<(ostream&,const Tree<F>&);
};
template<typename T>
ostream& operator<<(ostream& out,const Tree<T>& tree){
    tree.schemePrint(tree.root);
    return out;
}

int main(){
    Node<int> node(5);
    Node<int> node1(6);
    Node<int> node2(12);
    Node<int> node3(2);
    Node<int> node4(3);
    node1.push(&node3);
    node4.push(&node1);
    node.push(&node4);
    node.push(&node2);
    Tree<int> tree;
    tree.addChild(&node);
    cout<<tree;
    return 0;
}
