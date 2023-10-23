/* 
 * @brief Defines Node Allocation for Tree
 *
 * We will use inheritance to represent our two possible node types. Allocating each will be different as they have different children
 * (either a list of page maps or a list of more internal nodes)
 */

#include "page_node.h"
#include "page_table.h"
#include <stdio.h>

InternalNode* allocateInternalNode(const PageTable& pageTable, const uint32_t nodeDepth)
{
    auto internalNode = new InternalNode{pageTable, pageTable.entryCounts[nodeDepth]};
    internalNode->nodeDepth = nodeDepth;
    for (auto childNode : internalNode->childNodes)
    {
        childNode = nullptr;
    }
    return internalNode;
}

LeafNode* allocateLeafNode(const PageTable& pageTable, const uint32_t nodeDepth)
{
    auto leafNode = new LeafNode{pageTable, pageTable.entryCounts[nodeDepth]};
    leafNode->nodeDepth = nodeDepth;
    for (auto pageMap : leafNode->pageMaps)
    {
        pageMap = nullptr;
    }
    return leafNode;
}

PageNode* allocateNode(const PageTable& pageTable, const uint32_t nodeDepth)
{
    return nodeDepth == pageTable.treeDepth - 1 ? // Account for index (leaf node of tree with 3 levels will have depth of 2)
    (PageNode*)allocateLeafNode(pageTable, nodeDepth) : 
    (PageNode*)allocateInternalNode(pageTable, nodeDepth);   
}