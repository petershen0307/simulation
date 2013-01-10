#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <climits>

using namespace std;

//structure
#include "..\include\struct_indexTree_VF.h"
#include "..\include\struct_seq_search.h"

//function
#include "..\include\func_indexTree_VF.h"
#include "..\include\func_build_seq.h"

//debug
#include "..\debug_func.h"

//organize total function
void createBroadcastSeq( vector<DataItemVF>& dataItem, deque<Seq_min_fields *>& seq_min )
{
	VFTree *root;
	buildVFTree( root, dataItem );


	drawTheTree( *root );

	deque<VFTree *> index_seq;
	buildVFTree( root, dataItem );
	vfTreeToSeq_min_fields( *root, dataItem, seq_min, index_seq );
	//printBroadcastSeq( seq_min );
	delete root;

}

//find Index node witch is Leaf
//會將已讀過的index node改成data，只可以用一次
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

//stable_sort max_element
//小到大排
bool isMoreThanFunc_seq( Seq_basic *lhs, Seq_basic *rhs )
{
	return ( lhs > rhs );
}

//search_n
bool search_n_seq( Seq_basic *t, int minId )
{
	return ( t->key == minId );
}

bool search_n_seq_min( Seq_min_fields *t, int minId )
{
	return ( t->key == minId );
}

bool search_n_vftree( VFTree *t, int key )
{
	return ( t->key == key );
}

//tree to broadcast sequence are down below
//VF tree to optimal order sequence
//對應到struct Seq_basic
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
		//cout << node->key << endl;
		//index_seq.push_front( new Seq_basic( node->key, node->rangeMaxId, node->minId ) );//not necessary
		index_seq.push_front( node );


//step3 插入ai到SEQ,在最大access frequency的child node前
		/*
		因為有些index底下有data node,有些只有index node,所以要分開處理
		*/
		deque<Seq_basic *>::iterator it;
		if( node->minId == 0 )//deal with no data node's index
		{
			//利用在seq的位置由小到大排
			//vector<Seq_basic *> temp;
			int min_dist = INT_MAX;
			for( unsigned int i = 0; i != node->children.size(); ++i )
			{
				/*cout << "debug " << node->children[i]->key << endl;
				for( unsigned int c=0;c!=seq.size();++c )
				{
					cout << "seq key " << seq[c]->key << endl;
				}*/

				it = search_n( seq.begin(), seq.end(), 1, node->children[i]->key, search_n_seq );
				if( int(it - seq.begin()) < min_dist )
					min_dist = int( it - seq.begin() );
				//temp.push_back( *it );
			}
			/*if( temp.size() > 1 )
			{
				stable_sort( temp.begin(), temp.end(), isMoreThanFunc_seq );
			}
			seq.insert( ( search_n( seq.begin(), seq.end(), 1, temp.front()->key, search_n_seq ) ),
						 new Seq_basic( node->key, node->rangeMaxId, node->minId ) );*/
			seq.insert( seq.begin()+min_dist,
						new Seq_basic( node->key, node->rangeMaxId,  node->minId ) );
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
						break;//找到之後就該break
					}
				}
			}
		}
		//distance to root
		seq[i]->distanceToRoot = seq.size() - i;
	}
}

