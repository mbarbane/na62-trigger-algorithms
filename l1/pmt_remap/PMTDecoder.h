/*
 * PMTDecoder.h
 *
 *  Created on: 30/gen/2015
 *      Author: Valeria Fascianelli
 *      Email: vxf441@hep.ph.bham.ac.uk
 */

#ifndef L1_PMT_REMAP_PMTDECODER_H_  /// VEDERE CHI LO CHIAMA!!!
#define L1_PMT_REMAP_PMTDECODER_H_

#include "ConfFileReader.hpp"

#include <iostream>
#include <vector>

using namespace std;

class PMTDecoder {
public:

	PMTDecoder();
	virtual ~PMTDecoder();

	int getChannelRemap(int);
	vector<int> getChannelRemap();

	int nChannels;
	vector<int> channelRemap;

};

#endif /* L1_PMT_REMAP_PMTDECODER_H_ */
