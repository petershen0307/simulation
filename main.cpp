#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
data item輸入進來後
先不要給編號key的值
先對資料的frequency做sort
然後再給編號key的值
*/

//--------------VF algorithm structure--------------
#include "include\struct_indexTree_VF.h"
//--------------VF function prototype---------------
#include "include\func_indexTree_VF.h"

int main()
{
	vector<DataItemVF> dataItem;
    inputData(dataItem);
    //callCValgorithm(dataItem);

    VFTree *root;
	root = buildVFTree(dataItem);
	cout<<endl;
	printVFTree(*root);
    return 0;
}

void inputData(vector<DataItemVF>& dataItem)
{
    char fileName[] = "paper_test_11_balance.txt";
    fstream file;
    file.open(fileName,fstream::in | fstream::out | fstream::app);
    if(!file)
    {
        cout<<"開檔失敗"<<endl;
    }
    for(int i=0; file.good(); i++)
    {
        DataItemVF temp;
        file>>temp.key>>temp.freq;
        dataItem.push_back(temp);
        cout<<dataItem[i].key<<" "<<dataItem[i].freq<<endl;
    }
    file.close();
}
