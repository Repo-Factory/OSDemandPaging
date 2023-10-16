/* 
 * @brief page_table is a templated class
 *
 * See page_table.h for more details
 */

#include "page_table.h"

/*
 * Given a virtual address, apply the given bit mask and shift right by the given number of
 * bits. Returns the virtual page number. This function can be used to extract the VPN of
 * any level or the full VPN by supplying the appropriate parameters.
 */
const unsigned int getVPNFromVirtualAddress(const unsigned int virtualAddress, const unsigned int mask, const unsigned int shift)
{
    return (virtualAddress & mask) >> shift;
}


