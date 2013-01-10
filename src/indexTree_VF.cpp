#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <climits>
//functional  numeric 是呼叫accumulate所必須include
#include <functional>
#include <numeric>


using namespace std;


//structure
#include "..\include\struct_indexTree_VF.h"
#include "..\include\struct_seq_search.h"

//function
#include "..\include\func_indexTree_VF.h"



//accumulate
float sumFunc(float x, DataItemVF y)
{
    return x + y.freq;
}
//stable_sort max_element
bool isMoreThanFuncFreq(DataItemVF *lhs, DataItemVF *rhs)
{
    return (lhs->freq > rhs->freq);
}

bool isMoreThanFuncFreq_sort(DataItemVF lhs, DataItemVF rhs)
{
    return (lhs.freq > rhs.freq);
}

bool isMoreThanFuncFreq_root(VFTree* lhs, VFTree* rhs)
{
    return (lhs->freq > rhs->freq);
}


//search_n
bool searchDataItemVF (DataItemVF i, int j)
{
	return (i.key == j);
}


//計算y(i)
int calculateYi(vector<DataItemVF>& data, float& yOfi)
{
    vector<DataItemVF *> setOfyi;
    int m = (int)data.size();
    if(m <= 2)
    {
        yOfi = -1;
        return -1;
    }
    float sum1ToI = 0.0;
    float sumIadd1ToM = 0.0;
    for(int i=1; i != (m-1); ++i)
    {
        //accumulate 累加
        //1 to i
        sum1ToI = accumulate(data.begin(), data.begin()+i, 0.0, sumFunc);
        //i+1 to m
        sumIadd1ToM = accumulate(data.begin()+i, data.end(), 0.0, sumFunc);
        setOfyi.push_back(new DataItemVF(data[i-1].key,(m-i-1) * sum1ToI - sumIadd1ToM));
    }
    stable_sort(setOfyi.begin(), setOfyi.end(), isMoreThanFuncFreq);
    yOfi = setOfyi[0]->freq;
    return setOfyi[0]->key;
}

//below build VF tree
static int indexCounterVFTree = 0;
//old 記錄index key  從data key的下一個開始   if data key最大到11 index key從12開始
//now index key is negative, data key is positive

//implement VF algorithm
void buildVFTree( VFTree*& root, vector<DataItemVF>& dataItem )
{
    //create tree root，將所有data加入到root, 將data先做排序frequency由大到小
    //indexCounterVFTree = dataItem.size();
    for(unsigned i=1; i != dataItem.size(); ++i)
    {
        ///判斷是否有由大到小排
        if(dataItem[i-1].freq < dataItem[i].freq)
        {
            stable_sort(dataItem.begin(), dataItem.end(), isMoreThanFuncFreq_sort);
        }
    }
    float totalFreq = accumulate(dataItem.begin(), dataItem.end(), 0.0, sumFunc);
    /*VFTree *root = new VFTree(VFTree::Index, ++indexCounterVFTree, totalFreq,
                              dataItem.back().key, dataItem.front().key,
                              dataItem.back().key, dataItem.front().key);//index key是正數*/
	/*VFTree *root = new VFTree(VFTree::Index, --indexCounterVFTree, totalFreq,
                              dataItem.back().key, dataItem.front().key,
                              dataItem.back().key, dataItem.front().key);//index key是負數*/
    root = new VFTree(VFTree::Index, --indexCounterVFTree, totalFreq,
                              dataItem.back().key, dataItem.front().key,
                              dataItem.back().key, dataItem.front().key);//取代return的方法
    for(unsigned i=0; i != dataItem.size(); ++i)
    {
        ///設定data key(id)
        dataItem[i].key = i+1;
        root->children.push_back(new VFTree(VFTree::Data, dataItem[i].key, dataItem[i].freq));
    }
    //partition
    partitionVF(*root, dataItem);
    //return tree root
    indexCounterVFTree = 0;
    enterId(*root);
    //return root;
}

