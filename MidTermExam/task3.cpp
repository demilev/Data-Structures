#include<iostream>
#include<string>
#include<stack>
using namespace std;

string shuntingYard(string expression) {
    expression = "(" + expression + ")";
    string result;
    stack<char> hs;
    for (int i = 0; i < expression.size(); i++) {
        switch(expression[i]) {
        case '(':
            hs.push('(');
            break;
        case ')':
            while(hs.top() != '(') {
                char crr = hs.top();
                hs.pop();
                result.push_back(crr);
            }
            hs.pop();
            break;
        case '$':
            hs.push(expression[i]);
            break;
        case '@':
            hs.push(expression[i]);
            break;
        default:
            result.push_back(expression[i]);
            break;
        }
    }

    return result;
}
int min(int a,int b)
{
    if(a<b)
        return a;
    return b;
}
int calculate(char*& expr)
{

    if((*expr)=='@')
        return (calculate(--expr)+calculate(--expr))%10;

    if((*expr)=='$')
        return min(calculate(--expr),calculate(--expr));
    if((*expr)>='0'&&(*expr)<='9')
        return ((*expr)-'0');

}
int main()
{
    string expr = shuntingYard("((1@(6@2))$3)@5");
    char *e=&expr[expr.length()-1];
    cout<<calculate(e)<<endl;
}
