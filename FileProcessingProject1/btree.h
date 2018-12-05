// btree.h

#ifndef BTREE_H
#define BTREE_H

template <class keyType>
class BTree;

#include "btnode.h"
#include "recfile.h"
#include "fixfld.h"
#include "indbuff.h"

// btree needs to be able to pack, unpack, read and
// 	write index records
// 	also, handle the insert, delete, split, merge,
//	growth of the tree
//	a pool of nodes will be managed
//	keep at least a branch of the tree in memory
//	

template <class keyType>
class BTree
	// this is the full version of the BTree
{
public:
	BTree(int order, int keySize = sizeof(keyType), int unique = 1);
	~BTree();
	int Open(char * name, int mode);
	int Create(char * name, int mode);
	int Close();
	int Insert(const keyType key, const int recAddr);
	int Remove(const keyType key, const int recAddr = -1);
	int Search(const keyType key, const int recAddr = -1);
	void Print(ostream &);
	void Print(ostream &, int nodeAddr, int level);


protected:
	typedef BTreeNode<keyType> BTNode;// useful shorthand
	BTNode * FindLeaf(const keyType key);
	// load a branch into memory down to the leaf with key
	BTNode * NewNode();
	BTNode * Fetch(const int recaddr);
	int Store(BTNode *);
	BTNode Root;
	int Height; // height of tree
	int Order; // order of tree
	int PoolSize;
	BTNode ** Nodes; // pool of available nodes
					 // Nodes[1] is level 1, etc. (see FindLeaf)
					 // Nodes[Height-1] is leaf
	FixedFieldBuffer Buffer;
	RecordFile<BTNode> BTreeFile;

	int getHeight();	// get Height of BTree when root node is loaded.
	int NodeSize = sizeof(int) + sizeof(bool) + sizeof(int) + Order * sizeof(keyType) + Order * sizeof(int);
	// NumKeys : integer, isLeaf : bool, NextNode : int, 
	BTNode * FindLeft(const keyType key, int level);				 // if there's no sibling in thisNode, find another node at same level, and return candidate
	BTNode * FindRight(const keyType key, int level);
};

const int MaxHeight = 5;
// modified
template <class keyType>
BTree<keyType>::BTree(int order, int keySize, int unique)
	: Buffer(1 + 1 + 1/* field of isLeaf and NextNode */ + 2 * order, sizeof(int) + sizeof(bool) + sizeof(int)  /* size of isLeaf and NextNode */ + order * keySize + order * sizeof(int)),
	BTreeFile(Buffer), Root(order), Order(order)
{
	Height = 1;
	Order = order;
	PoolSize = MaxHeight * 2;
	Nodes = new BTNode *[PoolSize];
	BTNode::InitBuffer(Buffer, order);
	Nodes[0] = &Root;
}


template <class keyType>
BTree<keyType>::~BTree()
{
	Close();
	delete Nodes;
}
// find Height from BTreeFile when it is opened
template <class keyType>
int BTree<keyType>::getHeight() {
	// we have only root node, so doing DFS and find leaf node, we got height because all leaf nodes appear in same level
	// goes down with most left child
	BTNode *tmp = Nodes[0]; // Root
	int recaddr = tmp->RecAddrs[0];
	int h = 1;
	while (1) {
		tmp = Fetch(recaddr);
		h++;
		if (tmp->isLeaf) {
			delete tmp;
			break;
		}
		recaddr = tmp->RecAddrs[0];
		delete tmp;
	}
	return h;
}

template <class keyType>
int BTree<keyType>::Open(char * name, int mode)
{
	int result;
	result = BTreeFile.Open(name, mode);
	if (!result) return result;
	// load root
	int rootaddr = BTreeFile.Read(Root);
	Root.RecAddr = rootaddr;
	Nodes[0] = &Root;
	// TODO
	Height = getHeight(); // find height from BTreeFile!
	return 1;
}

template <class keyType>
int BTree<keyType>::Create(char * name, int mode)
{
	int result;
	result = BTreeFile.Create(name, mode);
	if (!result) return result;
	// append root node
	result = BTreeFile.Write(Root);
	Root.RecAddr = result;
	return result != -1;
}

template <class keyType>
int BTree<keyType>::Close()
{
	Height = 1;
	delete Nodes;
	Nodes = new BTNode *[PoolSize];
	for (int i = 0; i < PoolSize; i++) Nodes[i] = NULL;
	BTNode::InitBuffer(Buffer, Order);
	Nodes[0] = &Root;
	int result;
	result = BTreeFile.Rewind();
	if (!result) return result;
	result = BTreeFile.Write(Root);
	if (result == -1) return 0;
	return BTreeFile.Close();
}

