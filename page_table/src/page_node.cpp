/* 
 * @brief page_functions does something cool
 *
 * more
 * more
 * more
 */

#include "page_node.h"
#include "page_table.h"

InternalNode* allocateInternalNode(const PageTable& pageTable, const int nodeDepth, const int bitsPerLevel)
{
    auto internalNode = new InternalNode{pageTable, bitsPerLevel};
    internalNode->nodeDepth = nodeDepth;
    for (auto childNode : internalNode->childNodes)
    {
        childNode = nullptr;
    }
    return internalNode;
}

LeafNode* allocateLeafNode(const PageTable& pageTable, const int nodeDepth, const int bitsPerLevel)
{
    auto leafNode = new LeafNode{pageTable, bitsPerLevel};
    leafNode->nodeDepth = nodeDepth;
    for (auto pageMap : leafNode->pageMaps)
    {
        pageMap = nullptr;
    }
    return leafNode;
}

PageNode* allocateNode(const PageTable& pageTable, const int nodeDepth, const int bitsPerLevel)
{
    return nodeDepth == pageTable.treeDepth ? // Is leaf node
    (PageNode*)allocateLeafNode(pageTable, nodeDepth, bitsPerLevel) : 
    (PageNode*)allocateInternalNode(pageTable, nodeDepth, bitsPerLevel);   
}

