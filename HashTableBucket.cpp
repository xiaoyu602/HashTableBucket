#include "HashTableBucket.h"


void HashTableListTest()
{
	HashTableBucket<int,int> ht;
	for(size_t i=0;i<50;++i)
	{
		ht.Insert(i,i);
	}
	ht.Insert(107,32);
	ht.Insert(54,45);
	//ht.Insert(65,32);
	/*HashTableNode<int,int>* ret = ht.Find(1);
	if(ret)
	{
		cout<<"["<<ret->_key<<","<<ret->_value<<"]"<<endl;
	}*/
	//ht.Remove(54);
	ht.Remove(1);
	//ht.Print();
}

void HashTableTest()
{
	HashTableBucket<int,int> ht;
	ht.Insert(1,1);
	ht.Insert(11,11);
	ht.Insert(21,21);
	ht.Insert(12,12);
	ht.Insert(23,23);
	ht.Insert(54,57);
	ht.Insert(42,12);
	//ht.Print();
	HashTableBucket<int,int> ht1(ht);
	//ht1.Print();

	HashTableBucket<int,int> ht2;
	ht2 = ht1;
	ht2.Print();

}


void DircFindTest()
{
	HashTableBucket<string,string> ht;
	/*ht.Insert("zhang","张");
	ht.Insert("xiao","小");
	ht.Insert("yu","雨");*/
	//ht.Insert("angzh","田");

	ht.Insert("字典","dirc");
	ht.Insert("钱","money");
	ht.Insert("吃","eat");
	ht.Insert("玩","happy");
	ht.Print();
}

int main()
{
	//HashTableListTest();
	HashTableTest();
    DircFindTest();
	return 0;
}
