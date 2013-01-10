#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>

using namespace std;

#include "..\include\struct_indexTree_VF.h"

void open_test_data( vector<DataItemVF>& dataItem, string data_dir )
{
    ifstream file( data_dir );

    if( !file.is_open() || !file )
    {
        cout<<"開檔失敗"<<endl;
    }
    DataItemVF temp;
    while( file>>temp.key>>temp.freq )
    {
        dataItem.push_back(temp);
        //cout<<dataItem[i].key<<" "<<dataItem[i].freq<<endl;
    }


    //
}

static int times = 0;
void write_result_data( string filename, string result, int users )
{
	++times;
	//取出檔名中的資料總數n與skewed s
	vector<string> split_file_name;
	boost::split( split_file_name, filename, boost::is_any_of( "_txt" ), boost::token_compress_on );
	vector<string>::iterator it_find = find( split_file_name.begin(), split_file_name.end(), "n" );
	string total_data_number = *(++it_find);//total data number
	it_find = find( split_file_name.begin(), split_file_name.end(), "s" );
	string skewed = *(++it_find);//skewed
	boost::replace_last( skewed, ".", "" );
	stringstream fileName;
	fileName << "u_" << users << ".csv";
	cout << fileName.str() << endl;
	fstream file;
    file.open( fileName.str(),fstream::in | fstream::out | fstream::app );
	if( times == 1 )
	{
		file << "u,n,s,avg tuning time,avg access time,max tuning time,max access time" << endl;
	}
	stringstream final_result;
	final_result << users << "," << total_data_number << "," << skewed << "," << result;
	file << final_result.str() << endl;
	file.close();

}
