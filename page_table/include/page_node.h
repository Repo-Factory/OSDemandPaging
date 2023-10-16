#ifndef PAGE_NODE_H
#define PAGE_NODE_H

#include "page_map.h"
#include <array>
#include <cstddef>

constexpr int TWO_TO_POWER_OF(const int X)
{
    return 1 << X; // Bit shift to replicate 2^X
}

struct PageNode
{
    int depth;
};

template <int Bits>
struct InternalNode : public PageNode
{
    std::array<InternalNode*, TWO_TO_POWER_OF(Bits)> childNodes;
};

template <int Bits>
struct LeafNode : public PageNode
{
    std::array<PageMap*, TWO_TO_POWER_OF(Bits)> pageMaps; 
};

template <int Bits>
InternalNode<Bits>* allocateInternalNode(const int depth)
{
    InternalNode<Bits>* internalNode = new InternalNode<Bits>;
    internalNode->depth = depth;
    for (InternalNode<Bits>* childNode : internalNode->childNodes)
    {
        childNode = NULL;
    }
    return internalNode;
}

template <int Bits>
LeafNode<Bits>* allocateLeafNode(const int depth)
{
    LeafNode<Bits>* leafNode = new LeafNode<Bits>;
    leafNode->depth = depth;
    for (LeafNode<Bits>* pageMap : leafNode->pageMaps)
    {
        pageMap = NULL;
    }
    return leafNode;
}

template <int Bits>
PageNode* allocateNode(int depth, int totalLevels)
{
    return depth == totalLevels-1 ? allocateLeafNode<Bits>(depth) : allocateInternalNode<Bits>(depth);   // Subtract by one to offset index
}

#endif
