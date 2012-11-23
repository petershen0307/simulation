#ifndef STRUCT_SEQ_SEARCH_H_INCLUDED
#define STRUCT_SEQ_SEARCH_H_INCLUDED

struct Seq_basic
{
//commons of index and data node
	int key;//index node is negative, data node is positive
	int distanceToRoot;/*root會在目前slot後第幾個出現,
	例總共12個slot,目前在第11個,離root有2個slot(11,12,1...) (slots)*/
	int rangeMaxDataKey;/*目前index底下可找到最大的data key/id(lowest access frequency);
	data用來判斷想要的data是否在同一個parent底下*/
//only index node have below types
	int nextSameLevelIndex;//next index with same level (slots)
	int minDataKey;//0表示index底下沒有data

//constructor
	//initial給值得時候,也要依照宣告時的順序
	Seq_basic( int key, int rangeMaxDataKey = 0, int minDataKey = 0, int distanceToRoot = 0, int nextSameLevelIndex = 0 )
	:key(key), distanceToRoot(distanceToRoot), rangeMaxDataKey(rangeMaxDataKey),
	nextSameLevelIndex(nextSameLevelIndex), minDataKey(minDataKey)
	{}
};

#endif // STRUCT_SEQ_SEARCH_H_INCLUDED
