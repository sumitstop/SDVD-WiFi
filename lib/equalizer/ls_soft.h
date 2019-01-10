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

#ifndef INCLUDED_IEEE802_11_EQUALIZER_LS_H
#define INCLUDED_IEEE802_11_EQUALIZER_LS_H

#include "base_soft.h"
#include <vector>

namespace gr {
namespace ieee802_11 {
namespace equalizer_soft {

class ls_soft: public base_soft {
public:
	virtual void equalize_soft(gr_complex *in, int n, gr_complex *symbols, gr_complex *symbols_oai, float *noise_vec, int scaling, int threshold, uint8_t *bits, float *llr, boost::shared_ptr<gr::digital::constellation> mod_soft, int d_frame_symbols);

	//virtual void equalize_soft_dc(gr_complex *in, gr_complex *in_1, int n, gr_complex *symbols, gr_complex *symbols_1, gr_complex *symbols_oai, gr_complex *symbols_oai_1, float *noise_vec, float *noise_vec_1, int scaling, int threshold, uint8_t *bits, uint8_t *bits_1, float *llr, float *llr_1, boost::shared_ptr<gr::digital::constellation> mod_soft, int d_frame_symbols);

	virtual double get_snr_soft();
private:
	gr_complex d_H_soft[64];
	float d_N_soft_loc[64];
	float d_N_soft_conv[64];
	//double CSI[64] = {};
	double d_H_Var[64] = {}; // normalized channel
	//float d_threshold = 3; // configurable param according to noise level 
	double d_snr_soft;
	int d_interference = 0; // interference present or not	
	float d_NLR = 0;
	float d_NLR2 = 0;
        double d_temp = 0;
        double temp_symbols[48] = {};
	
};

} /* namespace channel_estimation */
} /* namespace ieee802_11 */
} /* namespace gr */

#endif /* INCLUDED_IEEE802_11_EQUALIZER_LS_H */

