#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <sstream>

#include <boost/filesystem.hpp>


using namespace std;
namespace BFS = boost::filesystem;


//--------------VF algorithm structure--------------
#include "include\struct_indexTree_VF.h"
#include "include\struct_seq_search.h"

//--------------VF function prototype---------------
#include "include\func_build_seq.h"

//--------------main function prototype-----------------------
void complete_program();

//--------------open file function prototype------------------
#include "include\func_open_file.h"

//debug
#include "debug_func.h"

//simulator
#include "include\func_simulator.h"

#define USERS 1000

int main()
{
	complete_program();
    return 0;
}

/*
包含開檔讀檔
建廣播環(包含建tree)
執行simulation
產生結果檔
*/
void complete_program()
{
	BFS::path open_dir( "./test_datum" );//測試data

	if( !exists( open_dir ) )
	{
		cout << "no test_datum directory" << endl;
		exit( EXIT_FAILURE );
	}
	vector<string> datum_dir;
	for( BFS::directory_iterator it( open_dir ); it!=BFS::directory_iterator(); ++it )
	{
		datum_dir.push_back( (it->path()).string() );
	}
//-----------------以上從資料夾取出所有測試檔的路徑----------------------

	vector<DataItemVF> dataItem;//測試資料
	deque<Seq_min_fields *> seq_min;//廣播序列
	map<int,int> output_tuning_time;//模擬結果
	map<int,int> output_access_time;//模擬結果
	int users = USERS;//模擬人數
//-----------------將所有測試data都跑一遍---------------------------------
	for( vector<string>::iterator it = datum_dir.begin(); it != datum_dir.end(); ++it )
	{
		dataItem.clear();
		seq_min.clear();
		output_tuning_time.clear();
		output_access_time.clear();
		//------------以上跑下一個測試data的時候,上一個資料要清除
		cout << *it << endl;
		//system( "pause" );
		open_test_data( dataItem, *it );
		createBroadcastSeq( dataItem, seq_min );
		simulator( output_tuning_time, output_access_time, dataItem, seq_min, users );
		//---------模擬結束---------------------
		//---------寫入資料---------------------

		double avgTuning = accumulate( output_tuning_time.begin(), output_tuning_time.end(), 0.0,
													  []( double init, pair<int,int> first )
														{
															return init + double(first.second);
														}
													) / output_tuning_time.size();
		double avgAccess = accumulate( output_access_time.begin(), output_access_time.end(), 0.0,
													  []( double init, pair<int,int> first )
														{
															return init + double(first.second);
														}
													) / output_access_time.size();
		int max_tuning_time = max_element( output_tuning_time.begin(), output_tuning_time.end(),
												[]( pair<int,int> large, pair<int,int> first )
												{
													return large.second < first.second;
												})->second;
		int max_access_time = max_element( output_access_time.begin(), output_access_time.end(),
												[]( pair<int,int> large, pair<int,int> first )
												{
													return large.second < first.second;
												})->second;
		stringstream result_str;
		result_str << avgTuning << "," << avgAccess << "," << max_tuning_time << "," << max_access_time;
		cout << result_str.str() << endl;
		write_result_data( *it, result_str.str(), users );
		/*cout << "average tuning time " << avgTuning << endl;
		cout << "average access time " << avgAccess << endl;

		cout << "max tuning time " << max_tuning_time << endl;
		cout << "max access time " << max_access_time << endl;*/

	}
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
