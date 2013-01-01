#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <random>

using namespace std;


//--------------VF algorithm structure--------------
#include "include\struct_indexTree_VF.h"
#include "include\struct_seq_search.h"

//--------------VF function prototype---------------
#include "include\func_build_seq.h"

//--------------main function-----------------------
void inputData(vector<DataItemVF>& );

//debug
#include "debug_func.h"

//simulator
#include "include\func_simulator.h"


int main()
{
	vector<DataItemVF> dataItem;
    inputData(dataItem);
    deque<Seq_basic *> seq;
	deque<Seq_min_fields *> seq_min;
	cout << dataItem.size() << endl;
	createBroadcastSeq( dataItem, seq, seq_min );

	//simulator_initial( dataItem, seq_min.size() );
	simulator( dataItem, seq_min );

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
        //cout<<dataItem[i].key<<" "<<dataItem[i].freq<<endl;
    }
    file.close();
}
