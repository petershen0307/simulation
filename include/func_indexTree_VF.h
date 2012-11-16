#ifndef FUNC_INDEXTREE_VF_H_INCLUDED
#define FUNC_INDEXTREE_VF_H_INCLUDED

void partitionVF(VFTree& , vector<DataItemVF>& );
void printDataItemVF(VFTree& , vector<DataItemVF>& , bool);
int calculateYi(vector<DataItemVF>& );
VFTree* buildVFTree(vector<DataItemVF>& );
void printVFTree(VFTree& );
void enterId(VFTree& );
VFTree* findIndexWithLeaves(VFTree* );
void vfTreeToSeq_basic( VFTree& , vector<DataItemVF>& , deque<Seq_basic *>& , deque<VFTree *>& );
void createBroadcastSeq( vector<DataItemVF>&, deque<Seq_basic *>& , deque<VFTree *>& );

#endif // FUNC_INDEXTREE_VF_H_INCLUDED
