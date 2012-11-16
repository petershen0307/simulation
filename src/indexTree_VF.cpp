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

bool isMoreThanFunc_seq( Seq_basic *lhs, Seq_basic *rhs )
{
	return ( lhs > rhs );
}

//search_n
bool searchDataItemVF (DataItemVF i, int j)
{
	return (i.key == j);
}

bool search_n_seq( Seq_basic *t, int minId )
{
	return ( t->key == minId );
}

bool search_n_vftree( VFTree *t, int key )
{
	return ( t->key == key );
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
VFTree* buildVFTree(vector<DataItemVF>& dataItem)
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
                              dataItem.back().key, dataItem.front().key);*/
	VFTree *root = new VFTree(VFTree::Index, --indexCounterVFTree, totalFreq,
                              dataItem.back().key, dataItem.front().key,
                              dataItem.back().key, dataItem.front().key);
    for(unsigned i=0; i != dataItem.size(); ++i)
    {
        ///設定data key(id)
        dataItem[i].key = i+1;
        root->children.push_back(new VFTree(VFTree::Data, dataItem[i].key, dataItem[i].freq));
    }
    //partition
    partitionVF(*root, dataItem);
    //return tree root
    return root;
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

//debug
void printDataItemVF(VFTree& root, vector<DataItemVF>& data, bool rootOrVect)
{
    if(rootOrVect)
    {
        cout << "root size " << root.children.size() << endl;
        for(unsigned int i=0; i!=root.children.size(); ++i)
        {
            VFTree* tmp = root.children[i];
            switch(tmp->type)
            {
            case VFTree::Index:
                cout << "[index node] ";
                break;
            case VFTree::Data:
                cout << "[data node] ";
                break;
            default:
                cout << "unknown";
            }
            cout << "key " << tmp->key;
            cout << " freq " << tmp->freq << endl;
        }
    }
    else
    {
        cout << "data size" << data.size() << endl;
        for(unsigned int i=0; i!=data.size(); ++i)
        {
            cout << "key " << data[i].key;
            cout << " freq " << data[i].freq << endl;
        }
    }
}

