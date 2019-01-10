/*
 * Copyright (C) 2013 Bastian Bloessl <bloessl@ccs-labs.org>
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
#include "moving_average_ff_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace ieee802_11 {

moving_average_ff_impl::moving_average_ff_impl(int length) :
		sync_block("moving_average_ff",
			io_signature::make(1, 1, sizeof(float)),
			io_signature::make(1, 1, sizeof(float))),
		d_length(length),
		d_new_length(length),
		d_updated(false) {
	set_history(length);
}

void
moving_average_ff_impl::set_length(int length) {
	d_new_length = length;
	d_updated = true;
}

int
moving_average_ff_impl::work(int noutput_items, gr_vector_const_void_star
		&input_items, gr_vector_void_star &output_items) {

	if(d_updated) {
		d_length = d_new_length;
		set_history(d_length);
		d_updated = false;
		return 0; // history requirements might have changed
	}

	const float *in = (const float *)input_items[0];
	float *out = (float *)output_items[0];

	float sum = 0;
	for(int i = 0; i < d_length-1; i++) {
		sum += in[i];
	}

	for(int i = 0; i < noutput_items; i++) {
		sum += in[i + d_length - 1];
		out[i] = sum;
		sum -= in[i];
	}

	return noutput_items;
}

moving_average_ff::sptr
moving_average_ff::make(int length) {
        return gnuradio::get_initial_sptr(new moving_average_ff_impl(length));
}


} /* namespace blocks */
} /* namespace gr */
