#include <iostream>
#include <cstring>
#include <string>
#include <stack>
using namespace std;

string fullExp(char* exp)
{
    int size = strlen(exp);
    stack<string> operations;
    stack<string> result;

    for(int i=0;i<size;i++)
    {
        if(exp[i]>='0'&&exp[i]<='9')
        {
            string crrDigit;
            crrDigit.push_back(exp[i]);
            result.push(crrDigit);
        }
        if(exp[i]=='+'||
           exp[i]=='-'||
           exp[i]=='*'||
           exp[i]=='/')
        {
            string crrOp;
            crrOp.push_back(exp[i]);
            operations.push(crrOp);
        }
        if(exp[i]==')')
        {
            string secondOperand = result.top();
            result.pop();

            string firstOperand = result.top();
            result.pop();

            string operation = operations.top();
            operations.pop();

            string newExp = "("+firstOperand + operation + secondOperand +")";
            result.push(newExp);
        }
    }
    return result.top();
}
int main()
{
    char exp[]="1+2)*3-4)*5+6)))";
    cout<<fullExp(exp)<<endl;
}
