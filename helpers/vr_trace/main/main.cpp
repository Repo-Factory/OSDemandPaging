/*******************************************************************************
 * @author Strix Elixel
 * @name vr_trace
 * 
 * @brief Reads words from two files passed as cmd line arguments 
 * 
 * @description For each line in file2, it will output to stdout the numbers of substrings
 * 
 * Program must be called like this or will gracefully exit: ./program arg1 arg2
 * 
 * Relevant entities
 * - Dependencies
 *      - ProgressBar
 * - ArgsHandling       - For command line arguments
 * - InputHandling      - For file operations
 * 
 * See cpp file of each for more details in each respective area
 * 
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "vaddr_tracereader.h"

int main(int argc, char **argv)
{
  FILE *ifp;	        /* trace file */
  unsigned long i = 0;  /* instructions processed */
  p2AddrTr trace;	/* traced address */

  /* check usage */
  if(argc != 2) {
    fprintf(stderr,"usage: %s input_byutr_file\n", argv[0]);
    exit(1);
  }
  
  /* attempt to open trace file */
  if ((ifp = fopen(argv[1],"rb")) == NULL) {
    fprintf(stderr,"cannot open %s for reading\n",argv[1]);
    exit(1);
  }
	
  while (!feof(ifp)) {
    /* get next address and process */
    if (NextAddress(ifp, &trace)) {
      AddressDecoder(&trace, stdout);
      i++;
      if ((i % 100000) == 0)
	fprintf(stderr,"%dK samples processed\r", i/100000);
    }
  }	

  /* clean up and return success */
  fclose(ifp);
  return (0);
}
