#include <iostream>
#include <algorithm>
#include <list>
#include <fstream>
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
    int maxNumberOfChilds(Node<T>* subTreeRoot,int currentMax){

        int current=subTreeRoot->children.size();
        if(current>currentMax)
            currentMax=current;
        typename list<Node<T>*>::iterator iterator;
        for (iterator = subTreeRoot->children.begin(); iterator != subTreeRoot->children.end();iterator++) {
             int crr=maxNumberOfChilds((*iterator),currentMax);
             if(crr>currentMax)
                currentMax=crr;
        }
        return currentMax;
    }
    int height(Node<T>* subTreeRoot, int currentHeight, int maxHeight){
        if(subTreeRoot==NULL)
            return currentHeight;
        if(currentHeight>maxHeight)
            maxHeight=currentHeight;
        typename list<Node<T>*>::iterator iterator;
        for (iterator = subTreeRoot->children.begin(); iterator != subTreeRoot->children.end();iterator++) {
             int crr=height((*iterator),1+currentHeight,maxHeight);
             if(crr>maxHeight)
                maxHeight=crr;
        }
        return maxHeight;

    }

    bool member(const T& element, Node<T>* subTreeRoot){
        if(subTreeRoot==NULL)
            return false;
        if(subTreeRoot->data==element)
            return true;
        typename list<Node<T>*>::iterator iterator;
        for (iterator = subTreeRoot->children.begin(); iterator != subTreeRoot->children.end();iterator++) {
             if(member(element,(*iterator)))
                return true;
        }
        return false;
    }
    static Node*<int> parseTree(Node<T>* subTreeRoot, ifstream& in){
        assert(in.peek()=='(');
        in.get(); //махаме скобата
        if(in.peek()==')'){
            in.get();
            return NULL;
`       }
        int data;
        in>>data;
        Node<int> crr=new Node<int>(data,NULL);
        if(in.peek()==','){

            return crr;
        }



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
    int maxNumberOfChilds(){
        return maxNumberOfChilds(root,0);
    }
    int height(){
        return height(root,0,0);
    }
    bool member(const T& element){
        return member(element,root);
    }
    static Tree<int> deserializeIntTree(ifstream& in){
        Tree result();
        result.addChild(parseTree(in,root))
        return result;
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
    Node<int> node5(1);
    Node<int> node6(19);
    Node<int> node7(123);
    Node<int> node8(1234);
    Node<int> node9(1235);
    Node<int> node10(1236);
    Node<int> node11(1238);
    node3.push(&node7);
    node1.push(&node3);
    node4.push(&node1);
    node4.push(&node5);
    node4.push(&node6);
    node.push(&node4);

    node9.push(&node10);
    node8.push(&node9);
    node11.push(&node8);
    node2.push(&node11);

    node.push(&node2);
    Tree<int> tree;
    tree.addChild(&node);
    cout<<tree;
    cout<<endl;
    cout<<tree.maxNumberOfChilds();
    cout<<endl;
    cout<<tree.height();
    cout<<endl;
    cout<<tree.member(1)<<endl;
    cout<<tree.member(12)<<endl;
    cout<<tree.member(1236)<<endl;
    cout<<tree.member(444)<<endl;

    // to do (5 ((9 ()),(1 ((4 ()),(12 ()),(42 ()))))) - - - - > 5->(9,1->(4,12,42)) + най-скъп път

    return 0;
}
