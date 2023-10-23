#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "page_table.h"
#include <stdint.h>
#include <cstddef>

using Inserted = bool;

///// Forward Declarations /////
struct PageTable; 
struct PageNode; 
struct InternalNode;
struct LeafNode;
//////////////////////////////////

struct PageMap
{
    int32_t frame_number;
    bool    valid;
};

const uint32_t getVPNFromVirtualAddress(const uint32_t virtualAddress, const uint32_t mask, const uint32_t shift);
const uint32_t getJumpIndex(PageNode* pageNode, const uint32_t vpn);
Inserted insertVpn2PfnMapping(PageNode* pageNode, const uint32_t vpn, const uint32_t frame);
Inserted insertVpn2PfnMapping(PageTable* pageTable, const uint32_t vpn, const uint32_t frame);
PageMap* findVpn2PfnMapping(PageTable* pageTable, const uint32_t vpn);
PageMap* findVpn2PfnMapping(PageNode* pageNode, const uint32_t vpn);

#endif