//debug
void printVFTree(VFTree& root)
{
    if(root.type == VFTree::Index)
        cout << endl << "index key " << root.key << "  index freq "
             << root.freq << " root children size " << root.children.size() << endl << "-----------" << endl;
    for(unsigned int i=0; i!=root.children.size(); ++i)
    {
        if(root.children[i]->type == VFTree::Data)
        {
            cout << "data key " << root.children[i]->key << "  data freq" << root.children[i]->freq << endl;
			cout << "range Min Id " << root.children[i]->rangeMinId << endl;
            cout << "range Max Id " << root.children[i]->rangeMaxId << endl;
		}
        else if(root.children[i]->type == VFTree::Index)
        {
            cout << "index key " << root.children[i]->key << "  index freq" << root.children[i]->freq << endl;
            cout << "range Min Id " << root.children[i]->rangeMinId << endl;
            cout << "range Max Id " << root.children[i]->rangeMaxId << endl;
            cout << "min Id " << root.children[i]->minId << endl;
            cout << "max Id " << root.children[i]->maxId << endl;
            cout << "parent key" << root.children[i]->parentKey << endl;
        }
        cout << "-----------" << endl;
    }
    for(unsigned int i=0; i!=root.children.size(); ++i)
    {
        if(root.children[i]->type == VFTree::Index)
        {
            printVFTree(*root.children[i]);
        }
    }
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

//VF tree to optimal order sequence
void vfTreeToSeq_basic( VFTree& root, vector<DataItemVF>& dataItems, deque<Seq_basic *>& seq,
					    deque<VFTree *>& index_seq )
{
//step1 sort the data
	for( unsigned int i = 0; i != dataItems.size(); ++i )
	{
		seq.push_back( new Seq_basic( dataItems[i].key ) );
	}
//step2 選擇index node ai,ai的child nodes都是leaves,移除ai
	VFTree *node = 0;
	/*
	while是將tree中的index取出,
	while結束後,填上distanceToRoot, nextSameLevelIndex
	利用
	*/
	while( node != &root )
	{
		node = findIndexWithLeaves( &root );
		cout << node->key << endl;
		//index_seq.push_front( new Seq_basic( node->key, node->rangeMaxId, node->minId ) );//not necessary
		index_seq.push_front( node );


//step3 插入ai到SEQ,在最大access frequency的child node前
		/*
		因為有些index底下有data node,有些只有index node,所以要分開處理
		*/
		deque<Seq_basic *>::iterator it;
		if( node->minId == 0 )//deal with no data node's index
		{
			vector<Seq_basic *> temp;
			for( unsigned int i = 0; i != node->children.size(); ++i )
			{
				/*cout << "debug " << node->children[i]->key << endl;
				for( unsigned int c=0;c!=seq.size();++c )
				{
					cout << "seq key " << seq[c]->key << endl;
				}*/
				it = search_n( seq.begin(), seq.end(), 1, node->children[i]->key, search_n_seq );
				temp.push_back( *it );
			}
			if( temp.size() > 1 )
			{
				stable_sort( temp.begin(), temp.end(), isMoreThanFunc_seq );
			}
			seq.insert( ( search_n( seq.begin(), seq.end(), 1, temp.front()->key, search_n_seq ) ),
						 new Seq_basic( node->key, node->rangeMaxId, node->minId ) );
		}
		else//index with data node
		{
			it = search_n( seq.begin(), seq.end(), 1, node->minId, search_n_seq );
			seq.insert( it, new Seq_basic( node->key, node->rangeMaxId, node->minId ) );
		}
//step4 goto setp2 until T become a single node(root)
	}
	//填入distance to root, next same level index
	for( unsigned int i = 0; i != seq.size(); ++i )
	{
		//next same level index
		if( seq[i]->key < 0 )
		{
			deque<VFTree *>::iterator front;
			front = search_n( index_seq.begin(), index_seq.end(), 1, seq[i]->key, search_n_vftree );
			/*if( it != index_seq.end() )
			{
				deque<Seq_basic *>::iterator it_seq;
				it_seq = search_n( seq.begin(), seq.end(), 1, (*it)->parentKey, search_n_seq );
				seq[i]->nextSameLevelIndex
			}*/
			for( unsigned int j = i+1; j != seq.size(); ++j )
			{
				if(seq[j]->key < 0)
				{
					deque<VFTree *>::iterator back;
					back = search_n( index_seq.begin(), index_seq.end(), 1, seq[j]->key, search_n_vftree );
					if( (*front)->parentKey == (*back)->parentKey && seq[i]->key != (*back)->parentKey )
					{
						seq[i]->nextSameLevelIndex = j - i;
					}
				}
			}

		}
		//distance to root
		seq[i]->distanceToRoot = seq.size() - i;
	}
}

VFTree* findIndexWithLeaves(VFTree* root)
{
	bool isDataNodes = true;
	VFTree *node;
	for( int i = 0; i != (int)root->children.size(); ++i )
	{
		if( root->children[i]->type == VFTree::Index )
		{
			isDataNodes = false;
			node = findIndexWithLeaves( root->children[i] );
			if( node == root->children[i] )
			{
				//root->children.erase( root->children.begin()+i );
				root->children[i]->type = VFTree::Data;
			}
			break;
		}
	}
	if( isDataNodes == true )
	{
		return root;
	}
	return node;
}

void createBroadcastSeq( vector<DataItemVF>& dataItem, deque<Seq_basic *>& seq, deque<VFTree *>& index_seq )
{
	VFTree *root;
	root = buildVFTree(dataItem);
	cout<<endl;
	enterId(*root);
	printVFTree(*root);
	vfTreeToSeq_basic( *root, dataItem, seq, index_seq);
	delete root;
}
