#ifndef PAGE_MAP_H
#define PAGE_MAP_H

#include <stdint.h>
#include <cstddef>

///// Forward Declarations /////
template<size_t Levels, int Bits>
struct PageTable; 

template<size_t Levels, int Bits>
struct PageNode; 

template<size_t Levels, int Bits>
struct InternalNode;

template<size_t Levels, int Bits>
struct LeafNode;
//////////////////////////////////

struct PageMap
{
    int32_t frame_number;
    bool    valid;
};

/* 
 * Given a page table and a VPN, return the mapping of the VPN to physical frame
 * We'll use a recursive approach and enter the recursion stack with an overloaded proxy
 */

template <size_t Levels, int Bits>
PageMap* findVpn2PfnMapping(PageTable<Levels, Bits>* pageTable, unsigned int vpn)
{
    return findVpn2PfnMapping(pageTable->level_zero, vpn);
}

template <size_t Levels, int Bits>
PageMap* findVpn2PfnMapping(PageNode<Levels, Bits>* pageNode, unsigned int vpn)
{
    if (pageNode==NULL) return NULL;
    const unsigned int jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth)
    {
        auto currentNode = (LeafNode<Levels, Bits>*)pageNode;
        return currentNode->pageMaps[jumpIndex];  
    }
    auto currentNode = (InternalNode<Levels, Bits>*)pageNode;
    return findVpn2PfnMapping(currentNode->childNodes[jumpIndex], vpn);
}

#endif
