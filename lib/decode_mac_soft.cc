/*
 * Copyright (C) 2013, 2016 Bastian Bloessl <bloessl@ccs-labs.org>
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
#include <ieee802-11/decode_mac_soft.h>
#include <fstream>
#include "utils.h"
#include "viterbi_decoder.h"
#include "soft_viterbi_decoder.h"
#include <boost/crc.hpp>
#include <gnuradio/io_signature.h>

using namespace gr::ieee802_11;

#define LINKTYPE_IEEE802_11 105 /* http://www.tcpdump.org/linktypes.html */

class decode_mac_soft_impl : public decode_mac_soft {

public:
decode_mac_soft_impl(bool log, bool debug) :
	block("decode_mac_soft",
			gr::io_signature::make(1, 1, 48*sizeof(float)), // takes float llr input (only bpsk now)
			gr::io_signature::make(0, 0, 0)),
	d_log(log),
	d_debug(debug),
	d_snr(0),
	d_nom_freq(0.0),
	d_freq_offset(0.0),
	d_ofdm(BPSK_1_2),
	d_frame(d_ofdm, 0),
	d_frame_complete(true) {

	message_port_register_out(pmt::mp("out"));
}

int general_work (int noutput_items, gr_vector_int& ninput_items,
		gr_vector_const_void_star& input_items,
		gr_vector_void_star& output_items) {

	const float *in = (const float*)input_items[0]; // the soft bits from ls_soft.ccc 

	int i = 0;

	std::vector<gr::tag_t> tags;
	const uint64_t nread = this->nitems_read(0);

	dout << "Decode MAC: input " << ninput_items[0] << std::endl;

	while(i < ninput_items[0]) {
                
		get_tags_in_range(tags, 0, nread + i, nread + i + 1, 
			pmt::string_to_symbol("wifi_start"));

		if(tags.size()) { 
			if (d_frame_complete == false) {
				dout << "Warning: starting to receive new frame before old frame was complete" << std::endl;
				dout << "Already copied " << copied << " out of " << d_frame.n_sym << " symbols of last frame" << std::endl;
			}
			d_frame_complete = false;

			pmt::pmt_t dict = tags[0].value;
			int len_data = pmt::to_uint64(pmt::dict_ref(dict, pmt::mp("frame_bytes"), pmt::from_uint64(MAX_PSDU_SIZE+1)));
			int encoding = pmt::to_uint64(pmt::dict_ref(dict, pmt::mp("encoding"), pmt::from_uint64(0)));
			d_snr = pmt::to_double(pmt::dict_ref(dict, pmt::mp("snr"), pmt::from_double(0)));
			d_nom_freq = pmt::to_double(pmt::dict_ref(dict, pmt::mp("freq"), pmt::from_double(0)));
			d_freq_offset = pmt::to_double(pmt::dict_ref(dict, pmt::mp("freq_offset"), pmt::from_double(0)));

			ofdm_param ofdm = ofdm_param((Encoding)encoding);
			frame_param frame = frame_param(ofdm, len_data);

			
			if(frame.n_sym <= MAX_SYM && frame.psdu_size <= MAX_PSDU_SIZE) {
				d_ofdm = ofdm;
				d_frame = frame;
				copied = 0;
				dout << "Decode MAC: frame start -- len " << len_data
					<< "  symbols " << frame.n_sym << "  encoding "
					<< encoding << std::endl;
			} else {
				dout << "Dropping frame which is too large (symbols or bits)" << std::endl;
			}
		}

		if(copied < d_frame.n_sym) {
                        
			//dout << "copy one symbol, copied " << copied << " out of " << d_frame.n_sym << std::endl;
			std::memcpy(d_rx_soft_symbols + (copied * 48), in, 48*sizeof(float));
			copied++;
                
			if(copied == d_frame.n_sym) {
				//dout << "received complete frame - decoding" << std::endl;
		// at this point everything is copied in "d_rx_soft_symbols" now call decode function
                // input to decode() is "d_rx_soft_symbols"
                                
				oai_decode();
				//decode(); // GNU Radio Viterbi Decoder
				in += 48;
				i++;
				d_frame_complete = true;
                                break;
			}
		}

		in += 48;
		i++;
	}

	consume(0, i);

	return 0;
}

void oai_decode()
{
        //std::ofstream myfile;
	//myfile.open ("/home/john/myBUG.csv");
	for(int i = 0; i < d_frame.n_sym * 48; i++) 
	{
		if(d_rx_soft_symbols[i*d_ofdm.n_bpsc] >7)
			d_rx_soft_symbols[i*d_ofdm.n_bpsc] = 7;
		else if (d_rx_soft_symbols[i*d_ofdm.n_bpsc] < -8)
			d_rx_soft_symbols[i*d_ofdm.n_bpsc] = -8;
		else		
			d_rx_soft_symbols[i*d_ofdm.n_bpsc] = (char)d_rx_soft_symbols[i*d_ofdm.n_bpsc];

		d_rx_soft_bits[i*d_ofdm.n_bpsc] = d_rx_soft_symbols[i*d_ofdm.n_bpsc];
		//myfile <<(int)d_rx_soft_bits[i*d_ofdm.n_bpsc] << ","<<std::endl;		
	}
	//myfile.close();
  		
        gnu_deinterleave();
	memset(oai_decoded_bytes,0,sizeof(oai_decoded_bytes)); // check point ! 
	memset(oai_decoded_bits,0,sizeof(oai_decoded_bits)); // check point ! 

s_decoder.oai_decode(d_deinterleaved_soft_bits,oai_decoded_bytes,oai_decoded_bits, d_ofdm.n_dbps*d_frame.n_sym);
//std::cout << "yo yo " << std::endl;
        //std::ofstream myfile;
        //myfile.open ("/home/john/OAIVITIBIT.csv");
	//for(int i = 0; i < 264; i++)
	//{
	//	myfile << (int)oai_decoded_bits[i] << "," << std::endl;
	//}
	//myfile.close();
        //myfile.open ("/home/john/OAIVITIBYTE.csv");
	//for(int i = 0; i < 33; i++)
	//{
	//	myfile << (int)oai_decoded_bytes[i] << "," << std::endl;
	//}
	//myfile.close();

	descramble_gnu(oai_decoded_bits);
	//descramble(oai_decoded_bytes, d_frame.psdu_size);

	gnu_print_output();
//std::cout << "here" << std::endl;
	// skip service field
	boost::crc_32_type result;
	result.process_bytes(oai_out_bytes + 2, d_frame.psdu_size);
	if(result.checksum() != 558161692) {
		dout << "grrr checksum wrong -- dropping" << std::endl;
		return;
	}
	

	mylog(boost::format("encoding: %1% - length: %2% - symbols: %3%")
			% d_ofdm.encoding % d_frame.psdu_size % d_frame.n_sym);

	// create PDU
	pmt::pmt_t blob = pmt::make_blob(oai_out_bytes + 2, d_frame.psdu_size - 4);
	pmt::pmt_t enc = pmt::from_uint64(d_ofdm.encoding);
	pmt::pmt_t dict = pmt::make_dict();
	dict = pmt::dict_add(dict, pmt::mp("encoding"), enc);
	dict = pmt::dict_add(dict, pmt::mp("snr"), pmt::from_double(d_snr));
	dict = pmt::dict_add(dict, pmt::mp("nomfreq"), pmt::from_double(d_nom_freq));
	dict = pmt::dict_add(dict, pmt::mp("freqofs"), pmt::from_double(d_freq_offset));
	dict = pmt::dict_add(dict, pmt::mp("dlt"), pmt::from_long(LINKTYPE_IEEE802_11));
	message_port_pub(pmt::mp("out"), pmt::cons(dict, blob));
}


void gnu_deinterleave() {

	int n_cbps = d_ofdm.n_cbps;
	int first[n_cbps];
	int second[n_cbps];
	int s = std::max(d_ofdm.n_bpsc / 2, 1);

	for(int j = 0; j < n_cbps; j++) {
		first[j] = s * (j / s) + ((j + int(floor(16.0 * j / n_cbps))) % s);
	}

	for(int i = 0; i < n_cbps; i++) {
		second[i] = 16 * i - (n_cbps - 1) * int(floor(16.0 * i / n_cbps));
	}

	int count = 0;
	//std::ofstream afile;
	//afile.open ("/home/john/OAIInterleave.csv");
	for(int i = 0; i < d_frame.n_sym; i++) {
		for(int k = 0; k < n_cbps; k++) {
			d_deinterleaved_soft_bits[i * n_cbps + second[first[k]]] = d_rx_soft_bits[i * n_cbps + k];
	//afile <<(int)d_deinterleaved_soft_bits[i * n_cbps + second[first[k]]] << ","<<std::endl;
		}
	}
//afile.close();
//std::cout << "oai" << std::endl;
}

void descramble_gnu (uint8_t *oai_decoded_bits) {

	int state = 0;
	std::memset(oai_out_bytes, 0, d_frame.psdu_size+2);
	//std::ofstream afile;
	//afile.open ("/home/john/OAIDS.csv");
	for(int i = 0; i < 7; i++) {
		if(oai_decoded_bits[i]) {
			state |= 1 << (6 - i);
		}
	}
	oai_out_bytes[0] = state;

	int feedback;
	int bit;
//std::cout << "d_frame.psdu_size*8+16" << (int)d_frame.psdu_size*8+16 << std::endl;
	for(int i = 7; i < d_frame.psdu_size*8+16; i++) {
		feedback = ((!!(state & 64))) ^ (!!(state & 8));
		bit = feedback ^ (oai_decoded_bits[i] & 0x1);
		//bit = (oai_decoded_bits[i] & 0x1);
		oai_out_bytes[i/8] |= bit << (i%8);
	//	afile <<(int)bit << ","<<std::endl;
	//	std::cout << "oai,"<<(int)(i/8) << std::endl;
		state = ((state << 1) & 0x7e) | feedback;
	}
	//afile.close();
}

void gnu_print_output() {

	dout << std::endl;
	dout << "The psdu size" << d_frame.psdu_size << std::endl;
	for(int i = 2; i < d_frame.psdu_size+2; i++) {
		dout << std::setfill('0') << std::setw(2) << std::hex << ((unsigned int)oai_out_bytes[i] & 0xFF) << std::dec << " ";
		if(i % 16 == 15) {
			dout << std::endl;
		}
	}
	dout << std::endl;
	for(int i = 2; i < d_frame.psdu_size+2; i++) {
		if((oai_out_bytes[i] > 31) && (oai_out_bytes[i] < 127)) {
			dout << ((char) oai_out_bytes[i]);
		} else {
			dout << ".";
		}
	}
	dout << std::endl;
}


private:
	bool d_debug;
	bool d_log;

