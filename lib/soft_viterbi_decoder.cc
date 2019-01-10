/*
 * Copyright 1995 Phil Karn, KA9Q
 * Copyright 2008 Free Software Foundation, Inc.
 * 2014 Added SSE2 implementation Bogdan Diaconescu
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *

 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * Viterbi decoder for K=7 rate=1/2 convolutional code
 * Some modifications from original Karn code by Matt Ettus
 * Major modifications by adding SSE2 code by Bogdan Diaconescu
 */
#include "soft_viterbi_decoder.h"
#include <cstring>
#include <iostream>

/* The basic Viterbi decoder operation, called a "butterfly"
 * operation because of the way it looks on a trellis diagram. Each
 * butterfly involves an Add-Compare-Select (ACS) operation on the two nodes
 * where the 0 and 1 paths from the current node merge at the next step of
 * the trellis.
 *
 * The code polynomials are assumed to have 1's on both ends. Given a
 * function encode_state() that returns the two symbols for a given
 * encoder state in the low two bits, such a code will have the following
 * identities for even 'n' < 64:
 *
 * 	encode_state(n) = encode_state(n+65)
 *	encode_state(n+1) = encode_state(n+64) = (3 ^ encode_state(n))
 *
 * Any convolutional code you would actually want to use will have
 * these properties, so these assumptions aren't too limiting.
 *
 * Doing this as a macro lets the compiler evaluate at compile time the
 * many expressions that depend on the loop index and encoder state and
 * emit them as immediate arguments.
 * This makes an enormous difference on register-starved machines such
 * as the Intel x86 family where evaluating these expressions at runtime
 * would spill over into memory.
 */

using namespace gr::ieee802_11;


soft_viterbi_decoder::soft_viterbi_decoder() 
{
//std::cout << (unsigned short)d_d_gdot11[0] << " " << (unsigned short)d_d_gdot11[1] << std::endl;
//std::cout << (unsigned short)d_d_gdot11_rev[0] << " " << (unsigned short)d_d_gdot11_rev[1] << std::endl;
// populate d_d_ccodedot11_table : ccodedot11_init()
{
  unsigned int  i, j, k, sum;

  for (i = 0; i < 128; i++) {
    d_ccodedot11_table[i] = 0;

    /* Compute R output bits */
    for (j = 0; j < 2; j++) {
      sum = 0;

      for (k = 0; k < 7; k++)
        if ((i & d_gdot11[j]) & (1 << k))
          sum++;

      /* Write the sum modulo 2 in bit j */
      d_ccodedot11_table[i] |= (sum & 1) << j;
      //printf("d_ccodedot11_table[%d],%d\n",i,d_ccodedot11_table[i]);
    }
  }
}

{
  unsigned int  i, j, k, sum;

  for (i = 0; i < 128; i++) {
    d_ccodedot11_table_rev[i] = 0;

    /* Compute R output bits */
    for (j = 0; j < 2; j++) {
      sum = 0;

      for (k = 0; k < 7; k++)
        if ((i & d_gdot11_rev[j]) & (1 << k))
          sum++;

      /* Write the sum modulo 2 in bit j */
      d_ccodedot11_table_rev[i] |= (sum & 1) << j;
    }
  }
}

}

soft_viterbi_decoder::~soft_viterbi_decoder() {}

void soft_viterbi_decoder::oai_decode(char *y,unsigned char *decoded_bytes,unsigned char *decoded_bits, unsigned short n)
{

//printf("n is %d\n", n);

//printf("here no simd\n");
  /*  y is a pointer to the input
      decoded_bytes is a pointer to the decoded output
      n is the size in bits of the coded block, with the tail */


  char *in = y;
  short m0 = 0;
  short m1 = 0;
  short w[4] = {0,0,0,0};
  short max_metric = 0;
  short position = 0;
  unsigned short prev_state0 = 0;
  unsigned short prev_state1 = 0;
  unsigned short state = 0;

  d_partial_metrics[0] = 0;

  for (state=1; state<64; state++)
    d_partial_metrics[state] = -127;

  for (position=0; position<n; position++) {

//        printf("Channel Output %d = (%d,%d)\n",position,*in,*(in+1));

    //        printf("%d %d\n",in[0],in[1]);

    w[3] = in[0] + in[1];  // 1,1
    w[0] = -w[3];          // -1,-1
    w[1] = in[0] - in[1];  // -1, 1
    w[2] = -w[1];          // 1 ,-1

    max_metric = -127;

        //printf("w: %d %d %d %d\n",w[0],w[1],w[2],w[3]);
    for (state=0; state<64 ; state++) {

      // input 0
      prev_state0 = (state<<1);
      m0 = d_partial_metrics[prev_state0%64] + w[d_ccodedot11_table[prev_state0]];
      /*
      if (position < 8)
      printf("%d,%d : prev_state0 = %d,m0 = %d,w=%d (%d)\n",position,state,prev_state0%64,m0,w[d_ccodedot11_table[prev_state0]],d_partial_metrics[prev_state0%64]);
      */
      // input 1
      prev_state1 = (1+ (state<<1));
      m1 = d_partial_metrics[prev_state1%64] + w[d_ccodedot11_table[prev_state1]];

      /*
      if (position <8)
      printf("%d,%d : prev_state1 = %d,m1 = %d,w=%d (%d)\n",position,state,prev_state1%64,m1,w[d_ccodedot11_table[prev_state1]],d_partial_metrics[prev_state0%64]);
      */
      if (m0>m1) {
        d_partial_metrics_new[state] = m0;
        d_survivors[state][position] = prev_state0%64;
        d_inputs[state][position] = (state>31) ? 1 : 0;

        if (m0>max_metric)
          max_metric = m0;
      } else {
        d_partial_metrics_new[state] = m1;
        d_survivors[state][position] = prev_state1%64;
        d_inputs[state][position] = (state>31) ? 1 : 0;

        if (m1>max_metric)
          max_metric = m1;
      }

    }

    for (state=0 ; state<64; state++) {

      d_partial_metrics[state] = d_partial_metrics_new[state]- max_metric;
            //printf("%d d_partial_metrics[%d] = %d\n",position,state,d_partial_metrics[state]);
    }

    in+=2;
  }

/*
for(int i = 0; i < 64; i++)
{
for(int j = 0; j < 24; j++)
{
printf("%d,",d_inputs[i][j]);
}
printf("\n");
}
*/
  // Traceback
  prev_state0 = 0;

  for (position = n-1 ; position>-1; position--) 
{
//    printf("prev_state0-%d, position-%d\n",prev_state0, position);
    decoded_bytes[(position)>>3] += (d_inputs[prev_state0][position]<<(position%8));

    decoded_bits[position] = d_inputs[prev_state0][position];

//    printf("pos %d,%d\n",decoded_bytes[(position)>>3],(position%8));
 //      if (position%8==0)
   //       printf("%d\n",decoded_bytes[(position)>>3]);


    prev_state0 = d_survivors[prev_state0][position];

  }

//std::cout << "exitted" << std::endl;

}
