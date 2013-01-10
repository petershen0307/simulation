#ifndef FUNC_BUILD_SEQ_H_INCLUDED
#define FUNC_BUILD_SEQ_H_INCLUDED

void createBroadcastSeq( vector<DataItemVF>&, deque<Seq_min_fields *>& );
VFTree* findIndexWithLeaves(VFTree* );
void vfTreeToSeq_basic( VFTree& , vector<DataItemVF>& , deque<Seq_basic *>& , deque<VFTree *>& );

void vfTreeToSeq_min_fields( VFTree&, vector<DataItemVF>&, deque<Seq_min_fields *>&, deque<VFTree *>& );

#endif // FUNC_BUILD_SEQ_H_INCLUDED
