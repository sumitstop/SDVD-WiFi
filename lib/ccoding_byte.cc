/*************************************************************************

  Functions to initialize the code tables

*************************************************************************/
unsigned char  ccodedot11_table[128];
unsigned char  ccodedot11_table_rev[128];
unsigned short gdot11[] = { 0133, 0171 }; // {A,B}
unsigned short gdot11_rev[] = { 0155, 0117 }; // {A,B}

/* Basic code table initialization for constraint length 7 */
/* Input in MSB, followed by state in 6 LSBs */

void ccodedot11_init(void)
{
  unsigned int  i, j, k, sum;

  for (i = 0; i < 128; i++) {
    ccodedot11_table[i] = 0;

    /* Compute R output bits */
    for (j = 0; j < 2; j++) {
      sum = 0;

      for (k = 0; k < 7; k++)
        if ((i & gdot11[j]) & (1 << k))
          sum++;

      /* Write the sum modulo 2 in bit j */
      ccodedot11_table[i] |= (sum & 1) << j;
    }
  }
}

/* Input in LSB, followed by state in 6 MSBs */
void ccodedot11_init_inv(void)
{
  unsigned int  i, j, k, sum;

  for (i = 0; i < 128; i++) {
    ccodedot11_table_rev[i] = 0;

    /* Compute R output bits */
    for (j = 0; j < 2; j++) {
      sum = 0;

      for (k = 0; k < 7; k++)
        if ((i & gdot11_rev[j]) & (1 << k))
          sum++;

      /* Write the sum modulo 2 in bit j */
      ccodedot11_table_rev[i] |= (sum & 1) << j;
    }
  }
}
