#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "page_table.h"

using Success = bool;

const unsigned int getVPNFromVirtualAddress(const unsigned int virtualAddress, const unsigned int mask, const unsigned int shift);

template<size_t Levels, int Bits>
Success assignVPNToFrame(PageNode<Levels, Bits>* pageNode, const unsigned int jumpIndex, const int frame)
{
    auto leafNode = (LeafNode<Levels, Bits>*)pageNode;
    leafNode->pageMaps[jumpIndex] = new PageMap;
    leafNode->pageMaps[jumpIndex]->frame_number = frame;
    leafNode->pageMaps[jumpIndex]->valid = true;
    return true;
}

template<size_t Levels, int Bits>
PageNode<Levels, Bits>* allocateNextLevel(PageNode<Levels, Bits>* pageNode, const unsigned int jumpIndex)
{
    auto currentInternalNode = (InternalNode<Levels, Bits>*)pageNode;
    if (currentInternalNode->childNodes[jumpIndex] == nullptr) {
        currentInternalNode->childNodes[jumpIndex] = allocateNode(pageNode->pageTable, pageNode->nodeDepth+1);
    }
    return currentInternalNode->childNodes[jumpIndex];
}

template<size_t Levels, int Bits>
const unsigned int getJumpIndex(PageNode<Levels, Bits>* pageNode, const unsigned int vpn)
{
    const int nodeDepth = pageNode->nodeDepth;
    const auto pageTable = pageNode->pageTable;
    const int mask = pageTable.bitMasks[nodeDepth];
    const int shift = pageTable.bitShifts[nodeDepth];
    return (vpn & mask) >> shift;
}

template<size_t Levels, int Bits>
Success insertVpn2PfnMapping(PageNode<Levels, Bits>* pageNode, const unsigned int vpn, const int frame)
{
    const unsigned int jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth) // -1 To account for Index starting from 0 
    {
        return assignVPNToFrame(pageNode, jumpIndex, frame);   
    }
    auto internalNode = allocateNextLevel(pageNode, jumpIndex);
    return insertVpn2PfnMapping(internalNode, vpn, frame);
}

template<size_t Levels, int Bits>
Success insertVpn2PfnMapping(PageTable<Levels, Bits>* pageTable, const unsigned int vpn, const int frame)
{
    return insertVpn2PfnMapping(pageTable->level_zero, vpn, frame);
}

#endif