//對應到struct Seq_min_fields
void vfTreeToSeq_min_fields( VFTree& root, vector<DataItemVF>& dataItems, deque<Seq_min_fields *>& seq,
					    deque<VFTree *>& index_seq )
{
//step1 sort the data
	for( unsigned int i = 0; i != dataItems.size(); ++i )
	{
		seq.push_back( new Seq_min_fields( dataItems[i].key ) );
	}
//step2 選擇index node ai,ai的child nodes都是leaves,移除ai
	VFTree *node = 0;
	/*
	while是將tree中的index取出,
	while結束後,填上distanceToRoot, nextBranch
	利用
	*/
	while( node != &root )
	{
		node = findIndexWithLeaves( &root );
		index_seq.push_front( node );

//step3 插入ai到SEQ,在最大access frequency的child node前
		/*
		因為有些index底下有data node,有些只有index node,所以要分開處理
		*/
		deque<Seq_min_fields *>::iterator it;
		if( node->minId == 0 )//處理index底下只有index的node
		{
			//計算在seq中誰比較前面，index在插在最前面的data前
			int min_dist = INT_MAX;
			for( unsigned int i = 0; i != node->children.size(); ++i )
			{
				it = search_n( seq.begin(), seq.end(), 1, node->children[i]->key, search_n_seq_min );
				//計算在seq中誰比較前面，index在插在最前面的data前
				if( int(it - seq.begin()) < min_dist )
					min_dist = int( it - seq.begin() );
			}
			seq.insert( seq.begin()+min_dist,
						new Seq_min_fields( node->key, node->rangeMinId, node->rangeMaxId, node->maxId, node->minId ) );
		}
		else//index with data node
		{
			it = search_n( seq.begin(), seq.end(), 1, node->minId, search_n_seq_min );
			seq.insert( it, new Seq_min_fields( node->key, node->rangeMinId, node->rangeMaxId, node->maxId, node->minId ) );
		}
//step4 goto setp2 until T become a single node(root)
	}
	//填入distance to root index與data都一樣
	//index 的next branch, next index;
	//填入data node的 max data key, next branch, next index
		//在填入data時用，紀錄data的parent
		//因為在sequence裡都會是先index在data
	unsigned int theDataParent = 0;
	for( unsigned int i = 0; i != seq.size(); ++i )
	{
		//next branch
		if( seq[i]->key < 0 )
		{
			theDataParent = i;
			deque<VFTree *>::iterator front;
			front = search_n( index_seq.begin(), index_seq.end(), 1, seq[i]->key, search_n_vftree );
			for( unsigned int j = i+1; j != seq.size(); ++j )
			{
				if(seq[j]->key < 0)
				{
					deque<VFTree *>::iterator back;
					back = search_n( index_seq.begin(), index_seq.end(), 1, seq[j]->key, search_n_vftree );
					if( (*front)->parentKey == (*back)->parentKey && seq[i]->key != (*back)->parentKey )
					{
						seq[i]->nextBranch = j - i;
						break;//找到之後就該break
					}
				}
			}
			//若沒有兄弟在(seq裡)自己之後，nextBranch繼承parent的nextBranch-(child與parent在seq的距離)
			//nextBranch = 0表示沒有兄弟
			if( seq[i]->nextBranch == 0 )
			{
				deque<VFTree *>::iterator now_seq_i;
				now_seq_i = search_n( index_seq.begin(), index_seq.end(), 1, seq[i]->key, search_n_vftree );
				deque<Seq_min_fields *>::iterator now_seq_parent;
				now_seq_parent = search_n( seq.begin(), seq.end(), 1, (*now_seq_i)->parentKey, search_n_seq_min );
				if( (*now_seq_parent)->nextBranch != 0 )
				{
					//int(now_seq_parent-seq.begin())這是計算seq[i]的parent與第一個的距離
					seq[i]->nextBranch = (*now_seq_parent)->nextBranch - (i-int(now_seq_parent-seq.begin()));
				}
			}
		}
		//distance to root
		seq[i]->distanceToRoot = seq.size() - i;

		//next index
		seq[i]->nextIndex = 0;
		/*
		看目前位置seq[i]的後面seq[k]的parent是seq[i]
		表示seq[i]與seq[k]為兄弟
		*/
		for( unsigned int k = i+1; k < seq.size(); ++k )
		{
			if( seq[k]->key < 0 )
			{
				deque<VFTree *>::iterator nextNode;
				nextNode = search_n( index_seq.begin(), index_seq.end(), 1, seq[k]->key, search_n_vftree );
				if( (*nextNode)->parentKey == seq[i]->key )
				{
					if( int(k-i) > seq[i]->nextIndex )
					{
						seq[i]->nextIndex = int(k-i);
						break;
					}
				}
			}
		}

		//填入data node的rangeMaxDataKey, min data key, max data key, next branch, next index與index node的一樣
		if( seq[i]->key > 0 )
		{
			seq[i]->maxDataKey = seq[theDataParent]->maxDataKey;
			seq[i]->rangeMaxDataKey = seq[theDataParent]->rangeMaxDataKey;
			if( seq[theDataParent]->nextBranch > 0 )
			{
				seq[i]->nextBranch = seq[theDataParent]->nextBranch - (i - theDataParent);
			}
			if( seq[theDataParent]->nextIndex > 0 )
			{
				seq[i]->nextIndex = seq[theDataParent]->nextIndex - (i - theDataParent);
			}
		}
	}
}
