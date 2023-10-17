#ifndef PAGE_NODE_H
#define PAGE_NODE_H

#include "page_map.h"
#include <vector>
#include <cstddef>

#define BIT 1

constexpr int TWO_TO_POWER_OF(const uint32_t X)
{
    return BIT << X; // Bit shift to replicate 2^X
}

struct PageTable;  // Forward Declaration

struct PageNode
{
    PageNode(const PageTable& pageTable, const uint32_t bitsPerLevel) 
        : pageTable{pageTable} 
    {}
    const PageTable& pageTable;
    int nodeDepth;
}; // Page Node can't exist without a Pagetable so we will pass const ref in initializer list

struct InternalNode : public PageNode
{
    InternalNode(const PageTable& pageTable, const uint32_t bitsPerLevel) : 
        childNodes(TWO_TO_POWER_OF(bitsPerLevel)),  // Init vectors to appropriate size
        PageNode(pageTable, bitsPerLevel)           // Inherit from parent node
    {}
    std::vector<PageNode*> childNodes;
};

struct LeafNode : public PageNode
{
    LeafNode(const PageTable& pageTable, const uint32_t bitsPerLevel) : 
        pageMaps(TWO_TO_POWER_OF(bitsPerLevel)),    // Init vectors to appropriate size
        PageNode(pageTable, bitsPerLevel)           // Inherit from parent node
    {}
    std::vector<PageMap*> pageMaps; 
};

InternalNode* allocateInternalNode(const PageTable& pageTable, const uint32_t nodeDepth, const uint32_t bitsPerLevel);
LeafNode* allocateLeafNode(const PageTable& pageTable, const uint32_t nodeDepth, const uint32_t bitsPerLevel);
PageNode* allocateNode(const PageTable& pageTable, const uint32_t nodeDepth, const uint32_t bitsPerLevel);

#endif
