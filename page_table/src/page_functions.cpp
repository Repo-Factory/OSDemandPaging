/* 
 * @brief page_functions does something cool
 *
 * more
 * more
 * more
 */

#include "page_functions.h"

Success assignVPNToFrame(PageNode* pageNode, const uint32_t jumpIndex, const uint32_t frame)
{
    auto leafNode = (LeafNode*)pageNode;
    if (leafNode->pageMaps[jumpIndex] == nullptr) {
        leafNode->pageMaps[jumpIndex] = new PageMap;
        leafNode->pageMaps[jumpIndex]->frame_number = frame;
        leafNode->pageMaps[jumpIndex]->valid = true;
        return true;
    }
    return false;
}

PageNode* allocateNextLevel(PageNode* pageNode, const uint32_t jumpIndex)
{
    auto currentInternalNode = (InternalNode*)pageNode;
    if (currentInternalNode->childNodes[jumpIndex] == nullptr) {
        currentInternalNode->childNodes[jumpIndex] = allocateNode(pageNode->pageTable, pageNode->nodeDepth+1);
    }
    return currentInternalNode->childNodes[jumpIndex];
}

const uint32_t getJumpIndex(PageNode* pageNode, const uint32_t vpn)
{
    const uint32_t nodeDepth = pageNode->nodeDepth;
    const auto pageTable = pageNode->pageTable;
    const uint32_t mask = pageTable.bitMasks[nodeDepth];
    const uint32_t shift = pageTable.bitShifts[nodeDepth];
    return (vpn & mask) >> shift;
}

Success insertVpn2PfnMapping(PageNode* pageNode, const uint32_t vpn, const uint32_t frame)
{
    const uint32_t jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth) // -1 To account for Index starting from 0 
    {
        return assignVPNToFrame(pageNode, jumpIndex, frame);   
    }
    auto internalNode = allocateNextLevel(pageNode, jumpIndex);
    return insertVpn2PfnMapping(internalNode, vpn, frame);
}

Success insertVpn2PfnMapping(PageTable* pageTable, const uint32_t vpn, const uint32_t frame)
{
    return insertVpn2PfnMapping(pageTable->level_zero, vpn, frame);
}


