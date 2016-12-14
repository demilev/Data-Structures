#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

void permutations(string symbols,string crrPermutation,vector<string> &allPermutations)
{
    if(symbols.size()==0)
    {
        allPermutations.push_back(crrPermutation);
        return;
    }
    for(int i=0;i<symbols.size();i++)
    {
        string nextPermutation=crrPermutation;
        nextPermutation.push_back(symbols[i]);
        string next = symbols;
        next.erase(i,1);
        permutations(next,nextPermutation,allPermutations);
    }

}

struct TraversePermutation{

    string builded;
    string remaining;
    TraversePermutation(string b,string r):builded(b),remaining(r){}
};

vector<string> permutations_stack(string symbols)
{
    vector<string> result;
    stack<TraversePermutation> permutations;
    TraversePermutation start(string(),symbols);
    permutations.push(start);

    while(!permutations.empty())
    {
        TraversePermutation topPermutation = permutations.top();
        permutations.pop();

        if(topPermutation.remaining.empty())
            result.push_back(topPermutation.builded);
        else
        {
            for(int i=0;i<topPermutation.remaining.size();i++)
            {
                string newBuilded = topPermutation.builded;
                newBuilded.push_back(topPermutation.remaining[i]);
                string newRemaining = topPermutation.remaining;
                newRemaining.erase(i,1);
                permutations.push(TraversePermutation(newBuilded,newRemaining));
            }
        }
    }
    return result;
}

int main()
{

    string symbols = "abc";
    vector<string> allPermutations = permutations_stack(symbols);
    for(int i=0;i<allPermutations.size();i++)
    {
        cout<<allPermutations[i]<<endl;
    }

}
