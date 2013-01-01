#ifndef FUNC_INDEXTREE_VF_H_INCLUDED
#define FUNC_INDEXTREE_VF_H_INCLUDED

void partitionVF(VFTree& , vector<DataItemVF>& );
int calculateYi(std::vector<DataItemVF>& );
VFTree* buildVFTree(std::vector<DataItemVF>& );
void enterId(VFTree& );

#endif // FUNC_INDEXTREE_VF_H_INCLUDED
