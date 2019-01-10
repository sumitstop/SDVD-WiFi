/*
 * Copyright (C) 2016 Bastian Bloessl <bloessl@ccs-labs.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef INCLUDED_IEEE802_11_SOFT_VITERBI_DECODER_H
#define INCLUDED_IEEE802_11_SOFT_VITERBI_DECODER_H

namespace gr {
namespace ieee802_11 {

// Maximum number of traceback bytes
#define TRACEBACK_MAX 24

class soft_viterbi_decoder
{
public:

	soft_viterbi_decoder(); // constructor 
	virtual ~soft_viterbi_decoder(); // destructor 

//	uint8_t* decode(ofdm_param *ofdm, frame_param *frame, uint8_t *in);
	void oai_decode(char *y,unsigned char *decoded_bytes,unsigned char *decoded_bits,unsigned short n);

	unsigned char  d_ccodedot11_table[128]; // use std::vector 
	unsigned char  d_ccodedot11_table_rev[128]; // use std::vector 
	unsigned short d_gdot11[2] = { 0133, 0171 }; // {A,B} // use std::vector 
	unsigned short d_gdot11_rev[2] = { 0155, 0117 }; // {A,B} // use std::vector
	unsigned char d_inputs[64][12264] = {}; // static ? prev in .cc file under the function defn 
	unsigned short d_survivors[64][12264] = {}; // static ? 
	short d_partial_metrics[64],d_partial_metrics_new[64]; // static ? 




private:

//	unsigned char  d_ccodedot11_table[128];
//	unsigned char  d_ccodedot11_table_rev[128];
 
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_VITERBI_DECODER_H */
