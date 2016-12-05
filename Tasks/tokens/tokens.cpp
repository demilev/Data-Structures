#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
using namespace std;

#define FILE1 "first.txt"
#define FILE2 "second.txt"
#define FILE3 "third.txt"

unordered_map<string,int> fileToMap(string file)
{
	unordered_map<string,int> result;
	ifstream in(file);
	char word[10000];
	while(!in.eof())
	{
		in>>word;
		char *crrToken=strtok(word,".,!?\t :");
    	while(crrToken != NULL)
    	{	
    		string crr(crrToken);
    		result[crr]=1;
    		crrToken=strtok(NULL,".,!?\t :");
		}	
	}
	in.close();
	return result;
}
bool fileHave(char *word,unordered_map<string,int> file)
{
	string crrWord(word);
	return (file.find(crrWord) != file.end());
}
vector<string> unionOfFIles(char* text)
{	
	vector<string> result;
    unordered_map<string,int> firstFile = fileToMap(FILE1);
    unordered_map<string,int> secondFile = fileToMap(FILE2);
    unordered_map<string,int> thirdFile = fileToMap(FILE3);
    bool inFirst=false;
    bool inSecond = false;
    bool inThird = false;
    char *crrToken=strtok(text,".,!?\t :");
    while(crrToken != NULL)
    {	

    	if(inFirst&&inSecond&&inThird)
    	{
    		return result;
    	}
    	if(!inFirst)
    	{
    		if(fileHave(crrToken,firstFile))
    		{	
    			inFirst=true;
    			result.push_back("first.txt");
    		}
    	}

    	if(!inSecond)
    	{
    		if(fileHave(crrToken,secondFile))
    		{	
    			inSecond=true;
    			result.push_back("second.txt");
    		}
    	}

    	if(!inThird)
    	{
    		if(fileHave(crrToken,thirdFile))
    		{	
    			inThird=true;
    			result.push_back("third.txt");
    		}
    	}
    	crrToken=strtok(NULL,".,!?\t :");
    }
    return result;
}



int main()
{	
	char str[]="zz,aaa bbb";
	vector<string> filesUnion=unionOfFIles(str);
	for(int i=0;i<filesUnion.size();i++)
	{
		cout<<filesUnion[i]<<endl;
	}
	return 0;
}
