#include <ctime>
#include <cstdlib>
#include <cassert>

#include <iostream>
#include <algorithm>
#include <deque>
#include <vector>

#include "../include/structure.h"

using namespace std;


//由小到大排
bool treeNodeLessThan( TreeNode *lhs, TreeNode *rhs )
{
    return (lhs->freq < rhs->freq);
}

//由大到小排
bool accessFrequencyMoreThan(Sequence *lhs, Sequence *rhs)//left(right) hand side
{
    return (lhs->content.dataNode.accessFrequency > rhs->content.dataNode.accessFrequency);
}

//計算tree的深度 printTree()用
static int depth = 0;
//印出tree
void printTree( TreeNode * root )
{

    using namespace std;

    if( !root ) return;

    ++depth;
    printTree( root->left );

    for( int c = 0; c != depth; ++c )
        cout << "\t";

    cout << "(";

    switch( root->type )
    {
    case TreeNode::Index:
        cout << "Index";
        cout << "," << root->key << "," << root->freq << ")" << endl;
        break;
    case TreeNode::Node:
        cout << "Node";
        cout << "," << root->content.dataNode.key << "," << root->freq << "," << root->content.dataNode.segment << ")" << endl;
        break;
    default:
        assert( false );
        break;
    }

    printTree( root->right );
    --depth;
}

/*
利用CF(huffman code algorithm)演算法造出index tree
*/
static int keyCounter = 0;
TreeNode* createIndexTreeCF(vector<DataItem>& data)
{

    /*
    初始是一個array
    隨著tree開始建置
    到最後nodes只剩一個root
    */
    deque<TreeNode*> nodes;
    for( unsigned int i = 0; i != data.size(); ++i, ++keyCounter )
    {
        nodes.push_back( new TreeNode(TreeNode::Node,
                                      data[i].accessFrequency,
                                      keyCounter,
                                      data[i].key,
                                      data[i].accessFrequency,
                                      data[i].segment
                                     ) );
    }
    /*
    先做sort，取出access frequency最小的2個
    產生一個index放回nodes
    nodes剩一個點時表示是root
    */
    while( nodes.size() > 1 )
    {
        sort( nodes.begin(), nodes.end(), treeNodeLessThan );

        TreeNode * left = nodes[ 0 ],
                          * right = nodes[ 1 ];

        TreeNode * indexNode = new TreeNode(TreeNode::Index, left->freq+right->freq, keyCounter);
        ++keyCounter;
        //left node bigger than right node
        if(left->freq > right->freq)
        {
            indexNode->left = left;
            indexNode->right = right;
            indexNode->parent = indexNode;
        }
        else
        {
            indexNode->left = right;
            indexNode->right = left;
            indexNode->parent = indexNode;
        }
        left->parent = indexNode;
        right->parent = indexNode;

        if(left->type == TreeNode::Node || right->type == TreeNode::Node)
        {
            if(left->type == TreeNode::Node)
            {
                if(right->type == TreeNode::Node)
                {
                    if(left->freq > right->freq)
                    {
                        indexNode->content.indexNode.maxDatakey = left->content.dataNode.key;
                        indexNode->content.indexNode.minDatakey = right->content.dataNode.key;
                    }
                    else
                    {
                        indexNode->content.indexNode.maxDatakey = right->content.dataNode.key;
                        indexNode->content.indexNode.minDatakey = left->content.dataNode.key;
                    }
                }
                else
                {
                    indexNode->content.indexNode.maxDatakey = left->content.dataNode.key;
                    indexNode->content.indexNode.minDatakey = left->content.dataNode.key;
                }
            }
            else
            {
                if(right->type == TreeNode::Node)
                {
                    indexNode->content.indexNode.maxDatakey = right->content.dataNode.key;
                    indexNode->content.indexNode.minDatakey = right->content.dataNode.key;
                }
            }
        }

        nodes.push_back( indexNode );
        nodes.pop_front();
        nodes.pop_front();
    }

    printTree( nodes.front() );
    return nodes.front();
}

/*
在做broadcast sequence之前先把data放入bcastSeq
按照access frequency由大到小做sort
每個node的key到最後完成時在填上去

void initial_bcastSeq(vector<DataItem>& data, vector<Sequence*>& bcastSeq)
{
	for(unsigned int i = 0; i != data.size(); ++i)
	{
		bcastSeq.push_back(new Sequence(0, Sequence::Data, 0,	data[i].key,
										data[i].accessFrequency, data[i].segment));
	}
	sort(bcastSeq.begin(), bcastSeq.end(), accessFrequencyMoreThan);
}
*/

/*
當sequence產生後再去判斷next index的位置
再產生一個只裝index的vector
每個sequence node的key到最後完成時在填上去

有問題!!!!!!
*/
//最後填入next index -1表示沒有data
void indexTreeCF_To_Sequence(TreeNode* treeRoot, vector<Sequence*>& bcastSeq)
{
    if(treeRoot->type == TreeNode::Index)
    {
        if(treeRoot->left->type == TreeNode::Node && treeRoot->right->type == TreeNode::Node)
        {
            if(treeRoot->left->content.dataNode.key > treeRoot->right->content.dataNode.key)
            {
                bcastSeq.push_back(new Sequence(treeRoot->key, Sequence::Index, 0,
												treeRoot->left->content.dataNode.key,
												treeRoot->right->content.dataNode.key,
												0));
            }
            else
            {
                bcastSeq.push_back(new Sequence(treeRoot->key, Sequence::Index, 0,
												treeRoot->right->content.dataNode.key,
												treeRoot->left->content.dataNode.key,
												0));
            }
        }
        else
        {
			bcastSeq.push_back(new Sequence(treeRoot->key, Sequence::Index, 0, -1, -1, 0));
        }
    }
    else if(treeRoot->type == TreeNode::Node)
    {
		bcastSeq.push_back(new Sequence(treeRoot->key, Sequence::Data, 0,
										treeRoot->content.dataNode.key,
										treeRoot->content.dataNode.accessFrequency,
										treeRoot->content.dataNode.segment
										));
		return;
    }
    indexTreeCF_To_Sequence(treeRoot->left, bcastSeq);
    indexTreeCF_To_Sequence(treeRoot->right, bcastSeq);
}

/*
將next index連接上
*/
void connectIndex(vector<Sequence*>& bcastSeq)
{
	int front = 0;
	for(unsigned int i=0;i != bcastSeq.size();i++)
	{
		if(bcastSeq[i]->type == Sequence::Index)
		{
			bcastSeq[front]->content.indexNode.nextIndexPosition = i;
			front = i;
		}
	}
}

/*
buildBroadcastSeq
*/
void buildBroadcastSeq(vector<DataItem>& data, vector<Sequence*>& bcastSeq)
{
	TreeNode *treeRoot;
	treeRoot = createIndexTreeCF(data);
	indexTreeCF_To_Sequence(treeRoot, bcastSeq);
	connectIndex(bcastSeq);
	delete treeRoot;
}

void deleteBcastseq(vector<Sequence*>& bcastSeq)
{
	for(unsigned int i=0;i != bcastSeq.size();i++)
	{
		delete bcastSeq[i];
	}
}
