#ifndef FUNC_INDEXTREE_VF_H_INCLUDED
#define FUNC_INDEXTREE_VF_H_INCLUDED

void partitionVF( VFTree& , vector<DataItemVF>& );
int calculateYi( std::vector<DataItemVF>& );
void buildVFTree( VFTree*&, std::vector<DataItemVF>& );//將return的寫法，改成把root當參數傳進去
void enterId( VFTree& );

#endif // FUNC_INDEXTREE_VF_H_INCLUDED
