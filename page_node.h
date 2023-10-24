#ifndef PAGE_NODE_H
#define PAGE_NODE_H

#include "page_table.h"
#include <vector>

#define BIT 1

struct PageTable;  // Forward Declaration
struct PageMap;

struct PageNode
{
    PageNode(const PageTable& pageTable, const uint32_t bitsAtLevel) 
        : pageTable{pageTable} 
    {}
    const PageTable& pageTable;
    int nodeDepth;
}; // Page Node can't exist without a Pagetable so we will pass const ref in initializer list

struct InternalNode : public PageNode
{
    InternalNode(const PageTable& pageTable, const uint32_t bitsAtLevel) : 
        childNodes(bitsAtLevel),  // Init vectors to appropriate size
        PageNode(pageTable, bitsAtLevel)           // Inherit from parent node
    {}
    std::vector<PageNode*> childNodes;
};

struct LeafNode : public PageNode
{
    LeafNode(const PageTable& pageTable, const uint32_t bitsAtLevel) : 
        pageMaps(bitsAtLevel),    // Init vectors to appropriate size
        PageNode(pageTable, bitsAtLevel)           // Inherit from parent node
    {}
    std::vector<PageMap*> pageMaps; 
};

InternalNode* allocateInternalNode(const PageTable& pageTable, const uint32_t nodeDepth);
LeafNode* allocateLeafNode(const PageTable& pageTable, const uint32_t nodeDepth);
PageNode* allocateNode(const PageTable& pageTable, const uint32_t nodeDepth);

#endif
