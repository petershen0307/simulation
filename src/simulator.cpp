#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <random>
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;

//--------------structure--------------
#include "..\include\struct_indexTree_VF.h"
#include "..\include\struct_seq_search.h"

#include "..\include\func_simulator.h"

#define USERS 10


static map<int, int> user_desire_key;
static map<int, int> user_wake_up_time;//time = slot
static map<int, int> user_tuning_time;//time = slot
static map<int, int> user_access_time;//time = slot

//計算結果
double avg_result( map<int, int>& result )
{
	double sum = 0.0;
	for( map<int, int>::iterator it = result.begin(); it != result.end(); ++it )
	{
		sum += it->second;
	}
	return sum/result.size();
}

void simulator( vector<DataItemVF>& dataItem, deque<Seq_min_fields *>& seq )
{
	int userN=100;
	cout << "input users = ";
	cin >> userN;
	simulator_initial( dataItem, seq.size(), userN );

	for( int i=1; i<=userN; ++i )
	{
		searching( i, user_desire_key[i], user_wake_up_time[i], seq );

		/*searching( i, 31-i, user_wake_up_time[i], seq );//test
		cout << "desire key" << 31-i << endl;
		cout << "wake up time" << user_wake_up_time[i] << endl;
		cout << "access time" << user_access_time[i] << endl
			 << "tuning time" << user_tuning_time[i] << "---------------" << endl;*/
	}
	cout << "mean tuning time " << avg_result( user_tuning_time ) << endl;
	cout << "mean access time " << avg_result( user_access_time );
}
//search_n
bool search_n_seq( Seq_min_fields *t, int minId )
{
	return ( t->key == minId );
}

void searching( int userId, int desire_key, int slots, deque<Seq_min_fields *>& seq )
{
	int t_time = 0;//tuning time
	int a_time = 1;//access time 都直接計算好，所以不用在每次進入slot前作加一的動作
	int c_slot = slots;//current slot

	//方法看圖
	while(true)
	{
		++t_time;
		if( seq[c_slot]->key < 0 )//index node
		{
			if( desire_key < seq[c_slot]->rangeMinDataKey )//回到root
			{
				a_time += seq[c_slot]->distanceToRoot;
				c_slot = 0;
			}
			else if( desire_key <= seq[c_slot]->rangeMaxDataKey )//同branch
				{
					if( seq[c_slot]->minDataKey <= desire_key
						&& desire_key <= seq[c_slot]->maxDataKey )//在目前index底下
					{
						++a_time;
						++c_slot;
					}
					else
					{
						a_time += seq[c_slot]->nextIndex;
						c_slot += seq[c_slot]->nextIndex;
					}
				}
				else if( desire_key > seq[c_slot]->rangeMaxDataKey )//不同branch
					{
						a_time += seq[c_slot]->nextBranch;
						c_slot += seq[c_slot]->nextBranch;
					}
		}
		else //data node
		{
			if( seq[c_slot]->key == desire_key )
			{
				break;
			}
			else
			{
				if( desire_key < seq[c_slot]->key )//回到root
				{
					a_time += seq[c_slot]->distanceToRoot;
					c_slot = 0;
				}
				else if( desire_key <= seq[c_slot]->rangeMaxDataKey )//同branch
					{
						if( desire_key <= seq[c_slot]->maxDataKey )
						{
							++a_time;
							++c_slot;
						}
						else
						{
							a_time += seq[c_slot]->nextIndex;
							c_slot += seq[c_slot]->nextIndex;
						}
					}
					else if( desire_key > seq[c_slot]->rangeMaxDataKey )//不同branch
						{
							a_time += seq[c_slot]->nextBranch;
							c_slot += seq[c_slot]->nextBranch;
						}
			}
		}
	}
	//將結果記錄下來
	user_tuning_time[userId] = t_time;
	user_access_time[userId] = a_time;
}

void simulator_initial( vector<DataItemVF>& dataItem, int seq_size, int user_number )
{
	fstream filestr;
	filestr.open("desire.txt",fstream::in | fstream::out | fstream::trunc);
//simulation
	vector<double> pmf;
	for( unsigned int i=0; i != dataItem.size();++i )
	{
		if( i == 0 )
			pmf.push_back( dataItem[i].freq );
		else
			pmf.push_back( dataItem[i].freq + pmf[i-1] );
	}


	//produce user desire key
	unsigned int seed1 = time(NULL);
	default_random_engine generator( seed1 );
	uniform_real_distribution<double> distribution( 0.0, 1.0 );
	uniform_int_distribution<int> random_time( 0, seq_size-1 );
	for( int i=1; i<=user_number; ++i )
	{
		double number = distribution( generator );
		int desire_key = find_range_pmf( pmf, number );
		//i為user id
		user_desire_key.insert( pair<int, int>( i, desire_key ) );
		user_wake_up_time.insert( pair<int, int>( i, random_time( generator ) ) );
		filestr << i << "," << desire_key << "\n";
	}
	/*cout << "desire key" << endl;
	for( map<int, int>::iterator it = user_desire_key.begin(); it != user_desire_key.end(); ++it )
	{
		cout << it->first << " " << it->second << endl;
	}
	cout << "wake up time" << endl;
	for( map<int, int>::iterator it = user_wake_up_time.begin(); it != user_wake_up_time.end(); ++it )
	{
		cout << it->first << " " << it->second << endl;
	}*/
}

//return desire data key
int find_range_pmf( vector<double>& pmf, double number )
{
	unsigned int i=0;
	for( i=0; i != pmf.size(); ++i )
	{
		if( number <= pmf[i] )
			break;
	}
	return i+1;
}