//partition
void partitionVF(VFTree& root, vector<DataItemVF>& data)
{
    //計算y(i) 得到i值
    float yOfi = 0;//y(i)
    int dataI = 0;//data id with max y(i)
    dataI = calculateYi(data, yOfi);
    //判斷y(i) if y(i)<=0 return
    if(yOfi <= 0)
        return;
    //增加一個node hx 將data i+1 to m加入這個node hx
    //VFTree* newNode = new VFTree(VFTree::Index, ++indexCounterVFTree, 0);
    VFTree* newNode = new VFTree(VFTree::Index, --indexCounterVFTree, 0);
    float totalFreq = 0;
    vector<DataItemVF> biggerData;//rear of pivot
	///找到y(i)的i所在的位置，因為data有sort過，所把i前後的data做一個切個
	vector<DataItemVF>::iterator it;
	it = search_n(data.begin(), data.end(), 1, dataI, searchDataItemVF);
	int positon = int(it - data.begin());

    for(unsigned int i=positon+1; i!=root.children.size();)
    {
		newNode->children.push_back(root.children[i]);
		root.children.erase(root.children.begin()+i);
    }
    for(unsigned int i=positon+1; i!=data.size(); ++i)
    {
		biggerData.push_back(data[i]);
    }
    ///以上partition
    totalFreq = accumulate(biggerData.begin(), biggerData.end(), 0.0, sumFunc);
    newNode->freq = totalFreq;
    root.children.push_back(newNode);

	stable_sort(biggerData.begin(), biggerData.end(), isMoreThanFuncFreq_sort);
	stable_sort(root.children.begin(), root.children.end(), isMoreThanFuncFreq_root);

    //partition data( i+1 to m )
    partitionVF(*newNode, biggerData);

    //將hx加入data( 1 to i )產生list(h1,h2,h3...,hi,hx)，並且做sort =>list(h1,h2,h3...,hi,hi+1)
    vector<DataItemVF> smallerData;//front of pivot
    ///partition
    for(int i=0; i!=positon+1; ++i)
    {
    	smallerData.push_back(data[i]);
    }
    ///以上partition
    ///增加index node
    DataItemVF hx;
    hx.freq = newNode->freq;
    hx.key = newNode->key;
    smallerData.push_back(hx);
    ///增加index node
    stable_sort(smallerData.begin(), smallerData.end(), isMoreThanFuncFreq_sort);
    //partition list(h1,h2,h3...,hi,h+1)
    partitionVF(root, smallerData);
    //return
}


//enter range of id(key) to index and data node
void enterId(VFTree& root)
{
	/*
	將maxId, minId, rangeMaxId, rangeMinId給相對應的值,給只有data node的index node
	且將data node的rangeMaxId, rangeMinId給跟index node的值一樣
	*/
	bool isDataNodes = true;
	for( unsigned int i = 0; i != root.children.size(); ++i )
	{
		if( root.children[i]->type == VFTree::Index )
		{
			isDataNodes = false;
		}
	}
	//index底下都是data node
	if(isDataNodes == true)
	{
		root.maxId = (*(root.children.end()-1))->key;
		root.minId = (*(root.children.begin()))->key;
		root.rangeMaxId = (*(root.children.end()-1))->key;
		root.rangeMinId = (*(root.children.begin()))->key;
		//將data node的rangeMaxId與rangeMinId設成跟parent index一樣
		for( unsigned int i = 0; i != root.children.size(); ++i )
		{
			root.children[i]->rangeMaxId = root.rangeMaxId;
			root.children[i]->rangeMinId = root.rangeMinId;
		}
		return;
	}
	/*
	go to children
	function return後,要解決rangeMaxId跟rangeMinId
	只有是index才會call enterId
	*/
	int maxId = 0;
	int minId = INT_MAX;
	int rangeMaxId = 0;
	int rangeMinId = INT_MAX;
	for( unsigned int i = 0; i != root.children.size(); ++i )
	{
		if( root.children[i]->type == VFTree::Index )
		{
			//recursion
			root.parentKey = root.key;
			enterId(*(root.children[i]));
			root.children[i]->parentKey = root.key;
			if( root.children[i]->rangeMaxId > rangeMaxId )
			{
				rangeMaxId = root.children[i]->rangeMaxId;
			}
			if( root.children[i]->rangeMinId < rangeMinId )
			{
				rangeMinId = root.children[i]->rangeMinId;
			}
		}
		else//Data node
		{
			if( root.children[i]->key > maxId )
			{
				maxId = root.children[i]->key;
			}
			if( root.children[i]->key < minId )
			{
				minId = root.children[i]->key;
			}
		}
	}
	//設定index node
	root.rangeMaxId = rangeMaxId > maxId ? rangeMaxId : maxId;
	root.rangeMinId = rangeMinId < minId ? rangeMinId : minId;
	root.maxId = maxId;
	root.minId = ( minId == INT_MAX ) ? 0 : minId;
	//設定data node
	for( unsigned int i = 0; i != root.children.size(); ++i )
	{
		if( root.children[i]->type == VFTree::Data )
		{
			root.children[i]->rangeMaxId = root.rangeMaxId;
			root.children[i]->rangeMinId = root.rangeMinId;
		}
	}
}