	frame_param d_frame;
	ofdm_param d_ofdm;
	double d_snr;  // dB
	double d_nom_freq;  // nominal frequency, Hz
	double d_freq_offset;  // frequency offset, Hz
	soft_viterbi_decoder s_decoder;

	float d_rx_soft_symbols[48 * MAX_SYM]; // float type to store llrs coming from ls_soft.cc
// per llr one float value 
	char d_rx_soft_bits[MAX_ENCODED_BITS]; // char type to store llrs to be fed to deinterleaver
// per soft bit one char value (-8 to +7)
	char d_deinterleaved_soft_bits[MAX_ENCODED_BITS]; // char type to store deinterleaved llrs
// per deinterleaved llr one char value 
        uint8_t oai_out_bytes[MAX_PSDU_SIZE + 2]; 
	unsigned char decoded_bytes[MAX_ENCODED_BITS * 3 / 4]; 
	unsigned char oai_decoded_bytes[MAX_ENCODED_BITS * 3 / 4];
	unsigned char oai_decoded_bits[MAX_ENCODED_BITS * 3 / 4];
	
	int copied;
	bool d_frame_complete;
};

decode_mac_soft::sptr
decode_mac_soft::make(bool log, bool debug) {
	return gnuradio::get_initial_sptr(new decode_mac_soft_impl(log, debug));
}
