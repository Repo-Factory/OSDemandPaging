#ifndef PAGE_MAP_H
#define PAGE_MAP_H

#include <stdint.h>

// template<size_t Levels>

// struct PageTable;
// struct PageNode;

struct PageMap
{
    int32_t frame_number;
    bool    valid;
};

/* 
 *
 *Given a page table and a VPN, return the mapping of the VPN to physical frame from
 * the page table, return type could be a custom Map structure or just a frame number. If the
 * virtual page is not found or the mapping is not with valid flag, return NULL or an invalid
 * frame number. Note that if you use a different signature than the one proposed, the
 * function name and idea should be the same. If findVpn2PfnMapping was a method of the
 * C++ class PageTable, the function signature could change in an expected way: Map *
 * 3 | P a g e
 * PageTable:: findVpn2PfnMapping (unsigned int vpn). This advice should be applied to
 * other page table functions as a
 * 
 */

// template <size_t Levels>
// PageMap* findVpn2PfnMapping(PageTable<Levels>* pageTable, unsigned int vpn)
// {
//     return NULL;
// }

#endif
