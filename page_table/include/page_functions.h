#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "page_table.h"

unsigned int getVPNFromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift);

template<size_t Levels, int Bits>
void assignVPNToFrame(PageNode<Levels, Bits>* pageLevel, const unsigned int jumpIndex, int frame)
{
    auto leafNode = (LeafNode<Levels, Bits>*)pageLevel;
    leafNode->pageMaps[jumpIndex] = new PageMap;
    leafNode->pageMaps[jumpIndex]->frame_number = frame;
}

template<size_t Levels, int Bits>
InternalNode<Levels, Bits>* allocateNextLevel(PageNode<Levels, Bits>* pageLevel, const unsigned int jumpIndex)
{
    auto internalNode = (InternalNode<Levels, Bits>*)pageLevel;
    internalNode->childNodes[jumpIndex] = new InternalNode(pageLevel->pageTable);
    internalNode->childNodes[jumpIndex]->nodeDepth = pageLevel->nodeDepth+1;
    return internalNode->childNodes[jumpIndex];
}

template<size_t Levels, int Bits>
void insertVpn2PfnMapping(PageNode<Levels, Bits>* pageLevel, unsigned int vpn, int frame)
{
    const int nodeDepth = pageLevel->nodeDepth;
    const auto pageTable = pageLevel->pageTable;
    const int mask = pageTable.bitMasks[nodeDepth];
    const int shift = pageTable.bitShifts[nodeDepth];
    const unsigned int jumpIndex = (vpn & mask) >> shift;
    if (nodeDepth == pageTable.treeDepth - 1)
    {
        return assignVPNToFrame(pageLevel, jumpIndex, frame);   
    }
    auto internalNode = allocateNextLevel(pageLevel, jumpIndex);
    insertVpn2PfnMapping(internalNode, vpn, frame);
}

template<size_t Levels, int Bits>
void insertVpn2PfnMapping(PageTable<Levels, Bits>* pageTable, unsigned int vpn, int frame)
{
    insertVpn2PfnMapping(pageTable->next_level, vpn, frame);
}

#endif