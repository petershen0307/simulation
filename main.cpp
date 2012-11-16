#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

//debug------
#include "include\struct_seq_search.h"

//--------------VF algorithm structure--------------
#include "include\struct_indexTree_VF.h"
//--------------VF function prototype---------------
void createBroadcastSeq( vector<DataItemVF>&, deque<Seq_basic *>& , deque<VFTree *>& );

//--------------main function-----------------------
void inputData(vector<DataItemVF>& );

int main()
{
	vector<DataItemVF> dataItem;
    inputData(dataItem);
    deque<Seq_basic *> seq;
	deque<VFTree *> index_seq;
	createBroadcastSeq( dataItem, seq, index_seq );
	cout << "key        ";
	for( unsigned int i = 0; i < seq.size(); ++i )
	{
		cout << seq[i]->key << " ";
	}
	cout << endl << "distance   ";
	for( unsigned int i = 0; i < seq.size(); ++i )
	{
		cout << seq[i]->distanceToRoot << " ";
	}
	cout << endl << "same level ";
	for( unsigned int i = 0; i < seq.size(); ++i )
	{
		cout << seq[i]->nextSameLevelIndex << " ";
	}

    return 0;
}

/*
input data must had been sorted
*/
void inputData(vector<DataItemVF>& dataItem)
{
    char fileName[] = "paper_test_11.txt";
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
