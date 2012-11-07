#include <iostream>
#include <algorithm>
#include <vector>
//functional  numeric 是呼叫accumulate所必須include
#include <functional>
#include <numeric>

#include "..\include\struct_indexTree_VF.h"

using namespace std;

void partitionVF(VFTree& , vector<DataItemVF>& );
void printDataItemVF(VFTree& , vector<DataItemVF>& , bool);

//accumulate
float sumFunc(float x, DataItemVF y)
{
    return x + y.freq;
}
//max_element
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

//search
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

//build VF tree
static int indexCounterVFTree = 0;//記錄index key  從data key的下一個開始   if data key最大到11 index key從12開始
//implement VF algorithm
VFTree* buildVFTree(vector<DataItemVF>& dataItem)
{
    //create tree root，將所有data加入到root, 將data先做排序frequency由大到小
    indexCounterVFTree = dataItem.size();
    for(unsigned i=1; i != dataItem.size(); ++i)
    {
        ///判斷是否有由大到小排
        if(dataItem[i-1].freq < dataItem[i].freq)
        {
            stable_sort(dataItem.begin(), dataItem.end(), isMoreThanFuncFreq_sort);
        }
    }
    float totalFreq = accumulate(dataItem.begin(), dataItem.end(), 0.0, sumFunc);
    VFTree *root = new VFTree(VFTree::Index, ++indexCounterVFTree, totalFreq,
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
    VFTree* newNode = new VFTree(VFTree::Index, ++indexCounterVFTree, 0);
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

void printVFTree(VFTree& root)
{
    if(root.type == VFTree::Index)
        cout << "-----------" << endl << "index key " << root.key << "  index freq "
             << root.freq << " root children size " << root.children.size() << endl;
    for(unsigned int i=0; i!=root.children.size(); ++i)
    {
        if(root.children[i]->type == VFTree::Data)
        {
            cout << "data key " << root.children[i]->key << "  data freq" << root.children[i]->freq << endl;
        }
        else if(root.children[i]->type == VFTree::Index)
        {
            cout << "index key " << root.children[i]->key << "  index freq" << root.children[i]->freq << endl;
        }
    }
    for(unsigned int i=0; i!=root.children.size(); ++i)
    {
        if(root.children[i]->type == VFTree::Index)
        {
            printVFTree(*root.children[i]);
        }
    }
}
