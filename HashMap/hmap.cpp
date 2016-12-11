#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>

using namespace std;

template <class KeyType>
bool identity(const KeyType& key)
{
	return true;
}

bool startsWithK(const string& key)
{
	return key[0] == 'k' || key[0] == 'K';
}

template <class KeyType>
using hashFnType = size_t (*) (const KeyType&, 
	     							 size_t);
template <class KeyType>
using predicateFunction = bool (*) (const KeyType&);

template <class KeyType, class ValueType>
class HashMap
{
public:
	struct KeyValue
	{
		KeyType key;
		ValueType value;
		KeyValue (const KeyType& _k, const ValueType& _v):key(_k),value(_v){}
	};

	//итератор за итериране по ключовете
	class Iterator
	{
		public:
		
		Iterator (const vector<list<KeyValue>> &_table, bool=false, predicateFunction<KeyType> p = identity);

		KeyType operator * ();
		Iterator& operator ++ ();
		bool operator != (typename HashMap<KeyType,ValueType>::Iterator &);

		private:

		void goToActualEntry();
		
		typename vector<list<KeyValue>>::const_iterator currentTableEntry;
		typename list<KeyValue>::const_iterator currentCollision;
		const vector<list<KeyValue>> &table;
		predicateFunction<KeyType> predicate;

	};


	Iterator begin() const;
	Iterator end() const;

	HashMap (size_t size, hashFnType<KeyType> f);

	ValueType& operator [] (const KeyType&);
	ValueType operator [] (const KeyType&) const;
	bool containsKey (const KeyType&) const;
	double efficiency() const;
	ostream& operator<<(ostream&) const;

	void resize (size_t size);
	size_t size () const;
	hashFnType<KeyType> getHashFunction () const;

	void map(void (*) (ValueType&));
	void mapKeys(KeyType (*) (const KeyType&));
private:
	hashFnType<KeyType> hashFunction;
	vector<list<KeyValue>> table;

};


template <class KeyType, class ValueType>
using HMIterator 
   = typename HashMap<KeyType,ValueType>::Iterator;

template <class KeyType, class ValueType>
void HashMap<KeyType,ValueType>::mapKeys(KeyType (*f) (const KeyType&))
{
	vector<list<KeyValue>> newTable;
	newTable.assign (table.size(),list<KeyValue>());
	for(KeyType key : *this)
	{
		newTable[hashFunction(f(key),table.size())].
			push_back(KeyValue(f(key),(*this)[key]));
	}
	table = newTable;
}
template <class KeyType, class ValueType>
void HashMap<KeyType,ValueType>::map(void (*f) (ValueType&))
{
	for(KeyType key : *this)
	{
		f((*this)[key]); 
	}
}
template <class KeyType, class ValueType>
hashFnType<KeyType> HashMap<KeyType,ValueType>::getHashFunction () const
{
	return hashFunction;
}

template <class KeyType, class ValueType>
size_t HashMap<KeyType,ValueType>::size () const
{
	return table.size();
}

template <class KeyType, class ValueType>
void HashMap<KeyType,ValueType>::resize (size_t size)
{
	assert (size > 0);
	if (size == table.size())
		return;

	vector<list<KeyValue>> newTable;
	newTable.assign (size,list<KeyValue>());

	for (KeyType key : *this)
	{
		newTable[hashFunction(key,size)]
		   .push_back (KeyValue(key,(*this)[key]));
	}

	table = newTable;
	//	std::move (table,newTable); или нещо подобно
}

template <class KeyType, class ValueType>
HashMap<KeyType,ValueType>::HashMap (size_t size, hashFnType<KeyType> f)
{
	hashFunction = f;
	table.assign (size,list<KeyValue>());
}

template <class KeyType, class ValueType>
ValueType& HashMap<KeyType,ValueType>::operator [] (const KeyType &key)
{
	//hashTable
	//hasFunction

	size_t index = hashFunction (key,table.size());

	for (KeyValue &pair : table[index])
	{
		if (pair.key == key)
			return pair.value;
	}

    table[index].push_back (KeyValue(key,ValueType()));
	return table[index].back().value;


}
template <class KeyType, class ValueType>
ValueType HashMap<KeyType,ValueType>::operator [] (const KeyType &key) const
{
	size_t index = hashFunction (key,table.size());

	for (KeyValue &pair : table[index])
	{
		if (pair.key == key)
			return pair.value;
	}
	
	assert (false);
    return ValueType();
}

