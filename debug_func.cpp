#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

//structure
#include "include/struct_indexTree_VF.h"
#include "include/struct_seq_search.h"

void traceTree( VFTree& root, fstream& filestr )
{
	for(unsigned int i=0; i!=root.children.size(); ++i)
    {
    	filestr << root.key << "--" << root.children[i]->key << ";\n";
        if(root.children[i]->type == VFTree::Index)
        {
            traceTree( *(root.children[i]), filestr );
        }
        else
        {
			filestr << root.children[i]->key << "[shape=box];\n";
        }
    }
}

void drawTheTree( VFTree& root )
{
	fstream filestr;
	filestr.open( "graph.gz", fstream::in | fstream::out | fstream::trunc );
	filestr << "graph graphname\n"
			<< "{\n";

	traceTree( root, filestr );

	filestr << "}";


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
    {
        cout << endl << "index key " << root.key << "  index freq "
             << root.freq << " root children size " << root.children.size() << endl << "-----------" << endl;
    }

    for(unsigned int i=0; i!=root.children.size(); ++i)
    {
        if(root.children[i]->type == VFTree::Data)
        {
            cout << "data key " << root.children[i]->key << "  data freq" << root.children[i]->freq << endl;
			/*cout << "range Min Id " << root.children[i]->rangeMinId << endl;
            cout << "range Max Id " << root.children[i]->rangeMaxId << endl;*/
		}
        else if(root.children[i]->type == VFTree::Index)
        {
            cout << "index key " << root.children[i]->key << "  index freq" << root.children[i]->freq << endl;
           /* cout << "range Min Id " << root.children[i]->rangeMinId << endl;
            cout << "range Max Id " << root.children[i]->rangeMaxId << endl;
            cout << "min Id " << root.children[i]->minId << endl;
            cout << "max Id " << root.children[i]->maxId << endl;
            cout << "parent key" << root.children[i]->parentKey << endl;*/
        }
        cout << "-----------" << endl;
    }
cout << "************" << endl;
    for(unsigned int i=0; i!=root.children.size(); ++i)
    {
        if(root.children[i]->type == VFTree::Index)
        {
            printVFTree(*root.children[i]);
        }
    }
}

//印出sequence
//Seq_min_fields
void printBroadcastSeq( deque<Seq_min_fields *>& seq_min )
{
	cout << "slot       ";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << i << " ";
	}
	cout.width(0);
	cout << endl << "key        ";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->key << " ";
	}
	cout.width(0);
	cout << endl << "distance   ";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->distanceToRoot << " ";
	}
	cout.width(0);
	cout << endl << "minrangekey";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->rangeMinDataKey << " ";
	}
	cout.width(0);
	cout << endl << "maxrangekey";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->rangeMaxDataKey << " ";
	}
	cout.width(0);
	cout << endl << "next branch";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->nextBranch << " ";
	}
	cout.width(0);
	cout << endl << "next Index ";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->nextIndex << " ";
	}
	cout.width(0);
	cout << endl << "max datakey";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->maxDataKey << " ";
	}
	cout.width(0);
	cout << endl << "min datakey";
	for( unsigned int i = 0; i < seq_min.size(); ++i )
	{
		cout.width(3);
		cout << seq_min[i]->minDataKey << " ";
	}
	cout << endl;
}

void printBroadcastSeq( vector<Seq_min_fields *>& seq_v )
{
	cout << "key        ";

	for( unsigned int i = 0; i < seq_v.size(); ++i )
	{
		cout.width(3);
		cout << seq_v[i]->key << " ";
	}
	cout.width(0);
	cout << endl << "distance   ";
	for( unsigned int i = 0; i < seq_v.size(); ++i )
	{
		cout.width(3);
		cout << seq_v[i]->distanceToRoot << " ";
	}
	cout.width(0);
	cout << endl << "next Index ";
	for( unsigned int i = 0; i < seq_v.size(); ++i )
	{
		cout.width(3);
		cout << seq_v[i]->nextIndex << " ";
	}
	cout << endl;
}
//Seq_basic
void printBroadcastSeq( deque<Seq_basic *>& seq )
{
	cout << "key        ";

	for( unsigned int i = 0; i < seq.size(); ++i )
	{
		cout.width(3);
		cout << seq[i]->key << " ";
	}
	cout.width(0);
	cout << endl << "distance   ";
	for( unsigned int i = 0; i < seq.size(); ++i )
	{
		cout.width(3);
		cout << seq[i]->distanceToRoot << " ";
	}
	cout.width(0);
	cout << endl << "same level ";
	for( unsigned int i = 0; i < seq.size(); ++i )
	{
		cout.width(3);
		cout << seq[i]->nextSameLevelIndex << " ";
	}
	cout << endl;
}


