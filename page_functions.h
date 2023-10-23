#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "page_table.h"

using Success = bool;

const uint32_t getVPNFromVirtualAddress(const uint32_t virtualAddress, const uint32_t mask, const uint32_t shift);
const uint32_t getJumpIndex(PageNode* pageNode, const uint32_t vpn);
Success insertVpn2PfnMapping(PageNode* pageNode, const uint32_t vpn, const uint32_t frame);
Success insertVpn2PfnMapping(PageTable* pageTable, const uint32_t vpn, const uint32_t frame);

#endif