template <class keyType>
int BTree<keyType>::Insert(const keyType key, const int recAddr)
{
	int result; int level = Height - 1;
	int newLargest = 0; keyType prevKey, largestKey;
	BTNode * thisNode, *newNode=NULL, *parentNode;
	thisNode = FindLeaf(key);

	// test for special case of new largest key in tree
	if (key > thisNode->LargestKey())
	{
		newLargest = 1; prevKey = thisNode->LargestKey();
	}

	result = thisNode->Insert(key, recAddr);

	// handle special case of new largest key in tree
	// update internal nodes
	if (newLargest)
		for (int i = 0; i<Height - 1; i++)
		{
			Nodes[i]->UpdateKey(prevKey, key);
			if (i>0) Store(Nodes[i]);
		}

	while (result == -1) // if overflow and not root
	{
		//remember the largest key
		largestKey = thisNode->LargestKey();
		// split the node
		newNode = NewNode();
		thisNode->Split(newNode);
		// if splitted nodes are leaf, mark it's leaf : modified
		if (level == Height - 1) {
			thisNode->isLeaf = true;
			newNode->isLeaf = true;
		}
		else {
			thisNode->isLeaf = false;
			newNode->isLeaf = false;
		}
		Store(thisNode); Store(newNode);
		level--; // go up to parent level
		if (level < 0) break;	// parent is root
								// insert newNode into parent of thisNode
		parentNode = Nodes[level];
		result = parentNode->UpdateKey
		(largestKey, thisNode->LargestKey());
		result = parentNode->Insert
		(newNode->LargestKey(), newNode->RecAddr);
		thisNode = parentNode;
	}
	Store(thisNode);
	if (level >= 0) return 1;// insert complete
							 // else we just split the root
							 // original code just append root node to file, so it invokes problem.
							 // because when we open BTreeFile, Consider that there's Root node at the head of the file. so should be modified
							 // Figure out that Close() handles this problem
	int newAddr = BTreeFile.Append(Root); // put previous root into file
										  // insert 2 keys in new root node
	Root.Keys[0] = thisNode->LargestKey();
	Root.RecAddrs[0] = newAddr;
	Root.Keys[1] = newNode->LargestKey();
	Root.RecAddrs[1] = newNode->RecAddr;
	Root.NumKeys = 2;
	Height++;
	return 1;
}

template <class keyType>
BTreeNode<keyType> *BTree<keyType>::FindLeft(const keyType key, int level) {
	BTNode *parent = Nodes[level - 1];
	int idx = parent->Find(key, -1, 0);			// inexact Find
	if (idx == 0) {								// there's no left sibling in parent's node

	}
}

template <class keyType>
BTreeNode<keyType> *BTree<keyType>::FindRight(const keyType key, int level) {
	BTNode *parent = Nodes[level - 1];
	int n = parent->numKeys();
	int idx = parent->Find(key, -1, 0);
	if (idx < n - 1) {					// There's right sibling in the parent's node

	}
}

// scenario of Delete operation
// Search such key and return recaddr to main program.
// the Job of Remove Methods in BTree is only modifying BTree
// Trimming of data file will happen in main program, the method here should handle index to get consistent with dat file
// So, this method will be called only if there's key in data file which means the BTree must have too
template <class keyType>
int BTree<keyType>::Remove(const keyType key, const int recAddr)
{
	// left for exercise
	// Start Remove
	// If Remove successfully done, need to modify all the records in the index file and dat file
	int result; int level = Height - 1;
	int newLargest = 0; keyType prevKey, largestKey;
	int deladdr;							// record address is needed to modify address of index nodes
	BTNode * thisNode, *newNode, *parentNode;
	thisNode = FindLeaf(key);				// 

											// if key we want to delete is the largest key in thisNode, we must update Parent's key
											// need second largest Key!
	if (key >= thisNode->LargestKey()) {
		newLargest = 1; prevKey = key;
		largestKey = thisNode->Keys[thisNode->numKeys() - 2];
	}

	deladdr = thisNode->Search(key);
	result = thisNode->Remove(key, recaddr);

	if (newLargest) {
		for (int i = 0; i < Height - 1; i++) {
			Nodes[i]->UpdateKey(prevKey, largestKey);
			if (i > 0) Store(Nodes[i]);
		}
	}

	while (result == -1) {					// if underflow and not root
											// first, try to borrow(redistribution) from right

											// second, try to merge with right

											// third, try to borrow from left

											// fourth, try to merge with left

	}

	return -1;
}

template <class keyType>
int BTree<keyType>::Search(const keyType key, const int recAddr)
{
	BTNode * leafNode;
	leafNode = FindLeaf(key);
	return leafNode->Search(key, recAddr, 1);
}

template <class keyType>
void BTree<keyType>::Print
(ostream & stream, int nodeAddr, int level)
{
	BTNode * thisNode = Fetch(nodeAddr);
	stream << "Node at level " << level << " address " << nodeAddr << ' ';
	thisNode->Print(stream);
	if (Height>level)
	{
		level++;
		for (int i = 0; i<thisNode->numKeys(); i++)
		{
			Print(stream, thisNode->RecAddrs[i], level);
		}
		stream << "end of level " << level << endl;
	}
}

template <class keyType>
void BTree<keyType>::Print(ostream & stream)
{
	stream << "BTree of height " << Height << " is " << endl;
	Root.Print(stream);
	if (Height>1)
		for (int i = 0; i<Root.numKeys(); i++)
		{
			Print(stream, Root.RecAddrs[i], 2);
		}
	stream << "end of BTree" << endl;
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::FindLeaf(const keyType key)
// load a branch into memory down to the leaf with key
{
	int recAddr, level;
	for (level = 1; level < Height; level++)
	{
		recAddr = Nodes[level - 1]->Search(key, -1, 0);//inexact search
		Nodes[level] = Fetch(recAddr);
	}
	return Nodes[level - 1];
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::NewNode()
{// create a fresh node, insert into tree and set RecAddr member
	BTNode * newNode = new BTNode(Order);
	int recAddr = BTreeFile.Append(*newNode);
	newNode->RecAddr = recAddr;
	return newNode;
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::Fetch(const int recaddr)
{// load this node from File into a new BTreeNode
	int result;
	BTNode * newNode = new BTNode(Order);
	result = BTreeFile.Read(*newNode, recaddr);
	if (result == -1) return NULL;
	newNode->RecAddr = result;
	return newNode;
}

template <class keyType>
int BTree<keyType>::Store(BTNode * thisNode)
{
	return BTreeFile.Write(*thisNode, thisNode->RecAddr);
}


#endif
