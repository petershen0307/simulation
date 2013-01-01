#ifndef DEBUG_FUNC_H_INCLUDED
#define DEBUG_FUNC_H_INCLUDED

//draw tree
void traceTree( VFTree& , fstream& );
void drawTheTree( VFTree& );


void printDataItemVF(VFTree& , vector<DataItemVF>& , bool );
void printVFTree(VFTree& );
//Seq_min_fields
void printBroadcastSeq( deque<Seq_min_fields *>& );
void printBroadcastSeq( vector<Seq_min_fields *>& );
//Seq_basic
void printBroadcastSeq( deque<Seq_basic *>& );

#endif // DEBUG_FUNC_H_INCLUDED
