#include <iostream>
#include <string>
#include <map>
using namespace std;

string mostCommonWord(string text)
{
	map<string,int> occurences;
	string word;
	for(int i=0;i<text.size();i++)
	{
		if(text[i]!=' ')
		{
			word.push_back(text[i]);
		}
		else if (word.size() > 3)
		{	
			if(occurences.find(word) != occurences.end())
			{
				occurences[word]+=1;
			}
			else
			{
				occurences[word]=0;
			}
			string empty;
			word=empty;
		}
		else
		{
			string empty;
			word=empty;	
		}
	}
	string maxWord;
	int maxOccurences=0;
	typedef map<string,int>::iterator it_type;
	for(it_type iterator = occurences.begin(); iterator != occurences.end(); iterator++)
	{
		if(iterator->second > maxOccurences)
		{
			maxOccurences = iterator->second;
			maxWord = iterator->first;
		}
	}
	return maxWord;
}

int main()
{
	string text="In computing a hash table hash map is a data structure used to implement an associative array a structure that can map keys to values A hash table uses a hash function to compute an index into an array of buckets or slots from which the correct value can be found";
	cout<<mostCommonWord(text)<<endl;
}