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
 *
 */


#ifndef INCLUDED_IEEE802_11_SBMRC_DC_H
#define INCLUDED_IEEE802_11_SBMRC_DC_H

#include <ieee802-11/api.h>
#include <gnuradio/block.h>

enum Equalizer_sbmrc {
	LS_sb   = 0,
	LMS_sb  = 1,
	COMB_sb = 2,
	STA_sb  = 3,
};

namespace gr {
namespace ieee802_11 {

class IEEE802_11_API sbmrc_dc : virtual public gr::block
{

public:
	typedef boost::shared_ptr<sbmrc_dc> sptr;
	static sptr make(Equalizer_sbmrc algo, double freq, double bw, int scaling, int threshold,
			bool log, bool debug);
	virtual void set_algorithm(Equalizer_sbmrc algo) = 0;
	virtual void set_bandwidth(double bw) = 0;
	virtual void set_frequency(double freq) = 0;
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_SBMRC_DC_H */