template <class KeyType, class ValueType>
bool HashMap<KeyType,ValueType>::containsKey (const KeyType &key) const
{
	size_t index = hashFunction (key,table.size());

	for (const KeyValue &pair : table[index])
	{
		if (pair.key == key)
			return true;
	}

	return false;

}
template <class KeyType, class ValueType>
double HashMap<KeyType,ValueType>::efficiency () const
{
	int all=0;
	int coliding=0;
	for(int i=0;i<table.size();i++)
	{
		int crrBucketSize = table[i].size();
		all+=crrBucketSize;
		if(crrBucketSize>1)
			coliding+=crrBucketSize;
	}
	return (all-coliding) / all;
}
template <class KeyType, class ValueType>
ostream& HashMap<KeyType,ValueType>::operator<< (ostream& out) const
{
	for(int i=0;i<table.size();i++)
	{
		for(KeyValue pair : table[i])
		{
			out<<"Key: "<<pair.key<<", Value: "<<pair.value;	
			out<<"\n";
		}
			
	}

	return out;
}


template <class KeyType, class ValueType>
HashMap<KeyType,ValueType>::Iterator::Iterator 
     (const vector<list<typename HashMap<KeyType,ValueType>::KeyValue>> &_table,
     	bool goToEnd, predicateFunction<KeyType> p)
       :table(_table),predicate(p)
{
	currentTableEntry = _table.begin();
	currentCollision = (*currentTableEntry).begin();
	
	if (goToEnd)
	{
		currentTableEntry = table.end();
	} else 
	{

		goToActualEntry();

	}
}

template <class KeyType, class ValueType>
void HashMap<KeyType,ValueType>::Iterator::goToActualEntry()
{

	/* Кога спираме:
		1. currentTableEntry == table.end() 
		  (няма къде повече да търсим)
		2. currentCollision != (*currentTableEntry).end() 
		  (сме намерили каквото търсим)
	*/


	while (currentTableEntry != table.end() &&
		   currentCollision == (*currentTableEntry).end())
	{
		++currentTableEntry;
		if (currentTableEntry != table.end())
			currentCollision = (*currentTableEntry).begin();

	}

}


template <class KeyType, class ValueType>
KeyType HashMap<KeyType,ValueType>::Iterator::operator* ()
{
	assert (currentTableEntry != table.end());

	while(predicate((*currentCollision).key) == false)
	{	

		assert (currentTableEntry != table.end());
		++(*this);
		if (currentTableEntry == table.end());
			return KeyType();
	}

	return (*currentCollision).key;

}


template <class KeyType, class ValueType>
typename HashMap<KeyType,ValueType>::Iterator& 
  HashMap<KeyType,ValueType>::Iterator::operator++ ()
{
	++currentCollision;
	goToActualEntry();
	return *this;
}


template <class KeyType, class ValueType>
bool HashMap<KeyType,ValueType>::Iterator::operator!= 
    (HashMap<KeyType,ValueType>::Iterator &other)
{
		

	if (currentTableEntry != other.currentTableEntry)
		return true;

	if (currentTableEntry == table.end())
	   return false;

	return currentCollision != other.currentCollision;


/*	return (currentTableEntry == table.end()) !=
	       (other.currentTableEntry == other.table.end());

*/
}

template <class KeyType, class ValueType>
typename HashMap<KeyType,ValueType>::Iterator
   HashMap<KeyType,ValueType>::begin() const
{
	return HashMap<KeyType,ValueType>::Iterator (table);
}


template <class KeyType, class ValueType>
typename HashMap<KeyType,ValueType>::Iterator
   HashMap<KeyType,ValueType>::end() const
{
  return HashMap<KeyType,ValueType>::Iterator (table,true);

}


