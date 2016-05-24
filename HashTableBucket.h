#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <class T,class V>
struct HashTableNode
{
	HashTableNode(const T& key,const V& value)
		:_key(key)
		,_value(value)
		,_next(NULL)
	{}
	T _key;
	V _value;
	HashTableNode<T,V>* _next;
};

template <class T>
struct __HashFunc
{
	size_t operator()(const T& key)
	{
		return key;
	}
};

//template <>
//struct __HashFunc<string>
//{
//	size_t operator()(const string& key)
//	{
//		size_t hash = 0;
//		for(size_t i=0;i<key.size();++i)
//		{
//			hash += key[i];
//		}
//		return hash;
//	}
//};

template <>
struct __HashFunc<string>
{
	//size_t BKDRHash(const char * str)
	size_t operator()(const string& key)
	{
		char * str = (char *)key.c_str();
		unsigned int seed = 131; // 31 131 1313 13131 131313
		unsigned int hash = 0;
		while (*str )
		{
			hash = hash * seed + (* str++);
		}
		return (hash & 0x7FFFFFFF);
	}
};


template <class T,class V,class HashFunc = __HashFunc<T>>
class HashTableBucket   //哈希桶
{
	typedef HashTableNode<T,V> Node;
	typedef HashTableBucket<T,V,HashFunc> Table;
public:
	//构造
	HashTableBucket()
		:_table(NULL)
		,_size(0)
	{}
	HashTableBucket(size_t capacity)
		:_size(0)
	{
		_table.resize(_CheckCapacity(capacity));
	}
	//析构
	~HashTableBucket()
	{
		for(size_t i=0;i<_table.size();++i)
		{
			Node* cur = _table[i];
			while(cur)
			{
				Node* del = cur;
				cur = cur->_next;
				delete del;
			}
			_table[i] = NULL;
		}
	}
	//拷贝
	HashTableBucket(const Table& ht)
		:_size(0)
	{
		_table.resize(ht._table.size()); //开辟空间
		for(size_t i=0;i<ht._table.size();++i)
		{
			Node* cur = ht._table[i];
			while(cur)
			{
				Insert(cur->_key,cur->_value);
				cur = cur->_next;
			}
		}
	}
	//赋值
	/*HashTableBucket<T,V>& operator=(HashTableBucket<T,V> ht)
	{
		_table.swap(ht._table);
		swap(_size,ht._size);
		return *this;
	}*/

	Table& operator=(Table& ht)
	{
		if(this != &ht)
		{
			Table tmp(ht);
			_table.swap(tmp._table);
			swap(_size,tmp._size);
		}
		return *this;
	}
public:
	bool Insert(const T& key,const V& value)
	{
		_CheckCapacity();//检查容量
		size_t index = _HashFunc(key,_table.size());
		Node* cur = _table[index];
		while(cur)
		{
			if(cur->_key == key)  //防冗余
			{
				return false;
			}
			cur = cur->_next;
		}
		//头插
		Node* newNode =new Node(key,value);
		newNode->_next = _table[index];
		_table[index] = newNode;
		++_size;
		return true;
	}
	Node* Find(const T& key)
	{
		size_t index = _HashFunc(key,_table.size());
		Node* cur = _table[index];
		while(cur)
		{
			if(cur->_key == key)
			{
				return cur;
			}
			cur = cur->_next;
		}
		return NULL;
	}
	bool Remove(const T& key)
	{
		size_t index = _HashFunc(key,_table.size());
		Node* cur = _table[index];
		Node* prev = NULL;
		Node* del = NULL;
		if(cur->_key == key)
		{
			del = cur;
			_table[index] = cur->_next;
			delete del;
			return true;
		}	
		prev = cur;
		cur = cur->_next;
		while(cur)
		{
			if(cur->_key == key)
			{
				del = cur;
				prev->_next = cur->_next;
				delete del;
				return true;
			}
			prev = cur;
			cur = cur->_next;
		}
		return false;
	}
	void Print()
	{
		for(size_t i=0;i<_table.size();++i)
		{
			printf("_table[%d]:",i);
			Node* cur = _table[i];
			while(cur)
			{
				cout<<"["<<cur->_key<<","<<cur->_value<<"]"<<"->";
				cur = cur->_next;
			}
			cout<<"NULL"<<endl;
		}	
	}
protected:
	size_t _HashFunc(const T& key,size_t capacity) //哈希函数
	{
		//return key%capacity;
		HashFunc hash;
		return hash(key)%capacity;
	}
	size_t _GetNextPrime(const size_t size)  //获取下一个素数
	{
		// 使用素数表对齐做哈希表的容量，降低哈希冲突
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList [_PrimeSize] =
		{
			53ul,         97ul,         193ul,       389ul,       769ul,

			1543ul,       3079ul,       6151ul,      12289ul,     24593ul,

			49157ul,      98317ul,      196613ul,    393241ul,    786433ul,

			1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,

			50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,

			1610612741ul, 3221225473ul, 4294967291ul
		};
		for(size_t i=0;i<_PrimeSize;++i)
		{
			if(_PrimeList[i] > size)
			{
				return _PrimeList[i];
			}
		}
		return _PrimeList[_PrimeSize-1];
	}

	void _CheckCapacity()
	{
		if(_size == _table.size())
		{
			size_t nextPrime = _GetNextPrime(_size);
			vector<Node*> newtable;
			newtable.resize(nextPrime);
			for(size_t i=0;i<_table.size();++i)
			{
				Node* cur = _table[i];
				while(cur)
				{
					//摘节点
					Node* tmp = cur;
					cur = cur->_next;

					//计算在新表中的位置，头插
					size_t index = _HashFunc(tmp->_key,nextPrime);
					newtable[index] = tmp;
				}
				_table[i] = NULL;
			}
			_table.swap(newtable); //size，capacity，内容
		}
	}
private:
	vector<Node*> _table;  //哈希表
	size_t _size;          //数据的个数
};

