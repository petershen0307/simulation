#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

/*
用一個vector儲存所有種類的資料(data item)

key跟index一樣

data structure
*/
/*
struct DataItem
{
	int key;//data的編號
	int accessFrequency;//data的熱門程度
	int segment;//data的分段，先用'0'表示無分段
};
*/

/*
在廣播環index node特有的參數

struct IndexSequence
{
    int maxDatakey;//data 編號範圍最大
    int minDatakey;//data 編號範圍最小
    int nextIndexPosition;
};*/
/*
在廣播環data node特有的參數

struct DataSequence
{
    int key;//data的編號
    int accessFrequency;//data的熱門程度
    int segment;//data的分段，先用'0'表示無分段
};*/

/*
建index tree時使用

struct TreeNode
{
    // member types
    enum Type
    {
        Index, Node
    };

    // data members
    Type type;//判斷是index or data
    int freq;//每個node的access frequency，若是index node為底下所有node的frequency總合
    int key;//data or index identify key
    union
    {
        IndexSequence indexNode;
        DataSequence dataNode;
    } content;
    TreeNode *left, *right, *parent;

    // constructor
    TreeNode( Type type, int freq, int key, int maxOrkey = 0, int minOrfreq = 0, int nextOrSeg = 0 )
        : type( type ), freq( freq ), key(key),
        left( 0 ), right( 0 ) //指標給初始值 NULL表示void *  所以用0就可以
    {
        if(type == TreeNode::Index)
        {
            content.indexNode.maxDatakey = maxOrkey;
            content.indexNode.minDatakey = minOrfreq;
            content.indexNode.nextIndexPosition = nextOrSeg;
        }
        else if(type == TreeNode::Node)
        {
            content.dataNode.key = maxOrkey;
            content.dataNode.accessFrequency = minOrfreq;
            content.dataNode.segment = nextOrSeg;
        }
    }

    //destructor
    ~TreeNode()
    {
        delete left;
        delete right;
    }
};*/

/*
將index tree 轉成broadcast sequence

struct Sequence
{
    //member type
    enum Type
    {
        Index, Data
    };

    //data member
    int key;//every element has an unique number ex:0,1,2,3,...
    Type type;//determine this element is index or data
    int backToRoot;
    int freq;//每個node的access frequency，若是index node為底下所有node的frequency總合
    union
    {
        IndexSequence indexNode;
        DataSequence dataNode;
    } content;


    //constructor
    Sequence(int key, Type type, int backToRoot, int maxOrkey = 0, int minOrfreq = 0, int nextOrSeg = 0)
        :key(key), type(type), backToRoot(backToRoot)
    {
        if(type == Sequence::Index)
        {
            content.indexNode.maxDatakey = maxOrkey;
            content.indexNode.minDatakey = minOrfreq;
            content.indexNode.nextIndexPosition = nextOrSeg;
        }
        else if(type == Sequence::Data)
        {
            content.dataNode.key = maxOrkey;
            content.dataNode.accessFrequency = minOrfreq;
            content.dataNode.segment = nextOrSeg;
        }
    }
};*/


#endif // STRUCTURE_H_INCLUDED
