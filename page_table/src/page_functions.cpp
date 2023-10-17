/* 
 * @brief page_functions does something cool
 *
 * more
 * more
 * more
 */

#include "page_functions.h"

Success assignVPNToFrame(PageNode* pageNode, const unsigned int jumpIndex, const int frame)
{
    auto leafNode = (LeafNode*)pageNode;
    leafNode->pageMaps[jumpIndex] = new PageMap;
    leafNode->pageMaps[jumpIndex]->frame_number = frame;
    leafNode->pageMaps[jumpIndex]->valid = true;
    return true;
}

PageNode* allocateNextLevel(PageNode* pageNode, const unsigned int jumpIndex)
{
    auto currentInternalNode = (InternalNode*)pageNode;
    if (currentInternalNode->childNodes[jumpIndex] == nullptr) {
        currentInternalNode->childNodes[jumpIndex] = allocateNode(pageNode->pageTable, pageNode->nodeDepth+1, pageNode->pageTable.entryCounts[pageNode->nodeDepth+1]);
    }
    return currentInternalNode->childNodes[jumpIndex];
}

const unsigned int getJumpIndex(PageNode* pageNode, const unsigned int vpn)
{
    const int nodeDepth = pageNode->nodeDepth;
    const auto pageTable = pageNode->pageTable;
    const int mask = pageTable.bitMasks[nodeDepth];
    const int shift = pageTable.bitShifts[nodeDepth];
    return (vpn & mask) >> shift;
}

Success insertVpn2PfnMapping(PageNode* pageNode, const unsigned int vpn, const int frame)
{
    const unsigned int jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth) // -1 To account for Index starting from 0 
    {
        return assignVPNToFrame(pageNode, jumpIndex, frame);   
    }
    auto internalNode = allocateNextLevel(pageNode, jumpIndex);
    return insertVpn2PfnMapping(internalNode, vpn, frame);
}

Success insertVpn2PfnMapping(PageTable* pageTable, const unsigned int vpn, const int frame)
{
    return insertVpn2PfnMapping(pageTable->level_zero, vpn, frame);
}


