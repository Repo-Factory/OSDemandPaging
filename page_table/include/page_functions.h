#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "page_table.h"

using Success = bool;

const unsigned int getVPNFromVirtualAddress(const unsigned int virtualAddress, const unsigned int mask, const unsigned int shift);
Success assignVPNToFrame(PageNode* pageNode, const unsigned int jumpIndex, const int frame);
PageNode* allocateNextLevel(PageNode* pageNode, const unsigned int jumpIndex);
const unsigned int getJumpIndex(PageNode* pageNode, const unsigned int vpn);
Success insertVpn2PfnMapping(PageNode* pageNode, const unsigned int vpn, const int frame);
Success insertVpn2PfnMapping(PageTable* pageTable, const unsigned int vpn, const int frame);

#endif