/* 
 * @brief page_functions does something cool
 *
 * more
 * more
 * more
 */

#include "page_map.h"
#include "page_table.h"
#include "page_functions.h"

PageMap* findVpn2PfnMapping(PageTable* pageTable, const uint32_t vpn)
{
    return findVpn2PfnMapping(pageTable->level_zero, vpn);
}

PageMap* findVpn2PfnMapping(PageNode* pageNode, const uint32_t vpn)
{
    if (pageNode==NULL) return NULL;
    const uint32_t jumpIndex = getJumpIndex(pageNode, vpn);
    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth)
    {
        auto currentNode = (LeafNode*)pageNode;
        return currentNode->pageMaps[jumpIndex];  
    }
    auto currentNode = (InternalNode*)pageNode;
    return findVpn2PfnMapping(currentNode->childNodes[jumpIndex], vpn);
}