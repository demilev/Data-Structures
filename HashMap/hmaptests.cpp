#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "hmap.cpp"

size_t stringhash1 (const string& s, size_t htsize)
{

	size_t x = 0;

	for (char c : s)
	{
		x += c;
	}

	return x % htsize;
}

void grow(double& d)
{
	d+=0.1;
}

string appendSpecialInfo(const string& s)
{
	string result = s;
	result.push_back(s[0]);
	return result;
}
void testHMapCore ()
{

	//m(size,hashfn)
	HashMap<string,double> m(7,stringhash1);

	m["Kalin"] = 1.84;
	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;

	assert (m["Kalin"] == 1.85);
	assert (m["Ivan"] == 1.86);

	assert (m.containsKey("Kalin") == true);
	assert (m.containsKey("Petar") == false);

}
void testEfficiency(){
	HashMap<string,double> m(7,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;
	assert(m.efficiency() == 1);
	m["Kilan"] = 1.90;
	assert(m.efficiency() == (1/3));
}
void testHMapCannonical ()
{
	HashMap<string,double> m(7,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;

	HashMap<string,double> m1(m);
	assert (m1["Kalin"] == 1.85);
	assert (m1["Ivan"] == 1.86);
	assert (m1.containsKey("Kalin") == true);
	assert (m1.containsKey("Petar") == false);

	m["Petar"] = 1.75;
	assert (m1.containsKey("Petar") == false);

	m1  = m;
	assert (m1.containsKey("Petar") == true);

	m["Zdravko"] = 1.79;
	assert (m1.containsKey("Zdravko") == false);
}
void testOperatorOut()
{
	HashMap<string,double> m(7,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;
	m["Kilan"] = 1.90;
	//ofstream out("test.txt");
	m.operator<<(cout);
}

void testHMapIterator ()
{

	HashMap<string,double> m(7,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;

	int count = 0;
	for (string key : m)
	{
		assert (key == "Kalin" || key == "Ivan");
		cout << "key:" << count << "=" << key << endl;
		count++;
	}

	assert (count == 2);
}

void testHMapResize ()
{
	HashMap<string,double> m(1,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;

	m.resize (7);

	assert (m["Kalin"] == 1.85);
	assert (m["Ivan"] == 1.86);
	assert (m.containsKey("Kalin") == true);
	assert (m.containsKey("Petar") == false);

}

void testHMapMapFunction()
{
	HashMap<string,double> m(1,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;

	m.map(grow);
	
	assert (m["Kalin"] == (1.85+0.1));
	assert (m["Ivan"] == (1.86+0.1));
}

void testHMapMapKeyFunction()
{
	HashMap<string,double> m(1,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;

	m.mapKeys(appendSpecialInfo);

	assert (m["KalinK"] == 1.85);
	assert (m["IvanI"] == 1.86);
}
void testHMapWeirdIterator()
{
	HashMap<string,double> m(7,stringhash1);

	m["Kalin"] = 1.85;
	m["Ivan"] = 1.86;

	int count = 0;
	for (string key : m)
	{
		assert (key == "Kalin"||key == string());
		cout << "key:" << count << "=" << key << endl;
		count++;
	}

	assert (count == 2);
}
int main ()
{
	testHMapCore();
	testHMapCannonical();
	testEfficiency();
	testOperatorOut();
	testHMapIterator();
	testHMapResize();
	testHMapMapFunction();
	testHMapMapKeyFunction();
}