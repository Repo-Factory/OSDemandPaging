/* 
 * @brief page_functions defines tree insertion methods and helpers
 *
 * We will traverse the tree recursively, making sure to differentiate between internal nodes and leaf nodes
 */

#include "page_functions.h"

// This can be seen as leaf node case
Inserted assignVPNToFrame(PageNode* pageNode, const uint32_t jumpIndex, const uint32_t frame)
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

// This is the internal node case
PageNode* allocateNextLevel(PageNode* pageNode, const uint32_t jumpIndex)
{
    auto currentInternalNode = (InternalNode*)pageNode;
    if (currentInternalNode->childNodes[jumpIndex] == nullptr) {
        currentInternalNode->childNodes[jumpIndex] = allocateNode(pageNode->pageTable, pageNode->nodeDepth+1);
    }
    return currentInternalNode->childNodes[jumpIndex];
}

// Find next node to go to
const uint32_t getJumpIndex(PageNode* pageNode, const uint32_t vpn)
{
    const uint32_t nodeDepth = pageNode->nodeDepth;
    const auto pageTable = pageNode->pageTable;
    const uint32_t mask = pageTable.bitMasks[nodeDepth];
    const uint32_t shift = pageTable.bitShifts[nodeDepth];
    return (vpn & mask) >> shift;
}


// Entry point to recursion stack for insert function
Inserted insertVpn2PfnMapping(PageTable* pageTable, const uint32_t vpn, const uint32_t frame)
{
    return insertVpn2PfnMapping(pageTable->level_zero, vpn, frame);
}

// Traverse tree recursively to insert VPN and return if it was inserted (boolean) or not if the page already existed
Inserted insertVpn2PfnMapping(PageNode* pageNode, const uint32_t vpn, const uint32_t frame)
{
    const uint32_t jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth - 1) // -1 To account for Index starting from 0 
    {
        return assignVPNToFrame(pageNode, jumpIndex, frame);   
    }
    auto internalNode = allocateNextLevel(pageNode, jumpIndex);
    return insertVpn2PfnMapping(internalNode, vpn, frame);
}

// Entry point to recursion stack for insert function
PageMap* findVpn2PfnMapping(PageTable* pageTable, const uint32_t vpn)
{
    return findVpn2PfnMapping(pageTable->level_zero, vpn);
}

//Given a page table and a VPN, return the mapping of the VPN to physical frame 
PageMap* findVpn2PfnMapping(PageNode* pageNode, const uint32_t vpn)
{
    if (pageNode==NULL) return NULL;
    const uint32_t jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth - 1)  // -1 To account for Index starting from 0 
    {
        auto currentNode = (LeafNode*)pageNode;
        // const uint32_t offsetBits = pageNode->pageTable.offsetBits;
        return currentNode->pageMaps[jumpIndex];
    }
    auto currentNode = (InternalNode*)pageNode;
    return findVpn2PfnMapping(currentNode->childNodes[jumpIndex], vpn);
}