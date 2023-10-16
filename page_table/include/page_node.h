#ifndef PAGE_NODE_H
#define PAGE_NODE_H

#include "page_map.h"
#include <array>
#include <cstddef>

constexpr int TWO_TO_POWER_OF(const int X)
{
    return 1 << X; // Bit shift to replicate 2^X
}

template <size_t Levels, int Bits>
struct PageTable;  // Forward Declaration

template <size_t Levels, int Bits>
struct PageNode
{
    PageNode(const PageTable<Levels, Bits>& pageTable) : pageTable{pageTable} {}
    const PageTable<Levels, Bits>& pageTable;
    int nodeDepth;
}; // Page Node can't exist without a Pagetable

template <size_t Levels, int Bits>
struct InternalNode : public PageNode<Levels, Bits>
{
    InternalNode(const PageTable<Levels, Bits>& pageTable) : PageNode<Levels, Bits>(pageTable) {};
    std::array<InternalNode*, TWO_TO_POWER_OF(Bits)> childNodes;
};

template <size_t Levels, int Bits>
struct LeafNode : public PageNode<Levels, Bits>
{
    LeafNode(const PageTable<Levels, Bits>& pageTable) : PageNode<Levels, Bits>(pageTable) {};
    std::array<PageMap*, TWO_TO_POWER_OF(Bits)> pageMaps; 
};

template <size_t Levels, int Bits>
InternalNode<Levels, Bits>* allocateInternalNode(const PageTable<Levels, Bits>& pageTable, const int nodeDepth)
{
    auto internalNode = new InternalNode<Levels, Bits>{pageTable};
    internalNode->nodeDepth = nodeDepth;
    for (auto childNode : internalNode->childNodes)
    {
        childNode = nullptr;
    }
    return internalNode;
}

template <size_t Levels, int Bits>
LeafNode<Levels, Bits>* allocateLeafNode(const PageTable<Levels, Bits>& pageTable, const int nodeDepth)
{
    auto leafNode = new LeafNode<Levels, Bits>{pageTable};
    leafNode->nodeDepth = nodeDepth;
    for (auto pageMap : leafNode->pageMaps)
    {
        pageMap = nullptr;
    }
    return leafNode;
}

template <size_t Levels, int Bits>
PageNode<Levels, Bits>* allocateNode(const PageTable<Levels, Bits>& pageTable, const int nodeDepth)
{
    return nodeDepth == pageTable.treeDepth -1 ? // Subtract by one to offset index
    (PageNode<Levels, Bits>*)allocateLeafNode<Levels, Bits>(pageTable, nodeDepth) : 
    (PageNode<Levels, Bits>*)allocateInternalNode<Levels, Bits>(pageTable, nodeDepth);   
}

#endif
