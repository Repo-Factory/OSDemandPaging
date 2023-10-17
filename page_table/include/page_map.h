#ifndef PAGE_MAP_H
#define PAGE_MAP_H

#include <stdint.h>
#include <cstddef>

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

/* 
 * Given a page table and a VPN, return the mapping of the VPN to physical frame
 * We'll use a recursive approach and enter the recursion stack with an overloaded proxy
 */

PageMap* findVpn2PfnMapping(PageTable* pageTable, unsigned int vpn);

PageMap* findVpn2PfnMapping(PageNode* pageNode, unsigned int vpn);

#endif
