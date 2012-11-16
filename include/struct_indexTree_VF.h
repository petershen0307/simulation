#ifndef STRUCT_INDEXTREE_VF_H_INCLUDED
#define STRUCT_INDEXTREE_VF_H_INCLUDED

/*
VF algorithm
*/
//data item
struct DataItemVF
{
	int key;
	float freq;

	//constructor
	DataItemVF(int key = 0, float freq = 0)
	:key(key), freq(freq)
	{ }
};

//index tree
struct VFTree
{
	//type
	enum Type
	{ Index, Data };

	//member
	Type type;
	//int id;//每個node一個固定ID
	int key;//data and index
	float freq;//probability
	//the range of under the index's data
	int maxId;//在index底下max data id
	int minId;//在index底下min data id
	//解決index底下child都是index的問題
	int rangeMaxId;
	int rangeMinId;
	std::vector<VFTree *> children;
	int parentKey;//方便判斷是否是同層index

	//constructor
	VFTree(VFTree::Type type, int key, float freq, int maxId = 0, int minId = 0, int rangeMaxId = 0, int rangeMinId = 0)
	:type(type), key(key), freq(freq), maxId(maxId), minId(minId), rangeMaxId(rangeMaxId), rangeMinId(rangeMinId)
	{ }

	//destructor
	~VFTree()
	{
		for(unsigned int i=0; i != children.size();++i)
		{
			delete children[i];
		}
	}
};


#endif // STRUCT_INDEXTREE_VF_H_INCLUDED
