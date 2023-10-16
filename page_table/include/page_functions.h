#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "page_table.h"

unsigned int getVPNFromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift);

template<size_t Levels, int Bits>
void assignVPNToFrame(PageNode<Levels, Bits>* pageNode, const unsigned int jumpIndex, int frame)
{
    auto leafNode = (LeafNode<Levels, Bits>*)pageNode;
    leafNode->pageMaps[jumpIndex] = new PageMap;
    leafNode->pageMaps[jumpIndex]->frame_number = frame;
}

template<size_t Levels, int Bits>
PageNode<Levels, Bits>* allocateNextLevel(PageNode<Levels, Bits>* pageNode, const unsigned int jumpIndex)
{
    auto currentInternalNode = (InternalNode<Levels, Bits>*)pageNode;
    auto nextInternalNode = currentInternalNode->childNodes[jumpIndex];
    nextInternalNode = allocateNode(pageNode->pageTable, pageNode->nodeDepth+1);
    return nextInternalNode;
}

template<size_t Levels, int Bits>
const unsigned int getJumpIndex(PageNode<Levels, Bits>* pageNode, unsigned int vpn)
{
    const int nodeDepth = pageNode->nodeDepth;
    const auto pageTable = pageNode->pageTable;
    const int mask = pageTable.bitMasks[nodeDepth];
    const int shift = pageTable.bitShifts[nodeDepth];
    return (vpn & mask) >> shift;
}

template<size_t Levels, int Bits>
void insertVpn2PfnMapping(PageNode<Levels, Bits>* pageNode, unsigned int vpn, int frame)
{
    const unsigned int jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth-1) // -1 To account for Index starting from 0 
    {
        return assignVPNToFrame(pageNode, jumpIndex, frame);   
    }
    auto internalNode = allocateNextLevel(pageNode, jumpIndex);
    return insertVpn2PfnMapping(internalNode, vpn, frame);
}

template<size_t Levels, int Bits>
void insertVpn2PfnMapping(PageTable<Levels, Bits>* pageTable, unsigned int vpn, int frame)
{
    insertVpn2PfnMapping(pageTable->next_level, vpn, frame);
}

#endif