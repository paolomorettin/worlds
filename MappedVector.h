#pragma once
#include<vector>
#include<map>
#include<stdexcept>

using namespace std;

template <class T, class K>
class MappedVector
{
public:
	//set
	void addElement(T element, K key)
	{
		vector<K> keyVector;
		keyVector.push_back(key);
		addElement(element, keyVector);
	}
	void addElement(T element, vector<K> keyVector)
	{
		v.push_back(element);
		for(int i = 0; i < keyVector.size(); i++)
			m[keyVector[i]] = v.size() - 1;
	}

	//get
	bool exists(K key) const { return m.find(key) != m.end(); } //true if exists an element with this key. False otherwise
	T &getElement(K key)
	{
		if(exists(key))
			return v[m[key]];
		
		throw std::out_of_range("key not found");
	}
	const T &getElement(K key) const
	{
		if(exists(key))
			return v[m.find(key)->second];
		
		throw std::out_of_range("key not found");
	}
	unsigned int size() { return v.size(); }

	//operator
	T &operator[](K key)
	{
		return getElement(key);
	}
	const T &operator[](K key) const
	{
		return getElement(key);
	}

private:
	vector<T> v;
	map<K, int> m;
};
