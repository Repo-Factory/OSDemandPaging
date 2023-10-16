/* 
 * @brief page_table does something cool
 *
 * more
 * more
 * more
 */

#include "page_table.h"

/*
 * Given a virtual address, apply the given bit mask and shift right by the given number of
 * bits. Returns the virtual page number. This function can be used to extract the VPN of
 * any level or the full VPN by supplying the appropriate parameters.
 */
unsigned int getVPNFromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift)
{
    return (virtualAddress & mask) >> shift;
}


