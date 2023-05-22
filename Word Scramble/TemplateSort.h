#pragma once
#include<iostream>
using namespace std;
template <class T>
class TemplateSort
{
	T* temp;
	int size = 100;
	int count = 0;
public:
	TemplateSort()
	{
		this->size = 0;
		this->count = 0;
		this->temp = nullptr;
	}
	TemplateSort(int size)
	{
		this->size = size;
		this->temp = new T[this->size];
	}
	TemplateSort(int size, string str)
	{
		this->size = size;
		this->temp = new T[this->size]{ str };
	}
	TemplateSort(const TemplateSort& TS)
	{
		this->size = TS.size;
		this->count = TS.count;
		this->temp = new T[this->size];
		for (int i = 0; i < this->size; i++)
			this->temp[i] = TS.temp[i];
	}
	const TemplateSort& operator=(const TemplateSort& TS)
	{
		this->size = TS.size;
		this->count = TS.count;
		this->temp = new T[this->size];
		for (int i = 0; i < this->size; i++)
			this->temp[i] = TS.temp[i];
		return *this;
	}
	void add(T n)
	{
		if (count == size)
		{
			T* HA = new T[size * 2];
			for (int i = 0; i < count; i++)
				HA[i] = this->temp[i];
			size *= 2;
			delete[] this->temp;
			this->temp = HA;
		}

		this->temp[count] = n;
		count++;
	}
	void print()
	{
		for (int i = 0; i < size; i++)
			cout << '"' << this->temp[i] << '"' << "\n";
	}
	int linearSearch(T n)
	{
		for (int i = 0; i < count; i++)
		{
			if (n == this->temp[i])
				return i;
		}
		return -1;
	}
	void del(T n)
	{
		int ind = linearSearch(n);
		if (ind != -1)
		{
			for (int i = ind; i < count; i++)
				this->temp[i] = this->temp[i + 1];
			count--;
		}
	}
	T operator[](int n)
	{
		return this->temp[n];
	}
	T* returnTemp()
	{
		return this->temp;
	}
	int returnCount()
	{
		return this->count;
	}
	int returnSize()
	{
		return this->size;
	}
	~TemplateSort()
	{
		delete[] this->temp;
	}
};


