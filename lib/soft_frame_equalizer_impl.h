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

#ifndef INCLUDED_IEEE802_11_SOFT_FRAME_EQUALIZER_IMPL_H
#define INCLUDED_IEEE802_11_SOFT_FRAME_EQUALIZER_IMPL_H

#include <ieee802-11/soft_frame_equalizer.h>
#include <ieee802-11/constellations.h>
#include "equalizer/base_soft.h"
#include "viterbi_decoder.h"
#include "soft_viterbi_decoder.h"

namespace gr {
namespace ieee802_11 {

class soft_frame_equalizer_impl : virtual public soft_frame_equalizer
{

public:
	soft_frame_equalizer_impl(Equalizer_soft algo, double freq, double bw, int scaling, int threshold, bool log, bool debug);
	~soft_frame_equalizer_impl();

	void set_algorithm(Equalizer_soft algo);
	void set_bandwidth(double bw);
	void set_frequency(double freq);
	void set_scaling(int scaling);
	void set_threshold(int threshold);

	void forecast (int noutput_items, gr_vector_int &ninput_items_required);
	int general_work(int noutput_items,
			gr_vector_int &ninput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);

private:
	bool parse_signal(uint8_t *signal);
	bool decode_signal_field(uint8_t *rx_bits);
	bool s_decode_signal_field(float *rx_scaled_llr); // rx_scaled_llr are coming from ls_soft.cc

	void deinterleave(uint8_t *rx_bits);
	void s_deinterleave(float *rx_scaled_llr);

	equalizer_soft::base_soft *d_equalizer;
	gr::thread::mutex d_mutex;
	std::vector<gr::tag_t> tags;
	bool d_debug;
	bool d_log;
	int  d_current_symbol;
	viterbi_decoder d_decoder;
	soft_viterbi_decoder s_decoder; // soft decision decoder 

	// freq offset
	double d_freq;  // Hz
	double d_freq_offset_from_synclong;  // Hz, estimation from "sync_long" block
	double d_bw;  // Hz
	double d_er;
	double d_epsilon0;
	int d_scaling;
	int d_threshold;
	gr_complex d_prev_pilots[4];

	int  d_frame_bytes;
	int  d_frame_symbols;
	int  d_frame_encoding;

	uint8_t d_deinterleaved[48];
	char s_deinterleaved[48];
	unsigned char s_decoded_bytes[3];// becz oai decoder writes 1 bit in 1 bit field
	unsigned char s_decoded_bits[24];// becz oai decoder writes 1 bit in 1 bit field
	uint8_t s_decoded[24]; // becz GR writes 1 bit in 1 byte field: only for signal field 24 bits 
	gr_complex symbols[48];
	float noise_vec[64];

	boost::shared_ptr<gr::digital::constellation> d_frame_mod;
	constellation_bpsk::sptr d_bpsk;
	constellation_qpsk::sptr d_qpsk;
	constellation_16qam::sptr d_16qam;
	constellation_64qam::sptr d_64qam;

	static const int interleaver_pattern[48];
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_SOFT_FRAME_EQUALIZER_IMPL_H */
